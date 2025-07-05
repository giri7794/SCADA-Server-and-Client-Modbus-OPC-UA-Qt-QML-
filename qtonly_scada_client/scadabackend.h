#ifndef SCADABACKEND_H
#define SCADABACKEND_H

#pragma once

#include <QObject>
#include <QtSerialBus/QModbusTcpClient>
#include <QtOpcUa/QOpcUaClient>
#include <QtOpcUa/QOpcUaNode>
#include <QtOpcUa/QOpcUaProvider>

class ScadaBackend : public QObject {
    Q_OBJECT
    Q_PROPERTY(int analog0 READ analog0 NOTIFY dataUpdated)
    Q_PROPERTY(int analog1 READ analog1 NOTIFY dataUpdated)
    Q_PROPERTY(bool digital0 READ digital0 NOTIFY dataUpdated)
    Q_PROPERTY(bool digital1 READ digital1 NOTIFY dataUpdated)

public:
    explicit ScadaBackend(QObject *parent = nullptr);
    void start();

    int analog0() const { return m_analog[0]; }
    int analog1() const { return m_analog[1]; }
    bool digital0() const { return m_digital[0]; }
    bool digital1() const { return m_digital[1]; }

signals:
    void dataUpdated();

private slots:
    void writeToModbus();
    void handleOpcUaDataChange(QOpcUa::NodeAttribute attr, const QVariant &value);

private:
    QModbusClient *modbusClient;
    QOpcUaClient *opcClient;

    QVector<QOpcUaNode*> opcNodes;

    int m_analog[2] = {0, 0};
    bool m_digital[2] = {false, false};

    void setupModbus();
    void setupOpcUa();
};

#endif // SCADABACKEND_H
