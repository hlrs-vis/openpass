/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <iostream>
#include <algorithm>
#include "agent.h"
#include "channel.h"
#include "log.h"

namespace SimulationSlave
{

Channel::Channel(int id) :
    agent(nullptr),
    id(id),
    sourceLinkId(Undefined),
    source(nullptr),
    buffer(nullptr)
{}

void Channel::SetAgent(Agent *agent)
{
    this->agent = agent;
}

Agent *Channel::GetAgent() const
{
    return agent;
}

bool Channel::SetSource(Component *source, int sourceLinkId)
{
    if(this->source || Undefined != this->sourceLinkId)
    {
        LOG_INTERN(LogLevel::Warning) << "trying to override channel source";
        return false;
    }

    this->source = source;
    this->sourceLinkId = sourceLinkId;

    return true;
}

bool Channel::AttachSourceBuffer(ChannelBuffer *buffer)
{
    if(this->buffer)
    {
        LOG_INTERN(LogLevel::Warning) << "trying to override channel source";
        return false;
    }

    this->buffer = buffer;
    return true;
}

bool Channel::AddTarget(Component *target, int targetLinkId)
{
    for(auto &item : targets)
    {
        // do not compare linkIds since two links in different components each may have the same name
        if(std::get<static_cast<size_t>(TargetLinkType::Component)>(item) == target)
        {
            LOG_INTERN(LogLevel::Warning) << "targets must be unique";
            return false;
        }
    }

    targets.push_back(std::make_tuple(targetLinkId, target));
    return true;
}

Component *Channel::GetSource() const
{
    return source;
}

const std::list<std::tuple<int, Component*>> &Channel::GetTargets() const
{
                                             return targets;
}

} // namespace SimulationSlave
