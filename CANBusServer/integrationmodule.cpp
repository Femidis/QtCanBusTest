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

    mode_ = static_cast<Mode>(commandIndex);
    qDebug() << "Change Mode to" << mode_;
    emit modeChanged();

}


