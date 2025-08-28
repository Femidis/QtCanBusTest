#include "canIClient.h"
#include <bitset>

CanIClient::CanIClient(const char* devName)  : CanInterface(devName) {}


CanIClient::~CanIClient(){}


void CanIClient::sendCommand(const uint8_t &comm_index)
{
    QByteArray objectData;
    objectData.append(comm_index);

    SendFrame(1, 1, objectData, 1);
}


void CanIClient::receiveFrames(){


    while (device->framesAvailable()) {

        const QCanBusFrame frame = device->readFrame();

        // qDebug() << frame.payload().toStdString();

        // std::string byteArray = frame.payload().toStdString();

        // std::string bits;
        // for (int i = 0; i < byteArray.size(); ++i) {
        //     auto byte = static_cast<unsigned char>(byteArray[i]);
        //     bits += std::bitset<8>(byte).to_string(); // Преобразует байт в 8 бит
        // }
        // qDebug() << "Binary: " << bits;

        reciveMessage(frame.payload());

        const QString time = QString::fromLatin1("%1.%2  ")
                                 .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                                 .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        const QString id = QString::number(frame.frameId(), 16).toUpper();
        const QString dlc = QString::number(frame.payload().size());

        qDebug() << "Recieved frame with id:" << id << "at time" << time;
    }
}

bool CanIClient::reciveMessage(const QByteArray& data){

    // int addrFrom = (data[0] & 0b11110000) >> 4;
    // int addrTo =  (data[0] & 0b00001111);

    int objCount = (data.size()-1) / 7;

    QVector<targetObject> tgObjs;

    for (int obj = 0; obj < objCount; obj++){

        QByteArray y = data.sliced(1 + obj*7 ,7);
        targetObject t(y);
        // t.print();
        tgObjs.append(t);
    }

    emit activeTargetsChanged(tgObjs);
    return 1;

}
