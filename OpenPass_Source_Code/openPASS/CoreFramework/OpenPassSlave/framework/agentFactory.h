/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  agentFactory.h
//! @brief This file contains the singleton which creates agent instances
//!
//! The agent instances are created based on the agent types which are given by
//! configuration. Agent instances will be created on the fly during the
//! simulation run by spawn points.
//-----------------------------------------------------------------------------

#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#include <map>
#include <list>
#include "runConfig.h"
#include "worldInterface.h"

namespace SimulationSlave
{

class Agent;
class AgentType;
class ModelBinding;
class FrameworkConfig;
class Stochastics;
class SpawnItemParameter;
class ObservationNetwork;

class AgentFactory
{
public:
    AgentFactory(FrameworkConfig *frameworkConfig,
                 ModelBinding *modelBinding,
                 WorldInterface *world,
                 Stochastics *stochastics,
                 ObservationNetwork *observationNetwork);
    AgentFactory(const AgentFactory&) = delete;
    AgentFactory(AgentFactory&&) = delete;
    AgentFactory& operator=(const AgentFactory&) = delete;
    AgentFactory& operator=(AgentFactory&&) = delete;
    virtual ~AgentFactory();

    //-----------------------------------------------------------------------------
    //! Clears the agent types and tries to reload them from the agent configuration
    //! obained from the framework configuration.
    //!
    //! @return                         Flag if the reload was successful
    //-----------------------------------------------------------------------------
    bool ReloadAgentTypes();

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
    //! @param[in]  agentSpawnItem      Agent as defined in the run configuration
    //! @param[in]  spawnItemParameter  Parameters of the spawned item
    //! @param[in]  spawnTime           Point of time at which the agent is spawned
    //!
    //! @return                         The added agent
    //-----------------------------------------------------------------------------
    Agent *AddAgent(const AgentSpawnItem *agentSpawnItem,
                    const SpawnItemParameter &spawnItemParameter,
                    int spawnTime);

private:
    //-----------------------------------------------------------------------------
    //! Deletes the stored agent types.
    //-----------------------------------------------------------------------------
    void ClearAgentTypes();

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
    //! @param[in]  agentSpawnItem      Agent as defined in the run configuration
    //! @param[in]  spawnItemParameter  Parameters of the spawned item
    //! @param[in]  spawnTime           Point of time at which the agent is spawned
    //!
    //! @return                         The created agent
    //-----------------------------------------------------------------------------
    Agent *CreateAgent(int id,
                       const AgentSpawnItem *agentSpawnItem,
                       const SpawnItemParameter &spawnItemParameter,
                       int spawnTime);

    int lastAgentId = 0;
    FrameworkConfig *frameworkConfig;
    ModelBinding *modelBinding;
    WorldInterface *world;
    Stochastics *stochastics;
    ObservationNetwork *observationNetwork;
    std::map<int, const AgentType*> agentTypes;

    std::list<const Agent*> agentList;
};

} // namespace SimulationSlave

#endif // AGENTFACTORY_H
