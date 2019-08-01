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
/** \file  ComponentStateChangeManipulator.cpp */
//-----------------------------------------------------------------------------

#include "ComponentStateChangeManipulator.h"
#include "Common/componentStateChangeEvent.h"
#include <QtGlobal>
#include <array>

std::vector<std::string> TokenizeString(const std::string& str, const char delimiter)
{
    std::stringstream stream(str);

    std::string intermediateString;
    std::vector<std::string> tokens;
    while (getline(stream, intermediateString, delimiter))
    {
        boost::algorithm::trim(intermediateString);
        tokens.push_back(intermediateString);
    }

    return tokens;
}

ComponentStateChangeManipulator::ComponentStateChangeManipulator(WorldInterface *world,
                                               ParameterInterface *parameters,
                                               SimulationSlave::EventNetworkInterface *eventNetwork,
                                               const CallbackInterface *callbacks):
    ManipulatorCommonBase(world,
                          parameters,
                          eventNetwork,
                          callbacks)
{
    cycleTime = 100;
    priority = PriorityLevel::PriorityLevelThree;

    eventCategory = EventDefinitions::EventCategory::AgentBased;
    eventType = EventDefinitions::EventType::ComponentStateChange;

    try
    {
        componentName = parameters->GetParametersString().at("Name");
        componentStateName = parameters->GetParametersString().at("ComponentState");
    }
    catch(...)
    {
        const std::string msg = COMPONENTNAME + ":ComponentStateChangeManipulator could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}
ComponentStateChangeManipulator::ComponentStateChangeManipulator(WorldInterface *world,
                                std::shared_ptr<UserDefinedCommandAction> action,
                                SimulationSlave::EventNetworkInterface *eventNetwork,
                                const CallbackInterface *callbacks):
    ManipulatorCommonBase(world,
                          action,
                          eventNetwork,
                          callbacks)
{
    auto commandTokens = TokenizeString(action->GetCommand(), ' ');
    if (commandTokens[0] != "SetComponentState")
    {
        const std::string msg = COMPONENTNAME + ": ComponentStateChangeManipulator provided invalid command for initialization";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }

    try
    {
        componentName = commandTokens[1];
        componentStateName = commandTokens[2];
    }
    catch (const std::out_of_range& error)
    {
        Q_UNUSED(error);
        const std::string msg = COMPONENTNAME + ": ComponentStateChangeManipulator provided insufficient command information";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }

    cycleTime = 100;
    priority = PriorityLevelThree;
    eventCategory = EventDefinitions::EventCategory::AgentBased;
    eventType = EventDefinitions::EventType::ComponentStateChange;
}

void ComponentStateChangeManipulator::Trigger(int time)
{
    Q_UNUSED(time)

    for (std::shared_ptr<EventInterface> eventInterface : GetEvents())
    {
        std::shared_ptr<AgentBasedEvent> triggeringEvent = std::dynamic_pointer_cast<AgentBasedEvent>(eventInterface);

        for (const auto& actor : GetActors(triggeringEvent))
        {
            std::shared_ptr<ComponentChangeEvent> event =
                std::make_shared<ComponentChangeEvent>(time,
                                                            COMPONENTNAME,
                                                            sequenceName,
                                                            eventType,
                                                            actor->GetId(),
                                                            componentName,
                                                            componentStateName);

            event->SetTriggeringEventId(triggeringEvent->GetId());

            eventNetwork->InsertEvent(event);
        }
    }
}
