/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <list>
#include <map>
#include "Interfaces/agentFactoryInterface.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "Interfaces/samplerInterface.h"
#include "Interfaces/scenarioInterface.h"

namespace SimulationSlave
{
class SpawnPoint;

class SpawnPointNetworkInterface
{
public:
    SpawnPointNetworkInterface() = default;
    SpawnPointNetworkInterface(const SpawnPointNetworkInterface&) = delete;
    SpawnPointNetworkInterface(SpawnPointNetworkInterface&&) = delete;
    SpawnPointNetworkInterface& operator=(const SpawnPointNetworkInterface&) = delete;
    SpawnPointNetworkInterface& operator=(SpawnPointNetworkInterface&&) = delete;
    virtual ~SpawnPointNetworkInterface() = default;

    //-----------------------------------------------------------------------------
    //! Gets the spawn point instance library either from the already stored libraries
    //! or create a new instance (which is then also stored), then create a new spawn
    //! point using the provided parameters.
    //!
    //! @param[in]  libraryPath         Path of the spawnpoint library
    //! @param[in]  agentFactory        Factory for the agents
    //! @param[in]  agentSampler        AgentSampelr
    //! @param[in]  parameters          SpawnPoint parameters
    //! @param[in]  sampler             Sampler
    //! @param[in]  scenario            Scenario
    //! @return                         true, if successful
    //-----------------------------------------------------------------------------
    virtual bool Instantiate(std::string libraryPath,
                             AgentFactoryInterface *agentFactory,
                             AgentBlueprintProviderInterface *agentBlueprintProvider,
                             ParameterInterface *parameters,
                             const SamplerInterface &sampler,
                             ScenarioInterface *scenario) = 0;

    //-----------------------------------------------------------------------------
    //! Clears the spawnpoint.
    //-----------------------------------------------------------------------------
    virtual void Clear() = 0;

    //-----------------------------------------------------------------------------
    //! Gets the spawnpoint
    //!
    //! @return                         Pointer to spawnpoint
    //-----------------------------------------------------------------------------
    virtual SpawnPoint* GetSpawnPoint() = 0;
};

} //SimulationSlave
