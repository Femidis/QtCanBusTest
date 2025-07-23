#ifndef INTEGRATIONMODULE_H
#define INTEGRATIONMODULE_H

#include <QObject>
#include <QQmlEngine>

class IntegrationModule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Mode mode READ getMode NOTIFY modeChanged)


public:

    enum class Mode{
        DEZ,
        BOEVOI
    };Q_ENUM(Mode)

    explicit IntegrationModule(QObject* parent = nullptr);


    Mode getMode()const;


signals:
    void modeChanged();

public slots:
    void recieveCommand(int commandIndex);


private:
    Mode mode_;



};

#endif // INTEGRATIONMODULE_H
