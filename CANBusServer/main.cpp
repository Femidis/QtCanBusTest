#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>


#include "canIServer.h"
#include "integrationmodule.h"
#include "customlogger.h"




int main(int argc, char *argv[])
{
    QString logFileName = "server";

    QFile::remove(logFileName+"_log.txt");

    CustomLogger::setLogFile(logFileName + "_log.txt");


    qInstallMessageHandler(CustomLogger::messageHandler);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;



    CanIServer server = CanIServer("vcan0");
    IntegrationModule *integrModule = new IntegrationModule(&app);

    qmlRegisterSingletonInstance("App.Core", 1, 0, "IntegrationModule", integrModule);
    qmlRegisterSingletonInstance("App.Core", 1, 0, "CanInterface", &server);


    QObject::connect(
        &server,
        &CanIServer::chainSendCommand,
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
