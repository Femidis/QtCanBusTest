#include "caninterface.h"

CanInterface::CanInterface(const char* devName) {

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

}

CanInterface::~CanInterface(){

    if(device){
        device->disconnectDevice();
        device = nullptr;
    }

}


void CanInterface::handleText(const QString &text){

    SendFrame(text.toLatin1());

}


bool CanInterface::SendFrame(QByteArray data){

    QCanBusFrame::FrameId id = 0;

    qDebug() << "size of data" <<  data.size();

    const bool canFdSupported = device->configurationParameter(
                                            QCanBusDevice::CanFdKey).toBool();

    qDebug() << "canFdSupported" << canFdSupported;

    QByteArray payload = data;

    int align_byte = data.size();


    qDebug() << "size of data" <<  (align_byte % 4);

    align_byte = (align_byte>8) * (align_byte % 4);

    payload.append(QByteArray(align_byte, 0));


    qDebug() << "size of payload" << payload.size();

    QCanBusFrame frame(id, payload);

    frame.setFlexibleDataRateFormat(true);

    // frame.setFrameId(id);
    // frame.setPayload(data);

    if(!frame.isValid())
        qDebug() << frame.error();


    if (device->writeFrame(frame)) {
        qDebug() << "ID:" << id << "Data:" << data.toHex();
    } else {
        qDebug() << "Ошибка отправки:" << device->errorString();
    }
    return 1;
}
