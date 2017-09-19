/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "spawnPointBinding.h"
#include "spawnPointLibrary.h"
#include "spawnPointInterface.h"
#include "runConfig.h"
#include "frameworkConfig.h"

namespace SimulationSlave
{

SpawnPointBinding::SpawnPointBinding(const FrameworkConfig *frameworkConfig,
                                     SimulationCommon::Callbacks *callbacks) :
    frameworkConfig(frameworkConfig),
    callbacks(callbacks)
{}

SpawnPointBinding::~SpawnPointBinding()
{
    Unload();
}

SpawnPoint *SpawnPointBinding::Instantiate(SimulationCommon::RunConfig::SpawnPointInstance *spawnPointInstance,
                                           AgentFactory *agentFactory,
                                           StochasticsInterface *stochastics,
                                           WorldInterface *world)
{
    SpawnPointLibrary *library;

    try
    {
        library = libraries.at(spawnPointInstance->GetLibraryName());
    }
    catch(const std::out_of_range&)
    {
        library = nullptr;
    }

    if(!library)
    {
        library = new (std::nothrow) SpawnPointLibrary(frameworkConfig->GetLibraryPath(),
                                                       spawnPointInstance->GetLibraryName(),
                                                       callbacks);
        if(!library)
        {
            return nullptr;
        }

        if(!library->Init())
        {
            delete library;
            return nullptr;
        }

        if(!libraries.insert({spawnPointInstance->GetLibraryName(), library}).second)
        {
            delete library;
            return nullptr;
        }
    }

    return library->CreateSpawnPoint(spawnPointInstance,
                                     agentFactory,
                                     stochastics,
                                     world);
}

void SpawnPointBinding::Unload()
{
    for(std::pair<const std::string, SpawnPointLibrary*> &item : libraries)
    {
        delete item.second;
    }

    libraries.clear();
}

} // namespace SimulationSlave
