#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "scadabackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ScadaBackend backend;
    backend.start();
    engine.rootContext()->setContextProperty("scada", &backend);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("qtonly_scada_client", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
