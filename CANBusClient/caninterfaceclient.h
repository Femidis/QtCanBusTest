#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <QDebug>
#include <QCanBus>
#include <QGuiApplication>

class CanInterfaceClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dynamicText READ dynamicText NOTIFY dynamicTextChanged)

public:
    CanInterfaceClient(const char*);
    ~CanInterfaceClient();

    QString dynamicText() const;

    QCanBusDevice *device; // перенести в приват


public slots:
    void handleText(const QString &text);
    void receiveFrames();
    void updateText(const QString &newText);

signals:
    void dynamicTextChanged();

private:
    // attrs
    QString m_dynamicText;

    // methods
    bool SendFrame(QByteArray);



};

#endif // CANINTERFACE_H
