#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <thread>    // For std::thread

#include "client_data.h"
#include "modbus_writer.h"
#include "opcua_reader.h"
#include "scada_viewmodel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    ClientData data;
    std::thread(modbus_writer, std::ref(data)).detach();
    std::thread(opcua_reader, std::ref(data)).detach();

    ScadaViewModel model(data);
    engine.rootContext()->setContextProperty("scada", &model);
    engine.loadFromModule("scada_client", "Main");

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML root object!";
        return -1;
    }

    return app.exec();
}
