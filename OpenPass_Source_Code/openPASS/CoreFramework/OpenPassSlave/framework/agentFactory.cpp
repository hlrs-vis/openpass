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
#include <list>
#include <sstream>
#include "agent.h"
#include "agentFactory.h"
#include "agentType.h"
#include "channel.h"
#include "channelBuffer.h"
#include "Interfaces/componentInterface.h"
#include "CoreFramework/CoreShare/log.h"
#include "modelBinding.h"
#include "Interfaces/observationNetworkInterface.h"
#include "CoreFramework/CoreShare/parameters.h"
#include "spawnPoint.h"
#include "stochastics.h"
#include "Interfaces/worldInterface.h"

namespace SimulationSlave
{

AgentFactory::AgentFactory(ModelBinding *modelBinding,
                           WorldInterface *world,
                           Stochastics *stochastics,
                           ObservationNetworkInterface *observationNetwork,
                           EventNetworkInterface *eventNetwork) :
    modelBinding(modelBinding),
    world(world),
    stochastics(stochastics),
    observationNetwork(observationNetwork),
    eventNetwork(eventNetwork)
{
}

AgentFactory::~AgentFactory()
{
}

void AgentFactory::ResetIds()
{
    lastAgentId = 0;
}

void AgentFactory::Clear()
{
    for(auto &item : agentList)
    {
        delete item;
    }
    agentList.clear();
}

Agent* AgentFactory::AddAgent(AgentBlueprintInterface* agentBlueprint,
                              int spawnTime)
{
    Agent *agent = CreateAgent(lastAgentId,
                               agentBlueprint,
                               spawnTime);
    if(!agent)
    {
        LOG_INTERN(LogLevel::Error) << "could not create agent";
        return nullptr;
    }

    if(!world->AddAgent(lastAgentId, agent->GetAgentAdapter()))
    {
        LOG_INTERN(LogLevel::Error) << "could not add agent to network";
        delete agent;
        return nullptr;
    }

    lastAgentId++;

    agentList.push_back(agent);

    return agent;
}

Agent *AgentFactory::CreateAgent(int id,
                                 AgentBlueprintInterface* agentBlueprint,
                                 int spawnTime)
{
    LOG_INTERN(LogLevel::DebugCore) << "instantiate agent (id " << id << ")";

    Agent *agent = new (std::nothrow) Agent(id,
                                            agentBlueprint,
                                            spawnTime,
                                            world);
    if(!agent)
    {
        return nullptr;
    }

    if(!agent->Instantiate(agentBlueprint,
                           modelBinding,
                           stochastics,
                           observationNetwork,
                           eventNetwork))
    {
        LOG_INTERN(LogLevel::Error) << "agent could not be instantiated";
        delete agent;
        return nullptr;
    }

    // link agent internal components
    if(!ConnectAgentLinks(agent))
    {
        LOG_INTERN(LogLevel::Error) << "agent channels could not be created";
        delete agent;
        return nullptr;
    }

    return agent;
}

bool AgentFactory::ConnectAgentLinks(Agent *agent)
{
    for(const std::pair<const std::string, ComponentInterface*> &itemComponent : agent->GetComponents())
    {
        if(!itemComponent.second)
        {
            return false;
        }

        for(const std::pair<const int, Channel*> &itemChannel : itemComponent.second->GetOutputLinks())
        {
            int outputLinkId = itemChannel.first;
            Channel *channel = itemChannel.second;
            if(!channel)
            {
                return false;
            }

            ComponentInterface *source = channel->GetSource();
            if(!source)
            {
                return false;
            }

            ChannelBuffer *buffer = source->CreateOutputBuffer(outputLinkId);
            if(!buffer || !(channel->AttachSourceBuffer(buffer)))
            {
                return false;
            }

            // channel buffer is now attached to channel and will be released when deleting the agent
            for(const std::tuple<int, ComponentInterface*> &item : channel->GetTargets())
            {
                int targetLinkId = std::get<static_cast<size_t>(Channel::TargetLinkType::LinkId)>(item);
                ComponentInterface *targetComponent = std::get<static_cast<size_t>(Channel::TargetLinkType::Component)>(item);
                targetComponent->SetInputBuffer(targetLinkId, buffer);
            }
        }
    }

    return true;
}

} // namespace SimulationSlave
