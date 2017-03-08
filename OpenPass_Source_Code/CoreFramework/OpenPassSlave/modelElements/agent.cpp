/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include <QDomDocument>
#include <QFile>
#include "channel.h"
#include "componentType.h"
#include "component.h"
#include "agent.h"
#include "agentType.h"
#include "modelBinding.h"
#include "parameters.h"
#include "spawnPoint.h"
#include "observationNetwork.h"
#include "log.h"

namespace SimulationSlave
{

Agent::Agent(int id,
             const AgentType *agentType,
             int spawnTime,
             WorldInterface *world):
    spawnTime(spawnTime),
    id(id),
    agentType(agentType),
    world(world)
{
    SetAgentAdapter(world->CreateAgentAdapterForAgent());
}

Agent::~Agent()
{
    LOG_INTERN(LogLevel::DebugCore) << "deleting agent " << id;

    for(std::pair<const int, Component*> &item : components)
    {
        Component *component = item.second;

        if(!component->ReleaseFromLibrary())
        {
            LOG_INTERN(LogLevel::Error) << "component could not be released by agent " << id;
        }

        delete component;
    }
    components.clear();

    for(std::pair<const int, Channel*> &item : channels)
    {
        Channel *channel = item.second;
        delete channel;
    }
    channels.clear();
}

bool Agent::Instantiate(const AgentSpawnItem *agentSpawnItem,
                        const SpawnItemParameter &spawnItemParameter,
                        ModelBinding *modelBinding,
                        StochasticsInterface *stochastics,
                        ObservationNetwork *observationNetwork)
{
    // setup
    if(!agentInterface->InitAgentParameter(id,
                                         agentType->GetId(),
                                           spawnTime,
                                         agentSpawnItem,
                                         spawnItemParameter))
    {
        return false;
    }

    // instantiate channels
    for(int channelId : agentType->GetChannels())
    {
        LOG_INTERN(LogLevel::DebugCore) << "- instantiate channel " << channelId;

        Channel *channel = new (std::nothrow) Channel(channelId);
        if(!channel)
        {
            LOG_INTERN(LogLevel::Error) << "agent could not be instantiated";
            return false;
        }

        channel->SetAgent(this);

        if(!AddChannel(channelId, channel))
        {
            LOG_INTERN(LogLevel::Error) << "agent could not be instantiated";
            delete channel;
            return false;
        }

    }

    // instantiate components
    for(const std::pair<const int, ComponentType*> &itemComponentType : agentType->GetComponents())
    {
        int componentId = itemComponentType.first;
        ComponentType *componentType = itemComponentType.second;

        LOG_INTERN(LogLevel::DebugCore) << "- instantiate component " << componentId;

        Component *component = modelBinding->Instantiate(componentType,
                                                         componentId,
                                                         stochastics,
                                                         world,
                                                         observationNetwork,
                                                         this);
        if(!component)
        {
            LOG_INTERN(LogLevel::Error) << "agent could not be instantiated";
            return false;
        }

        if(!AddComponent(componentId, component))
        {
            LOG_INTERN(LogLevel::Error) << "agent could not be instantiated";
            delete component;
            return false;
        }

        // from here component will be deleted implicitely by agent destructor since it is linked into its container

        // instantiate inputs
        for(std::pair<const int, int> &item : componentType->GetInputLinks())
        {
            int linkId = item.first;
            int channelRef = item.second;

            LOG_INTERN(LogLevel::DebugCore) << "  * instantiate input channel " << channelRef << " (linkId " << linkId << ")";

            Channel *channel = GetChannel(channelRef);
            if(!channel)
            {
                return false;
            }

            if(!component->AddInputLink(channel, linkId))
            {
                return false;
            }

            if(!channel->AddTarget(component, linkId))
            {
                return false;
            }
        }

        // instantiate outputs
        for(std::pair<const int, int> &item : componentType->GetOutputLinks())
        {
            int linkId = item.first;
            int channelRef = item.second;

            LOG_INTERN(LogLevel::DebugCore) << "  * instantiate output channel " << channelRef << " (linkId " << linkId << ")";

            Channel *channel = GetChannel(channelRef);
            if(!channel)
            {
                return false;
            }

            if(!component->AddOutputLink(channel, linkId))
            {
                return false;
            }

            if(!channel->SetSource(component, linkId))
            {
                return false;
            }
        }
    } // component loop

    return true;
}

AgentInterface *Agent::GetAgentAdapter() const
{
    return agentInterface;
}

void Agent::SetAgentAdapter(AgentInterface *agentAdapt)
{
    agentInterface = agentAdapt;
}

bool Agent::AddComponent(int id, Component *component)
{
    if(!components.insert({id, component}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "components must be unique";
        return false;
    }

    return true;
}

bool Agent::AddChannel(int id, Channel *channel)
{
    if(!channels.insert({id, channel}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "channels must be unique";
        return false;
    }

    return true;
}

int Agent::GetAgentPriority() const
{
    return agentType->GetAgentPriority();
}

Channel *Agent::GetChannel(int id) const
{
    Channel *channel;

    try
    {
        channel = channels.at(id);
    }
    catch(const std::out_of_range&)
    {
        channel = nullptr;
    }

    return channel;
}

Component *Agent::GetComponent(int id) const
{
    Component *component;

    try
    {
        component = components.at(id);
    }
    catch(const std::out_of_range&)
    {
        component = nullptr;
    }

    return component;
}

const std::map<int, Component*> &Agent::GetComponents() const
{
    return components;
}

} // namespace SimulationSlave
