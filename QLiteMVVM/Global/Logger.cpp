#include "Logger.h"
#include <stdexcept>

Logger* Logger::m_pLoggerInstance = nullptr;

Logger* Logger::GetInstance()
{
    if (m_pLoggerInstance == nullptr) {
        m_pLoggerInstance = new Logger();
        m_pLoggerInstance->m_nLogStarted = QDateTime::currentMSecsSinceEpoch();
        qInstallMessageHandler(Logger::LoggerMessageHandler);
        qInfo() << "---------------------------------------";
        qInfo() << "The Logger System is inilized at " << m_pLoggerInstance->m_nLogStarted;
        qInfo() << "---------------------------------------";
    }
    return m_pLoggerInstance;
}

void Logger::SetLogLevel(Level level)
{
    if ( m_pLoggerInstance == nullptr )  return;
    m_pLoggerInstance->setLogLevel(level);
}

Level Logger::LogLevel()
{
    if ( m_pLoggerInstance == nullptr )  return LOGGERNO;
    return m_pLoggerInstance->level();
}

void Logger::LoggerMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char* pFile = context.file ? context.file : "";
//    const char * pFunc = context.function ? context.function : "";
    int idx = Logger::SplitFileName(pFile);
    int nRet = 0;
    QByteArray byte;
    switch (type) {
    case QtDebugMsg:
        nRet = fprintf(stdout, "Debug (%s:%u): %s\n",
                       &pFile[idx], context.line, localMsg.constData());
        byte.resize(nRet);
        sprintf(byte.data(), "Debug (%s:%u): %s\n",
                &pFile[idx], context.line, localMsg.constData());
        m_pLoggerInstance->debug(byte);
        break;
    case QtInfoMsg:
        nRet = fprintf(stdout, "Info (%s:%u): %s\n",
                       &pFile[idx], context.line, localMsg.constData());
        byte.resize(nRet);
        sprintf(byte.data(), "Info (%s:%u): %s\n",
                &pFile[idx], context.line, localMsg.constData());
        m_pLoggerInstance->info(byte);
        break;
    case QtWarningMsg:
        nRet = fprintf(stdout, "Warn (%s:%u): %s\n",
                       &pFile[idx], context.line, localMsg.constData());
        byte.resize(nRet);
        sprintf(byte.data(), "Warn (%s:%u): %s\n",
                &pFile[idx], context.line, localMsg.constData());
        m_pLoggerInstance->warn(byte);
        break;
    case QtCriticalMsg:
        nRet = fprintf(stderr, "Critical (%s:%u): %s\n",
                       &pFile[idx], context.line, localMsg.constData());
        byte.resize(nRet);
        sprintf(byte.data(), "Critical (%s:%u): %s\n",
                &pFile[idx], context.line, localMsg.constData());
        m_pLoggerInstance->error(byte);
        break;
    case QtFatalMsg:
        nRet = fprintf(stderr, "Fatal (%s:%u): %s\n",
                       &pFile[idx], context.line, localMsg.constData());
        byte.resize(nRet);
        sprintf(byte.data(), "Fatal (%s:%u): %s\n",
                &pFile[idx], context.line, localMsg.constData());
        m_pLoggerInstance->write(byte.data(), byte.length());
        break;
    }

    fflush(stdout);
    fflush(stderr);
}

int Logger::SplitFileName(const char* pData)
{
    int i = 0;
    int j = 0;
    while ( pData[i] != '\0') {
        if (pData[i] == '\\')    j = i + 1;
        i++;
    }
    return j;
}

Logger::Logger() : QObject (QCoreApplication::instance()),
    m_pAppInstance(QCoreApplication::instance()),
    m_bIsOdd(false),
    m_eLogLevel(LOGGERINFO)
{
    initByteBuff(LOGGER_BUFF_1MB);
    creatLogFile();
}

void Logger::initByteBuff(int nSize)
{
    m_vLogByteBuf.resize(2);
    m_nBuffSize = nSize;
    for (int i = 0; i < m_vLogByteBuf.size(); i++) {
        m_vLogByteBuf[i].reserve(nSize);
    }
}

void Logger::creatLogFile()
{
    QDir dir(m_paramLogFile.dir);
    //For Numbering the log_file;
//    dir.setFilter(QDir::Files);
//    dir.setSorting(QDir::Name);
//    dir.setNameFilters(QStringList() << "log*.txt");
////    dir.setNameFilters(QDir);
//    QStringList strList = dir.entryList();//(QStringList(QString("%1*.txt").arg(m_paramLogFile.fileName)));
////    qDebug() << strList;
//    m_pFile = new QFile(QString("%1/%2").arg(m_paramLogFile.dir).arg(m_paramLogFile.fileName));
    //Create a file every day;
    QDate date = QDate::currentDate();
    QString strDate = QString("%1_%2_%3").arg(date.year()).arg(date.month()).arg(date.day());
    m_pFile = new QFile(QString("%1/%2_%3.%4")
                        .arg(m_paramLogFile.dir)
                        .arg(m_paramLogFile.fileName)
                        .arg(strDate)
                        .arg(m_paramLogFile.fileExt), this);
    if (!m_pFile->open(QFile::Text | QFile::Append))   throw std::runtime_error("Create Log File error.");
}

void Logger::write(char* p, int nSize)
{
//    m_buffer.write(p, nSize);
    m_vLogByteBuf[m_bIsOdd].append(p, nSize);
//    m_pFile->write(p, nSize);
    isWrite();
}

bool Logger::isWrite()
{
    if (m_vLogByteBuf[m_bIsOdd].size() >= m_nBuffSize) {
        m_pFile->write(m_vLogByteBuf[m_bIsOdd]);
        m_pFile->flush();
        m_vLogByteBuf[m_bIsOdd].clear();
        m_vLogByteBuf[m_bIsOdd].reserve(m_nBuffSize);
        m_bIsOdd = !m_bIsOdd;
        return true;
    }
    return false;
}

void Logger::flushBuffer()
{
    if ( m_vLogByteBuf[m_bIsOdd].size() > 0) {
        m_pFile->write(m_vLogByteBuf[m_bIsOdd]);
        m_pFile->flush();
        m_vLogByteBuf[m_bIsOdd].clear();
        m_vLogByteBuf[m_bIsOdd].reserve(m_nBuffSize);
    }

    if ( m_vLogByteBuf[!m_bIsOdd].size() > 0) {
        m_pFile->write(m_vLogByteBuf[!m_bIsOdd]);
        m_pFile->flush();
        m_vLogByteBuf[!m_bIsOdd].clear();
        m_vLogByteBuf[!m_bIsOdd].reserve(m_nBuffSize);
    }
}

Logger::~Logger()
{
    qInfo() << "---------------------------------------";
    qInfo() << "The Logger System is terminated in " << QDateTime::currentMSecsSinceEpoch() - m_nLogStarted;
    qInfo() << "---------------------------------------";
    flushBuffer();
    if (m_pFile->isOpen())   m_pFile->close();
}

void Logger::info(QByteArray& log)
{
    if (m_eLogLevel > LOGGERINFO) return;
    insertDate();
    write(log.data(), log.size());
    emit(LoggerMessage(log));
}

void Logger::warn(QByteArray& warn)
{
    if (m_eLogLevel > LOGGERWARN) return;
    insertDate();
    write(warn.data(), warn.size());
    emit(LoggerMessage(warn));
}

void Logger::error(QByteArray& error)
{
    insertDate();
    write(error.data(), error.size());
    emit(LoggerMessage(error));
}

void Logger::debug(QByteArray& debug)
{
    if (m_eLogLevel > LOGGERDEBUG) return;
    insertDate();
    write(debug.data(), debug.size());
    emit(LoggerMessage(debug));
}

void Logger::insertDate()
{
    double dTm = QDateTime::currentMSecsSinceEpoch() - m_nLogStarted;
    QString tm = QString("[%1] ").arg(QString::number(dTm / 1000));
    write(tm.toLocal8Bit().data(), tm.length());
}
