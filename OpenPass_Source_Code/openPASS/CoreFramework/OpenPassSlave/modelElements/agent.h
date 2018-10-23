/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  agent.h
//! @brief This file contains the internal representation of an agent instance
//!        during a simulation run.
//-----------------------------------------------------------------------------

#ifndef AGENT_H
#define AGENT_H

#include <QtGlobal>
#include <functional>
#include <tuple>
#include <algorithm>
#include <utility>
#include <map>
#include <list>
#include "stochasticsInterface.h"
#include "runConfig.h"
#include "agentType.h"
#include "agentInterface.h"

namespace SimulationSlave
{

class Component;
class Channel;
class ModelBinding;
class ModelParameters;
class SpawnPoint;
class ObservationNetwork;
class SpawnItemParameter;

class Agent
{
public:
    Agent(int id,
          const AgentType *agentType,
          int spawnTime,
          WorldInterface *world);
    Agent(const Agent&) = delete;
    Agent(Agent&&) = delete;
    Agent& operator=(const Agent&) = delete;
    Agent& operator=(Agent&&) = delete;
    virtual ~Agent();

    bool AddChannel(int id, Channel *channel);
    bool AddComponent(int id, Component *component);
    int GetAgentPriority() const;
    Channel *GetChannel(int id) const;
    Component *GetComponent(int id) const;
    const std::map<int, Component*> &GetComponents() const;
    int GetId() const
    {
        return id;
    }

    const AgentType *GetAgentType() const
    {
        return agentType;
    }

    bool Instantiate(const AgentSpawnItem *agentSpawnItem,
                     const SpawnItemParameter &spawnItemParameter,
                     ModelBinding *modelBinding,
                     StochasticsInterface *stochastics,
                     ObservationNetwork *observationNetwork);

    bool IsValid() const
    {
        return agentInterface->IsValid();
    }

    AgentInterface *GetAgentAdapter() const;

    void SetAgentAdapter(AgentInterface *agentAdapt);

    int GetAgentId() const
    {
        return agentInterface->GetAgentId();
    }

private:
    // framework parameters
    std::vector<int> idsCollisionPartners;
    int spawnTime;
    int id;
    const AgentType *agentType = nullptr;
    WorldInterface *world = nullptr;
    std::map<int, Channel*> channels;
    std::map<int, Component*> components;

    AgentInterface *agentInterface = nullptr;
};

} // namespace SimulationSlave

#endif // AGENT_H
