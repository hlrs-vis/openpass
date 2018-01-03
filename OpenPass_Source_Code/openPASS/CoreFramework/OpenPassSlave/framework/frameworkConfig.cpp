/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "frameworkConfig.h"

namespace SimulationSlave {

FrameworkConfig::FrameworkConfig(const std::string &libraryPath,
                                 const std::string &observationResultPath,
                                 const std::string &agentConfigFile,
                                 const std::string &sceneryConfigFile,
                                 const std::string &runConfigFile,
                                 const std::string &logFile,
                                 int logLevel)
{
    SetLibraryPath(libraryPath);
    SetObservationResultPath(observationResultPath);
    SetAgentConfigFile(agentConfigFile);
    SetSceneryConfigFile(sceneryConfigFile);
    SetRunConfigFile(runConfigFile);
    SetLogFile(logFile);
    SetLogLevel(logLevel);
}

const std::string &FrameworkConfig::GetLibraryPath() const
{
    return libraryPath;
}

const std::string &FrameworkConfig::GetObservationResultPath() const
{
    return observationResultPath;
}

const std::string &FrameworkConfig::GetAgentConfigFile() const
{
    return agentConfigFile;
}

const std::string &FrameworkConfig::GetSceneryConfigFile() const
{
    return sceneryConfigFile;
}

const std::string &FrameworkConfig::GetRunConfigFile() const
{
    return runConfigFile;
}

const std::string &FrameworkConfig::GetLogFile() const
{
    return logFile;
}

int FrameworkConfig::GetLogLevel() const
{
    return logLevel;
}

void FrameworkConfig::SetLibraryPath(const std::string &modelLibraryPath)
{
    if (QDir(QString::fromStdString(modelLibraryPath)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        QString absolutePath = baseDir.absoluteFilePath(QString::fromStdString(modelLibraryPath));
        absolutePath = baseDir.cleanPath(absolutePath);
        this->libraryPath = absolutePath.toStdString();
    }
    else
    {
        this->libraryPath = modelLibraryPath;
    }
}

void FrameworkConfig::SetObservationResultPath(const std::string &observationResultPath)
{
    if (QDir(QString::fromStdString(observationResultPath)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        QString absolutePath = baseDir.absoluteFilePath(QString::fromStdString(observationResultPath));
        absolutePath = baseDir.cleanPath(absolutePath);
        this->observationResultPath = absolutePath.toStdString();
    }
    else
    {
        this->observationResultPath = observationResultPath;
    }
}

void FrameworkConfig::SetAgentConfigFile(const std::string &agentConfigFile)
{
    if (QDir(QString::fromStdString(agentConfigFile)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        QString absolutePath = baseDir.absoluteFilePath(QString::fromStdString(agentConfigFile));
        absolutePath = baseDir.cleanPath(absolutePath);
        this->agentConfigFile = absolutePath.toStdString();
    }
    else
    {
        this->agentConfigFile = agentConfigFile;
    }
}

void FrameworkConfig::SetSceneryConfigFile(const std::string &sceneryConfigFile)
{
    if (QDir(QString::fromStdString(sceneryConfigFile)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        QString absolutePath = baseDir.absoluteFilePath(QString::fromStdString(sceneryConfigFile));
        absolutePath = baseDir.cleanPath(absolutePath);
        this->sceneryConfigFile = absolutePath.toStdString();
    }
    else
    {
        this->sceneryConfigFile = sceneryConfigFile;
    }
}

void FrameworkConfig::SetRunConfigFile(const std::string &runConfigFile)
{
    if (QDir(QString::fromStdString(runConfigFile)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        QString absolutePath = baseDir.absoluteFilePath(QString::fromStdString(runConfigFile));
        absolutePath = baseDir.cleanPath(absolutePath);
        this->runConfigFile = absolutePath.toStdString();
    }
    else
    {
        this->runConfigFile = runConfigFile;
    }
}

void FrameworkConfig::SetLogFile(const std::string &logFile)
{
    if (QDir(QString::fromStdString(logFile)).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        QString absolutePath = baseDir.absoluteFilePath(QString::fromStdString(logFile));
        absolutePath = baseDir.cleanPath(absolutePath);
        this->logFile = absolutePath.toStdString();
    }
    else
    {
        this->logFile = logFile;
    }
}

void FrameworkConfig::SetLogLevel(int logLevel)
{
    this->logLevel = logLevel;
}

} // namespace SimulationSlave
