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
//! @file  SpawnPointBinding.h
//! @brief This file contains the interface to the spawn point libraries.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include <string>
#include "Interfaces/spawnPointInterface.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "Interfaces/agentFactoryInterface.h"
#include "CoreFramework/CoreShare/callbacks.h"
#include "Interfaces/samplerInterface.h"
#include "Interfaces/scenarioInterface.h"

namespace SimulationSlave {

class SpawnPointLibrary;
class SpawnPoint;

class SpawnPointBinding
{
public:
    SpawnPointBinding(CallbackInterface* callbacks);
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
    //! @param[in]  libraryPath             Path of the spawnpoint library
    //! @param[in]  agentFactory            Factory for the agents
    //! @param[in]  world                   World instance
    //! @param[in]  agentBlueprintProvider  AgentBlueprintProvider
    //! @param[in]  parameters              SpawnPoint parameters
    //! @param[in]  sampler                 Sampler
    //! @param[in]  scenario                Scenario
    //! @return                          Spawn point created from the library
    //-----------------------------------------------------------------------------
    SpawnPoint* Instantiate(std::string libraryPath,
                            AgentFactoryInterface* agentFactory,
                            WorldInterface* world,
                            AgentBlueprintProviderInterface* agentBlueprintProvider,
                            ParameterInterface* parameters,
                            const SamplerInterface& sampler,
                            ScenarioInterface* scenario);

    //-----------------------------------------------------------------------------
    //! Unloads the spawn point binding by deleting all stored spawn point libraries.
    //-----------------------------------------------------------------------------
    void Unload();

private:
    SpawnPointLibrary* library {nullptr};
    CallbackInterface* callbacks;
};

} // namespace SimulationSlave

