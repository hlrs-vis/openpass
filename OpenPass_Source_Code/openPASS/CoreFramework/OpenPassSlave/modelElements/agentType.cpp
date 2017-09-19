/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <algorithm>
#include <iostream>
#include <QDomDocument>
#include <QFile>
#include "agentType.h"
#include "componentType.h"
#include "parameters.h"
#include "log.h"

namespace SimulationSlave
{

AgentType::AgentType(int id, int priority):
    id(id),
    priority(priority)
{}

AgentType::~AgentType()
{
    LOG_INTERN(LogLevel::DebugCore) << "deleting agent type " << id;

    for(std::pair<const int, ComponentType*> &item : components)
    {
        delete item.second;
    }
}

int AgentType::GetId() const
{
    return id;
}

bool AgentType::AddComponent(int id, ComponentType *component)
{
    if(!components.insert({id, component}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "components must be unique";
        return false;
    }

    return true;
}

bool AgentType::AddChannel(int id)
{
    std::list<int>::iterator findIter = std::find(channels.begin(), channels.end(), id);
    if(channels.end() != findIter)
    {
        LOG_INTERN(LogLevel::Warning) << "components must be unique";
        return false;
    }

    channels.push_back(id);
    return true;
}

int AgentType::GetAgentPriority() const
{
    return priority;
}

const std::list<int> &AgentType::GetChannels() const
{
    return channels;
}

const std::map<int, ComponentType*> &AgentType::GetComponents() const
{
    return components;
}

} // namespace SimulationSlave
