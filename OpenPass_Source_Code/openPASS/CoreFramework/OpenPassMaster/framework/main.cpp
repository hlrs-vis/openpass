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
#include <QDir>
#include <QDebug>
#include <QCommandLineParser>
#include <sstream>
#include "frameworkConfigImporter.h"
#include "frameworkConfig.h"
#include "log.h"

using namespace SimulationMaster;

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    QCoreApplication::setApplicationName("OpenPass Master");
    QCoreApplication::setApplicationVersion("1.0.0");

    QCommandLineParser cmdLineParser;
    QCommandLineOption frameworkConfigFilePath("frameworkConfigFile", "Path to frameworkConfig file", "directory");
    cmdLineParser.addOption(frameworkConfigFilePath);
    cmdLineParser.process(application);
    QString frameworkConfigFile = cmdLineParser.value(frameworkConfigFilePath);

    if(frameworkConfigFile == ""){
        frameworkConfigFile = "frameworkConfiguration.xml";
    }

    FrameworkConfig *frameworkConfig = FrameworkConfigImporter::Import(frameworkConfigFile.toStdString());
    if(!frameworkConfig)
    {
        LOG_INTERN(LogLevel::Error) << "could not import framework configuration";
        exit(EXIT_FAILURE);
    }

    // set logging file and level, log first config values
    LogOutputPolicy::SetFile(frameworkConfig->GetLogFileMaster().c_str());
    LogFile::ReportingLevel() = static_cast<LogLevel>(frameworkConfig->GetLogLevel());
    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### master start ##";
    LOG_INTERN(LogLevel::DebugCore) << "library path: " << frameworkConfig->GetLibraryPath();
    LOG_INTERN(LogLevel::DebugCore) << "agent configuration: " << frameworkConfig->GetAgentConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "observation result path: " << frameworkConfig->GetObservationResultPath();
    LOG_INTERN(LogLevel::DebugCore) << "run configuration: " << frameworkConfig->GetRunConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "scenery configuration: " << frameworkConfig->GetSceneryConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "log file Master: " << frameworkConfig->GetLogFileMaster();
    LOG_INTERN(LogLevel::DebugCore) << "log file Slave: " << frameworkConfig->GetLogFileSlave();
    LOG_INTERN(LogLevel::DebugCore) << "log level: " << std::to_string(frameworkConfig->GetLogLevel());
    LOG_INTERN(LogLevel::DebugCore) << "slave path: " << frameworkConfig->GetSlavePath();

    // create folders for files which are generated during runtime
    if(!QDir(QString::fromStdString(frameworkConfig->GetObservationResultPath())).exists())
    {
        LOG_INTERN(LogLevel::DebugCore) << "created folder";
        QDir().mkpath(QString::fromStdString(frameworkConfig->GetObservationResultPath()));
    }

    QString appPath =  QString::fromStdString(frameworkConfig->GetSlavePath());

    QStringList arguments;
    arguments << "--libraryPath" << frameworkConfig->GetLibraryPath().c_str()
              << "--agentConfiguration" << frameworkConfig->GetAgentConfigFile().c_str()
              << "--observationResultPath" << frameworkConfig->GetObservationResultPath().c_str()
              << "--runConfiguration" << frameworkConfig->GetRunConfigFile().c_str()
              << "--sceneryConfiguration" << frameworkConfig->GetSceneryConfigFile().c_str()
              << "--logFile" << frameworkConfig->GetLogFileSlave().c_str()
              << "--logLevel" << QString::number(frameworkConfig->GetLogLevel());

    delete frameworkConfig;
    frameworkConfig = nullptr;

    QProcess *slave = new QProcess();
    slave->start(appPath, arguments);

    if(slave->processId() == 0){
        LOG_INTERN(LogLevel::Error) << appPath.toStdString() << " not started, check path.";
        exit(EXIT_FAILURE);
    }

    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### slave start ##";

    slave->waitForFinished(-1); //default timeout 30s, -1 :infinity

    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### slave finished ##";
    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### master finished ##";

    int exitCode = slave->exitCode();
    delete slave;

    return exitCode;
}
