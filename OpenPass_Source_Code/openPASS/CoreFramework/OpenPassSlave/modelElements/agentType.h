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
//! @file  agentType.h
//! @brief This file contains the internal representation of an agent type as
//!        given by the configuration.
//-----------------------------------------------------------------------------

#ifndef AGENTTYPE_H
#define AGENTTYPE_H

#include <map>
#include <list>

namespace SimulationSlave
{

class ComponentType;
class ChannelType;

class AgentType
{
public:
    AgentType(int id, int priority);
    AgentType(const AgentType&) = delete;
    AgentType(AgentType&&) = delete;
    AgentType& operator=(const AgentType&) = delete;
    AgentType& operator=(AgentType&&) = delete;
    virtual ~AgentType();

    bool AddChannel(int id);
    bool AddComponent(int id, ComponentType *component);
    int GetAgentPriority() const;
    const std::list<int> &GetChannels() const;
    const std::map<int, ComponentType*> &GetComponents() const;
    int GetId() const;

private:
    // components, etc. can not be stored in one global map due to redundant ids (same id could be used within different agents)
    int id;
    int priority;
    std::list<int> channels;
    std::map<int, ComponentType*> components;
};

} // namespace SimulationSlave

#endif // AGENTTYPE_H
