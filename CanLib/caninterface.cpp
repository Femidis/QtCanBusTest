#include "caninterface.h"

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex); // Для потокобезопасности



    QFile logFile("log.txt");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return;



    const char* typeStr = [](QtMsgType t) {
        switch(t) {
        case QtDebugMsg:    return "DEBUG";
        case QtInfoMsg:     return "INFO";
        case QtWarningMsg:  return "WARNING";
        case QtCriticalMsg: return "ERROR";
        case QtFatalMsg:    return "FATAL";
        default:            return "UNKNOWN";
        }
    }(type);

    // 4. Форматирование строки (самая эффективная конкатенация)
    QTextStream out(&logFile);
    out << '['
        << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
        << "]["
        << typeStr
        << "] "
        << msg
        << '\n';

    // 5. Явный сброс буфера (важно для критичных логов)
    out.flush();

    logFile.close();
}



void targetObject::print()
{
    qDebug() << "\n\tClass:" << cls << "\n"
             << "\tВероятность:" << prb << "\n"
             << "\tX:" << x << "\n"
             << "\tY:" << y << "\n"
             << "\tW:" << w << "\n"
             << "\tH:" << h;

}

QByteArray targetObject::toByteData()
{
    uint8_t message[7]{0};

    long xb = (x | 0b0000000000);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 10;

    xb = (y | xb);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 10;

    xb = (w | xb);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 10;

    xb = (h | xb);

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 8;

    xb = prb | xb;

    // qDebug() << std::bitset<56>{xb}.to_string();

    xb = xb << 8;

    xb = cls | xb;

    // qDebug() << std::bitset<56>{xb}.to_string();

    for(int i = 6; i >= 0;i--){

        message[i] = xb & 0b1111'1111;
        xb >>= 8;
    }

    // qDebug() << sizeof(message);

    QByteArray data = QByteArray(reinterpret_cast<const char*>(message), 7);

    // for(int i = 0; i < 7; i++) qDebug() << std::bitset<8>{message[i]}.to_string();

    return data;

}

targetObject::targetObject(QByteArray& data)
{

    // qDebug() << "Size of data:" <<data.size();

    cls = data[6];

    // qDebug() << data[6] << cls;

    prb = data[5];

    // qDebug() << data[5] << prb;

    uint8_t shift = 0;

    for(int j = 3; j >= 0; j-- ){


        uint16_t buf{0};
        buf = data[j] & 0b11111111;
        // qDebug() << std::bitset<16>{buf}.to_string();
        buf <<= 8;
        // qDebug() << std::bitset<16>{buf}.to_string();
        buf = (data[j+1] & 0b11111111) | buf;

        // qDebug() << std::bitset<16>{buf}.to_string();

        buf >>= shift;
        // qDebug() << std::bitset<16>{buf}.to_string();
        buf &= 0b1111111111;
        // qDebug() << std::bitset<16>{buf}.to_string();
        shift+=2;

        switch (j){
        case 3:
            h = buf;
            break;
        case 2:
            w = buf;
            break;
        case 1:
            y = buf;
            break;
        case 0:
            x = buf;
            break;
        }

        // qDebug() << j;

    }
}


CanInterface::CanInterface(const char* devName) {

    bool isConnected = false;

    QString errorString;
    device = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), QString(devName), &errorString);


    if (device){
        // Убираем попытку установки скорости (для vcan не требуется)
        device->setConfigurationParameter(QCanBusDevice::BitRateKey, QVariant()); // Сброс
        device->setConfigurationParameter(QCanBusDevice::CanFdKey, QVariant(true));
        isConnected = device->connectDevice();
    }
    else{
        qDebug() << "!device " << device->errorString();
        return;
    }


    if (isConnected){
        qInfo()<< "CAN device successfully connected" << "From CanInterface";
        connect(device, &QCanBusDevice::framesReceived,
                this, &CanInterface::receiveFrames);
    }
    else
        qInfo()<< "CAN device not connected with error" << device->errorString();

}

CanInterface::~CanInterface(){

    if(device){
        device->disconnectDevice();
        delete device;
        device = nullptr;
    }

}


void CanInterface::receiveFrames()
{
    while (device->framesAvailable()) {

        const QCanBusFrame frame = device->readFrame();

        // Какой-то функционал

        const QString time = QString::fromLatin1("%1.%2  ")
                                 .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                                 .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        const int id = frame.frameId();
        const QString dlc = QString::number(frame.payload().size());

        qDebug() << "Frame recieved with id:" << id << "in time" << time ;

        // Вызов функции обработчика наследника

        // switch (id) {
        // case 1:{
        //     // reciveCommand(frame.payload(), commandIndex);

        //     qDebug() << "Recieved frame with id:" << id;

        //     // emit chainSendCommand(commandIndex);
        //     break;

        // }
        // default:
        //     break;
        // }


    }
}

bool CanInterface::SendFrame(const int addressTo, const int id, const QByteArray &data, bool setFD)
{
    // Добавление адреса
    int addresses = (devNumber | 0) << 4;
    addresses = (addressTo | addresses);

    QByteArray payload = data;
    payload.prepend(addresses);

    //Выравнивание до числа байтов кратного 4
    const int alignedSize = std::ceil(payload.size() / 4.0) * 4;
    if (payload.size()>8)
        payload.append(QByteArray(alignedSize - payload.size(), 0));

    // Создание кадра
    QCanBusFrame frame;
    frame.setFrameId(id);
    frame.setPayload(payload);

    frame.setFlexibleDataRateFormat(setFD);

    // std::string byteArray = payload.toStdString();
    // std::string bits;
    // for (int i = 0; i < byteArray.size(); ++i) {
    //     auto byte = static_cast<unsigned char>(byteArray[i]);
    //     bits += std::bitset<8>(byte).to_string(); // Преобразует байт в 8 бит
    // }
    // qDebug() << bits;

    // Отправка кадра
    if (device->writeFrame(frame)) {
        qDebug() <<"Send Frame" << "ID:" << id << "Data:" << data.toHex();
    } else {
        qDebug() << "Ошибка отправки:" << device->errorString();
    }
    return 1;
}

