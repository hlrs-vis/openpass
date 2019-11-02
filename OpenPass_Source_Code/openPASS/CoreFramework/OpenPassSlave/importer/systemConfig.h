/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once
#include <vector>
#include "parameters.h"
#include "Interfaces/systemConfigInterface.h"
#include "Interfaces/agentTypeInterface.h"

namespace Configuration
{

class SystemConfig : public SystemConfigInterface
{
public:

    virtual std::map<int, std::shared_ptr<SimulationSlave::AgentTypeInterface>>& GetSystems() override;

    virtual void SetSystems(std::map<int, std::shared_ptr<SimulationSlave::AgentTypeInterface>> systems) override;

    virtual SimulationCommon::ModelParameters *AddModelParameters() override;

    virtual void AddModelParameters(std::shared_ptr<ParameterInterface> modelParameters) override;

private:
    std::map<int, std::shared_ptr<SimulationSlave::AgentTypeInterface>> systems {};
    std::vector<std::shared_ptr<ParameterInterface>> parameters {};
};

}

