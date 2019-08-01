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
/** \file  ManipulatorCommonBase.cpp */
//-----------------------------------------------------------------------------

#include "ManipulatorCommonBase.h"

ManipulatorCommonBase::ManipulatorCommonBase(WorldInterface *world,
                                             ParameterInterface* parameters,
                                             SimulationSlave::EventNetworkInterface *eventNetwork,
                                             const CallbackInterface *callbacks) :
    world(world),
    parameters(parameters),
    eventNetwork(eventNetwork),
    callbacks(callbacks)
{
    if (parameters != nullptr)
    {
        try
        {
            if (parameters->GetParametersStringVector().count(EVENT_DETECTOR) > 0)
            {
             std::vector<std::string> eventDetectors = parameters->GetParametersStringVector().at(EVENT_DETECTOR);

                for (std::string eventDetectorName : eventDetectors)
                {
                    eventTypes.push_back(EventDefinitions::eventTypesMap.at(eventDetectorName));
                }
            }

            if (parameters->GetParametersStringVector().count("Actors") > 0)
            {
                actorNames = parameters->GetParametersStringVector().at("Actors");
            }

            auto iterator = parameters->GetParametersString().find("SequenceName");
            if (iterator != parameters->GetParametersString().end())
            {
                sequenceName = (*iterator).second;
            }
        }
        catch(...)
        {
            const std::string msg = COMPONENTNAME + ":CommonBase could not init parameters";
            LOG(CbkLogLevel::Error, msg);
            throw std::runtime_error(msg);
        }
    }
}

ManipulatorCommonBase::ManipulatorCommonBase(WorldInterface *world,
                                             std::shared_ptr<ScenarioActionInterface> action,
                                             SimulationSlave::EventNetworkInterface *eventNetwork,
                                             const CallbackInterface *callbacks) :
    world(world),
    parameters(nullptr),
    eventNetwork(eventNetwork),
    callbacks(callbacks)
{
    actorNames = action->GetActors();
    sequenceName = action->GetSequenceName();

    for (const auto &eventDetectorName : action->GetEventDetectorNames())
    {
        eventTypes.push_back(EventDefinitions::eventTypesMap.at(eventDetectorName));
    }
}

int ManipulatorCommonBase::GetCycleTime()
{
    return cycleTime;
}

int ManipulatorCommonBase::GetPriority()
{
    return static_cast<int>(priority);
}

std::list<AgentInterface*> ManipulatorCommonBase::GetActors(std::shared_ptr<AgentBasedEvent> event)
{
    std::list<AgentInterface*> actors {};

    for(std::string actorName : actorNames)
    {
        actors.push_back(world->GetAgentByName(actorName));
    }

    if(actors.empty())
    {
        actors.push_back(world->GetAgent(event->agentId));
    }

    return actors;
}

std::list<std::shared_ptr<EventInterface>> ManipulatorCommonBase::GetEvents()
{
    std::list<std::shared_ptr<EventInterface>> filteredEvents;

    std::list<std::shared_ptr<EventInterface> > *eventList = eventNetwork->GetActiveEventCategory(eventCategory);

    if(eventList != nullptr)
    {
        for(auto event : *(eventList))
        {
            for(EventDefinitions::EventType eventType : eventTypes)
            {
                if(event->GetEventType() == eventType && event->GetSequenceName() == sequenceName)
                {
                    filteredEvents.push_back(event);
                    break;
                }
            }
        }
    }

    return filteredEvents;
}

void ManipulatorCommonBase::Log(CbkLogLevel logLevel,
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
