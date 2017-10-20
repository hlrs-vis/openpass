/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnPointBinding.h
//! @brief This file contains the interface to the spawn point libraries.
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINTBINDING_H
#define SPAWNPOINTBINDING_H

#include <map>
#include <string>
#include "spawnPointInterface.h"
#include "agentFactory.h"
#include "runConfig.h"
#include "callbacks.h"

namespace SimulationSlave
{

class SpawnPointLibrary;
class SpawnPoint;
class FrameworkConfig;

class SpawnPointBinding
{
public:
    SpawnPointBinding(const FrameworkConfig *frameworkConfig,
                      SimulationCommon::Callbacks *callbacks);
    SpawnPointBinding(const SpawnPointBinding&) = delete;
    SpawnPointBinding(SpawnPointBinding&&) = delete;
    SpawnPointBinding& operator=(const SpawnPointBinding&) = delete;
    SpawnPointBinding& operator=(SpawnPointBinding&&) = delete;
    virtual ~SpawnPointBinding();

    //-----------------------------------------------------------------------------
    //! Gets the spawn point instance library either from the already stored libraries
    //! or create a new instance (which is then also stored), then create a new spawn
    //! point using the provided parameters.
    //!
    //! @param[in]  spawnPointInstance  Spawn point instance that is instantiated
    //! @param[in]  agentFactory        Factory for the agents
    //! @param[in]  stochastics         Stochastics instance
    //! @param[in]  world               World instance
    //! @return                         Spawn point created from the library
    //-----------------------------------------------------------------------------
    SpawnPoint *Instantiate(SimulationCommon::RunConfig::SpawnPointInstance *spawnPointInstance,
                            AgentFactory *agentFactory,
                            StochasticsInterface *stochastics,
                            WorldInterface *world);

    //-----------------------------------------------------------------------------
    //! Unloads the spawn point binding by deleting all stored spawn point libraries.
    //-----------------------------------------------------------------------------
    void Unload();

private:
    const FrameworkConfig *frameworkConfig;
    std::map<std::string, SpawnPointLibrary*> libraries;
    SimulationCommon::Callbacks *callbacks;
};

} // namespace SimulationSlave

#endif // SPAWNPOINTBINDING_H
