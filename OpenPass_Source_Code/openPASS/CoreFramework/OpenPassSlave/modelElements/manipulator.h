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
//! @file  Manipulator.h
//! @brief This file contains the internal representation of the manipulator
//-----------------------------------------------------------------------------

#pragma once

#include <list>

#include "CoreFramework/CoreShare/parameters.h"
#include "Interfaces/manipulatorInterface.h"
#include "manipulatorLibrary.h"
#include "Interfaces/worldInterface.h"
#include "CoreFramework/CoreShare/log.h"

namespace SimulationSlave
{

class Manipulator
{
public:
    Manipulator(ManipulatorInterface *implementation,
                  ManipulatorLibrary *library) :
        library(library),
        implementation(implementation)
    {
        LOG_INTERN(LogLevel::DebugCore) << "created manipulator";
    }
    Manipulator(const Manipulator&) = delete;
    Manipulator(Manipulator&&) = delete;
    Manipulator& operator=(const Manipulator&) = delete;
    Manipulator& operator=(Manipulator&&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor, releases the manipulator from the stored library.
    //-----------------------------------------------------------------------------
    virtual ~Manipulator()
    {
        library->ReleaseManipulator(this);
        LOG_INTERN(LogLevel::DebugCore) << "destroyed manipulator ";
    }

    //-----------------------------------------------------------------------------
    //! Returns the manipulator interface.
    //!
    //! @return                         Manipulator interface
    //-----------------------------------------------------------------------------
    ManipulatorInterface *GetImplementation() const
    {
        return implementation;
    }

    //-----------------------------------------------------------------------------
    //! Returns the manipulator library.
    //!
    //! @return                         Manipultar library
    //-----------------------------------------------------------------------------
    ManipulatorLibrary *GetLibrary()
    {
        return library;
    }

private:
    ManipulatorLibrary *library;
    ManipulatorInterface *implementation;
};

} // namespace SimulationSlave


