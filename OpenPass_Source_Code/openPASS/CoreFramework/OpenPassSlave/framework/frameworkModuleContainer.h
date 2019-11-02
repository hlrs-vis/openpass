/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  FrameworkModuleContainer.h
*   \brief This file stores all core framework modules
*/
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/frameworkModuleContainerInterface.h"

#include "agentFactory.h"
#include "agentBlueprintProvider.h"
#include "Interfaces/configurationContainerInterface.h"
#include "directories.h"
#include "eventDetectorBinding.h"
#include "eventDetectorNetwork.h"
#include "eventNetwork.h"
#include "frameworkModules.h"
#include "manipulatorBinding.h"
#include "manipulatorNetwork.h"
#include "modelBinding.h"
#include "observationBinding.h"
#include "observationNetwork.h"
#include "sampler.h"
#include "spawnPointBinding.h"
#include "spawnPointNetwork.h"
#include "stochastics.h"
#include "stochasticsBinding.h"
#include "world.h"

namespace SimulationSlave {

//-----------------------------------------------------------------------------
/** \brief This class instantiates and stores all core framework modules
*   \details
*/
//-----------------------------------------------------------------------------
class FrameworkModuleContainer : public FrameworkModuleContainerInterface
{
public:
    FrameworkModuleContainer(FrameworkModules frameworkModules,
                             ConfigurationContainerInterface* configurationContainer,
                             CallbackInterface* callbacks);

    virtual ~FrameworkModuleContainer() override = default;

    AgentFactoryInterface* GetAgentFactory() override;

    EventDetectorNetworkInterface* GetEventDetectorNetwork() override;

    EventNetworkInterface* GetEventNetwork() override;

    ManipulatorNetworkInterface* GetManipulatorNetwork() override;

    ObservationNetworkInterface* GetObservationNetwork() override;

    const SamplerInterface& GetSampler() const override;

    SpawnPointNetworkInterface* GetSpawnPointNetwork() override;

    StochasticsInterface* GetStochastics() override;

    WorldInterface* GetWorld() override;

    AgentBlueprintProviderInterface* GetAgentBlueprintProvider() override;

private:
    WorldBinding worldBinding;
    World world;

    SpawnPointBinding spawnPointBinding;
    SpawnPointNetwork spawnPointNetwork;

    StochasticsBinding stochasticsBinding;
    Stochastics stochastics;

    ObservationBinding observationBinding;
    ObservationNetwork observationNetwork;

    EventDetectorBinding eventDetectorBinding;
    EventDetectorNetwork eventDetectorNetwork;

    ManipulatorBinding manipulatorBinding;
    ManipulatorNetwork manipulatorNetwork;

    ModelBinding modelBinding;

    AgentFactory agentFactory;

    const Sampler sampler;

    AgentBlueprintProvider agentBlueprintProvider;

    EventNetwork eventNetwork;
};

} //namespace SimulationSlave
