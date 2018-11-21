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
#include <list>
#include "slaveConfig.h"

namespace SimulationMaster {

class FrameworkConfig
{
public:

    FrameworkConfig(const std::string &logFileMaster,
                    int logLevel,
                    const std::string &slavePath,
                    std::list<SlaveConfig> &slaveConfigList);

    FrameworkConfig(const FrameworkConfig &) = delete;
    FrameworkConfig(FrameworkConfig &&) = delete;
    FrameworkConfig &operator=(const FrameworkConfig &) = delete;
    FrameworkConfig &operator=(FrameworkConfig &&) = delete;
    virtual ~FrameworkConfig();

    const std::string &GetLogFileMaster() const;
    int GetLogLevel() const;
    const std::string &GetSlavePath() const;

    std::list<SlaveConfig> GetSlaveConfigList() const;

private:
    std::string logFileMaster;
    int logLevel;
    std::string slavePath;
    std::list<SlaveConfig> slaveConfigList;
};

} // namespace SimulationMaster

#endif // FRAMEWORKCONFIG_H
