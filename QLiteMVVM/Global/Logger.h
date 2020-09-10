#ifndef LOGGER_H_
#define LOGGER_H_

#include <QCoreApplication>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QBuffer>
#include <QDateTime>

#include <QDebug>


//const int LOGGER_BUFF_1KB = 1024;
const int LOGGER_BUFF_1MB = 1024 * 1024;

typedef struct _LogFileParam {
private:
    QString fileName;
    QString fileExt;
    QString dir;
    int nSplit;
public:
    _LogFileParam(QString fileName = QString("log"),
                  QString fileExt = QString("txt"),
                  QString dir = QString("%1/%2").arg(QDir::currentPath()).arg("log"),
                  int nSplitSize = LOGGER_BUFF_1MB * 10)
    {
        this->fileName = fileName;
        this->dir = dir;
        this->nSplit = nSplitSize;
        this->fileExt = fileExt;
        QDir d(dir);
        if (!d.exists()) d.mkdir(dir);
    }
protected:
    friend class Logger;
} LogFileParam;

enum Level {
    LOGGERDEBUG,
    LOGGERINFO,
    LOGGERWARN,
    LOGGERERROR,
    LOGGERNO
};

class Logger : public QObject
{
    Q_OBJECT
public:


public:
    static Logger* Install();
    static void SetLogLevel(Level level);
    static Level LogLevel();
    static void LoggerMessageHandler(QtMsgType type, QMessageLogContext const& context, QString const& msg);
    static int SplitFileName(const char* pData);
    ~Logger() override;
    inline Level level() const
    {
        return m_eLogLevel;
    }
    void setLogLevel(Level level)
    {
        m_eLogLevel = level;
    }

public:

signals:
    void LoggerMessage(QString const& msg);

private:
    Logger();
    void initByteBuff(int nSize);
    void creatLogFile();
    void write(char* p, int nSize);
    bool isBuffFull();
    bool isWrite();
    void flushBuffer();
    void info(QByteArray& log);
    void warn(QByteArray& warn);
    void error(QByteArray& error);
    void debug(QByteArray& debug);
    void insertDate();

private:
    QCoreApplication* m_pAppInstance;
    static Logger* m_pLoggerInstance;
    QVector<QByteArray> m_vLogByteBuf;
    bool m_bIsOdd;
    QFile* m_pFile;
    LogFileParam m_paramLogFile;
    qint64 m_nLogStarted;
//    QBuffer m_buffer;
    Level m_eLogLevel;
    int m_nBuffSize;
    //    QVector<QFile *> m_pFiles;
};


#endif // LOGGER_H
