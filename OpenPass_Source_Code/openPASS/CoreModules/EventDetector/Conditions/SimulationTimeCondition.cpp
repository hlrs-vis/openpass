/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "SimulationTimeCondition.h"
#include "CoreFramework/CoreShare/log.h"

SimulationTimeCondition::SimulationTimeCondition(const ParameterInterface &parameters) :
    ConditionInterface()
{
    try
    {
        std::string conditionRuleAsString = parameters.GetParametersString().at("rule");
        if (conditionRuleAsString == "less_than")
        {
            rule = Rule::LessThan;
        }
        else if (conditionRuleAsString == "equal_to")
        {
            rule = Rule::EqualTo;
        }
        else if (conditionRuleAsString == "greater_than")
        {
            rule = Rule::GreaterThan;
        }
        else
        {
            throw std::out_of_range("Invalid rule " + conditionRuleAsString + "provided to SimulationTimeCondition");
        }
    }
    catch(const std::out_of_range& error)
    {
        LOG_INTERN(LogLevel::Error) << "Invalid rule provided to SimulationTimeCondition";
        throw error;
    }

    try
    {
        // we are receiving our value in seconds, but time is tracked in ms - we convert here
        targetValue = parameters.GetParametersDouble().at("value") * 1000;
    }
    catch (const std::out_of_range& error)
    {
        LOG_INTERN(LogLevel::Error) << "Invalid value provided to SimulationTimeCondition";
        throw error;
    }
}

bool SimulationTimeCondition::IsMet(double value) const
{
    if (rule == Rule::LessThan)
    {
        return value < targetValue;
    }

    if (rule == Rule::EqualTo)
    {
        return value == targetValue;
    }
    if (rule == Rule::GreaterThan)
    {
        return value > targetValue;
    }

    return false;
}
