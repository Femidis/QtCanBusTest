#include "customlogger.h"

QString CustomLogger::logFileName;
QMutex CustomLogger::mutex;

void CustomLogger::setLogFile(const QString& filename) {
    QMutexLocker locker(&mutex);
    logFileName = filename;
}

void CustomLogger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&mutex);
    if (logFileName.isEmpty()) return; // Если файл не задан

    QFile file(logFileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << formatMessage(type, msg) << Qt::endl;
    }
}

QString CustomLogger::formatMessage(QtMsgType type, const QString& msg) {
    QString level;
    switch (type) {
    case QtDebugMsg:    level = "DEBUG"; break;
    case QtInfoMsg:     level = "INFO"; break;
    case QtWarningMsg:  level = "WARNING"; break;
    case QtCriticalMsg: level = "CRITICAL"; break;
    case QtFatalMsg:    level = "FATAL"; break;
    }
    return QString("[%1] %2: %3")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(level)
        .arg(msg);
}


