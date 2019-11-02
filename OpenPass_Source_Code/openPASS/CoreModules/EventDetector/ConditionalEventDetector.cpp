/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "ConditionalEventDetector.h"

#include "SimulationTimeCondition.h"

ConditionalEventDetector::ConditionalEventDetector(WorldInterface *world,
                                                   ParameterInterface *parameters,
                                                   SimulationSlave::EventNetworkInterface *eventNetwork,
                                                   const CallbackInterface *callbacks,
                                                   StochasticsInterface *stochastics) :
           EventDetectorCommonBase(world,
                                   parameters,
                                   eventNetwork,
                                   callbacks,
                                   stochastics)
{
    ParseParameters(parameters);
    eventType = EventDefinitions::EventType::Conditional;
}

void ConditionalEventDetector::ParseParameters(const ParameterInterface* const parameters)
{
    if (parameters)
    {
        try
        {
            std::shared_ptr<ConditionInterface> newCondition;
            const auto conditionParametersListIterator = parameters->GetParameterLists().find("conditions");
            if (conditionParametersListIterator != parameters->GetParameterLists().end())
            {
                const auto &conditionParametersList = conditionParametersListIterator->second;

                for( const auto &conditionParameters : conditionParametersList )
                {
                    if(conditionParameters->GetParametersString().at("type") == "SimulationTimeCondition")
                    {
                        newCondition = std::make_shared<SimulationTimeCondition>(*conditionParameters);
                        conditions.emplace_back(newCondition);
                    }
                }
            }
        }
        catch(...)
        {
            const std::string msg = COMPONENTNAME + ":ConditionalEventDetector could not init parameters";
            LOG(CbkLogLevel::Error, msg);
            throw std::runtime_error(msg);
        }
    }
}

void ConditionalEventDetector::Trigger(int time)
{
    if (!triggered && AllConditionsMet(time))
    {
        TriggerEventInsertion(time);
    }
}

std::list<std::shared_ptr<ConditionInterface>> ConditionalEventDetector::GetConditions() const
{
    return conditions;
}

bool ConditionalEventDetector::AllConditionsMet(int time) const
{
    return std::all_of(conditions.begin(),
                       conditions.end(),
                       [time](const auto& condition)
                       {
                           return condition->IsMet(time);
                       });
}

void ConditionalEventDetector::TriggerEventInsertion(int time)
{
    for(const auto agent : GetActingScenarioAgents())
    {
        std::shared_ptr<AgentBasedEvent> event = std::make_shared<AgentBasedEvent>(time,
                                                                                   COMPONENTNAME,
                                                                                   sequenceName,
                                                                                   eventType,
                                                                                   agent->GetId());

        eventNetwork->InsertEvent(event);
    }
    triggered = true;
}
