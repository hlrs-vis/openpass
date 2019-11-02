/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  Agent.h
//! @brief This file contains the internal representation of an agent instance
//!        during a simulation run.
//-----------------------------------------------------------------------------

#pragma once

#include <QtGlobal>
#include <functional>
#include <tuple>
#include <algorithm>
#include <utility>
#include <map>
#include <list>

#include "Interfaces/stochasticsInterface.h"
#include "Interfaces/agentInterface.h"
#include "Interfaces/componentInterface.h"

namespace SimulationSlave
{

class Channel;
class ModelBinding;
class ModelParameters;
class SpawnPoint;
class ObservationNetworkInterface;
class SpawnItemParameter;

class Agent
{
public:
    Agent(int id,
          AgentBlueprintInterface* agentType,
          int spawnTime,
          WorldInterface *world);
    Agent(const Agent&) = delete;
    Agent(Agent&&) = delete;
    Agent& operator=(const Agent&) = delete;
    Agent& operator=(Agent&&) = delete;
    virtual ~Agent();

    bool AddChannel(int id, Channel *channel);
    bool AddComponent(std::string name, SimulationSlave::ComponentInterface *component);
    Channel *GetChannel(int id) const;
    ComponentInterface *GetComponent(std::string name) const;
    const std::map<std::string, ComponentInterface*> &GetComponents() const;
    int GetId() const
    {
        return id;
    }

    bool Instantiate(AgentBlueprintInterface* agentBlueprint,
                     ModelBinding *modelBinding,
                     StochasticsInterface *stochastics,
                     SimulationSlave::ObservationNetworkInterface *observationNetwork,
                     EventNetworkInterface *eventNetwork);

    bool IsValid() const
    {
        return agentInterface->IsValid();
    }

    AgentInterface *GetAgentAdapter() const;

    void SetAgentAdapter(AgentInterface *agentAdapt);

private:
    // framework parameters
    std::vector<int> idsCollisionPartners;
    int spawnTime;
    int id;
    AgentBlueprintInterface* agentBlueprint = nullptr;
    WorldInterface *world = nullptr;
    std::map<int, Channel*> channels;
    std::map<std::string, ComponentInterface*> components;

    AgentInterface *agentInterface = nullptr;
};

} // namespace SimulationSlave


