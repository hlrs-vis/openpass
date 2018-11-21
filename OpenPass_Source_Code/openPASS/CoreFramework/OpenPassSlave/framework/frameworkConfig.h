/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  frameworkConfig.h
//! @brief This file contains the representation of the framework
//!        configuration.
//-----------------------------------------------------------------------------

#ifndef FRAMEWORKCONFIG_H
#define FRAMEWORKCONFIG_H

#include <string>
#include <QApplication>
#include <QDir>
#include <QFileInfo>

namespace SimulationSlave {

class FrameworkConfig
{
public:
    FrameworkConfig(const std::string &libraryPath,
                    const std::string &observationResultPath,
                    const std::string &agentConfigFile,
                    const std::string &sceneryConfigFile,
                    const std::string &openScenarioConfigFile,
                    const std::string &runConfigFile,
                    const std::string &logFile,
                    int logLevel);
    FrameworkConfig(const FrameworkConfig &) = delete;
    FrameworkConfig(FrameworkConfig &&) = delete;
    FrameworkConfig &operator=(const FrameworkConfig &) = delete;
    FrameworkConfig &operator=(FrameworkConfig &&) = delete;
    virtual ~FrameworkConfig() = default;

    const std::string &GetLibraryPath() const;
    const std::string &GetObservationResultPath() const;
    const std::string &GetAgentConfigFile() const;
    const std::string &GetSceneryConfigFile() const;
    const std::string &GetOpenScenarioConfigFile() const;
    const std::string &GetRunConfigFile() const;
    const std::string &GetLogFile() const;
    int GetLogLevel() const;
    const std::string &GetPort() const;


    void SetLibraryPath(const std::string &libraryPath);
    void SetObservationResultPath(const std::string &observationResultPath);
    void SetAgentConfigFile(const std::string &agentConfigFile);
    void SetSceneryConfigFile(const std::string &sceneryConfigFile);
    void SetOpenScenarioConfigFile(const std::string &openScenarioConfigFile);
    void SetRunConfigFile(const std::string &runConfigFile);
    void SetLogFile(const std::string &logFile);
    void SetLogLevel(int logLevel);

private:
    std::string libraryPath;
    std::string observationResultPath;
    std::string agentConfigFile;
    std::string sceneryConfigFile;
    std::string openScenarioConfigFile;
    std::string runConfigFile;
    std::string logFile;
    int logLevel;
};

} // namespace SimulationSlave

#endif // FRAMEWORKCONFIG_H
