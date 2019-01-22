/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <QApplication>
#include <iostream>
#include <sstream>
#include <string>
#include <QDomDocument>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <list>
#include "log.h"
#include "frameworkConfig.h"
#include "frameworkConfigImporter.h"
#include "xmlParser.h"
#include "globalDefinitions.h"

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

namespace SimulationMaster {

FrameworkConfig *FrameworkConfigImporter::Import(const std::string &filename)
{
    std::locale::global(std::locale("C"));

    QString absoluteFilePath = "";

    if (QFileInfo(QString::fromStdString(filename)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        absoluteFilePath = baseDir.absoluteFilePath(QString::fromStdString(filename));
        absoluteFilePath = baseDir.cleanPath(absoluteFilePath);
    }
    else
    {
        absoluteFilePath = QString::fromStdString(filename);
    }

    if (!QFileInfo(absoluteFilePath).exists())
    {
        LOG_INTERN(LogLevel::Error) << "Framework Config does not exist.";
        return nullptr;
    }

    std::stringstream ss;

    QFile xmlFile(absoluteFilePath); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during configuration import";
        return nullptr;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    QString errorMsg;
    int errorLine;
    if (!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        LOG_INTERN(LogLevel::Warning) << "invalid xml file format of file " << filename;
        LOG_INTERN(LogLevel::Warning) << "in line " << errorLine << " : " << errorMsg.toStdString();
        return nullptr;
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        return nullptr;
    }

    // parse log file Master
    std::string logFileMaster;
    if (!SimulationCommon::ParseString(documentRoot, "LogFileMaster", logFileMaster)
            || logFileMaster.empty())
    {
        LOG_INTERN(LogLevel::Error) << "Error: no LogFileMaster found in framework config file";
        return nullptr;
    }

    // parse log level
    int logLevel;
    if (!SimulationCommon::ParseInt(documentRoot, "LogLevel", logLevel))
    {
        LOG_INTERN(LogLevel::Warning) << "no log level specified";
        logLevel = static_cast<int>(LogLevel::Warning);
    }

    if (static_cast<int>(LogLevel::Count) <= logLevel)
    {
        LOG_INTERN(LogLevel::Warning) << "log level greater than upper limit " << (static_cast<int>
                                                                                   (LogLevel::Count) - 1);
        logLevel = static_cast<int>(LogLevel::Count) - 1;
    }

    if (0 > logLevel)
    {
        LOG_INTERN(LogLevel::Warning) << "log level below lower limit 0";
        logLevel = 0;
    }

    // parse slave path
    std::string slavePath;
    if (!SimulationCommon::ParseString(documentRoot, "SlavePath", slavePath) || slavePath.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/OpenPassSlave";
        slavePath = ss.str();
    }

    std::list<SlaveConfig> slaveConfigList;

    QDomElement slaveConfigElement;
    if (documentRoot.tagName() == "FrameworkConfiguration")
    {
        slaveConfigElement = documentRoot;
    }
    else
    {
        SimulationCommon::GetFirstChildElement(documentRoot, "frameworkConfiguration",
                                               slaveConfigElement);
    }

    while (!slaveConfigElement.isNull())
    {
        // parse library path
        std::string libraryPath;
        if (!SimulationCommon::ParseString(slaveConfigElement, "LibraryPath", libraryPath)
                || libraryPath.empty())
        {
            libraryPath = QCoreApplication::applicationDirPath().toStdString() + SUBDIR_LIB_SIM;
        }

        // parse result path
        std::string resultPath;
        if (!SimulationCommon::ParseString(slaveConfigElement, "ResultPath", resultPath)
                || resultPath.empty())
        {
            resultPath = QCoreApplication::applicationDirPath().toStdString();
        }

        // parse agent configuration file
        std::string agentConfigFile;
        if (!SimulationCommon::ParseString(slaveConfigElement, "AgentConfigFile", agentConfigFile)
                || agentConfigFile.empty())
        {
            LOG_INTERN(LogLevel::Error) << "Error: no system configuration found in framework config file";
            return nullptr;
        }

        // parse run configuration file
        std::string runConfigFile;
        if (!SimulationCommon::ParseString(slaveConfigElement, "RunConfigFile", runConfigFile)
                || runConfigFile.empty())
        {
            LOG_INTERN(LogLevel::Error) << "Error: no run configuration found in framework config file";
            return nullptr;
        }

        // parse scenery configuration file
        std::string sceneryConfigFile;
        if (!SimulationCommon::ParseString(slaveConfigElement, "SceneryConfigFile", sceneryConfigFile)
                || sceneryConfigFile.empty())
        {
            LOG_INTERN(LogLevel::Error) << "Error: no scenery configuration found in framework config file";
            return nullptr;
        }

        // parse openScenario configuration file
        std::string openScenarioConfigFile;
        if (!SimulationCommon::ParseString(slaveConfigElement, "ScenarioConfigFile",
                                           openScenarioConfigFile)
                || openScenarioConfigFile.empty())
        {
            LOG_INTERN(LogLevel::Error) << "Error: no scenario configuration found in framework config file";
            return nullptr;
        }

        // parse log file Slave
        std::string logFileSlave;
        if (!SimulationCommon::ParseString(slaveConfigElement, "LogFileSlave", logFileSlave)
                || logFileSlave.empty())
        {
            LOG_INTERN(LogLevel::Error) << "Error: no LogFileSlave found in framework config file";
            return nullptr;
        }

        slaveConfigList.push_back(SlaveConfig(libraryPath,
                                              resultPath,
                                              agentConfigFile,
                                              runConfigFile,
                                              sceneryConfigFile,
                                              openScenarioConfigFile,
                                              logFileSlave));

        slaveConfigElement = slaveConfigElement.nextSiblingElement("frameworkConfiguration");
    }

    FrameworkConfig *configuration = new (std::nothrow) FrameworkConfig(logFileMaster,
                                                                        logLevel,
                                                                        slavePath,
                                                                        slaveConfigList);

    if (!configuration)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during configuration import";
        return nullptr;
    }

    return configuration;
}

} // namespace SimulationMaster
