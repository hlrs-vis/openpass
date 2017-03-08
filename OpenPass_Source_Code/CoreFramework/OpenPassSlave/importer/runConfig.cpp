/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "runConfig.h"

namespace SimulationCommon
{

RunConfig::RunConfig(int startTime, int endTime, int numberInvocations, std::uint32_t randomSeed, WorldParameter *worldParameter, StochasticsInstance *stochasticsInstance, CollisionDetectionInstance *collisionDetectionInstance) :
    startTime(startTime),
    endTime(endTime),
    numberInvocations(numberInvocations),
    randomSeed(randomSeed),
    worldParameter(worldParameter),
    stochasticsInstance(stochasticsInstance),
    collisionDetectionInstance(collisionDetectionInstance)
{}

RunConfig::~RunConfig()
{
    for(const AgentSpawnItem *agentSpawnItem : agentSpawnItems)
    {
        delete agentSpawnItem;
    }
    agentSpawnItems.clear();

    for(const SpawnPointInstance *spawnPointInstance : spawnPointInstances)
    {
        delete spawnPointInstance;
    }
    spawnPointInstances.clear();

    for(auto &it : observationInstances)
    {
        delete it.second;
    }
    observationInstances.clear();

    delete worldParameter;
    worldParameter = nullptr;
    delete stochasticsInstance;
    stochasticsInstance = nullptr;
    delete collisionDetectionInstance;
    collisionDetectionInstance = nullptr;
}

bool RunConfig::AddAgentSpawnItem(const AgentSpawnItem *agentSpawnItem)
{
    agentSpawnItems.push_back(agentSpawnItem);

    return true;
}

bool RunConfig::AddSpawnPointInstance(RunConfig::SpawnPointInstance *spawnPointInstance)
{
    spawnPointInstances.push_back(spawnPointInstance);

    return true;
}

bool RunConfig::AddObservationInstance(int observationId, RunConfig::ObservationInstance *observationInstance)
{
    if(!(observationInstances.insert({observationId, observationInstance}).second))
    {
        return false;
    }

    return true;
}

int RunConfig::GetStartTime() const
{
    return startTime;
}

int RunConfig::GetEndTime() const
{
    return endTime;
}

int RunConfig::GetNumberInvocations() const
{
    return numberInvocations;
}

std::uint32_t RunConfig::GetRandomSeed() const
{
    return randomSeed;
}

WorldParameter *RunConfig::GetWorldParameter() const
{
    return worldParameter;
}

RunConfig::StochasticsInstance *RunConfig::GetStochasticsInstance() const
{
    return stochasticsInstance;
}

RunConfig::CollisionDetectionInstance *RunConfig::GetCollisionDetectionInstance() const
{
    return collisionDetectionInstance;
}

const std::list<const AgentSpawnItem*> &RunConfig::GetAgentSpawnItems() const
{
    return agentSpawnItems;
}

const std::list<RunConfig::SpawnPointInstance*> &RunConfig::GetSpawnPointInstances() const
{
    return spawnPointInstances;
}

const std::map<int, RunConfig::ObservationInstance*> &RunConfig::GetObservationInstances() const
{
    return observationInstances;
}

} // namespace SimulationCommon
