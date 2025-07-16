#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "caninterface.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CanInterface server = CanInterface("vcan0");
    engine.rootContext()->setContextProperty("CanInterface", &server);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.loadFromModule("CANBus", "Main");






    return app.exec();
}
