#include "caninterface.h"
#include <cmath>
#include <iostream>
#include <bitset>

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

    connect(device, &QCanBusDevice::framesReceived,
            this, &CanInterface::receiveFrames);

}

CanInterface::~CanInterface(){

    if(device){
        device->disconnectDevice();
        device = nullptr;
    }

}


void CanInterface::handleText(const QString &text){
    qDebug() << "Received data from string:";
    // SendFrame(text.toLatin1());

}

void CanInterface::handleText(const QVariantMap &data){

    qDebug() << "Received data from QML:";

    for (auto it = data.begin(); it != data.end(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }

    targetObject t;

    if (data.contains("class")) t.cls = data["class"].toInt();
    if (data.contains("probability")) t.prb = data["probability"].toInt();
    if (data.contains("x")) t.x = data["x"].toInt();
    if (data.contains("y")) t.y = data["y"].toInt();
    if (data.contains("width")) t.w = data["width"].toInt();
    if (data.contains("height")) t.h = data["height"].toInt();

    t.print();

    SendFrameTargets(t);


}

void CanInterface::receiveFrames()
{
    while (device->framesAvailable()) {

        const QCanBusFrame frame = device->readFrame();

        const QString time = QString::fromLatin1("%1.%2  ")
                                 .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                                 .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        const int id = frame.frameId();
        const QString dlc = QString::number(frame.payload().size());


        switch (id) {
        case 1:{

            int commandIndex = 0;
            reciveCommand(frame.payload(), commandIndex);
            emit chainSendCommand(commandIndex);
            break;

        }
        default:
            break;
        }



    }
}


bool CanInterface::SendFrameTargets(const targetObject& targets){

    //example numbers of devicess
    int devNumber = 1;
    int devTarget = 15;

    QByteArray objectData;

    objToFrame(objectData, targets); // пока ест по одному


    int addresses = (devNumber | 0) << 4;
    addresses = (devTarget | addresses);

    QByteArray payload = objectData;

    payload.prepend(addresses);


    QCanBusFrame::FrameId id = 0;

    qDebug() << "size of data" <<  payload.size();

    const bool canFdSupported = device->configurationParameter(
                                            QCanBusDevice::CanFdKey).toBool();

    qDebug() << "canFdSupported" << canFdSupported;



    const int alignedSize = std::ceil(payload.size() / 4.0) * 4;

    if (payload.size()>8)
        payload.append(QByteArray(alignedSize - payload.size(), 0));


    qDebug() << "size of payload" << payload.size();

    QCanBusFrame frame(id, payload);

    frame.setFlexibleDataRateFormat(true);

    if(frame.isValid()){

        if (device->writeFrame(frame)) {
            qDebug() << "ID:" << id << "Data:" << objectData.toHex();
        } else {
            qDebug() << "Ошибка отправки:" << device->errorString();
        }
    }
    else
        qDebug() << frame.error();

    return 1;
}

bool CanInterface::reciveCommand(const QByteArray &data, int& command)
{
    int addrFrom = (data[0] & 0b11110000) >> 4;
    int addrTo =  (data[0] & 0b00001111);

    qDebug() << "addrFrom:" << addrFrom << "addrTo:"  << addrTo;

    command = data[1];

    qDebug() << "recieved command" << command;

    return 1;

}

bool CanInterface::objToFrame(QByteArray& data, const targetObject& target)
{


    uint8_t message[7]{0};

    long xb = (target.x | 0b0000000000);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 10;

    xb = (target.y | xb);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 10;

    xb = (target.w | xb);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 10;

    xb = (target.h | xb);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 8;

    xb = target.prb | xb;

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 8;

    xb = target.cls | xb;

    // qDebug() << std::bitset<56>{xb}.to_string();

    for(int i = 6; i >= 0;i--){

        message[i] = xb & 0b1111'1111;
        xb >>= 8;
    }

    // qDebug() << sizeof(message);

    data = QByteArray(reinterpret_cast<const char*>(message), 7);

    for(int i = 0; i < 7; i++) qDebug() << std::bitset<8>{message[i]}.to_string();

    return 1;

}

void targetObject::print()
{
    qDebug() << "\tClass:" << cls;
    qDebug() << "\tВероятность:" << prb;
    qDebug() << "\tX:" << x;
    qDebug() << "\tY:" << y;
    qDebug() << "\tW:" << w;
    qDebug() << "\tH:" << h;

}



/*
"00000000000000000000000000000000000000000000000010001001"
"00000000000000000000000000000000000000100010010110000010"
"00000000000000000000000000001000100101100000100001000111"
"00000000000000000010001001011000001000010001110000100100"
"00000000001000100101100000100001000111000010010001011100"
"00100010010110000010000100011100001001000101110000010001"

10001000 00111010 0010010000 1110001000 0100000110 1001000100


0010001001 0110000010 0001000111 0000100100 01011100 00010001

"00010001"
"01011100"
"00100100"
"00011100"
"00100001"
"01011000"
"00100010"
*/




