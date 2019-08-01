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

//-----------------------------------------------------------------------------
//! @file  AgentType.h
//! @brief This file contains the internal representation of an agent type as
//!        given by the configuration.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/agentTypeInterface.h"

namespace SimulationSlave
{

class ComponentType;
class ChannelType;

class AgentType : public AgentTypeInterface
{
public:
    ~AgentType() = default;

    virtual bool AddChannel(int id);
    virtual bool AddComponent(std::shared_ptr<ComponentType> component);

    //Getter
    virtual const std::list<int> &GetChannels() const;
    virtual const std::map<std::string, std::shared_ptr<ComponentType>> &GetComponents() const;

private:
    std::list<int> channels;
    std::map<std::string, std::shared_ptr<ComponentType>> components;
};

} // namespace SimulationSlave


