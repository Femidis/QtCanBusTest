#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <QDebug>
#include <QCanBus>
#include <QGuiApplication>

class CanInterface : public QObject
{
Q_OBJECT
public:
    CanInterface(const char*);
    ~CanInterface();


public slots:
    void handleText(const QString &text);

private:
    // attrs
    QCanBusDevice *device;

    // methods
    bool SendFrame(QByteArray);



};

#endif // CANINTERFACE_H
