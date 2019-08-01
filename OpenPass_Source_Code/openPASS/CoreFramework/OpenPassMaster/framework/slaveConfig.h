/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  slaveConfig.h
//! @brief This file contains the representation of the slave
//!        configuration.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <optional>

namespace SimulationMaster {
namespace Configuration {

class SlaveConfig
{
public:
    SlaveConfig(std::optional<std::string> logFile,
                std::optional<std::string> configs,
                std::optional<std::string> results) :
        logFile{logFile.value_or(defaultLogFile)},
        configs{configs.value_or(defaultConfigs)},
        results{results.value_or(defaultResults)}
    {}

    const std::string logFile;
    const std::string configs;
    const std::string results;

private:
    static constexpr char defaultLogFile[] = "OpenPassSlave.log";
    static constexpr char defaultConfigs[] = "configs";
    static constexpr char defaultResults[] = "results";
};

} // namespace Configuration
} // namespace SimulationMaster
