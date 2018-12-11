/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "WorldData.h"
#include "WorldImplementation.h"

WorldImplementation::WorldImplementation(const CallbackInterface *callbacks):
    agentNetwork(this, callbacks),
    callbacks(callbacks)
{}

WorldImplementation::~WorldImplementation()
{
}

bool WorldImplementation::AddAgent(int id, AgentInterface *agent)
{
    if(agentNetwork.AddAgent(id, agent))
    {
        worldObjects.push_back(agent);
        return true;
    }

    return false;
}

const AgentInterface *WorldImplementation::GetAgent(int id) const
{
    return agentNetwork.GetAgent(id);
}

const std::map<int, const AgentInterface*> &WorldImplementation::GetAgents() const
{
    return agentNetwork.GetAgents();
}

const std::list<const AgentInterface*> &WorldImplementation::GetRemovedAgents() const
{
    return agentNetwork.GetRemovedAgents();
}

void WorldImplementation::Clear()
{
    agentNetwork.Clear();
    worldObjects.clear();
}

void* WorldImplementation::GetGlobalDrivingView()
{
    return nullptr;
}

bool WorldImplementation::CreateGlobalDrivingView()
{
    return true;
}

void* WorldImplementation::GetGlobalObjects()
{
    return nullptr;
}

void* WorldImplementation::GetWorldData()
{
    return &worldData;
}

void WorldImplementation::QueueAgentUpdate(std::function<void(double)> func, double val)
{
    agentNetwork.QueueAgentUpdate(func, val);
}

void WorldImplementation::QueueAgentRemove(const AgentInterface *agent)
{
    agentNetwork.QueueAgentRemove(agent);

    auto it = std::find(worldObjects.begin(), worldObjects.end(), agent);
    if(it != worldObjects.end())
    {
        worldObjects.erase(it);
    }
}

void WorldImplementation::SyncGlobalData()
{
    agentNetwork.SyncGlobalData();
}

bool WorldImplementation::CreateWorldScenario(const std::string& scenarioFilename)
{
    Q_UNUSED(scenarioFilename);

    return false;
}

bool WorldImplementation::CreateWorldScenery(const std::string& sceneryFilename)
{
    Q_UNUSED(sceneryFilename);

    return false;
}

bool WorldImplementation::CreateScenery(SceneryInterface &scenery)
{
    this->scenery = &scenery;

    SceneryConverter converter(&scenery,
                               worldData,
                               callbacks);
    if (converter.Convert())
    {
        InitTrafficObjects();
        return true;
    }

    return false;
}

AgentInterface* WorldImplementation::CreateAgentAdapterForAgent()
{
    AgentInterface *agentAdapter = new AgentAdapter(this, callbacks);

    return agentAdapter;
}

int WorldImplementation::GetTimeOfDay() const
{
    return worldParameter->GetTimeOfDay();
}

const AgentInterface *WorldImplementation::GetSpecialAgent()
{
    const std::map<int, const AgentInterface*> agents = agentNetwork.GetAgents();

    for (auto iterator = agents.begin(); iterator != agents.end(); iterator++)
    {
        const AgentInterface *agent = iterator->second;

        if (agent->IsSpecialAgent())
        {
            return agent;
        }
    }

    return nullptr;
}

void WorldImplementation::SetTimeOfDay(int timeOfDay)
{
    // TODO: stub
}

Weekday WorldImplementation::GetWeekday() const
{
    // TODO: stub
    return Weekday::Undefined;
}

void WorldImplementation::SetWeekday(Weekday weekday)
{
    // TODO: stub
}

void WorldImplementation::SetParameter(WorldParameter *worldParameter)
{
    this->worldParameter = worldParameter;
}

const AgentInterface* WorldImplementation::GetBicycle() const
{
    return nullptr;
}

const AgentInterface* WorldImplementation::GetLastCarInlane(int laneNumber)
{
    // TODO: stub
    return nullptr;
}

Position WorldImplementation::GetPositionByDistanceAndLane(double distance, int laneNumber) const
{
    return Position{0.0, 0.0, 0.0, 0.0};    // TODO: stub
}

Position WorldImplementation::GetPositionByDistanceAndLaneWithOffset(double distanceOnLane, double offset, int laneId) const
{
    return Position{0.0, 0.0, 0.0, 0.0};    // TODO: stub
}

bool WorldImplementation::GetNextValidSOnLane(double distance, int laneNumber, double &nextValidS)
{
    // TODO: stub
    return false;
}

bool WorldImplementation::GetLastValidSOnLane(double distance, int laneNumber, double &lastValidSOnLane)
{
    // TODO: stub
    return false;
}

bool WorldImplementation::IsSValidOnLane(double distance, int laneNumber) //when necessary optional parameter with reference to get point
{
    // TODO: stub
    return true;
}

double WorldImplementation::GetDistanceToEndOfLane(double currentPosition, int laneId)
{
    // TODO: stub
    return 0.0;
}

void WorldImplementation::InitTrafficObjects()
{
    // TODO
}
