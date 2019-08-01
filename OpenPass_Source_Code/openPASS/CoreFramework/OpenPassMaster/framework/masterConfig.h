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
//! @file  MasterConfig.h
//! @brief This file contains the representation of the master
//!        configuration.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <optional>
#include <vector>

namespace SimulationMaster {
namespace Configuration {

class SlaveConfig;
using SlaveConfigs = std::vector<SlaveConfig>;

///
/// \brief Used as value storage of parsed configuration parameters
///
struct MasterConfig
{
public:
    MasterConfig(
        std::optional<int> logLevel,
        std::optional<std::string> logFileMaster,
        std::optional<std::string> slave,
        std::optional<std::string> libraries,
        SlaveConfigs slaveConfigs) :
        logLevel{CheckOrDefault(logLevel.value_or(defaultLogLevel))},
        logFileMaster{logFileMaster.value_or(defaultLogFileMaster)},
        slave{slave.value_or(defaultSlave)},
        libraries{libraries.value_or(defaultLibraries)},
        slaveConfigs{slaveConfigs}
    {}

    MasterConfig() :
        logLevel{defaultLogLevel},
        logFileMaster{defaultLogFileMaster},
        slave{defaultSlave},
        libraries{defaultLibraries},
        slaveConfigs{}
    {}

    const int logLevel;
    const std::string logFileMaster;
    const std::string slave;
    const std::string libraries;
    const SlaveConfigs slaveConfigs;

private:
    static constexpr int defaultLogLevel = 0;
    static constexpr char defaultLogFileMaster[] = "OpenPassMaster.log";
    static constexpr char defaultSlave[] = "OpenPassSlave";
    static constexpr char defaultLibraries[] = "lib";

    //-------------------------------------------------------------------------
    //! \brief Checks if the passed value is in between the minimum and maximum
    //!  	   log level boundaries.
    //! \param[in] logLevel Value to be checked.
    //! \returns logLevel if passed, defaultLogLevel if not
    //-------------------------------------------------------------------------
    static int CheckOrDefault(int logLevel)
    {
        return (logLevel >= 0 && logLevel <= 5) ? logLevel : defaultLogLevel;
    }
};

} // namespace Configuration
} // namespace SimulationMaster
