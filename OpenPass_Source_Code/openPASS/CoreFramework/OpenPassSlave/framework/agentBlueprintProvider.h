/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  AgentBlueprintProvider.h
//! @brief This file contains the AgentBlueprintProvider, which creates agentBlueprints according to
//!  either a given SceneryConfiguration or based on a sampled agent.
//-----------------------------------------------------------------------------

#pragma once

#include "dynamicAgentTypeGenerator.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "Interfaces/agentBlueprintInterface.h"
#include "agentProfileSampler.h"
#include "Interfaces/configurationContainerInterface.h"

class AgentBlueprintProvider : public AgentBlueprintProviderInterface
{
public:
    AgentBlueprintProvider(ConfigurationContainerInterface* configurationContainer, const SamplerInterface& sampler);

    /*!
    * \brief Samples an entire agent
    *
    * \details First samples the agent profile and then depending on wether it is a static or dynamic profile samples the dynamic
    * profiles and builds an AgentType from this or loads the AgentType from the specified SystemConfig
    *
    * @param[in/out]    agentBlueprint          All results get stored in the agent blueprint
    * @param[in]        laneCategory            Category of the lane the agent will be spawned in only relevant for common agents
    * @param[in]        scenarioAgentIterator    Iterator for scenario agent arrays.
    *
    * @return           true if Agent was succesfully sampled
    */
    virtual bool SampleAgent(AgentBlueprintInterface& agentBlueprint, LaneCategory laneCategory,
                             unsigned int scenarioAgentIterator) override;

    /*!
     * \brief Store sampled information in AgentBlueprint for dynamic AgentProfile
     * \param agentBlueprint            All results get stored in the agent blueprint
     * \param agentBuildInformation     AgentBuildInformation with information to store into the agentBlueprint
     * \param driverProfileName         name of sampled DriverProfile
     * \return
     */
    bool GenerateDynamicAgentBlueprint(AgentBlueprintInterface& agentBlueprint, AgentBuildInformation agentBuildInformation,
                                       std::string& driverProfileName);

    /*!
    * \brief Store sampled information in AgentBlueprint for static AgentProfile
    *
    * @param[in/out]    agentBlueprint          All results get stored in the agent blueprint
    * @param[in]        systemConfigName        Name of systemConfiguration holding the system information
    * @param[in]        systemId                Id of system to build (referes to given systemConfig
    * @param[in]        vehicleModelName        Name of vehicle model for system
    *
    * @return           true if Agent was succesfully sampled
    */
    bool GenerateStaticAgentBlueprint(AgentBlueprintInterface& agentBlueprint, std::string systemConfigName, int systemId,
                                      std::string vehicleModelName);

private:
    AgentProfileSampler agentProfileSampler;
    const SamplerInterface& sampler;
    ProfilesInterface* profiles;
    std::unordered_map<std::string, AgentProfile>& agentProfiles;
    VehicleModelsInterface* vehicleModels {nullptr};
    std::shared_ptr<SystemConfigInterface> systemConfigBlueprint;
    std::map<std::string, std::shared_ptr<SystemConfigInterface>>& systemConfigs;

};
