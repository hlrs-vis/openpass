/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  log.h
//! @brief This file contains the implementation of the debug logging.
//-----------------------------------------------------------------------------

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <QMutex>
#include <QThread>
#include <QMap>

#if defined(LOG_TIME_ENABLED)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>

inline std::string Log_NowTime()
{
    const int MAX_LEN = 200;
    char buffer[MAX_LEN];
    if (0 == GetTimeFormatA(LOCALE_USER_DEFAULT,
                            0,
                            0,
                            "HH':'mm':'ss",
                            buffer,
                            MAX_LEN))
    {
        return "Error in Log_NowTime()";
    }

    char result[100] = { 0 };
    static DWORD first = GetTickCount();
    std::sprintf(result,
                 "%s.%03ld",
                 buffer,
                 (long)(GetTickCount() - first) % 1000);
    return result;
}

#else // WIN32
#include <sys/time.h>

inline std::string Log_NowTime()
{
    char buffer[11];
    time_t t;
    time(&t);
    tm r;
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    char result[100] = { 0 };
    std::sprintf(result,
                 "%s.%03ld",
                 buffer,
                 (long)tv.tv_usec / 1000);
    return result;
}

#endif // WIN32
#endif // LOG_TIME_ENABLED

//! Severity of log
enum class LogLevel : int
{
    Error = 0, //!< Indicates abortion of program flow
    Warning, //!< Indicates change of intended program flow (doesn't necessarily lead to an abortion)
    Info, //!< Used for diagnostic information (must not be used for periodic events to prevent overflow of log)
    DebugUser, //!< Used for debugging information on model level (might be used for periodic events)
    DebugAPI, //!< Used for debugging information on framework level (functionality related to model interface)
    DebugCore, //!< Used for debugging information on framework level (internal functionality)
    Count
};

//! Coordinates logging
template<typename T>
class Log
{
public:
    Log() = default;
    Log(const Log &) = delete;
    Log(Log &&) = delete;
    Log &operator=(const Log &) = delete;
    Log &operator=(Log &&) = delete;
    virtual ~Log();

    //-----------------------------------------------------------------------------
    //! Retrieves output stream.
    //!
    //! Additional diagnostic information might be logged dependent on severity of
    //! log.
    //!
    //! @param[in]     file      File name where log is issued
    //! @param[in]     line      Line number where log is issued
    //! @param[in]     level     Severity of log.
    //! @return                  Output stream for the log message
    //-----------------------------------------------------------------------------
    std::ostringstream &Get(const char *file,
                            int line,
                            LogLevel level = LogLevel::Warning);

    //-----------------------------------------------------------------------------
    //! Retrieves current severity limit.
    //!
    //! Only log messages with a severity smaller or equal than this level will be
    //! printed.
    //!
    //! @return                  Current severity limit
    //-----------------------------------------------------------------------------
    static LogLevel &ReportingLevel();

    //-----------------------------------------------------------------------------
    //! Converts severity value to a string for printing purposes.
    //!
    //! @param[in]     level     Severity level
    //! @return                  Converted severity level
    //-----------------------------------------------------------------------------
    static std::string ToString(LogLevel level);

protected:
    std::ostringstream os; //!< output stream of current log session

private:
    static QMutex logGuard; //!< protects output stream from concurrent access
};

template<typename T> QMutex Log<T>::logGuard;

template<typename T>
std::ostringstream &Log<T>::Get(const char *file, int line, LogLevel level)
{
#if defined(LOG_TIME_ENABLED)
    os << Log_NowTime();
#endif // LOG_TIME_ENABLED
    os << " " << ToString(level) << ": ";

    if (static_cast<int>(LogLevel::Info) > static_cast<int>(level))
    {
        os << "(" << file << ":" << line << ") ";
    }

    os << "ThreadID: " << QThread::currentThreadId() << " ";

    return os;
}

template <typename T>
Log<T>::~Log()
{
//    logGuard.lock();
    os << std::endl;
    T::Output(os.str());
//    logGuard.unlock();
}

template <typename T>
LogLevel &Log<T>::ReportingLevel()
{
    static LogLevel reportingLevel = LogLevel::Warning;
    return reportingLevel;
}

template <typename T>
std::string Log<T>::ToString(LogLevel level)
{
    static const char *const buffer[] =
    {
        "Error",
        "Warning",
        "Info",
        "DebugUser",
        "DebugApi",
        "DebugCore"
    };

    return buffer[static_cast<int>(level)];
}

//! Handles access of file
class LogOutputPolicy
{
public:
    //-----------------------------------------------------------------------------
    //! Initializes output file.
    //!
    //! @param[in]     fileName      Name of file where logs are stored
    //-----------------------------------------------------------------------------
    static void SetFile(const std::string &fileName);

    //-----------------------------------------------------------------------------
    //! Verifies if output file has already been opened.
    //!
    //! @return      True if file is open
    //-----------------------------------------------------------------------------
    static bool IsOpen();

    //-----------------------------------------------------------------------------
    //! Logs message into file and standard output.
    //!
    //! @param[in]     message      Message to be logged.
    //-----------------------------------------------------------------------------
    static void Output(const std::string &message);

private:
    static QMap<int, std::ofstream *> logStreamMap; //!< output file stream
};

inline void LogOutputPolicy::SetFile(const std::string &fileName)
{
    long long threadId = (long long)QThread::currentThreadId();
    std::ofstream *logStream = new std::ofstream();
    logStream->open(fileName);
    logStreamMap.insert(threadId, logStream);
}

inline bool LogOutputPolicy::IsOpen()
{
    long long threadId = (long long)QThread::currentThreadId();
    if (logStreamMap.contains(threadId))
    {
        return logStreamMap[threadId]->is_open();
    }
    else
    {
        return false;
    }
}

inline void LogOutputPolicy::Output(const std::string &message)
{
    // print to standard output
//    std::cout << message.c_str();

    // print to file
    long long threadId = (long long)QThread::currentThreadId();
    if (logStreamMap.contains(threadId))
    {
        *logStreamMap[threadId] << message;
        logStreamMap[threadId]->flush();
    }
}

//! Bind logging mechanism to file
typedef Log<LogOutputPolicy> LogFile;

//! Macro used for printing log of framework external components
#define LOG_EXTERN(level, file, line) \
    if(LogLevel::Count <= level) ; \
    else if(static_cast<int>(level) > static_cast<int>(LogFile::ReportingLevel()) || !LogOutputPolicy::IsOpen()) ; \
    else LogFile().Get(file, line, level)

//! Macro used for printing log of framework internal components
#define LOG_INTERN(level) \
    if(LogLevel::Count <= level) ; \
    else if(static_cast<int>(level) > static_cast<int>(LogFile::ReportingLevel()) || !LogOutputPolicy::IsOpen()) ; \
    else LogFile().Get(__FILE__, __LINE__, level)

#endif // LOG_H
