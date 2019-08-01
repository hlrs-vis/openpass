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
//! @file  AgentFactory.h
//! @brief This file contains the singleton which creates agent instances
//!
//! The agent instances are created based on the agent types which are given by
//! configuration. Agent instances will be created on the fly during the
//! simulation run by spawn points.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include <list>

#include "Interfaces/agentFactoryInterface.h"
#include "Interfaces/eventNetworkInterface.h"
#include "Interfaces/worldInterface.h"

namespace SimulationSlave
{
class Agent;
class AgentType;
class ModelBinding;
class Stochastics;
class SpawnItemParameter;
class ObservationNetworkInterface;

class AgentFactory : public AgentFactoryInterface
{
public:
    AgentFactory(ModelBinding *modelBinding,
                 WorldInterface *world,
                 Stochastics *stochastics,
                 ObservationNetworkInterface *observationNetwork,
                 SimulationSlave::EventNetworkInterface *eventNetwork);
    AgentFactory(const AgentFactory&) = delete;
    AgentFactory(AgentFactory&&) = delete;
    AgentFactory& operator=(const AgentFactory&) = delete;
    AgentFactory& operator=(AgentFactory&&) = delete;
    virtual ~AgentFactory();

    //-----------------------------------------------------------------------------
    //! Sets the ID of the last added agent to 0.
    //-----------------------------------------------------------------------------
    void ResetIds();

    //-----------------------------------------------------------------------------
    //! Unloads the model bindings.
    //-----------------------------------------------------------------------------
    void Clear();

    //-----------------------------------------------------------------------------
    //! Creates a new agent based on the provided parameters, then adds it to the
    //! agent network in the world representation. Also adds agents during runtime.
    //!
    //! @param[in]  agentBlueprint      agentBlueprint contains all necessary
    //!                                 informations to create an agent
    //! @param[in]  spawnTime           Spawn time in ms
    //!
    //! @return                         The added agent
    //-----------------------------------------------------------------------------
    Agent *AddAgent(AgentBlueprintInterface* agentBlueprint,
                    int spawnTime);

private:
    //-----------------------------------------------------------------------------
    //! @brief Links all channels of the agent components.
    //!
    //! Gets the channels of the agent from all its components' output links,
    //! creates for each a channel buffer using its source and attaches it to the
    //! channel.
    //!
    //!
    //! @param[in]  agent               The agent whose channels are connected
    //!
    //! @return                         Flag if the connect was successful
    //-----------------------------------------------------------------------------
    bool ConnectAgentLinks(Agent *agent);

    //-----------------------------------------------------------------------------
    //! @brief Creates a new agent based on the provided parameters, initializes it
    //!         and links its agent channels.
    //!
    //! Gets the agent type via the reference stored in the agent run configuration
    //! definition, initializes a new agend using the parameters, resets its local
    //! driving view and initializes it, before finally linking the agent channels
    //! accordingly.
    //!
    //! @param[in]  id                  Agent ID
    //! @param[in]  agentBlueprint      agentBlueprint contains all necessary
    //!                                 informations to create an agent
    //! @param[in]  spawnTime           Spawn time in ms
    //!
    //! @return                         The created agent
    //-----------------------------------------------------------------------------
    Agent* CreateAgent(int id,
                       AgentBlueprintInterface* agentBlueprint,
                       int spawnTime);

    int lastAgentId = 0;
    ModelBinding *modelBinding;
    WorldInterface *world;
    Stochastics *stochastics;
    ObservationNetworkInterface *observationNetwork;
    EventNetworkInterface *eventNetwork;

    std::list<const Agent*> agentList;
};

} // namespace SimulationSlave


