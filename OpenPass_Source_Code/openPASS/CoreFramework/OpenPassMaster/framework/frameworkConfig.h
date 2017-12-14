/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
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
