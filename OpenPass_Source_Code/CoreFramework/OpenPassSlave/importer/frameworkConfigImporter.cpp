/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <QApplication>
#include <QCommandLineParser>
#include <iostream>
#include <sstream>
#include <QDomDocument>
#include <QFile>
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

namespace SimulationSlave
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

    // parse scenery configuration file
    std::string sceneryConfigFile;
    if(!SimulationCommon::ParseString(documentRoot, "SceneryConfigFile", sceneryConfigFile) || sceneryConfigFile.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/sceneryConfiguration.xml";
        sceneryConfigFile = ss.str();
    }

    // parse run configuration file
    std::string runConfigFile;
    if(!SimulationCommon::ParseString(documentRoot, "RunConfigFile", runConfigFile) || runConfigFile.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/runConfiguration.xml";
        runConfigFile = ss.str();
    }

    // parse log file
    std::string logFile;
    if(!SimulationCommon::ParseString(documentRoot, "LogFile", logFile) || logFile.empty())
    {
        ss.str(std::string());
        ss << QCoreApplication::applicationDirPath().toStdString() << "/logSlave.txt";
        logFile = ss.str();
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

    FrameworkConfig *configuration = new (std::nothrow) FrameworkConfig(libraryPath,
                                                                        resultPath,
                                                                        agentConfigFile,
                                                                        sceneryConfigFile,
                                                                        runConfigFile,
                                                                        logFile,
                                                                        logLevel);

    if(!configuration)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during configuration import";
        return nullptr;
    }

    return configuration;
}

FrameworkConfig *FrameworkConfigImporter::Import(QCoreApplication &application)
{
    //check if any arguments are given to application (one argument is always the
    //application path)
    //if no arguements are given, then try getting the arguments by using default
    //frameworkConfigFile
    //if there is no frameworkConfigFile then use default arguments
    if(application.arguments().count() < 2){
        QString frameworkConfigFile = QCoreApplication::applicationDirPath() + "/frameworkConfiguration.xml";
        QFile frameworkConfigurationFile(frameworkConfigFile);
        if(frameworkConfigurationFile.exists()){
            return FrameworkConfigImporter::Import(frameworkConfigFile.toStdString());
        }
    }

    QCommandLineParser cmdLineParser;

    QCommandLineOption libPath("libraryPath", "path of libraries", "directory");
    cmdLineParser.addOption(libPath);

    QCommandLineOption observationResultPath("observationResultPath", "path to observationResult", "directory");
    cmdLineParser.addOption(observationResultPath);

    QCommandLineOption agentConfiguration("agentConfiguration", "path to agentConfiguration", "directory");
    cmdLineParser.addOption(agentConfiguration);

    QCommandLineOption sceneryConfiguration("sceneryConfiguration", "path to sceneryConfiguration", "directory");
    cmdLineParser.addOption(sceneryConfiguration);

    QCommandLineOption runConfiguration("runConfiguration", "path to runConfiguration", "directory");
    cmdLineParser.addOption(runConfiguration);

    QCommandLineOption logFileCommand("logFile", "path to logFile", "directory");
    cmdLineParser.addOption(logFileCommand);

    QCommandLineOption logLevelCommand("logLevel", "Log Level", "logLevel");
    cmdLineParser.addOption(logLevelCommand);

    cmdLineParser.process(application);

    QString libraryPath = cmdLineParser.value(libPath);
    if(libraryPath == ""){
        libraryPath = QCoreApplication::applicationDirPath();
    }

    QString resultPath = cmdLineParser.value(observationResultPath);
    if(resultPath == ""){
        resultPath = QCoreApplication::applicationDirPath();
    }

    QString agentConfigFile = cmdLineParser.value(agentConfiguration);
    if(agentConfigFile == ""){
        agentConfigFile = QCoreApplication::applicationDirPath() + "/agentConfiguration.xml";
    }

    QString sceneryConfigFile = cmdLineParser.value(sceneryConfiguration);
    if(sceneryConfigFile == ""){
        sceneryConfigFile = QCoreApplication::applicationDirPath() + "/sceneryConfiguration.xml";
    }

    QString runConfigFile = cmdLineParser.value(runConfiguration);
    if(runConfigFile == ""){
        runConfigFile = QCoreApplication::applicationDirPath() + "/runConfiguration.xml";
    }

    QString logFile = cmdLineParser.value(logFileCommand);
    if((logFile == "") || (logFile == "working_directory")){
        logFile = QCoreApplication::applicationDirPath() + "/logSlave.txt";
    }

    QString logLevelString = cmdLineParser.value(logLevelCommand);
    int logLevel;
    if(logLevelString == ""){
        logLevel = static_cast<int> (LogLevel::Error);
    }else{
        logLevel = logLevelString.toInt();
        if(logLevel < 0){
            LOG_INTERN(LogLevel::Warning) << "log level below lower limit 0";
            logLevel = 0;
        }else {
            if(logLevel >= static_cast<int>(LogLevel::Count)){
                LOG_INTERN(LogLevel::Warning) << "log level greater than upper limit " << (static_cast<int>(LogLevel::Count) - 1);
                logLevel = static_cast<int>(LogLevel::Count) - 1;
            }
        }
    }

    FrameworkConfig *configuration = new (std::nothrow) FrameworkConfig(libraryPath.toStdString(),
                                                                        resultPath.toStdString(),
                                                                        agentConfigFile.toStdString(),
                                                                        sceneryConfigFile.toStdString(),
                                                                        runConfigFile.toStdString(),
                                                                        logFile.toStdString(),
                                                                        logLevel);

    if(!configuration)
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during configuration import";
        return nullptr;
    }

    return configuration;
}

} // namespace SimulationSlave
