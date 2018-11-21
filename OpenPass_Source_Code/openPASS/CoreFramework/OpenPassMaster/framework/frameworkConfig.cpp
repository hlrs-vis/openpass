/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "frameworkConfig.h"

namespace SimulationMaster {

FrameworkConfig::FrameworkConfig(const std::string &logFileMaster,
                                 int logLevel,
                                 const std::string &slavePath,
                                 std::list<SlaveConfig> &slaveConfigList) :
    logFileMaster(logFileMaster),
    logLevel(logLevel),
    slavePath(slavePath),
    slaveConfigList(slaveConfigList)
{}

FrameworkConfig::~FrameworkConfig()
{

}

const std::string &FrameworkConfig::GetLogFileMaster() const
{
    return logFileMaster;
}

int FrameworkConfig::GetLogLevel() const
{
    return logLevel;
}

const std::string &FrameworkConfig::GetSlavePath() const
{
    return slavePath;
}

std::list<SlaveConfig> FrameworkConfig::GetSlaveConfigList() const
{
    return slaveConfigList;
}

} // namespace SimulationMaster
