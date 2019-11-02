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
//! @file  SpawnPointNetwork.h
//! @brief This file implements a container of the spawn points during a
//!        simulation run.
//-----------------------------------------------------------------------------

#pragma once

#include <memory>
#include <map>
#include <list>

#include "Interfaces/worldInterface.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "Interfaces/samplerInterface.h"
#include "Interfaces/scenarioInterface.h"
#include "Interfaces/spawnPointNetworkInterface.h"

namespace SimulationSlave {
class SpawnPoint;
class SpawnPointBinding;

class SpawnPointNetwork : public SpawnPointNetworkInterface
{
public:
    SpawnPointNetwork(SpawnPointBinding* spawnPointBinding, WorldInterface* world);
    SpawnPointNetwork(const SpawnPointNetwork&) = delete;
    SpawnPointNetwork(SpawnPointNetwork&&) = delete;
    SpawnPointNetwork& operator=(const SpawnPointNetwork&) = delete;
    SpawnPointNetwork& operator=(SpawnPointNetwork&&) = delete;
    virtual ~SpawnPointNetwork();

    bool Instantiate(std::string libraryPath,
                     AgentFactoryInterface* agentFactory,
                     AgentBlueprintProviderInterface* agentBlueprintProvider,
                     ParameterInterface* parameters,
                     const SamplerInterface& sampler,
                     ScenarioInterface* scenario);

    void Clear();

    SpawnPoint* GetSpawnPoint()
    {
        return spawnPoint;
    }

private:
    SpawnPointBinding* spawnPointBinding;
    WorldInterface* world;
    SpawnPoint* spawnPoint {nullptr};
};

} // namespace SimulationSlave

