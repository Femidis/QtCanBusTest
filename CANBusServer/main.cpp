#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "caninterface.h"
#include "integrationmodule.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CanInterface server = CanInterface("vcan0");
    IntegrationModule *integrModule = new IntegrationModule(&app);

    engine.rootContext()->setContextProperty("CanInterface", &server);

    qmlRegisterSingletonInstance("App.Core", 1, 0, "IntegrationModule", integrModule);

    QObject::connect(
        &server,
        &CanInterface::chainSendCommand,
        integrModule,
        &IntegrationModule::recieveCommand);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.loadFromModule("CANBus", "Main");

    return app.exec();
}
