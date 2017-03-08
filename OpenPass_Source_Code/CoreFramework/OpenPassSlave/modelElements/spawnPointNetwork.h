/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnPointNetwork.h
//! @brief This file implements a container of the spawn points during a
//!        simulation run.
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINTNETWORK_H
#define SPAWNPOINTNETWORK_H

#include <memory>
#include <map>
#include <list>
#include "runConfig.h"
#include "worldInterface.h"

namespace SimulationSlave
{

class SpawnPoint;
class AgentFactory;
class SpawnPointBinding;
class Stochastics;

class SpawnPointNetwork
{
public:    
    SpawnPointNetwork(SpawnPointBinding *spawnPointBinding, WorldInterface *world);
    SpawnPointNetwork(const SpawnPointNetwork&) = delete;
    SpawnPointNetwork(SpawnPointNetwork&&) = delete;
    SpawnPointNetwork& operator=(const SpawnPointNetwork&) = delete;
    SpawnPointNetwork& operator=(SpawnPointNetwork&&) = delete;
    virtual ~SpawnPointNetwork();

    bool Instantiate(const std::list<SimulationCommon::RunConfig::SpawnPointInstance*> &spawnPointInstance,
                     const std::list<const AgentSpawnItem*> &agentSpawnItems,
                     AgentFactory *agentFactory,
                     Stochastics *stochastics);
    void Clear();
    std::map<int, SpawnPoint*> &GetSpawnPoints()
    {
        return spawnPoints;
    }

    std::list<const AgentSpawnItem*> *GetAgentSpawnItem(int spawnPointId);

private:
    SpawnPointBinding *spawnPointBinding;
    WorldInterface *world;
    std::map<int, SpawnPoint*> spawnPoints; // map spawn point id to spawn point instance
    std::map<int, std::list<const AgentSpawnItem*>*> agentSpawnItemMap; // map spawn point id to agent instance parameters
};

} // namespace SimulationSlave

#endif // SPAWNPOINTNETWORK_H
