/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
#include "frameworkConfigImporter.h"
#include "frameworkConfig.h"
#include "log.h"
#include "processManager.h"

using namespace SimulationMaster;

#ifndef OPENPASSMASTERLIBRARY
int run(int argc, char *argv[])
#else
extern "C" Q_DECL_EXPORT int run(int argc, char *argv[])
#endif //OPENPASSMASTERLIBRARY
{
    QString frameworkConfigFile = QCoreApplication::applicationDirPath() +
                                  "/frameworkConfiguration.xml";
    for (int i = 1; i < argc; i++)
    {
        QString arg = argv[i];
        if (arg == "--frameworkConfigFile")
        {
            if (++i < argc)
            {
                frameworkConfigFile = argv[i];
            }
        }
    }

    FrameworkConfig *frameworkConfig = FrameworkConfigImporter::Import(
                                           frameworkConfigFile.toStdString());

    if (!frameworkConfig)
    {
        exit(EXIT_FAILURE);
    }

    QString slavePath =  QString::fromStdString(frameworkConfig->GetSlavePath());

    QDir baseDir = QCoreApplication::applicationDirPath();
    QString logFile = baseDir.absoluteFilePath(QString::fromStdString(
                                                   frameworkConfig->GetLogFileMaster()));
    logFile = baseDir.cleanPath(logFile);

    LogOutputPolicy::SetFile(logFile.toStdString());
    LogFile::ReportingLevel() = static_cast<LogLevel>(frameworkConfig->GetLogLevel());

    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### master start ##";
    LOG_INTERN(LogLevel::DebugCore) << "log file Master: " << frameworkConfig->GetLogFileMaster();
    LOG_INTERN(LogLevel::DebugCore) << "log level: " << std::to_string(frameworkConfig->GetLogLevel());
    LOG_INTERN(LogLevel::DebugCore) << "slave path: " << slavePath.toStdString();
    LOG_INTERN(LogLevel::DebugCore) << "number of slaves: " <<
                                    frameworkConfig->GetSlaveConfigList().size();

#ifdef USESLAVELIBRARY
    QLibrary slaveLib(slavePath + ".dll");
    if (!slaveLib.load())
    {
        LOG_INTERN(LogLevel::Error) << " OpenPassSlave.dll could not be loaded: " <<
                                    slaveLib.errorString().toStdString();
        exit(EXIT_FAILURE);
    }

    typedef int (*SlaveRunFunction)(int argc, char *argv[]);
    SlaveRunFunction SlaveRunFunc = (SlaveRunFunction)slaveLib.resolve("run");

    if (SlaveRunFunc == nullptr)
    {
        LOG_INTERN(LogLevel::Error) << " OpenPassSlave.dll run function could not be resolved ";
        exit(EXIT_FAILURE);
    }
#endif // USESLAVELIBRARY

    foreach (SlaveConfig slaveConfig, frameworkConfig->GetSlaveConfigList())
    {
        LOG_INTERN(LogLevel::DebugCore) << "library path: " << slaveConfig.GetLibraryPath();
        LOG_INTERN(LogLevel::DebugCore) << "agent configuration: " << slaveConfig.GetAgentConfigFile();
        LOG_INTERN(LogLevel::DebugCore) << "observation result path: " <<
                                        slaveConfig.GetObservationResultPath();
        LOG_INTERN(LogLevel::DebugCore) << "run configuration: " << slaveConfig.GetRunConfigFile();
        LOG_INTERN(LogLevel::DebugCore) << "scenery configuration: " <<
                                        slaveConfig.GetSceneryConfigFile();
        LOG_INTERN(LogLevel::DebugCore) << "openScenario configuration: " <<
                                        slaveConfig.GetOpenScenarioConfigFile();
        LOG_INTERN(LogLevel::DebugCore) << "log file Slave: " << slaveConfig.GetLogFileSlave();

        // create folders for files which are generated during runtime
        if (!QDir(QString::fromStdString(slaveConfig.GetObservationResultPath())).exists())
        {
            LOG_INTERN(LogLevel::DebugCore) << "created folder";
            QDir().mkpath(QString::fromStdString(slaveConfig.GetObservationResultPath()));
        }

        QStringList arguments;
        arguments << "--libraryPath" << slaveConfig.GetLibraryPath().c_str()
                  << "--agentConfiguration" << slaveConfig.GetAgentConfigFile().c_str()
                  << "--observationResultPath" << slaveConfig.GetObservationResultPath().c_str()
                  << "--runConfiguration" << slaveConfig.GetRunConfigFile().c_str()
                  << "--sceneryConfiguration" << slaveConfig.GetSceneryConfigFile().c_str()
                  << "--openScenarioConfiguration" << slaveConfig.GetOpenScenarioConfigFile().c_str()
                  << "--logFile" << slaveConfig.GetLogFileSlave().c_str()
                  << "--logLevel" << QString::number(frameworkConfig->GetLogLevel());

#ifndef USESLAVELIBRARY

        if (slavePath.split(".").last() != "exe")
        {
            slavePath += ".exe";
        }

        if (!ProcessManager::getInstance().StartProcess(slavePath, arguments))
        {
            LOG_INTERN(LogLevel::Error) << slavePath.toStdString() << " not started, check path.";
            exit(EXIT_FAILURE);
        }

    }
    ProcessManager::getInstance().WaitAndClear();

#else
        QtConcurrent::run([arguments, &argv, &SlaveRunFunc]
        {
            int i = 0;
            char *argumentsValue[arguments.size() + 1 ];
            argumentsValue[i++] = argv[0];

            for (QString value : arguments)
            {
                argumentsValue[i] = new char [value.toLocal8Bit().size()];
                strcpy(argumentsValue[i], value.toLocal8Bit().constData());
                i++;
            }
            SlaveRunFunc(i, argumentsValue);
        });

    }
    QThreadPool::globalInstance()->waitForDone();

#endif // USESLAVELIBRARY

    delete frameworkConfig;
    frameworkConfig = nullptr;

    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### master finished ##";

    return 0;
}

#ifndef OPENPASSMASTERLIBRARY
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    return run(argc, argv);
}
#endif //OPENPASSMASTERLIBRARY
