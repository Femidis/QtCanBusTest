#include "caninterfaceclient.h"

CanInterfaceClient::CanInterfaceClient(const char* devName) {

    bool isConnected = false;

    QString errorString;
    device = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), QString(devName), &errorString);


    if (!device)
        qDebug() << "!device " << errorString;
    else
    {
        // Убираем попытку установки скорости (для vcan не требуется)
        device->setConfigurationParameter(QCanBusDevice::BitRateKey, QVariant()); // Сброс
        device->setConfigurationParameter(QCanBusDevice::CanFdKey, QVariant(true));
        isConnected = device->connectDevice();
    }

    qInfo() << isConnected;

    qInfo() << device->configurationParameter(QCanBusDevice::CanFdKey);

    connect(device, &QCanBusDevice::framesReceived,
            this, &CanInterfaceClient::receiveFrames);

}

CanInterfaceClient::~CanInterfaceClient(){

    if(device){
        device->disconnectDevice();
        device = nullptr;
    }

}

QString CanInterfaceClient::dynamicText() const {
    return m_dynamicText;
}

void CanInterfaceClient::updateText(const QString &newText) {
    if (m_dynamicText != newText) {
        m_dynamicText = newText;
        emit dynamicTextChanged();  // Уведомляем QML об изменении
    }
}


void CanInterfaceClient::handleText(const QString &text){

    SendFrame(text.toLocal8Bit());

}


void CanInterfaceClient::receiveFrames(){

    while (device->framesAvailable()) {

        const QCanBusFrame frame = device->readFrame();

        QString data;
        if (frame.frameType() == QCanBusFrame::ErrorFrame)
            data = device->interpretErrorFrame(frame);
        else
            qInfo() << frame.hasFlexibleDataRateFormat();
            data = QString::fromLatin1(frame.payload());

        const QString time = QString::fromLatin1("%1.%2  ")
                                 .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                                 .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));


        const QString id = QString::number(frame.frameId(), 16).toUpper();
        const QString dlc = QString::number(frame.payload().size());

        updateText(data);

    }

}


bool CanInterfaceClient::SendFrame(QByteArray data){

    QCanBusFrame::FrameId id = 0;

    QCanBusFrame frame;
    frame.setFrameId(id);
    frame.setPayload(data);

    if (device->writeFrame(frame)) {
        qDebug() << "ID:" << id << "Data:" << data.toHex();
    } else {
        qDebug() << "Ошибка отправки:" << device->errorString();
    }
    return 1;
}


