#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <QDebug>
#include <QCanBus>
#include <QGuiApplication>


class targetObject
{

public:
    int cls;
    int prb;
    int x,y,w,h;

    targetObject() = default;

    targetObject(const int& cls, const int& prb, const int& x, const int& y, const int& w, const int& h)
        : cls(cls), prb(prb), x(x), y(y), w(w), h(h)
    {
    }

    void print();
};



class CanInterfaceClient : public QObject
{
    Q_OBJECT

public:
    CanInterfaceClient(const char*);
    ~CanInterfaceClient();





public slots:

    void handleText(const QString& text);
    void sendCommand(const uint8_t& comm_index);
    void receiveFrames();
    // void updateText(const QString &newText);

signals:

    void activeTargetsChanged(QVector<targetObject>);

private:

    // attrs
    QCanBusDevice *device;

    // methods
    bool SendFrame(QByteArray);
    bool reciveMessage(const QByteArray &data);
};

#endif // CANINTERFACE_H
