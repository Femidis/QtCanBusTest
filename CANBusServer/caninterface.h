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

class CanInterface : public QObject
{
Q_OBJECT
public:
    CanInterface(const char*);
    ~CanInterface();

signals:
    void chainSendCommand(int commandIndex);


public slots:
    void handleText(const QString &text);
    void handleText(const QVariantMap &data);
    void receiveFrames();

private:
    // attrs
    QCanBusDevice *device;

    // methods
    // bool SendFrame(QByteArray);
    bool objToFrame(QByteArray&, const targetObject&);
    bool SendFrameTargets(const targetObject& targets);
    bool reciveCommand(const QByteArray &data, int&);



};





#endif // CANINTERFACE_H
