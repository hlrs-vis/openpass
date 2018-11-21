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
//! @file  slaveConfig.h
//! @brief This file contains the representation of the slave
//!        configuration.
//-----------------------------------------------------------------------------

#ifndef SLAVECONFIG_H
#define SLAVECONFIG_H

#include <string>

namespace SimulationMaster {
class SlaveConfig
{
public:
    SlaveConfig(const std::string &libraryPath,
                const std::string &observationResultPath,
                const std::string &agentConfigFile,
                const std::string &runConfigFile,
                const std::string &sceneryConfigFile,
                const std::string &openScenarioConfigFile,
                const std::string &logFileSlave);

    virtual ~SlaveConfig();

    const std::string &GetLibraryPath() const;
    const std::string &GetObservationResultPath() const;
    const std::string &GetAgentConfigFile() const;
    const std::string &GetRunConfigFile() const;
    const std::string &GetSceneryConfigFile() const;
    const std::string &GetOpenScenarioConfigFile() const;
    const std::string &GetLogFileSlave() const;

private:
    std::string libraryPath;
    std::string observationResultPath;
    std::string agentConfigFile;
    std::string runConfigFile;
    std::string sceneryConfigFile;
    std::string openScenarioConfigFile;
    std::string logFileSlave;
};

} // namespace SimulationMaster

#endif // SLAVECONFIG_H
