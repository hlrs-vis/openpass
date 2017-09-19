/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "frameworkConfig.h"

namespace SimulationSlave
{

FrameworkConfig::FrameworkConfig(const std::string &libraryPath,
                                 const std::string &observationResultPath,
                                 const std::string &agentConfigFile,
                                 const std::string &sceneryConfigFile,
                                 const std::string &runConfigFile,
                                 const std::string &logFile,
                                 int logLevel) :
    libraryPath(libraryPath),
    observationResultPath(observationResultPath),
    agentConfigFile(agentConfigFile),
    sceneryConfigFile(sceneryConfigFile),
    runConfigFile(runConfigFile),
    logFile(logFile),
    logLevel(logLevel)
{}

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
    this->libraryPath = modelLibraryPath;
}

void FrameworkConfig::SetAgentConfigFile(const std::string &agentConfigFile)
{
    this->agentConfigFile = agentConfigFile;
}

void FrameworkConfig::SetSceneryConfigFile(const std::string &sceneryConfigFile)
{
    this->sceneryConfigFile = sceneryConfigFile;
}

void FrameworkConfig::SetRunConfigFile(const std::string &runConfigFile)
{
    this->runConfigFile = runConfigFile;
}

} // namespace SimulationSlave
