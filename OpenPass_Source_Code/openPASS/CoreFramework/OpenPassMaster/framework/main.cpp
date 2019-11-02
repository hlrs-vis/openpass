/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2016, 2017, 2018, 2019 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//! @file  main.cpp
//! @brief This file contains the main entry point.
//!
//! The main module handles the initialization and starts the slave.
//!
//! @mainpage openPASS
//!
//! @section intro_sec Introduction
//!
//! Master component@n
//! This program component represent the master node of the pre-crash
//! simulation tool.
//-----------------------------------------------------------------------------

#include <QCoreApplication>
#include <list>
#include <QProcess>
#include <QThreadPool>
#include <QtConcurrent>
#include <QDir>
#include <QDebug>
#include <QCommandLineParser>
#include <sstream>
#include <QLibrary>
#include <memory>
#include <string>
#include <utility>
#include "masterConfigImporter.h"
#include "masterConfig.h"
#include "log.h"
#include "processManager.h"

using namespace SimulationMaster;

using Arguments = std::list<std::pair<std::string, std::string>>;

//-----------------------------------------------------------------------------
//! \brief Parses command line arguments for the masterConfig file.
//! \param[in] arguments The list of command line arguments to parse.
//! \returns The supplied masterConfig file name.
//-----------------------------------------------------------------------------
static QString ParseArguments(const QStringList& arguments);

//-----------------------------------------------------------------------------
//! \brief Creates the directories necessary to make the resultPath valid, if
//! 	   they don't currently exist.
//! \param[in] resultPath A string representing the desired path to results.
//-----------------------------------------------------------------------------
static void CreateResultPathIfNecessary(const std::string& resultPath);

//-----------------------------------------------------------------------------
//! \brief Initializes application logging.
//! \param[in] logPath The path identifying where to write the logs.
//! \param[in] logLevel The log reporting level.
//! \returns a std::string representing the path to the log file
//-----------------------------------------------------------------------------
std::string InitLogging(const std::string& logPath, int logLevel);

//-----------------------------------------------------------------------------
//! \brief Parses the master configuration file for master configuration
//! 	   details.
//! 	   Throws if the configuration import fails.
//! \param[in] masterConfigFile The file contatining the configuration details.
//! \returns A Configuration::MasterConfig object containing the master
//! 	     configuration details.
//-----------------------------------------------------------------------------
Configuration::MasterConfig ParseMasterConfig(const QString& masterConfigFile);

#ifndef USESLAVELIBRARY
//-----------------------------------------------------------------------------
//! \brief Retrieve the name of the slaves executable
//-----------------------------------------------------------------------------
std::string GetExecutable(std::string slave);
#else
typedef int (*SlaveRunFunction)(int argc, char* argv[]);

//-----------------------------------------------------------------------------
//! \brief Retrieve the run function of the slave
//-----------------------------------------------------------------------------
SlaveRunFunction GetExecutable(std::string slave);
#endif // USESLAVELIBRARY

#ifndef OPENPASSMASTERLIBRARY
//-----------------------------------------------------------------------------
//! \brief Main entry point called by main
//! \param[in] argc		argument counter
//! \param[in] argv[]   argument vector
//-----------------------------------------------------------------------------
int run(int argc, char* argv[])
#else
//-----------------------------------------------------------------------------
//! \brief Main entry point, called by external program
//! \param[in] argc		argument counter
//! \param[in] argv[]   argument vector
//-----------------------------------------------------------------------------
extern "C" Q_DECL_EXPORT int run(int argc, char* argv[])
#endif //OPENPASSMASTERLIBRARY
{
    QCoreApplication app(argc, argv);
    QString masterConfigFile = ParseArguments(app.arguments());

    auto masterConfig = ParseMasterConfig(masterConfigFile);
    auto logFile = InitLogging(masterConfig.logFileMaster, masterConfig.logLevel);
    auto slave =  GetExecutable(masterConfig.slave);

    LOG_INTERN(LogLevel::DebugCore) << "\n\n## master start ##";
    LOG_INTERN(LogLevel::DebugCore) << "log level: " << masterConfig.logLevel;
    LOG_INTERN(LogLevel::DebugCore) << "log file master: " << logFile;
    LOG_INTERN(LogLevel::DebugCore) << "slave: " << slave;
    LOG_INTERN(LogLevel::DebugCore) << "libraries: " << masterConfig.libraries;
    LOG_INTERN(LogLevel::DebugCore) << "number of slaves: " << masterConfig.slaveConfigs.size();

    for (const auto& slaveConfig : masterConfig.slaveConfigs)
    {
        CreateResultPathIfNecessary(slaveConfig.results);

        Arguments arguments
        {
            { "--logLevel", std::to_string(masterConfig.logLevel) },
            { "--logFile",  slaveConfig.logFile },
            { "--lib",      masterConfig.libraries },
            { "--configs",  slaveConfig.configs },
            { "--results",  slaveConfig.results }
        };

        #ifndef USESLAVELIBRARY

        if (ProcessManager::getInstance().StartProcess(slave, arguments))
        {
            std::stringstream strStream;
            for (const auto& [command, value] : arguments)
            {
                strStream << command << " " << value << "\n";
            }
            LOG_INTERN(LogLevel::Info) << slave << " started with \n" << strStream.str() << std::endl;
        }
        else
        {
            LOG_INTERN(LogLevel::Error) << slave << " not started, check path.";
            exit(EXIT_FAILURE);
        }
    }
    ProcessManager::getInstance().WaitAndClear();

        #else
        QtConcurrent::run([arguments, &argv, &slave]
        {
            int i = 0;
            char* argumentsValue[arguments.size() + 1 ];
            argumentsValue[i++] = argv[0];

            for (QString value : arguments)
            {
                argumentsValue[i] = new char [value.toLocal8Bit().size()];
                strcpy(argumentsValue[i], value.toLocal8Bit().constData());
                i++;
            }
            slave(i, argumentsValue);
        });

    }
    QThreadPool::globalInstance()->waitForDone();
        #endif // USESLAVELIBRARY

    LOG_INTERN(LogLevel::DebugCore) << "## master finished ##";
    return 0;
}

std::string InitLogging(const std::string& logPath, int logLevel)
{
    QDir baseDir = QCoreApplication::applicationDirPath();
    QString logFile = baseDir.absoluteFilePath(QString::fromStdString(logPath));
    logFile = baseDir.cleanPath(logFile);

    LogOutputPolicy::SetFile(logFile.toStdString());
    LogFile::ReportingLevel() = static_cast<LogLevel>(logLevel);
    return logFile.toStdString();
}

QString ParseArguments(const QStringList& arguments)
{
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();

    QCommandLineOption optionMasterConfigFile(
        QStringList() << "c" << "config",
        "configuration <masterConfig.xml>",
        "masterConfig",
        QCoreApplication::applicationDirPath() + "/masterConfig.xml");

    commandLineParser.addOption(optionMasterConfigFile);
    commandLineParser.process(arguments);

    return commandLineParser.value(optionMasterConfigFile);
}

void CreateResultPathIfNecessary(const std::string& resultPath)
{
    auto qResultPath = QString::fromStdString(resultPath);
    if (!QDir(qResultPath).exists())
    {
        LOG_INTERN(LogLevel::DebugCore) << "created result folder " << resultPath;
        QDir().mkpath(qResultPath);
    }
}

Configuration::MasterConfig ParseMasterConfig(const QString& masterConfigFile)
{
    try
    {
        return Configuration::MasterConfigImporter::Import(masterConfigFile);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Configuration related error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

#ifndef USESLAVELIBRARY
std::string GetExecutable(std::string slave)
{
    #ifndef USESLAVELIBRARY

    #if WIN32
    if (QString::fromStdString(slave).split(".").last() != "exe")
    {
        slave += ".exe";
    }
    #endif

    #endif // USESLAVELIBRARY
    return slave;
}
#else
SlaveRunFunction GetExecutable(std::string slave)
{
    QLibrary slaveLib(QString::fromStdString(slave));
    if (!slaveLib.load())
    {
        LOG_INTERN(LogLevel::Error) << " failed to load library " << slave << " " << slaveLib.errorString().toStdString();
        exit(EXIT_FAILURE);
    }

    SlaveRunFunction SlaveRunFunc = (SlaveRunFunction)slaveLib.resolve("run");

    if (!SlaveRunFunc)
    {
        LOG_INTERN(LogLevel::Error) << " unable to resolve run function in library " << slave;
        exit(EXIT_FAILURE);
    }

    return SlaveRunFunc;
}
#endif // USESLAVELIBRARY

#ifndef OPENPASSMASTERLIBRARY
int main(int argc, char* argv[])
{
    return run(argc, argv);
}
#endif //OPENPASSMASTERLIBRARY
