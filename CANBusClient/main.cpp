#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

#include "canIClient.h"
#include "targetsmodel.h"
#include "customlogger.h"



int main(int argc, char *argv[])
{

    char* deviceName = "vcan1";
    QString logFileName = "client";


    switch(argc){

    case 2:
        deviceName = argv[1];
        break;
    case 3:
        deviceName = argv[1];
        logFileName = argv[2];
        break;
    }

    qDebug()<< deviceName;


    QFile::remove(logFileName+"_log.txt");

    CustomLogger::setLogFile(logFileName + "_log.txt");


    qInstallMessageHandler(CustomLogger::messageHandler);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CanIClient client = CanIClient(deviceName);
    TargetsModel model;

    QObject::connect(&client, &CanIClient::activeTargetsChanged, &model, &TargetsModel::renewTargets);

    // auto a = targetObject{25,95,45,369,78,42};

    // model.addTarget(a);

    qmlRegisterSingletonInstance("App.Core", 1, 0,"CanInterface", &client);
    qmlRegisterSingletonInstance("App.Core", 1, 0,"TargetModel", &model);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.loadFromModule("CANBus", "Main");



    return app.exec();
}
