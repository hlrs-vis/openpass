/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "Interfaces/agentFactoryInterface.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "Interfaces/configurationContainerInterface.h"
#include "Interfaces/eventDetectorNetworkInterface.h"
#include "Interfaces/eventNetworkInterface.h"
#include "Interfaces/manipulatorNetworkInterface.h"
#include "Interfaces/observationNetworkInterface.h"
#include "Interfaces/samplerInterface.h"
#include "Interfaces/spawnPointNetworkInterface.h"
#include "Interfaces/stochasticsInterface.h"
#include "Interfaces/worldInterface.h"

namespace SimulationSlave {

class FrameworkModuleContainerInterface
{
public:
    virtual ~FrameworkModuleContainerInterface() = default;

    /*!
    * \brief Returns a pointer to the AgentFactory
    *
    * @return        AgentFactory pointer
    */
    virtual AgentFactoryInterface* GetAgentFactory() = 0;

    /*!
    * \brief Returns a pointer to the EventDetectorNetwork
    *
    * @return        EventDetectorNetwork pointer
    */
    virtual EventDetectorNetworkInterface* GetEventDetectorNetwork() = 0;

    /*!
    * \brief Returns a pointer to the EventNetwork
    *
    * @return        EventNetwork pointer
    */
    virtual EventNetworkInterface* GetEventNetwork() = 0;

    /*!
    * \brief Returns a pointer to the ManipulatorNetwork
    *
    * @return        ManipulatorNetwork pointer
    */
    virtual ManipulatorNetworkInterface* GetManipulatorNetwork() = 0;

    /*!
    * \brief Returns a pointer to the ObservationNetwork
    *
    * @return        ObservationNetwork pointer
    */
    virtual ObservationNetworkInterface* GetObservationNetwork() = 0;

    /*!
    * \brief Returns the instance of the used sampler
    *
    * @return        Sampler
    */
    virtual const SamplerInterface& GetSampler() const = 0;

    /*!
    * \brief Returns a pointer to the SpawnPointNetwork
    *
    * @return        SpawnPointNetwork pointer
    */
    virtual SpawnPointNetworkInterface* GetSpawnPointNetwork() = 0;

    /*!
    * \brief Returns a pointer to the Stochastics
    *
    * @return        Stochastics pointer
    */
    virtual StochasticsInterface* GetStochastics() = 0;

    /*!
    * \brief Returns a pointer to the World
    *
    * @return        World pointer
    */
    virtual WorldInterface* GetWorld() = 0;


    /*!
    * \brief Returns a pointer to the agentBlueprintProvider
    *
    * @return        agentBlueprintProvider pointer
    */
    virtual AgentBlueprintProviderInterface* GetAgentBlueprintProvider() = 0;
};

} //namespace SimulationSlave
