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

#include <algorithm>
#include <string>
#include <QFile>
#include "AgentAdapter.h"
#include "AgentNetwork.h"

AgentNetwork::AgentNetwork(WorldInterface* world, const CallbackInterface* callbacks) :
    world(world),
    callbacks(callbacks)
{}

AgentNetwork::~AgentNetwork()
{
    Clear();
}

void AgentNetwork::Clear()
{
    updateQueue.clear();
    removeQueue.clear();

    for (const std::pair<const int, const AgentInterface*>& item : agents)
    {
        delete item.second;
    }

    agents.clear();

    for (const auto& removedAgent : removedAgents)
    {
        delete removedAgent;
    }

    removedAgents.clear();
}

bool AgentNetwork::AddAgent(int id, AgentInterface* agent)
{
    if (!agents.insert({id, agent}).second)
    {
        LOG(CbkLogLevel::Warning, "agents must be unique");
        return false;
    }

    return true;
}

AgentInterface* AgentNetwork::GetAgent(int id) const
{
    if (agents.find(id) == agents.end())
    {
        return nullptr;
    }

    return agents.at(id);
}

const std::map<int, AgentInterface*>& AgentNetwork::GetAgents() const
{
    return agents;
}

const std::list<const AgentInterface*>& AgentNetwork::GetRemovedAgents() const
{
    return removedAgents;
}

void AgentNetwork::QueueAgentUpdate(std::function<void()> func)
{
    updateQueue.push_back(func);
}

void AgentNetwork::QueueAgentRemove(const AgentInterface* agent)
{
    removeQueue.push_back(agent);
}

// udpate global data at occurrence of time step
void AgentNetwork::SyncGlobalData()
{
    while (!updateQueue.empty())
    {
        const auto& func = updateQueue.front();
        func();
        updateQueue.pop_front();
    }

    // remove agents
    while (!removeQueue.empty())
    {
        // remove from framework
        const AgentInterface* agent = removeQueue.front();
        removeQueue.pop_front();

        if (1 != agents.erase(agent->GetId()))
        {
            LOG(CbkLogLevel::Warning, "trying to remove non-existent agent");
        }

        removedAgents.push_back(agent);

        agent->Unregister();
    }

    for (auto& item : agents)
    {
        AgentInterface* agent = item.second;

        agent->Unlocate();

        if (!agent->Update())
        {
            LOG(CbkLogLevel::Warning, "Could not locate agent");
        }

        if (!agent->IsAgentInWorld())
        {
            agent->RemoveAgent();
            continue;
        }
    }
}
