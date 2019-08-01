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
//! @file  SpawnPointLibrary.h
//! @brief This file contains the internal representation of the library of a
//!        spawn point.
//-----------------------------------------------------------------------------

#pragma once

#include <list>
#include <string>
#include <QLibrary>
#include "Interfaces/agentFactoryInterface.h"
#include "spawnPointBinding.h"
#include "Interfaces/spawnPointInterface.h"
#include "Interfaces/callbackInterface.h"
#include "Interfaces/agentBlueprintInterface.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "Interfaces/scenarioInterface.h"

namespace SimulationSlave
{

class SpawnPointLibrary
{
public:    
    typedef const std::string &(*SpawnPointInterface_GetVersion)();
    typedef SpawnPointInterface *(*SpawnPointInterface_CreateInstanceType)(WorldInterface *world,
                                                                           const ParameterInterface *parameters,
                                                                           const CallbackInterface *callbacks,
                                                                           AgentBlueprintProviderInterface* agentBlueprintProvider,
                                                                           SamplerInterface *sampler,
                                                                           ScenarioInterface *scenario);
    typedef void (*SpawnPointInterface_DestroyInstanceType)(SpawnPointInterface *implementation);
    typedef bool (*SpawnPointInterface_GenerateAgentType)(SpawnPointInterface *implementation,
                                                          AgentBlueprintInterface* agentBlueprint);

    SpawnPointLibrary(const std::string &libraryPath,
                      CallbackInterface *callbacks) :
        libraryPath(libraryPath),
        callbacks(callbacks)
    {}
    SpawnPointLibrary(const SpawnPointLibrary&) = delete;
    SpawnPointLibrary(SpawnPointLibrary&&) = delete;
    SpawnPointLibrary& operator=(const SpawnPointLibrary&) = delete;
    SpawnPointLibrary& operator=(SpawnPointLibrary&&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor, deletes the stored library (unloads it if necessary), if the list
    //! of stored spawn points is empty.
    //-----------------------------------------------------------------------------
    virtual ~SpawnPointLibrary();

    //-----------------------------------------------------------------------------
    //! Creates a QLibrary based on the path from the constructor and stores function
    //! pointer for getting the library version, creating and destroying instances
    //! and setting the spawn item (see typedefs for corresponding signatures).
    //!
    //! @return                 Null pointer
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    //! Find spawn point in the sored list of spawn points, then call the "destroy
    //! instance" function pointer with its implementation and remove it from the list
    //! of stored spawn points.
    //!
    //! @param[in]  spawnPoint          Spawn point to release
    //! @return                         Flag if the release was successful
    //-----------------------------------------------------------------------------
    bool ReleaseSpawnPoint(SpawnPoint *spawnPoint);

    //-----------------------------------------------------------------------------
    //! Make sure that the library exists and is loaded, then call the "create instance"
    //! function pointer using the parameters from the spawn point instance to get
    //! a spawn point interface, which is then used to instantiate a spawn point
    //! which is also stored in the list of spawn points.
    //!
    //! @param[in]  world                   World instance
    //! @param[in]  parameters              Spawn point parameters
    //! @param[in]  agentFactory            Factory for the agents
    //! @param[in]  agentBlueprintProvider  AgentBluePrintProvider
    //! @param[in]  sampler                 Sampler instance
    //! @param[in]  scenario                Scenario from configs
    //! @return                         Spawn point created
    //-----------------------------------------------------------------------------
    SpawnPoint *CreateSpawnPoint(ParameterInterface *parameters,
                                 AgentFactoryInterface *agentFactory,
                                 WorldInterface *world,
                                 AgentBlueprintProviderInterface *agentBlueprintProvider,
                                 SamplerInterface *sampler,
                                 ScenarioInterface *scenario);

    //-----------------------------------------------------------------------------
    //! Calls the "generateAgent" function pointer with the provided parameters and
    //! returns the result of this call.
    //!
    //! @param[in]  spawnPointInstance  Spawn point instance that is instantiated
    //! @param[out]  spawnItem          AgentBlueprint which is used to create an agent.
    //! @return                         Flag if generate agent was successful
    //-----------------------------------------------------------------------------
    bool GenerateAgent(SpawnPointInterface *implementation, AgentBlueprintInterface* agentBlueprint)
    {
        return generateAgentFunc(implementation, agentBlueprint);
    }

private:    
    const std::string DllGetVersionId = "OpenPASS_GetVersion";
    const std::string DllCreateInstanceId = "OpenPASS_CreateInstance";
    const std::string DllDestroyInstanceId = "OpenPASS_DestroyInstance";
    const std::string DllGenerateAgentId = "OpenPASS_GenerateAgent";

    std::string libraryPath;
    std::list<SpawnPoint*> spawnPoints;
    QLibrary *library = nullptr;
    CallbackInterface *callbacks;
    SpawnPointInterface_GetVersion getVersionFunc{nullptr};
    SpawnPointInterface_CreateInstanceType createInstanceFunc{nullptr};
    SpawnPointInterface_DestroyInstanceType destroyInstanceFunc{nullptr};
    SpawnPointInterface_GenerateAgentType generateAgentFunc{nullptr};
};

} // namespace SimulationSlave

