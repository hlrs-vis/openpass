/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <QApplication>
#include <iostream>
#include <sstream>
#include <string>
#include <QDomDocument>
#include <QFile>
#include <list>
#include "log.h"
#include "frameworkConfig.h"
#include "frameworkConfigImporter.h"
#include "xmlParser.h"

// local helper macros
#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
{ \
    LOG_INTERN(LogLevel::Error) << "an error occurred during configuration import"; \
    return nullptr; \
    } \
    } \
    while(0);

namespace SimulationMaster
{

FrameworkConfig *FrameworkConfigImporter::Import(const std::string &filename)
{
    std::locale::global(std::locale("C"));

    std::stringstream ss;

    QFile xmlFile(filename.c_str()); // automatic object will be closed on destruction
    if(!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during configuration import";
        return nullptr;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    QString errorMsg;
    int errorLine;
    if(!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml file format of file " << filename;
        LOG_INTERN(LogLevel::Warning) << "in line " << errorLine << " : " << errorMsg.toStdString();
        return nullptr;
    }

    QDomElement documentRoot = document.documentElement();
    if(documentRoot.isNull())
    {
        return nullptr;
    }

    // parse library path
    std::string libraryPath;
    if(!SimulationCommon::ParseString(documentRoot, "LibraryPath", libraryPath) || libraryPath.empty())
    {
        libraryPath = QCoreApplication::applicationDirPath().toStdString();
    }

    // parse result path
    std::string resultPath;
    if(!SimulationCommon::ParseString(documentRoot, "ResultPath", resultPath) || resultPath.empty())
    {
        resultPath = QCoreApplication::applicationDirPath().toStdString();
    }

    // parse agent configuration file
    std::string agentConfigFile;
    if(!SimulationCommon::ParseString(documentRoot, "AgentConfigFile", agentConfigFile) || agentConfigFile.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/agentConfiguration.xml";
        agentConfigFile = ss.str();
    }

    // parse run configuration file
    std::string runConfigFile;
    if(!SimulationCommon::ParseString(documentRoot, "RunConfigFile", runConfigFile) || runConfigFile.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/runConfiguration.xml";
        runConfigFile = ss.str();
    }

    // parse scenery configuration file
    std::string sceneryConfigFile;
    if(!SimulationCommon::ParseString(documentRoot, "SceneryConfigFile", sceneryConfigFile) || sceneryConfigFile.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/sceneryConfiguration.xml";
        sceneryConfigFile = ss.str();
    }

    // parse log file Master
    std::string logFileMaster;
    if(!SimulationCommon::ParseString(documentRoot, "LogFileMaster", logFileMaster) || logFileMaster.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/OpenPassMaster.log";
        logFileMaster = ss.str();
    }

    // parse log file Slave
    std::string logFileSlave;
    if(!SimulationCommon::ParseString(documentRoot, "LogFileSlave", logFileSlave) || logFileSlave.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/OpenPassSlave.log";
        logFileSlave = ss.str();
    }

    // parse log level
    int logLevel;
    if(!SimulationCommon::ParseInt(documentRoot, "LogLevel", logLevel))
    {
        LOG_INTERN(LogLevel::Warning) << "no log level specified";
        logLevel = static_cast<int>(LogLevel::Warning);
    }

    if(static_cast<int>(LogLevel::Count) <= logLevel)
    {
        LOG_INTERN(LogLevel::Warning) << "log level greater than upper limit " << (static_cast<int>(LogLevel::Count) - 1);
        logLevel = static_cast<int>(LogLevel::Count) - 1;
    }

    if(0 > logLevel)
    {
        LOG_INTERN(LogLevel::Warning) << "log level below lower limit 0";
        logLevel = 0;
    }

    // parse slave path
    std::string slavePath;
    if(!SimulationCommon::ParseString(documentRoot, "SlavePath", slavePath) || slavePath.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "OpenPassSlave.exe";
        slavePath = ss.str();
    }


    FrameworkConfig *configuration = new (std::nothrow) FrameworkConfig(libraryPath,
                                                                        resultPath,
                                                                        agentConfigFile,
                                                                        runConfigFile,
                                                                        sceneryConfigFile,
                                                                        logFileMaster,
                                                                        logFileSlave,
                                                                        logLevel,
                                                                        slavePath);

    if(!configuration)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during configuration import";
        return nullptr;
    }

    return configuration;
}

} // namespace SimulationMaster
