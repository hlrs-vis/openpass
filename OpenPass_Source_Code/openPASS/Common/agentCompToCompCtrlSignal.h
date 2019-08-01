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
//! \file  agentCompToCompCtrlSignal.h
//! \brief This file contains all functions for class AgentCompToCompCtrlSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <sstream>

#include "Interfaces/signalInterface.h"
#include "Common/globalDefinitions.h"

class AgentCompToCompCtrlSignal : public SignalInterface
{
public:
    const std::string COMPONENTNAME = "AgentCompToCompCtrlSignal";
    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    AgentCompToCompCtrlSignal(ComponentType componentType,
                              const std::string& agentComponentName,
                              ComponentState currentState):
        componentType(componentType),
        agentComponentName(agentComponentName),
        currentState(currentState)
    {}

    AgentCompToCompCtrlSignal() = delete;
    AgentCompToCompCtrlSignal(const AgentCompToCompCtrlSignal&) = delete;
    AgentCompToCompCtrlSignal(AgentCompToCompCtrlSignal&&) = delete;
    AgentCompToCompCtrlSignal& operator=(const AgentCompToCompCtrlSignal&) = delete;
    AgentCompToCompCtrlSignal& operator=(AgentCompToCompCtrlSignal&&) = delete;

    virtual ~AgentCompToCompCtrlSignal() {}

    //-----------------------------------------------------------------------------
    //! Returns the content/payload of the signal as an std::string
    //!
    //! \return     Content/payload of the signal as an std::string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::ostringstream stream;
        std::string currentStateStr{"Undefined"};

        stream << COMPONENTNAME << ": Intended Component States for " << agentComponentName << ": ";

        for (auto stateStringAndState : ComponentStateMapping)
        {
            if (currentState == stateStringAndState.second)
            {
                currentStateStr = stateStringAndState.first;
            }
        }

        stream << "Current State: " << currentStateStr;

        return stream.str();
    }

    /*!
     * \brief GetComponentType returns the type of the agent component sending the signal
     *
     * \return Type of the component
     */
    ComponentType GetComponentType() const {
        return componentType;
    }

    /*!
     * \brief GetAgentComponentName returns the name of the agent component sending the signal
     *
     * \return The name of the agent component sending the signal as a std::string
     */
    std::string GetAgentComponentName() const {
        return agentComponentName;
    }

    /*!
     * \brief GetCurrentState returns the current state of the agent component sending the signal
     *
     * \return The current state of the agent component sending the signal as a ComponentState
     */
    ComponentState GetCurrentState() const {
        return currentState;
    }

private:
    const ComponentType componentType;      //!< Type of the component
    const std::string agentComponentName;   //!< Name of the component
    const ComponentState currentState;      //!< Current component state
};

class VehicleCompToCompCtrlSignal : public AgentCompToCompCtrlSignal
{
public:
    const std::string COMPONENTNAME = "VehicleCompToCompCtrlSignal";
    VehicleCompToCompCtrlSignal(const ComponentType componentType,
                                               const std::string& agentComponentName,
                                               const ComponentState currentState,
                                               const AdasType adasType):
        AgentCompToCompCtrlSignal(componentType,
                                                 agentComponentName,
                                                 currentState),
        adasType(adasType)
    {}

    VehicleCompToCompCtrlSignal() = delete;
    VehicleCompToCompCtrlSignal(const VehicleCompToCompCtrlSignal&) = delete;
    VehicleCompToCompCtrlSignal(VehicleCompToCompCtrlSignal&&) = delete;
    VehicleCompToCompCtrlSignal& operator=(const VehicleCompToCompCtrlSignal&) = delete;
    VehicleCompToCompCtrlSignal& operator=(VehicleCompToCompCtrlSignal&&) = delete;

    virtual ~VehicleCompToCompCtrlSignal() {}

    AdasType GetAdasType() const {
        return adasType;
    }

private:
    const AdasType adasType;
};
