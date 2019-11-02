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
#include <map>
#include <vector>
#include <iostream>
#include <QDomDocument>
#include <QFile>

#include "agent.h"
#include "agentType.h"
#include "channel.h"
#include "componentType.h"
#include "component.h"
#include "CoreFramework/CoreShare/log.h"
#include "modelBinding.h"
#include "Interfaces/observationNetworkInterface.h"
#include "CoreFramework/CoreShare/parameters.h"
#include "spawnPoint.h"

namespace SimulationSlave
{

Agent::Agent(int id,
             AgentBlueprintInterface* agentBlueprint,
             int spawnTime,
             WorldInterface *world):
    spawnTime(spawnTime),
    id(id),
    agentBlueprint(agentBlueprint),
    world(world)
{
    SetAgentAdapter(world->CreateAgentAdapterForAgent());
}

Agent::~Agent()
{
    LOG_INTERN(LogLevel::DebugCore) << "deleting agent " << id;

    for(std::pair<const std::string, ComponentInterface*> &item : components)
    {
        ComponentInterface *component = item.second;

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

bool Agent::Instantiate(AgentBlueprintInterface* agentBlueprint,
                        ModelBinding *modelBinding,
                        StochasticsInterface *stochastics,
                        ObservationNetworkInterface *observationNetwork,
                        EventNetworkInterface *eventNetwork)
{
    // setup
    if(!agentInterface->InitAgentParameter(id,
                                           spawnTime,
                                           agentBlueprint))
    {
        return false;
    }

    // instantiate channels
    for(int channelId : agentBlueprint->GetAgentType().GetChannels())
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
    for(const std::pair<const std::string, std::shared_ptr<ComponentType>> &itemComponentType : agentBlueprint->GetAgentType().GetComponents())
    {
        std::string componentName = itemComponentType.first;
        std::shared_ptr<ComponentType> componentType = itemComponentType.second;

        LOG_INTERN(LogLevel::DebugCore) << "- instantiate component " << componentName;

        ComponentInterface *component = modelBinding->Instantiate(componentType,
                                                         componentName,
                                                         stochastics,
                                                         world,
                                                         observationNetwork,
                                                         this,
                                                         eventNetwork);
        if(!component)
        {
            LOG_INTERN(LogLevel::Error) << "agent could not be instantiated";
            return false;
        }

        if(!AddComponent(componentName, component))
        {
            LOG_INTERN(LogLevel::Error) << "agent could not be instantiated";
            delete component;
            return false;
        }

        // from here component will be deleted implicitely by agent destructor since it is linked into its container

        // instantiate inputs
        for(const std::pair<const int, int> &item : componentType->GetInputLinks())
        {
            int linkId = item.first;
            int channelRef = item.second;

            LOG_INTERN(LogLevel::DebugCore) << "  * instantiate input channel " << channelRef << " (linkId " << linkId << ")";

            Channel *channel = GetChannel(channelRef);
            if(!channel)
            {
                continue;
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
        for(const std::pair<const int, int> &item : componentType->GetOutputLinks())
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

bool Agent::AddComponent(std::string name, ComponentInterface *component)
{
    if(!components.insert(std::make_pair<std::string&, ComponentInterface*&>(name, component)).second)
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

ComponentInterface *Agent::GetComponent(std::string name) const
{
    ComponentInterface *component;

    try
    {
        component = components.at(name);
    }
    catch(const std::out_of_range&)
    {
        component = nullptr;
    }

    return component;
}

const std::map<std::string, ComponentInterface*> &Agent::GetComponents() const
{
    return components;
}

} // namespace SimulationSlave
