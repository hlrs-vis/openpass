/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <algorithm>
#include <list>
#include <sstream>
#include "worldInterface.h"
#include "runConfig.h"
#include "agentFactory.h"
#include "agent.h"
#include "agentType.h"
#include "modelBinding.h"
#include "agentTypeImporter.h"
#include "frameworkConfig.h"
#include "component.h"
#include "channel.h"
#include "parameters.h"
#include "spawnPoint.h"
#include "observationNetwork.h"
#include "channelBuffer.h"
#include "stochastics.h"
#include "log.h"

namespace SimulationSlave
{

AgentFactory::AgentFactory(FrameworkConfig *frameworkConfig,
                           ModelBinding *modelBinding,
                           WorldInterface *world,
                           Stochastics *stochastics,
                           ObservationNetwork *observationNetwork) :
    frameworkConfig(frameworkConfig),
    modelBinding(modelBinding),
    world(world),
    stochastics(stochastics),
    observationNetwork(observationNetwork)
{}

AgentFactory::~AgentFactory()
{
    ClearAgentTypes();
}

bool AgentFactory::ReloadAgentTypes()
{
    LOG_INTERN(LogLevel::DebugCore) << "import agent types...";

    ClearAgentTypes();

    if(!AgentTypeImporter::Import(frameworkConfig->GetAgentConfigFile(), agentTypes))
    {
        ClearAgentTypes();
        return false;
    }

    return true;
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

Agent *AgentFactory::AddAgent(const AgentSpawnItem *agentSpawnItem,
                              const SpawnItemParameter &spawnItemParameter,
                              int spawnTime)
{
    Agent *agent = CreateAgent(lastAgentId,
                               agentSpawnItem,
                               spawnItemParameter,
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
                                 const AgentSpawnItem *agentSpawnItem,
                                 const SpawnItemParameter &spawnItemParameter,
                                 int spawnTime)
{
    LOG_INTERN(LogLevel::DebugCore) << "instantiate agent (id " << id << ", type ref " << agentSpawnItem->GetReference() << ")";

    const AgentType *agentType;
    try
    {
        agentType = agentTypes.at(agentSpawnItem->GetReference());
    }
    catch(const std::out_of_range&)
    {
        LOG_INTERN(LogLevel::Error) << "agent types not available yet for agent creation";
        return nullptr;
    }

    if(!agentType)
    {
        return nullptr;
    }

    Agent *agent = new (std::nothrow) Agent(id,
                                            agentType,
                                            spawnTime,
                                            world);
    if(!agent)
    {
        return nullptr;
    }

    if(!agent->Instantiate(agentSpawnItem,
                           spawnItemParameter,
                           modelBinding,
                           stochastics,
                           observationNetwork))
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

void AgentFactory::ClearAgentTypes()
{
    for(auto &item : agentTypes)
    {
        delete item.second;
    }

    agentTypes.clear();
}

bool AgentFactory::ConnectAgentLinks(Agent *agent)
{
    for(const std::pair<const int, Component*> &itemComponent : agent->GetComponents())
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

            Component *source= channel->GetSource();
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
            for(const std::tuple<int, Component*> &item : channel->GetTargets())
            {
                int targetLinkId = std::get<static_cast<size_t>(Channel::TargetLinkType::LinkId)>(item);
                Component *targetComponent = std::get<static_cast<size_t>(Channel::TargetLinkType::Component)>(item);
                targetComponent->SetInputBuffer(targetLinkId, buffer);
            }
        }
    }

    return true;
}

} // namespace SimulationSlave
