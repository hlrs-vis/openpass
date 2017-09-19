/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  frameworkConfig.h
//! @brief This file contains the representation of the framework
//!        configuration.
//-----------------------------------------------------------------------------

#ifndef FRAMEWORKCONFIG_H
#define FRAMEWORKCONFIG_H

#include <string>
#include <list>

namespace SimulationMaster
{

class FrameworkConfig
{
public:    

    FrameworkConfig(const std::string &libraryPath,
                    const std::string &observationResultPath,
                    const std::string &agentConfigFile,
                    const std::string &runConfigFile,
                    const std::string &sceneryConfigFile,
                    const std::string &logFileMaster,
                    const std::string &logFileSlave,
                    int logLevel,
                    const std::string &slavePath);

    FrameworkConfig(const FrameworkConfig&) = delete;
    FrameworkConfig(FrameworkConfig&&) = delete;
    FrameworkConfig& operator=(const FrameworkConfig&) = delete;
    FrameworkConfig& operator=(FrameworkConfig&&) = delete;
    virtual ~FrameworkConfig();

    const std::string &GetLibraryPath() const;
    const std::string &GetObservationResultPath() const;
    const std::string &GetAgentConfigFile() const;
    const std::string &GetRunConfigFile() const;
    const std::string &GetSceneryConfigFile() const;
    const std::string &GetLogFileMaster() const;
    const std::string &GetLogFileSlave() const;
    int GetLogLevel() const;
    const std::string &GetSlavePath() const;

private:
    std::string libraryPath;
    std::string observationResultPath;
    std::string agentConfigFile;
    std::string runConfigFile;
    std::string sceneryConfigFile;
    std::string logFileMaster;
    std::string logFileSlave;
    int logLevel;
    std::string slavePath;
};

} // namespace SimulationMaster

#endif // FRAMEWORKCONFIG_H
