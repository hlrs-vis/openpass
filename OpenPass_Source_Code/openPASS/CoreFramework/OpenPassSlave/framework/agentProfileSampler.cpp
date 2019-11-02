/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "agentProfileSampler.h"

void AgentProfileSampler::SampleAgentProfileName(AgentBlueprintInterface &agentBlueprint, LaneCategory laneCategory, unsigned int scenarioAgentIterator)
{
    switch (agentBlueprint.GetAgentCategory())
    {
        case AgentCategory::Ego:
            agentBlueprint.SetAgentProfileName(egoEntity.catalogReference.entryName);
            break;

        case AgentCategory::Scenario:
            if(scenarioAgentIterator < scenarioEntities.size())
            {
                agentBlueprint.SetAgentProfileName(scenarioEntities.at(scenarioAgentIterator)->catalogReference.entryName);
            }
            else
            {
                throw std::runtime_error("Not enough scenario agents defined in scenario");
            }
            break;

        default:
            StringProbabilities possibleAgentProfiles;

            switch(laneCategory)
            {
                case LaneCategory::RegularLane:
                    possibleAgentProfiles = trafficConfig.regularLaneAgents;
                    break;

                case LaneCategory::RightMostLane:
                    possibleAgentProfiles = trafficConfig.rightMostLaneAgents;
                    break;

                default:
                    throw std::logic_error("Invalid LaneCategory could not sample AgentProfile");
            }

            std::string agentProfileName = sampler.SampleStringProbability(possibleAgentProfiles);

            agentBlueprint.SetAgentProfileName(agentProfileName);
    }
}
