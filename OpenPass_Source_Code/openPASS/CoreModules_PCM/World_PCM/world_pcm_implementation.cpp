/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "world_pcm_implementation.h"

World_PCM_Implementation::World_PCM_Implementation(const CallbackInterface *callbacks):
    agentNetwork(this, callbacks),
    callbacks(callbacks)
{

    pcmData.SetCallbacks(callbacks);
}

World_PCM_Implementation::~World_PCM_Implementation()
{
    Clear();
}

bool World_PCM_Implementation::AddAgent(int id, AgentInterface *agent)
{
    return agentNetwork.AddAgent(id, agent);
}

const AgentInterface *World_PCM_Implementation::GetAgent(int id) const
{
    return agentNetwork.GetAgent(id);
}

const std::map<int, const AgentInterface *> &World_PCM_Implementation::GetAgents() const
{
    return agentNetwork.GetAgents();
}

void World_PCM_Implementation::SetParameter(WorldParameter *worldParameter)
{
    weekday = worldParameter->GetWeekday();
    timeOfDay = worldParameter->GetTimeOfDay();
}

void World_PCM_Implementation::Clear()
{
    agentNetwork.Clear();
    pcmData.Clear();
    for (auto &trajectoryItem : trajectories)
    {
        trajectoryItem.second.Clear();
    }
    trajectories.clear();
    timeOfDay = 0;
    weekday = Weekday::Undefined;
}

bool World_PCM_Implementation::CreateGlobalDrivingView()
{
    return true;
}

void World_PCM_Implementation::QueueAgentUpdate(std::function<void (double)> func, double val)
{
    agentNetwork.QueueAgentUpdate(func, val);
}

void World_PCM_Implementation::QueueAgentRemove(const AgentInterface *agent)
{
    agentNetwork.QueueAgentRemove(agent);
}

void World_PCM_Implementation::SyncGlobalData()
{
    agentNetwork.SyncGlobalData();
    UpdatePcmAgentData();
}

bool World_PCM_Implementation::CreateScenery(SceneryInterface &scenery)
{
    Q_UNUSED(scenery);
    return true;
}

AgentInterface *World_PCM_Implementation::CreateAgentAdapterForAgent()
{
    AgentInterface *agentAdapter = new AgentAdapter(this, callbacks);

    return agentAdapter;
}

int World_PCM_Implementation::GetTimeOfDay() const
{
    return timeOfDay;
}

Weekday World_PCM_Implementation::GetWeekday() const
{
    return weekday;
}

void World_PCM_Implementation::SetTimeOfDay(int timeOfDay)
{
    this->timeOfDay = timeOfDay;
}

void World_PCM_Implementation::SetWeekday(Weekday weekday)
{
    this->weekday = weekday;
}

bool World_PCM_Implementation::CreateWorldScenery(const std::string &sceneryFilename)
{
    return sceneryImporterPCM.Import(sceneryFilename, pcmData, trajectories);
}

bool World_PCM_Implementation::CreateWorldScenario(const std::string &scenarioFilename)
{
    return scenarioImporterPCM.Import(scenarioFilename, trajectories);
}

const PCM_Data *World_PCM_Implementation::GetPCM_Data() const
{
    return &pcmData;
}

const PCM_Trajectory *World_PCM_Implementation::GetTrajectory(int agentId) const
{
    return &trajectories.find(agentId)->second;
}

void World_PCM_Implementation::UpdatePcmAgentData()
{
    pcmData.ClearAgentData();
    std::map<int, const AgentInterface *> agents = GetAgents();
    for (std::map<int, const AgentInterface *>::iterator it = agents.begin();
            it != agents.end(); ++it)
    {

        const AgentInterface *agent = it->second;
        pcmData.AddPCM_Agent(agent->GetAgentId(),
                             agent->GetPositionX(),
                             agent->GetPositionY(),
                             agent->GetYawAngle(),
                             agent->GetWidth(),
                             agent->GetHeight());
    }
}
