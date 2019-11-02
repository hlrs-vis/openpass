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
//! @file  ManipulatorBinding.h
//! @brief This file contains the interface to the event detector libraries.
//-----------------------------------------------------------------------------

#pragma once

#include "CoreFramework/CoreShare/callbacks.h"
#include "Interfaces/worldInterface.h"
#include "Interfaces/scenarioInterface.h"

namespace SimulationSlave
{

class ManipulatorLibrary;
class Manipulator;
class EventNetworkInterface;

class ManipulatorBinding
{
public:
    ManipulatorBinding(CallbackInterface *callbacks);
    ManipulatorBinding(const ManipulatorBinding&) = delete;
    ManipulatorBinding(ManipulatorBinding&&) = delete;
    ManipulatorBinding& operator=(const ManipulatorBinding&) = delete;
    ManipulatorBinding& operator=(ManipulatorBinding&&) = delete;
    virtual ~ManipulatorBinding();

    //-----------------------------------------------------------------------------
    //! Creates all manipulators and returns them as a vector
    //!
    //! @param[in]  libraryPath         Path to the library
    //! @param[in]  scenario            Scenario
    //! @param[in]  eventNetwork        Interface of the eventNetwork
    //! @param[in]  world               World instance
    //! @return                         Vector of created manipulators
    //-----------------------------------------------------------------------------
    std::vector<const Manipulator *> Instantiate(const std::string libraryPath,
                                                 ScenarioInterface *scenario,
                                                 EventNetworkInterface *eventNetwork,
                                                 WorldInterface *world);

    //-----------------------------------------------------------------------------
    //! Unloads the spawn point binding by deleting all stored spawn point libraries.
    //-----------------------------------------------------------------------------
    void Unload();

private:
    void ValidateManipulator(Manipulator *manipulator, std::vector<Manipulator *> *manipulatorList, std::string manipulatorType);

    std::vector<std::string> defaultManipulatorTypes = {"CollisionManipulator"};

    ManipulatorLibrary* library = nullptr;
    CallbackInterface *callbacks {nullptr};
};

} // namespace SimulationSlave


