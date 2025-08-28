#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include "CanLib_global.h"
#include <QDebug>
#include <QFile>
#include <QCanBus>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <bitset>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);


class CANLIB_EXPORT targetObject
{

public:
    int cls;
    int prb;
    int x,y,w,h;

    targetObject() = default;
    targetObject(QByteArray& data); // Получение объекта из массива байтов

    targetObject(const int& cls, const int& prb, const int& x, const int& y, const int& w, const int& h)
        : cls(cls), prb(prb), x(x), y(y), w(w), h(h)
    {
    }

    void print(); // Вывод на экран

    QByteArray toByteData(); // Представление объекта в виде массива байтов

    // targetObject fromByteData(QByteArray& data); // Получение объекта из массива байтов


};


class CANLIB_EXPORT CanInterface : public QObject
{
    Q_OBJECT
public:
    CanInterface(const char*);
    ~CanInterface();


public slots:
    virtual void receiveFrames();


protected:
    bool SendFrame(const int addressTo, const int id, const QByteArray &data, bool setFD); // Отправка данных по СAN шине
    QCanBusDevice *device; // Указатель на устройство
    uint8_t devNumber; // Номер устройства в "сети", используется для идентификации

private:






};

#endif // CANINTERFACE_H
