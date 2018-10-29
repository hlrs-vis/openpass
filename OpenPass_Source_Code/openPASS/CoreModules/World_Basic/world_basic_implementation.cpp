/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#include "world_basic_implementation.h"

World_Basic_Implementation::World_Basic_Implementation(const CallbackInterface *callbacks):
    agentNetwork(this, callbacks),
    callbacks(callbacks)
{}

World_Basic_Implementation::~World_Basic_Implementation()
{
    Clear();
}

bool World_Basic_Implementation::AddAgent(int id, AgentInterface *agent)
{
    return agentNetwork.AddAgent(id, agent);
}

const AgentInterface *World_Basic_Implementation::GetAgent(int id) const
{
    return agentNetwork.GetAgent(id);
}

const std::map<int, const AgentInterface *> &World_Basic_Implementation::GetAgents() const
{
    return agentNetwork.GetAgents();
}

void World_Basic_Implementation::SetParameter(WorldParameter *worldParameter)
{
    weekday = worldParameter->GetWeekday();
    timeOfDay = worldParameter->GetTimeOfDay();
}

void World_Basic_Implementation::Clear()
{
    agentNetwork.Clear();

    timeOfDay = 0;
    weekday = Weekday::Undefined;
}

bool World_Basic_Implementation::CreateGlobalDrivingView()
{
    return true;
}

void World_Basic_Implementation::QueueAgentUpdate(std::function<void (double)> func, double val)
{
    agentNetwork.QueueAgentUpdate(func, val);
}

void World_Basic_Implementation::QueueAgentRemove(const AgentInterface *agent)
{
    agentNetwork.QueueAgentRemove(agent);
}

void World_Basic_Implementation::SyncGlobalData()
{
    agentNetwork.SyncGlobalData();
}

bool World_Basic_Implementation::CreateScenery(SceneryInterface &scenery)
{
    Q_UNUSED(scenery);
    return true;
}

AgentInterface* World_Basic_Implementation::CreateAgentAdapterForAgent()
{
    AgentInterface *agentAdapter = new AgentAdapter(this, callbacks);

    return agentAdapter;
}

int World_Basic_Implementation::GetTimeOfDay() const
{
    return timeOfDay;
}

Weekday World_Basic_Implementation::GetWeekday() const
{
    return weekday;
}

void World_Basic_Implementation::SetTimeOfDay(int timeOfDay)
{
    this->timeOfDay = timeOfDay;
}

void World_Basic_Implementation::SetWeekday(Weekday weekday)
{
    this->weekday = weekday;
}
