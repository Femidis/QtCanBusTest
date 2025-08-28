#ifndef CANISERVER_H
#define CANISERVER_H

#include <QDebug>
#include <QCanBus>
#include <QGuiApplication>
#include "caninterface.h"


class CanIServer : public CanInterface
{
Q_OBJECT
public:
    CanIServer(const char*);
    ~CanIServer();

signals:
    void chainSendCommand(int commandIndex);


public slots:
    void handleText(const QString &text);
    void handleText(const QVariantMap &data);

    void receiveFrames() override;

private:
    bool reciveCommand(const QByteArray &data, int&);



};





#endif // CANISERVER_H
