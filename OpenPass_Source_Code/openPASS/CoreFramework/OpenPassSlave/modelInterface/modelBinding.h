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
//! @file  modelBinding.h
//! @brief This file contains the interface to the model libraries.
//-----------------------------------------------------------------------------

#ifndef MODELBINDING_H
#define MODELBINDING_H

#include <map>
#include <string>
#include "modelInterface.h"
#include "callbacks.h"
#include "log.h"

namespace SimulationSlave
{

class ModelLibrary;
class Component;
class Agent;
class ComponentType;
class ObservationNetwork;
class FrameworkConfig;

class ModelBinding
{
public:
    ModelBinding(const FrameworkConfig *frameworkConfig,
                 SimulationCommon::Callbacks *callbacks);
    ModelBinding(const ModelBinding&) = delete;
    ModelBinding(ModelBinding&&) = delete;
    ModelBinding& operator=(const ModelBinding&) = delete;
    ModelBinding& operator=(ModelBinding&&) = delete;
    virtual ~ModelBinding();

    //-----------------------------------------------------------------------------
    //! @brief Creates a new component from the given parameters using the respective
    //!         model library
    //!
    //! Gets the model library from the component type (instantiating and initializing
    //! it, if not already done), then creates the new component from the given
    //! parameters using this library.
    //!
    //! @param[in]  componentType       Type of the component to instantiate
    //! @param[in]  componentId         ID of the component to instantiate
    //! @param[in]  stochastics         Stochastics interface
    //! @param[in]  world               World representation
    //! @param[in]  observationNetwork  Network of the observation modules
    //! @param[in]  agent               Agent that the component type is a part of
    //!
    //! @return                         The instantiated component
    //-----------------------------------------------------------------------------
    Component *Instantiate(ComponentType *componentType,
                           int componentId,
                           StochasticsInterface *stochastics,
                           WorldInterface *world,
                           ObservationNetwork *observationNetwork,
                           Agent *agent);

    //-----------------------------------------------------------------------------
    //! Unloads the model library by deleting the stored libraries
    //-----------------------------------------------------------------------------
    void Unload();

private:
    const FrameworkConfig *frameworkConfig;
    std::map<std::string, ModelLibrary*> modelLibraries;
    SimulationCommon::Callbacks *callbacks;
};

} // namespace SimulationSlave

#endif // MODELBINDING_H
