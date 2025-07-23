#include "integrationmodule.h"

IntegrationModule::Mode IntegrationModule::getMode()const
{
    return mode_;
}

IntegrationModule::IntegrationModule(QObject* parent) : QObject(parent) {

    mode_ = Mode::DEZ;

}


void IntegrationModule::recieveCommand(int commandIndex)
{

    qDebug() << "recieved Command" << commandIndex;
    mode_ = static_cast<Mode>(commandIndex);
    qDebug() << "recieved Mode" << mode_;
    emit modeChanged();

}


