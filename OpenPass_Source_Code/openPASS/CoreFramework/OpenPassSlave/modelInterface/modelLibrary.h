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
//! @file  ModelLibrary.h
//! @brief This file contains the internal representation of the library of a
//!        model.
//-----------------------------------------------------------------------------

#pragma once

#include <list>
#include <string>
#include <QLibrary>
#include "Interfaces/modelInterface.h"
#include "Interfaces/componentInterface.h"
#include "Interfaces/observationNetworkInterface.h"

namespace SimulationSlave
{

class Agent;
class ComponentType;

class ModelLibrary
{
public:    
    typedef const std::string &(*ModelInterface_GetVersion)();
    typedef ModelInterface *(*ModelInterface_CreateInstanceType)(std::string componentName,
                                                                 bool isInit,
                                                                 int priority,
                                                                 int offsetTime,
                                                                 int responseTime,
                                                                 int cycleTime,
                                                                 StochasticsInterface *stochastics,
                                                                 WorldInterface *world,
                                                                 const ParameterInterface *parameters,
                                                                 const std::map<int, ObservationInterface*> *observationLinks,
                                                                 AgentInterface *agent,
                                                                 const CallbackInterface *callbacks);
    typedef ModelInterface *(*UnrestrictedEventModelInterface_CreateInstanceType)(std::string componentName,
                                                                             bool isInit,
                                                                             int priority,
                                                                             int offsetTime,
                                                                             int responseTime,
                                                                             int cycleTime,
                                                                             StochasticsInterface *stochastics,
                                                                             WorldInterface *world,
                                                                             const ParameterInterface *parameters,
                                                                             const std::map<int, ObservationInterface*> *observationLinks,
                                                                             AgentInterface *agent,
                                                                             const CallbackInterface *callbacks,
                                                                             const SimulationSlave::EventNetworkInterface *eventNetwork);
    typedef void (*ModelInterface_DestroyInstanceType)(ModelInterface *implementation);
    typedef bool (*ModelInterface_UpdateInputType)(ModelInterface *implementation,
                                                   int localLinkId,
                                                   const std::shared_ptr<SignalInterface const> &data,
                                                   int time);
    typedef bool (*ModelInterface_UpdateOutputType)(ModelInterface *implementation,
                                                    int localLinkId,
                                                    std::shared_ptr<SignalInterface const> &data,
                                                    int time);
    typedef bool (*ModelInterface_TriggerType)(ModelInterface *implementation, int time);

    ModelLibrary(const std::string &modelLibraryPath, const std::string &modelLibraryName, CallbackInterface *callbacks) :
        modelLibraryPath(modelLibraryPath),
        modelLibraryName(modelLibraryName),
        callbacks(callbacks)
    {}
    ModelLibrary(const ModelLibrary&) = delete;
    ModelLibrary(ModelLibrary&&) = delete;
    ModelLibrary& operator=(const ModelLibrary&) = delete;
    ModelLibrary& operator=(ModelLibrary&&) = delete;
    virtual ~ModelLibrary();


    //-----------------------------------------------------------------------------
    //! Inits the model library by obtaining the function pointers to all
    //! "interface" functions defined via typedef.
    //!
    //! @return     Flag if init was successful
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    //! Releases a component instance by calling the destroyInstance method
    //! on the instance and removing the instance from the list of components.
    //!
    //! @return     Flag if release was successful
    //-----------------------------------------------------------------------------
    bool ReleaseComponent(ComponentInterface *component);

    //-----------------------------------------------------------------------------
    //! @brief Creates a new agent component and its implementation and link all
    //!     observation modules in the observation network to it.
    //!
    //! Checks that the library exists and is loaded, then creates a new component
    //! using the agent and world parameter, set its model library to this object and
    //! link all observation modules in the observation networks to it. Finally,
    //! create a model interface using the "create instance" function pointer, set
    //! the component's implementation to this model interface and store the created
    //! component in the list of stored components and return it.
    //!
    //! @param[in]     componentType        Agent as defined in the agent config file
    //! @param[in]     componentName        Name of the component
    //! @param[in]     stochastics          The stochastics interface
    //! @param[in]     world                The world interface
    //! @param[in]     observationNetwork   Network of the observation modules
    //! @param[in]     agent                Agent instance
    //! @return
    //-----------------------------------------------------------------------------
    ComponentInterface *CreateComponent(std::shared_ptr<ComponentType> componentType,
                               std::string componentName,
                               StochasticsInterface *stochastics,
                               WorldInterface *world,
                               ObservationNetworkInterface *observationNetwork,
                               Agent *agent,
                               EventNetworkInterface *eventNetwork);

    //-----------------------------------------------------------------------------
    //! Calls the "update input" function pointer with the provided parameters and
    //! returns the result of this call.
    //!
    //! @param[in]     implementation       Model interface
    //! @param[in]     localLinkId          ID of the local link from the agent
    //!                                     configuration
    //! @param[in]     data                 Pointer to the signal interface
    //! @param[in]     time                 Current virtual simulation time
    //! @return                             Flag if update was successful?
    //-----------------------------------------------------------------------------
    bool UpdateInput(ModelInterface *implementation,
                     int localLinkId,
                     const std::shared_ptr<SignalInterface const> &data,
                     int time)
    {
        return updateInputFunc(implementation, localLinkId, data, time);
    }

    //-----------------------------------------------------------------------------
    //! Calls the "update output" function pointer with the provided parameters and
    //! returns the result of this call.
    //!
    //! @param[in]     implementation       Model interface
    //! @param[in]     localLinkId          ID of the local link from the agent
    //!                                     configuration
    //! @param[out]    data                 Pointer to the data as signal interface
    //! @param[in]     time                 Current virtual simulation time
    //! @return                             Flag if update was successful?
    //-----------------------------------------------------------------------------
    bool UpdateOutput(ModelInterface *implementation,
                      int localLinkId,
                      std::shared_ptr<SignalInterface const> &data,
                      int time)
    {
        return updateOutputFunc(implementation, localLinkId, data, time);
    }

    //-----------------------------------------------------------------------------
    //! Calls the "trigger" function pointer with the provided parameters and
    //! returns the result of this call.
    //!
    //! @param[in]     implementation       Model interface
    //! @param[in]     time                 Timepoint at which the trigger happens?
    //! @return                             Flag if triggering was successful?
    //-----------------------------------------------------------------------------
    bool Trigger(ModelInterface *implementation, int time)
    {
        return triggerFunc(implementation, time);
    }

private:    
    const std::string DllGetVersionId = "OpenPASS_GetVersion";
    const std::string DllCreateInstanceId = "OpenPASS_CreateInstance";
    const std::string DllDestroyInstanceId = "OpenPASS_DestroyInstance";
    const std::string DllUpdateInputId = "OpenPASS_UpdateInput";
    const std::string DllUpdateOutputId = "OpenPASS_UpdateOutput";
    const std::string DllTriggerId = "OpenPASS_Trigger";

    std::string modelLibraryPath;
    std::string modelLibraryName;
    std::list<ComponentInterface*> components;
    CallbackInterface *callbacks;
    QLibrary *library = nullptr;
    ModelInterface_GetVersion getVersionFunc{nullptr};
    ModelInterface_CreateInstanceType createInstanceFunc{nullptr};
    UnrestrictedEventModelInterface_CreateInstanceType createEventInstanceFunc{nullptr};
    ModelInterface_DestroyInstanceType destroyInstanceFunc{nullptr};
    ModelInterface_UpdateInputType updateInputFunc{nullptr};
    ModelInterface_UpdateOutputType updateOutputFunc{nullptr};
    ModelInterface_TriggerType triggerFunc{nullptr};
};

} // namespace SimulationSlave


