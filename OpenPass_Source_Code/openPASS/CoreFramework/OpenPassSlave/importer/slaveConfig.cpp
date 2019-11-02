/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  SlaveConfig.cpp */
//-----------------------------------------------------------------------------

#include "slaveConfig.h"

namespace Configuration
{

ExperimentConfig &SlaveConfig::GetExperimentConfig()
{
    return experimentConfig;
}

ScenarioConfig &SlaveConfig::GetScenarioConfig()
{
    return scenarioConfig;
}

EnvironmentConfig &SlaveConfig::GetEnvironmentConfig()
{
    return environmentConfig;
}

TrafficConfig &SlaveConfig::GetTrafficConfig()
{
    return trafficConfig;
}

const std::string &SlaveConfig::GetProfilesCatalog() const
{
    return profilesCatalog;
}

void SlaveConfig::SetProfilesCatalog(std::string profilesCatalog)
{
    this->profilesCatalog = profilesCatalog;
}

} //namespace Configuration
