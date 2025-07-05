#include "scadabackend.h"
#include <QtSerialBus/QModbusDataUnit>
#include <QTimer>
#include <QDateTime>

ScadaBackend::ScadaBackend(QObject *parent)
    : QObject(parent), modbusClient(new QModbusTcpClient(this)) {
}

void ScadaBackend::start() {
    setupModbus();
    setupOpcUa();
}

void ScadaBackend::setupModbus() {
    modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.29.18");
    modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, 1502);
    modbusClient->setTimeout(1000);
    modbusClient->setNumberOfRetries(3);
    modbusClient->connectDevice();

    QTimer *writeTimer = new QTimer(this);
    connect(writeTimer, &QTimer::timeout, this, &ScadaBackend::writeToModbus);
    writeTimer->start(2000);
}

void ScadaBackend::writeToModbus() {
    if (!modbusClient || modbusClient->state() != QModbusDevice::ConnectedState)
        return;

    static int value = 0;
    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, 0, 4);
    unit.setValue(0, value);
    unit.setValue(1, value + 10);
    unit.setValue(2, (value / 5) % 2);
    unit.setValue(3, (value / 5 + 1) % 2);
    value += 5;

    modbusClient->sendWriteRequest(unit, 1);
}

void ScadaBackend::setupOpcUa() {
    QOpcUaProvider provider;
    opcClient = provider.createClient("open62541");
    if (!opcClient)
        return;

    connect(opcClient, &QOpcUaClient::endpointsRequestFinished,
            this, [=](QList<QOpcUaEndpointDescription> endpoints, QOpcUa::UaStatusCode statusCode, QUrl) {
                if (statusCode != QOpcUa::Good || endpoints.isEmpty()) {
                    qWarning("No endpoints or failed request");
                    return;
                }

                // Choose first endpoint
                opcClient->connectToEndpoint(endpoints.first());
            });

    connect(opcClient, &QOpcUaClient::stateChanged, this, [=](QOpcUaClient::ClientState state) {
        if (state == QOpcUaClient::Connected) {
            const QStringList nodeNames = {"Analog0", "Analog1", "Digital0", "Digital1"};

            for (const QString &name : nodeNames) {
                QOpcUaNode *node = opcClient->node(QString("ns=1;s=%1").arg(name));
                connect(node, &QOpcUaNode::attributeUpdated,
                        this, &ScadaBackend::handleOpcUaDataChange);
                node->enableMonitoring(QOpcUa::NodeAttribute::Value,
                                       QOpcUaMonitoringParameters(500));
                opcNodes.append(node);
            }
        }
    });

    // Discover endpoints before connecting
    opcClient->requestEndpoints(QUrl("opc.tcp://192.168.29.18:4840"));
}

void ScadaBackend::handleOpcUaDataChange(QOpcUa::NodeAttribute attr, const QVariant &value) {
    QOpcUaNode *node = qobject_cast<QOpcUaNode *>(sender());
    if (!node || attr != QOpcUa::NodeAttribute::Value || !value.isValid())
        return;

    const QString nodeId = node->nodeId();

    if (nodeId.contains("Analog0")) m_analog[0] = value.toInt();
    else if (nodeId.contains("Analog1")) m_analog[1] = value.toInt();
    else if (nodeId.contains("Digital0")) m_digital[0] = value.toBool();
    else if (nodeId.contains("Digital1")) m_digital[1] = value.toBool();

    emit dataUpdated();
}

