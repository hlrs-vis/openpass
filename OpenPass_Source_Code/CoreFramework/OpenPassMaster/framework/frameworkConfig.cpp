/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "frameworkConfig.h"

namespace SimulationMaster
{

FrameworkConfig::FrameworkConfig(const std::string &libraryPath,
                                 const std::string &observationResultPath,
                                 const std::string &agentConfigFile,
                                 const std::string &runConfigFile,
                                 const std::string &sceneryConfigFile,
                                 const std::string &logFileMaster,
                                 const std::string &logFileSlave,
                                 int logLevel,
                                 const std::string &slavePath) :
    libraryPath(libraryPath),
    observationResultPath(observationResultPath),
    agentConfigFile(agentConfigFile),
    runConfigFile(runConfigFile),
    sceneryConfigFile(sceneryConfigFile),
    logFileMaster(logFileMaster),
    logFileSlave(logFileSlave),
    logLevel(logLevel),
    slavePath(slavePath)
{}

FrameworkConfig::~FrameworkConfig()
{

}

const std::string &FrameworkConfig::GetLibraryPath() const
{
    return libraryPath;
}

const std::string &FrameworkConfig::GetObservationResultPath() const
{
    return observationResultPath;
}

const std::string &FrameworkConfig::GetAgentConfigFile()const {
    return agentConfigFile;
}

const std::string &FrameworkConfig::GetRunConfigFile() const
{
    return runConfigFile;
}

const std::string &FrameworkConfig::GetSceneryConfigFile() const
{
    return sceneryConfigFile;
}

const std::string &FrameworkConfig::GetLogFileMaster() const
{
    return logFileMaster;
}

const std::string &FrameworkConfig::GetLogFileSlave() const
{
    return logFileSlave;
}

int FrameworkConfig::GetLogLevel() const
{
    return logLevel;
}

const std::string &FrameworkConfig::GetSlavePath() const
{
    return slavePath;
}


} // namespace SimulationMaster
