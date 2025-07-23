#include "caninterfaceclient.h"
#include <bitset>

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


// void CanInterfaceClient::updateText(const QString &newText) {
//     // if (m_dynamicText != newText) {
//     //     m_dynamicText = newText;
//     //     emit dynamicTextChanged();  // Уведомляем QML об изменении
//     // }
// }


void CanInterfaceClient::handleText(const QString &text){

    SendFrame(text.toLocal8Bit());

}

void CanInterfaceClient::sendCommand(const uint8_t &comm_index)
{
    int devNumber = 1;
    int devTarget = 15;

    QByteArray objectData;
    objectData.append(comm_index);


    int addresses = (devNumber | 0) << 4;
    addresses = (devTarget | addresses);

    QByteArray payload = objectData;

    payload.prepend(addresses);

    SendFrame(payload);
}


void CanInterfaceClient::receiveFrames(){

    while (device->framesAvailable()) {

        const QCanBusFrame frame = device->readFrame();

        reciveMessage(frame.payload());

        const QString time = QString::fromLatin1("%1.%2  ")
                                 .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                                 .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        const QString id = QString::number(frame.frameId(), 16).toUpper();
        const QString dlc = QString::number(frame.payload().size());

    }

}


bool CanInterfaceClient::SendFrame(QByteArray data){

    QCanBusFrame::FrameId id = 1;

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

bool CanInterfaceClient::reciveMessage(const QByteArray& data){

    int addrFrom = (data[0] & 0b11110000) >> 4;
    int addrTo =  (data[0] & 0b00001111);

    qDebug() << "addrFrom:" << addrFrom << "addrTo:"  << addrTo;

    int objCount = (data.size()-1) / 7;

    qDebug() << "Количество объектов" << objCount;

    QVector<targetObject> tgObjs;

    for (int obj = 1; obj <= objCount; obj++){

        targetObject t;

        for(int i=0;i<8;i++) qDebug() << std::bitset<8>{data[i]}.to_string();

        t.cls = data[obj*7];
        t.prb = data[obj*7-1];

        qDebug() << typeid(data[obj*7]).name();


        qDebug() << "Класс объекта" << t.cls;
        qDebug() << "Вероятность объекта" << t.prb;

        uint8_t shift = 0;

        for(int j = obj*7-3; j >= obj*7 - 6; j-- ){

            uint16_t buf{0};

            qDebug() << "Current" << j;

            qDebug() <<"start"<< std::bitset<16>{buf}.to_string();
            qDebug() <<"data[j]"<< std::bitset<16>{data[j]}.to_string();
            qDebug() <<"data[j]"<< std::bitset<16>{data[j+1]}.to_string();

            buf = data[j] & 0b11111111;
            qDebug() << "1:" << std::bitset<16>{buf}.to_string();
            buf <<= 8;
            qDebug() << "2:" << std::bitset<16>{buf}.to_string();
            buf = (data[j+1] & 0b11111111) | buf;
            qDebug() << "3:" << std::bitset<16>{buf}.to_string();
            buf >>= shift;
            qDebug() << "4:" << std::bitset<16>{buf}.to_string();
            buf &= 0b1111111111;
            qDebug() << "5:" << std::bitset<16>{buf}.to_string();
            shift+=2;
            qDebug() << "6:" << std::bitset<16>{buf}.to_string();



            switch (j%7 + 1){
            case 5:
                t.h = buf;
                break;
            case 4:
                t.w = buf;
                break;
            case 3:
                t.y = buf;
                break;
            case 2:
                t.x = buf;
                break;
            }




            // buf = data[j] & ((1 << shift)-1);

            qDebug() << std::bitset<16>{buf}.to_string();

            qDebug() << "Параметр"<< j%7-2 << buf;
        }

        t.print();

        tgObjs.append(t);
    }

    emit activeTargetsChanged(tgObjs);
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
