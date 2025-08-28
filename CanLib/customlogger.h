#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>

class CustomLogger {
public:
    // Установка файла лога (вызывается из main)
    static void setLogFile(const QString& filename);

    // Статический обработчик сообщений (регистрируется через qInstallMessageHandler)
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QString formatMessage(QtMsgType type, const QString& msg);
    static QString logFileName;
    static QMutex mutex;
};

#endif // CUSTOMLOGGER_H
