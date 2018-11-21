/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#include <algorithm>
#include <string>
#include <QFile>
#include "agentAdapter.h"
#include "agentNetwork.h"

AgentNetwork::AgentNetwork(WorldInterface *world, const CallbackInterface *callbacks) :
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

    for(const std::pair<const int, const AgentInterface*> &item : agents)
    {
        delete item.second;
    }

    agents.clear();

    for(const auto &removedAgent : removedAgents)
    {
        delete removedAgent;
    }

    removedAgents.clear();
}

bool AgentNetwork::AddAgent(int id, const AgentInterface *agent)
{
    if(!agents.insert({id, agent}).second)
    {
        LOG(CbkLogLevel::Warning, "agents must be unique");
        return false;
    }

    return true;
}

const AgentInterface *AgentNetwork::GetAgent(int id) const
{
    if (agents.find(id) == agents.end())
    {
        return nullptr;
    }

    return agents.at(id);
}

const std::map<int, const AgentInterface*> &AgentNetwork::GetAgents() const
{
    return agents;
}

void AgentNetwork::QueueAgentUpdate(std::function<void(double)> func,
                                    double val)
{
    updateQueue.push_back(std::make_tuple(func, val));
}

void AgentNetwork::QueueAgentRemove(const AgentInterface *agent)
{
    removeQueue.push_back(agent);
}

// udpate global data at occurrence of time step
void AgentNetwork::SyncGlobalData()
{
    // update agent parameters
    while(!updateQueue.empty())
    {
        const auto &item = updateQueue.front();
        const auto &func = std::get<0>(item);
        double val = std::get<1>(item);
        func(val);
        updateQueue.pop_front();
    }

    for(auto &item : agents)
    {
        AgentInterface *agent = const_cast<AgentInterface*>(static_cast<const AgentInterface*>(item.second));

        if(!agent->IsAgentInWorld()){ //Remove if agents not in sections anymore
            agent->RemoveAgent();
            LOG(CbkLogLevel::Info, "Agent: " + std::to_string(agent->GetAgentId()) + " not in World located. Agent is removed from schedule");
        }
        if(!agent->Unlocate())    // clear section/lane objects
        {
            LOG(CbkLogLevel::Warning, "could not unlocate agent");
        }
    }

    // remove agents
    while(!removeQueue.empty())
    {
        // remove from framework
        const AgentInterface *agent = removeQueue.front();
        removeQueue.pop_front();

        if(1 != agents.erase(agent->GetAgentId()))
        {
            LOG(CbkLogLevel::Warning, "trying to remove non-existent agent");
        }

        removedAgents.push_back(agent);

        agent->Unregister();
    }

    // create section/lane objects
    for(auto &item : agents)
    {
        AgentInterface *agent = const_cast<AgentInterface*>(static_cast<const AgentInterface*>(item.second));

        if(!agent->Locate())
        {
            LOG(CbkLogLevel::Warning, "could not locate agent");
        }

        if(!agent->IsAgentInWorld()){
            agent->RemoveAgent();
            continue;
        }
    }
}
