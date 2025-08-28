#ifndef CANICLIENT_H
#define CANICLIENT_H

#include <QDebug>
#include <QCanBus>
#include <QGuiApplication>
#include "caninterface.h"


class CanIClient : public CanInterface
{
Q_OBJECT

public:
    CanIClient(const char*);
    ~CanIClient();

signals:
    void activeTargetsChanged(QVector<targetObject>);

public slots:
    void sendCommand(const uint8_t& comm_index);

    void receiveFrames() override;

private:
    bool reciveMessage(const QByteArray &data);
};

#endif // CANINTERFACE_H
