/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "systemConfig.h"

std::map<int, std::shared_ptr<SimulationSlave::AgentTypeInterface>> &Configuration::SystemConfig::GetSystems()
{
    return systems;
}

void Configuration::SystemConfig::SetSystems(std::map<int, std::shared_ptr<SimulationSlave::AgentTypeInterface>> systems)
{
    this->systems = systems;
}

SimulationCommon::ModelParameters* Configuration::SystemConfig::AddModelParameters()
{
    auto parameter = std::make_shared<SimulationCommon::ModelParameters>();
    parameters.push_back(parameter);
    return parameter.get();
}

void Configuration::SystemConfig::AddModelParameters(std::shared_ptr<ParameterInterface> modelParameters)
{
    parameters.push_back(modelParameters);
}
