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

#include "spawnPointBinding.h"
#include "spawnPointLibrary.h"

namespace SimulationSlave {

SpawnPointBinding::SpawnPointBinding(CallbackInterface* callbacks) :
    callbacks(callbacks)
{}

SpawnPointBinding::~SpawnPointBinding()
{
    Unload();
}

SpawnPoint* SpawnPointBinding::Instantiate(
    std::string libraryPath,
    AgentFactoryInterface* agentFactory,
    WorldInterface* world,
    AgentBlueprintProviderInterface* agentBlueprintProvider,
    ParameterInterface* parameters,
    const SamplerInterface& sampler,
    ScenarioInterface* scenario)
{
    if (!library)
    {
        library = new (std::nothrow) SpawnPointLibrary(libraryPath,
                callbacks);
        if (!library)
        {
            return nullptr;
        }

        if (!library->Init())
        {
            delete library;
            return nullptr;
        }
    }

    return library->CreateSpawnPoint(parameters,
                                     agentFactory,
                                     world,
                                     agentBlueprintProvider,
                                     // Library boundary: hard to keep const reference
                                     const_cast<SamplerInterface*>(&sampler),
                                     scenario);
}

void SpawnPointBinding::Unload()
{
    delete library;
}

} // namespace SimulationSlave
