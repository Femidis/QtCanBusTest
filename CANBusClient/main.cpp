#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "caninterfaceclient.h"
#include "targetsmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CanInterfaceClient client = CanInterfaceClient("vcan0");
    TargetsModel model;

    QObject::connect(&client, &CanInterfaceClient::activeTargetsChanged, &model, &TargetsModel::renewTargets);

    auto a = targetObject{25,95,45,369,78,42};

    model.addTarget(a);

    engine.rootContext()->setContextProperty("CanInterfaceClient", &client);
    engine.rootContext()->setContextProperty("TargetModel", &model);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.loadFromModule("CANBus", "Main");



    return app.exec();
}
