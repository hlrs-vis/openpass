/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  channel.h
//! @brief This file contains the internal representation of a channel instance
//!        during a simulation run.
//-----------------------------------------------------------------------------

#ifndef CHANNEL_H
#define CHANNEL_H

#include <list>
#include <string>
#include <tuple>
#include <cstddef>
#include "modelInterface.h"

namespace SimulationSlave
{

class Component;
class Agent;
class ChannelBuffer;

class Channel
{
public:
    // index within tuple to identify (link/component pair) of channel targets
    enum class TargetLinkType : std::size_t
    {
        LinkId = 0,
        Component
    };

    Channel(int id);
    Channel(const Channel&) = delete;
    Channel(Channel&&) = delete;
    Channel& operator=(const Channel&) = delete;
    Channel& operator=(Channel&&) = delete;
    virtual ~Channel() = default;

    void SetAgent(Agent *agent);
    Agent *GetAgent() const;
    bool SetSource(Component *source, int sourceLinkId);
    bool AttachSourceBuffer(ChannelBuffer *buffer);
    bool AddTarget(Component *target, int targetLinkId);
    Component *GetSource() const;
    const std::list<std::tuple<int, Component*>> &GetTargets() const;

private:
    static const int Undefined = -1;

    Agent *agent;
    int id;
    int sourceLinkId;
    Component *source;
    std::list<std::tuple<int, Component*>> targets;
    ChannelBuffer *buffer;
};

} // namespace SimulationSlave

#endif // CHANNEL_H
