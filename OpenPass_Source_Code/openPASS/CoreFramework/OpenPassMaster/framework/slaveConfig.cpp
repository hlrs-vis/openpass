/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "slaveConfig.h"

namespace SimulationMaster {

SlaveConfig::SlaveConfig(const std::string &libraryPath,
                         const std::string &observationResultPath,
                         const std::string &agentConfigFile,
                         const std::string &runConfigFile,
                         const std::string &sceneryConfigFile,
                         const std::string &openScenarioConfigFile,
                         const std::string &logFileSlave) :
    libraryPath(libraryPath),
    observationResultPath(observationResultPath),
    agentConfigFile(agentConfigFile),
    runConfigFile(runConfigFile),
    sceneryConfigFile(sceneryConfigFile),
    openScenarioConfigFile(openScenarioConfigFile),
    logFileSlave(logFileSlave)
{}

SlaveConfig::~SlaveConfig()
{

}

const std::string &SlaveConfig::GetLibraryPath() const
{
    return libraryPath;
}

const std::string &SlaveConfig::GetObservationResultPath() const
{
    return observationResultPath;
}

const std::string &SlaveConfig::GetAgentConfigFile()const
{
    return agentConfigFile;
}

const std::string &SlaveConfig::GetRunConfigFile() const
{
    return runConfigFile;
}

const std::string &SlaveConfig::GetSceneryConfigFile() const
{
    return sceneryConfigFile;
}

const std::string &SlaveConfig::GetLogFileSlave() const
{
    return logFileSlave;
}

const std::string &SlaveConfig::GetOpenScenarioConfigFile() const
{
    return openScenarioConfigFile;
}

} // namespace SimulationMaster

