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
/** @file  EventDetectorCommonBase.cpp */
//-----------------------------------------------------------------------------

#include "EventDetectorCommonBase.h"

EventDetectorCommonBase::EventDetectorCommonBase(WorldInterface *world,
                                                 ParameterInterface* parameters,
                                                 SimulationSlave::EventNetworkInterface *eventNetwork,
                                                 const CallbackInterface *callbacks,
                                                 StochasticsInterface *stochastics):
    world(world),
    eventNetwork(eventNetwork),
    callbacks(callbacks),
    parameters(parameters),
    stochastics(stochastics)
{
    if (parameters != nullptr)
    {
        auto triggeringAgentsParameterIterator = parameters->GetParametersStringVector().find("TriggeringAgents");
        if (triggeringAgentsParameterIterator != parameters->GetParametersStringVector().end())
        {
            triggeringAgents = (*triggeringAgentsParameterIterator).second;
        }

        auto actingAgentsParameterIterator = parameters->GetParametersStringVector().find("Actors");
        if (actingAgentsParameterIterator != parameters->GetParametersStringVector().end())
        {
            actingAgents = (*actingAgentsParameterIterator).second;
        }

        auto parametersStringIterator = parameters->GetParametersString().find("SequenceName");
        if (parametersStringIterator != parameters->GetParametersString().end())
        {
            sequenceName = (*parametersStringIterator).second;
        }

        parametersStringIterator = parameters->GetParametersString().find("AgentGroup");
        if (parametersStringIterator != parameters->GetParametersString().end())
        {
            std::string agentGroupName = (*parametersStringIterator).second;

            auto agentCategoryIt = agentCategoryMap.find(agentGroupName);
            if (agentCategoryIt != agentCategoryMap.end())
            {
                agentCategory = (*agentCategoryIt).second;
            }
        }
    }
}

EventDetectorCommonBase::~EventDetectorCommonBase()
{

}

int EventDetectorCommonBase::GetCycleTime()
{
    return cycleTime;
}

void EventDetectorCommonBase::Reset()
{
    return;
}

void EventDetectorCommonBase::Log(CbkLogLevel logLevel,
         const char *file,
         int line,
         const std::string &message)
{
    if(callbacks)
    {
        callbacks->Log(logLevel,
                       file,
                       line,
                       message);
    }
}

std::list<AgentInterface *> EventDetectorCommonBase::GetTriggeringScenarioAgents()
{
    if(triggeringAgents.size() > 0 )
    {
        std::list<AgentInterface *> agents;

        for(std::string agentName : triggeringAgents)
        {
            AgentInterface *agent = world->GetAgentByName(agentName);
            if(agent != nullptr)
            {
                agents.push_back(agent);
            }
        }
        return agents;
    }
    else
    {
       return world->GetAgentsByGroupType(agentCategory);
    }
}

std::list<AgentInterface *> EventDetectorCommonBase::GetActingScenarioAgents()
{

    if(actingAgents.size() > 0 )
    {
        std::list<AgentInterface *> agents;

        for(std::string agentName : actingAgents)
        {
            AgentInterface *agent = world->GetAgentByName(agentName);
            if(agent != nullptr)
            {
                agents.push_back(agent);
            }
        }

        return agents;
    }
    else
    {
       return world->GetAgentsByGroupType(agentCategory);
    }
}
