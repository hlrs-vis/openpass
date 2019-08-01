/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "Interfaces/configurationContainerInterface.h"
#include "Interfaces/samplerInterface.h"
#include "Interfaces/agentBlueprintInterface.h"

/*!
 * \brief The AgentProfileSampler class is used by the AgentBlueprintProvider and samplies the name of the AgentProfile
 */
class AgentProfileSampler
{
public:
    AgentProfileSampler(ConfigurationContainerInterface *configurationContainer, const SamplerInterface& sampler) :
        scenarioEntities(configurationContainer->GetScenario()->GetScenarioEntities()),
        egoEntity(configurationContainer->GetScenario()->GetEgoEntity()),
        trafficConfig(configurationContainer->GetSlaveConfig()->GetTrafficConfig()),
        sampler(sampler)
    {}

    /*!
    * \brief Samples the agent profile name
    *
    * \details  Samples the agent profile name according to the AgentCategory and the LaneCategory
    *           and inserts it into the blueprint
    *
    * @param[in/out]    agentBlueprint              Contains the AgentCategory and the result gets stored here
    * @param[in]        laneCategory                LaneCategory of the agent only relevant in case of common agents
    * @param[in]        scenarioAgentIterator       Iterator for scenario agent arrays.
    */
    void SampleAgentProfileName(AgentBlueprintInterface &agentBlueprint, LaneCategory laneCategory, unsigned int scenarioAgentIterator);

private:
    const std::vector<ScenarioEntity*>& scenarioEntities;
    const ScenarioEntity& egoEntity;
    const TrafficConfig& trafficConfig;
    const SamplerInterface& sampler;
};
