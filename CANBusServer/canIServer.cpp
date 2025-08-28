#include "canIServer.h"
#include <cmath>
#include <iostream>
#include <bitset>


CanIServer::CanIServer(const char* devName) : CanInterface(devName) {}

CanIServer::~CanIServer(){}


void CanIServer::handleText(const QString &text){
    qDebug() << "Received data from string:";
    // SendFrame(text.toLatin1());

}

void CanIServer::handleText(const QVariantMap &data){

    targetObject t;

    if (data.contains("class")) t.cls = data["class"].toInt();
    if (data.contains("probability")) t.prb = data["probability"].toInt();
    if (data.contains("x")) t.x = data["x"].toInt();
    if (data.contains("y")) t.y = data["y"].toInt();
    if (data.contains("width")) t.w = data["width"].toInt();
    if (data.contains("height")) t.h = data["height"].toInt();

    // t.print();

    SendFrame(15, 2,t.toByteData(), 1);


}

void CanIServer::receiveFrames()
{

    while (device->framesAvailable()) {

        const QCanBusFrame frame = device->readFrame();

        const QString time = QString::fromLatin1("%1.%2  ")
                                 .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                                 .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        const int id = frame.frameId();
        const QString dlc = QString::number(frame.payload().size());

        qDebug() << "Recieved frame with id:" << id << "at time" << time;

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

bool CanIServer::reciveCommand(const QByteArray &data, int& command)
{
    int addrFrom = (data[0] & 0b11110000) >> 4;
    int addrTo =  (data[0] & 0b00001111);

    // std::string byteArray = data.toStdString();

    // qDebug() << byteArray;

    // std::string bits;
    // for (int i = 0; i < byteArray.size(); ++i) {
    //     auto byte = static_cast<unsigned char>(byteArray[i]);
    //     bits += std::bitset<8>(byte).to_string(); // Преобразует байт в 8 бит
    // }
    // qDebug() << bits;

    command = data[1];

    qDebug() << "Recieved message" << "\n"
             << "From:" << addrFrom << "\n"
             << "To:" << addrTo << "\n"
             << "Message type: command" << "\n"
             << "Command:" << command;
    return 1;

}





