/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "frameworkModules.h"
#include "frameworkModuleContainer.h"

namespace SimulationSlave {

FrameworkModuleContainer::FrameworkModuleContainer(
    FrameworkModules frameworkModules,
    ConfigurationContainerInterface* configurationContainer,
    CallbackInterface* callbacks) :
    worldBinding(frameworkModules.worldLibrary, callbacks),
    world(&worldBinding),
    spawnPointBinding(callbacks),
    spawnPointNetwork(&spawnPointBinding, &world),
    stochasticsBinding(callbacks),
    stochastics(&stochasticsBinding),
    observationBinding(callbacks),
    observationNetwork(&observationBinding),
    eventDetectorBinding(callbacks),
    eventDetectorNetwork(&eventDetectorBinding, &world),
    manipulatorBinding(callbacks),
    manipulatorNetwork(&manipulatorBinding, &world),
    modelBinding(frameworkModules.libraryDir, callbacks),
    agentFactory(&modelBinding, &world, &stochastics, &observationNetwork, &eventNetwork),
    sampler(stochastics),
    agentBlueprintProvider(configurationContainer, sampler),
    eventNetwork()
{
}

AgentFactoryInterface* FrameworkModuleContainer::GetAgentFactory()
{
    return &agentFactory;
}

AgentBlueprintProviderInterface* FrameworkModuleContainer::GetAgentBlueprintProvider()
{
    return &agentBlueprintProvider;
}

EventDetectorNetworkInterface* FrameworkModuleContainer::GetEventDetectorNetwork()
{
    return &eventDetectorNetwork;
}

EventNetworkInterface* FrameworkModuleContainer::GetEventNetwork()
{
    return &eventNetwork;
}

ManipulatorNetworkInterface* FrameworkModuleContainer::GetManipulatorNetwork()
{
    return &manipulatorNetwork;
}

ObservationNetworkInterface* FrameworkModuleContainer::GetObservationNetwork()
{
    return &observationNetwork;
}

const SamplerInterface& FrameworkModuleContainer::GetSampler() const
{
    return sampler;
}

SpawnPointNetworkInterface* FrameworkModuleContainer::GetSpawnPointNetwork()
{
    return &spawnPointNetwork;
}

StochasticsInterface* FrameworkModuleContainer::GetStochastics()
{
    return &stochastics;
}

WorldInterface* FrameworkModuleContainer::GetWorld()
{
    return &world;
}

} //namespace SimulationSlave
