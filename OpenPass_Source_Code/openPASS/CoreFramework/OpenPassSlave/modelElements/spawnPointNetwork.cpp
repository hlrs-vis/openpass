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

#include "agentFactory.h"
#include "CoreFramework/CoreShare/log.h"
#include "spawnPoint.h"
#include "spawnPointNetwork.h"
#include "spawnPointBinding.h"

namespace SimulationSlave {

SpawnPointNetwork::SpawnPointNetwork(SpawnPointBinding* spawnPointBinding,
                                     WorldInterface* world) :
    spawnPointBinding(spawnPointBinding),
    world(world)
{}

SpawnPointNetwork::~SpawnPointNetwork()
{
    Clear();
}

void SpawnPointNetwork::Clear()
{
    if (spawnPoint != nullptr)
    {
        delete spawnPoint;
        spawnPoint = nullptr;
    }
}

bool SpawnPointNetwork::Instantiate(std::string libraryPath,
                                    AgentFactoryInterface* agentFactory,
                                    AgentBlueprintProviderInterface* agentBlueprintProvider,
                                    ParameterInterface* parameters,
                                    const SamplerInterface& sampler,
                                    ScenarioInterface* scenario)
{
    spawnPoint = spawnPointBinding->Instantiate(libraryPath,
                 agentFactory,
                 world,
                 agentBlueprintProvider,
                 parameters,
                 sampler,
                 scenario);
    if (!spawnPoint)
    {
        LOG_INTERN(LogLevel::DebugCore) << "could not spawn point";
        return false;
    }

    return true;
}
} // namespace SimulationSlave
