/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  ObservationBinding.h
//! @brief This file contains the interface to the observation module
//!        libraries.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include <string>
#include "Interfaces/eventNetworkInterface.h"
#include "Interfaces/stochasticsInterface.h"
#include "Interfaces/worldInterface.h"
#include "callbacks.h"

namespace SimulationSlave {
class ObservationLibrary;
class ObservationModule;

class ObservationBinding
{
public:
    ObservationBinding(CallbackInterface* callbacks);
    ObservationBinding(const ObservationBinding&) = delete;
    ObservationBinding(ObservationBinding&&) = delete;
    ObservationBinding& operator=(const ObservationBinding&) = delete;
    ObservationBinding& operator=(ObservationBinding&&) = delete;
    virtual ~ObservationBinding();

    //-----------------------------------------------------------------------------
    //! Creates an observation module based on the observation instance in the run
    //! config and the observation library stored in the mapping for the library
    //! name of the observation instance
    //!
    //! @param[in]     libraryPath          Path of the library
    //! @param[in]     parameters           Observation parameters
    //! @param[in]     stochastics          The stochastics interface
    //! @param[in]     world                The world interface
    //! @param[in]     eventNetwork         EventNetwork
    //! @return                             Observation module created from the
    //!                                     observation instance
    //-----------------------------------------------------------------------------
    ObservationModule* Instantiate(const std::string libraryPath,
                                   ParameterInterface* parameters,
                                   StochasticsInterface* stochastics,
                                   WorldInterface* world,
                                   SimulationSlave::EventNetworkInterface* eventNetwork);
    //-----------------------------------------------------------------------------
    //! Deletes the library mapping and all referenced observation library objects
    //-----------------------------------------------------------------------------
    void Unload();

private:
    ObservationLibrary* library {nullptr};
    CallbackInterface* callbacks {nullptr};
};

} // namespace SimulationSlave


