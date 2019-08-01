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
//! \file  CompCtrlToAgentCompSignal.h
//! \brief This file contains all functions for class CompCtrlToAgentCompSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include <string>

#include "Interfaces/signalInterface.h"
#include "Common/globalDefinitions.h"

class CompCtrlToAgentCompSignal : public SignalInterface
{
public:
    const std::string COMPONENTNAME = "CompCtrlToAgentCompSignal";
    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    CompCtrlToAgentCompSignal(const ComponentState &maxReachableState,
                              const std::map<std::string, std::pair<ComponentType, ComponentState>> &vehicleComponentStates):
        maxReachableState(maxReachableState),
        vehicleComponentStates(vehicleComponentStates)
    {}

    CompCtrlToAgentCompSignal() = delete;
    CompCtrlToAgentCompSignal(const CompCtrlToAgentCompSignal&) = delete;
    CompCtrlToAgentCompSignal(CompCtrlToAgentCompSignal&&) = delete;
    CompCtrlToAgentCompSignal& operator=(const CompCtrlToAgentCompSignal&) = delete;
    CompCtrlToAgentCompSignal& operator=(CompCtrlToAgentCompSignal&&) = delete;

    virtual ~CompCtrlToAgentCompSignal() = default;

    //-----------------------------------------------------------------------------
    //! Returns the content/payload of the signal as an std::string
    //!
    //! \return     Content/payload of the signal as an std::string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        return "";
    }

    /*!
     * \brief GetMaxReachableState returns the max reachable state of the target component as defined by the ComponentController
     *
     * \return The max reachable state of the target component as defined by the ComponentController as a ComponentState
     */
    ComponentState GetMaxReachableState() const {
        return maxReachableState;
    }

    /*!
     * \brief GetVehicleComponentStates returns a map linking vehicle component names to types and states
     *
     * \return A map of vehicle component names to pairs of types and states as std::map<std::string, std::pair<AdasType, ComponentState>>
     */
    const std::map<std::string, std::pair<ComponentType, ComponentState>>& GetVehicleComponentStates() const {
        return vehicleComponentStates;
    }

private:
    const ComponentState maxReachableState;     //!< Highest reachable state
    const std::map<std::string, std::pair<ComponentType, ComponentState>> vehicleComponentStates;   //!< Current state of all components
};
