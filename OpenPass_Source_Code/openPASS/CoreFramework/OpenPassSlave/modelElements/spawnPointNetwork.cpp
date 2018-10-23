/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "spawnPointNetwork.h"
#include "runConfig.h"
#include "spawnPoint.h"
#include "agentFactory.h"
#include "spawnPointBinding.h"
#include "stochastics.h"
#include "log.h"

namespace SimulationSlave
{

SpawnPointNetwork::SpawnPointNetwork(SpawnPointBinding *spawnPointBinding,
                                     WorldInterface *world) :
    spawnPointBinding(spawnPointBinding),
    world(world)
{}

SpawnPointNetwork::~SpawnPointNetwork()
{
    Clear();
}

void SpawnPointNetwork::Clear()
{
    for(std::pair<const int, SpawnPoint*> &item : spawnPoints)
    {
        delete item.second;
    }

    spawnPoints.clear();
}

bool SpawnPointNetwork::Instantiate(const std::list<SimulationCommon::RunConfig::SpawnPointInstance*> &spawnPointInstances,
                                    const std::list<const AgentSpawnItem*> &agentSpawnItems,
                                    AgentFactory *agentFactory,
                                    Stochastics *stochastics)
{
    for(SimulationCommon::RunConfig::SpawnPointInstance *spawnPointInstance : spawnPointInstances)
    {
        SpawnPoint *spawnPoint = spawnPointBinding->Instantiate(spawnPointInstance,
                                                                agentFactory,
                                                                stochastics,
                                                                world);
        if(!spawnPoint)
        {
            LOG_INTERN(LogLevel::DebugCore) << "could not instantiate spawn point from " << spawnPointInstance->GetLibraryName();
            return false;
        }

        if(!spawnPoints.insert({spawnPointInstance->GetId(), spawnPoint}).second)
        {
            LOG_INTERN(LogLevel::DebugCore) << "spawn point must be unique";
            delete spawnPoint;
            return false;
        }

        LOG_INTERN(LogLevel::DebugCore) << "link spawn point " << spawnPointInstance->GetId() << " to:";
        for(int agentRef : spawnPointInstance->GetAgentRefs())
        {
            for(const AgentSpawnItem *agentSpawnItem : agentSpawnItems)
            {
                if(agentSpawnItem->GetId() == agentRef)
                {
                    spawnPoint->AddAgentSpawnItem(agentSpawnItem);
                    LOG_INTERN(LogLevel::DebugCore) << "- agent " << agentSpawnItem->GetId();
                }
            }
        }
    }

    return true;
}

std::list<const AgentSpawnItem*> *SpawnPointNetwork::GetAgentSpawnItem(int spawnPointId)
{
    try
    {
        std::list<const AgentSpawnItem*> *result = agentSpawnItemMap.at(spawnPointId);

        return result;
    }
    catch(const std::out_of_range&)
    {
        // do nothing
    }

    return nullptr;
}

} // namespace SimulationSlave
