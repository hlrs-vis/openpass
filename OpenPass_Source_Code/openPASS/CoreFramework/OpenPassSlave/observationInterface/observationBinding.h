/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  observationBinding.h
//! @brief This file contains the interface to the observation module
//!        libraries.
//-----------------------------------------------------------------------------

#ifndef OBSERVATIONBINDING_H
#define OBSERVATIONBINDING_H

#include <map>
#include <string>
#include "stochasticsInterface.h"
#include "worldInterface.h"
#include "runConfig.h"
#include "callbacks.h"

namespace SimulationSlave
{

class ObservationLibrary;
class ObservationModule;
class FrameworkConfig;

class ObservationBinding
{
public:
    ObservationBinding(const FrameworkConfig *frameworkConfig,
                       SimulationCommon::Callbacks *callbacks);
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
    //! @param[in]     observationInstance  Observation instance from the run config
    //! @param[in]     stochastics          The stochastics interface
    //! @param[in]     world                The world interface
    //! @return                             Observation module created from the
    //!                                     observation instance
    //-----------------------------------------------------------------------------
    ObservationModule *Instantiate(SimulationCommon::RunConfig::ObservationInstance *observationInstance,
                                   StochasticsInterface *stochastics,
                                   WorldInterface *world);
    //-----------------------------------------------------------------------------
    //! Deletes the library mapping and all referenced observation library objects
    //-----------------------------------------------------------------------------
    void Unload();

private:
    const FrameworkConfig *frameworkConfig;
    std::map<std::string, ObservationLibrary*> libraries;
    SimulationCommon::Callbacks *callbacks;
};

} // namespace SimulationSlave

#endif // OBSERVATIONBINDING_H
