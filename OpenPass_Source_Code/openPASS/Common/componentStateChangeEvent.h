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
/** \addtogroup ComponentStateChangeEvent
* @{
* \file ComponentStateChangeEvent.h
* \brief This file contains the behaviours for ComponentStateChange events
*
* \details ComponentStateChange Events are intended to be used in conjunction
*          with the Vehicle Control Unit to toggle Vehicle Components
* @}
* */
//-----------------------------------------------------------------------------

#pragma once

#include "agentBasedEvent.h"
#include "Interfaces/signalInterface.h"

//-----------------------------------------------------------------------------
/** This class implements all functionality of the ComponentStateChangeEvent.
 *
 * \ingroup Event */
//-----------------------------------------------------------------------------
class ComponentChangeEvent : public AgentBasedEvent
{
public:
    ComponentChangeEvent(int time,
                              const std::string& source,
                              const std::string& sequenceName,
                              EventDefinitions::EventType eventType,
                              int agentId,
                              const std::string& componentName,
                              const std::string& goalStateName):
        AgentBasedEvent(time,
                        source,
                        sequenceName,
                        eventType,
                        agentId),
        componentName(componentName),
        goalStateName(goalStateName)
    {
        goalState = ComponentStateMapping.at(goalStateName);
    }
    ComponentChangeEvent(const AgentBasedEvent&) = delete;
    ComponentChangeEvent(AgentBasedEvent&&) = delete;
    ComponentChangeEvent& operator=(const AgentBasedEvent&) = delete;
    ComponentChangeEvent& operator=(AgentBasedEvent&&) = delete;
    virtual ~ComponentChangeEvent() = default;

    /*!
     * \brief Returns the component name for which the event is targeted
     * @return Component Name
     */
    std::string GetComponentName() const {
        return componentName;
    }

    /*!
     * \brief Returns the goal state name for which the event is targeted
     * @return State Name
     */
    std::string GetGoalStateName() const {
        return goalStateName;
    }

    /*!
     * \brief Returns the goal state as a result of the event
     * @return State
     */
    ComponentState GetGoalState() const {
        return goalState;
    }

    /*!
    * \brief Returns all parameters of the event as string list.
    * \details Returns the agentId as string list.
    *
    * @return	     List of string pairs of the event parameters.
    */
    virtual std::list<std::pair<std::string, std::string>> GetEventParametersAsString()
    {
        std::list<std::pair<std::string, std::string>> eventParameters = AgentBasedEvent::GetEventParametersAsString();

        eventParameters.push_back(std::pair<std::string, std::string>("ComponentName", componentName));
        eventParameters.push_back(std::pair<std::string, std::string>("State", goalStateName));

        return eventParameters;
    }

private:
    const std::string componentName;
    const std::string goalStateName;
    ComponentState goalState = ComponentState::Undefined;
};

