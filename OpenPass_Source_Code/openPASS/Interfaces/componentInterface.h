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
//! @file  ComponentInterface.h
//! @brief This file contains the interface of the component to interact
//!        with the framework.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"

namespace SimulationSlave {

class Channel;
class ModelLibrary;
class Agent;
class ChannelBuffer;
class ObservationModule;

class ComponentInterface
{
public:
    ComponentInterface() = default;
    ComponentInterface(const ComponentInterface&) = default;
    ComponentInterface(ComponentInterface&&) = default;
    ComponentInterface& operator=(const ComponentInterface&) = default;
    ComponentInterface& operator=(ComponentInterface&&) = default;
    virtual ~ComponentInterface() = default;

    //-----------------------------------------------------------------------------
    //! Sets the model interface implementation.
    //!
    //! @param[in]     implementation       Model interface implementation to set
    //-----------------------------------------------------------------------------
    virtual void SetImplementation(ModelInterface* implementation) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the stored agent.
    //!
    //! @return                             Stored agent
    //-----------------------------------------------------------------------------
    virtual Agent* GetAgent() const = 0;

    //-----------------------------------------------------------------------------
    //! Adds the provided channel with the provided ID to the stored list of input
    //! channels.
    //!
    //! @param[in]     input                Input channel to add
    //! @param[in]     linkId               ID of the channel to add
    //! @return                             Flag if adding the channel was successful
    //-----------------------------------------------------------------------------
    virtual bool AddInputLink(Channel* input, int linkId) = 0;

    //-----------------------------------------------------------------------------
    //! Adds the provided channel with the provided ID to the stored list of output
    //! channels.
    //!
    //! @param[in]     output               Output channel to add
    //! @param[in]     linkId               ID of the channel to add
    //! @return                             Flag if adding the channel was successful
    //-----------------------------------------------------------------------------
    virtual bool AddOutputLink(Channel* output, int linkId) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the observation modules map of the component
    //!
    //! @param[in]     observations          Observation modules to add
    //-----------------------------------------------------------------------------
    virtual void SetObservations(const std::map<int, ObservationModule*>& observations) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the map of IDs to stored input channels.
    //!
    //! @return                             Map of IDs to stored input channels
    //-----------------------------------------------------------------------------
    virtual std::map<int, Channel*>& GetInputLinks() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the map of IDs to stored output channels.
    //!
    //! @return                             Map of IDs to stored output channels
    //-----------------------------------------------------------------------------
    virtual std::map<int, Channel*>& GetOutputLinks() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the map of IDs to stored observation modules.
    //!
    //! @return                             Map of IDs to stored observation modules
    //-----------------------------------------------------------------------------
    virtual const std::map<int, ObservationInterface*>& GetObservations() const = 0;

    //-----------------------------------------------------------------------------
    //! Calls the Trigger method on the stored model library with the stored model
    //! interface implementation and the time parameter provided.
    //!
    //! @param[in]     time                 Observation module (as interface)to add
    //! @return                             False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool TriggerCycle(int time) = 0;

    //-----------------------------------------------------------------------------
    //! Gets the data on the channel with the provided ID by updating the output
    //! via the model library using the stored model instance implementation and
    //! the provided time stamp, then setting this data on the channel with the
    //! provided ID.
    //!
    //! @param[in]     linkId               ID of the output channel
    //! @param[in]     time                 Time stamp for the output
    //! @return                             False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool AcquireOutputData(int linkId, int time) = 0;

    //-----------------------------------------------------------------------------
    //! Releases the data on the channel with the provided ID.
    //!
    //! @param[in]     linkId               ID of the output channel
    //! @return                             False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool ReleaseOutputData(int linkId) = 0;

    //-----------------------------------------------------------------------------
    //! Updates the input data on the input channelwith the provided ID by updating
    //! the input via the model library using the stored model instance implementation
    //! and the provided time stamp.
    //!
    //! @param[in]     linkId               ID of the input channel
    //! @param[in]     time                 Time stamp for the output
    //! @return                             False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool UpdateInputData(int linkId, int time) = 0;

    //-----------------------------------------------------------------------------
    //! Creates a new channel buffer with the provided ID, store it in the stored
    //! list of output channel buffers and return it.
    //!
    //! @param[in]     linkId               ID of the channel output buffer to create
    //! @return                             Created output channel buffer
    //-----------------------------------------------------------------------------
    virtual ChannelBuffer* CreateOutputBuffer(int linkId) = 0;

    //-----------------------------------------------------------------------------
    //! Insert the provided channel buffer with the provided ID in the list of stored
    //! input channel buffers.
    //!
    //! @param[in]     linkId               ID of the channel input buffer
    //! @param[in]     buffer               Channel input bufer to set
    //! @return                             False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool SetInputBuffer(int linkId, ChannelBuffer* buffer) = 0;

    //-----------------------------------------------------------------------------
    //! Returns if the stored model interface implementation is defined as init module.
    //!
    //! @return                             Returns false if error occured, otherwise
    //!                                     the flag if stored model interface
    //!                                     implementation is defined as init module
    //-----------------------------------------------------------------------------
    virtual bool GetInit() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the priority of the stored model interface implementation.
    //!
    //! @return                             Returns std::numeric_limits<int>::min()
    //!                                     if error occured, otherwise the priority
    //!                                     of the stored model interface implementation
    //-----------------------------------------------------------------------------
    virtual int GetPriority() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the offset time of the stored model interface implementation, i.e.
    //! the time at which this item is inserted for scheduling.
    //!
    //! @return                             Returns 0 if error occured, otherwise
    //!                                     the offset time of the stored model
    //!                                     interface implementation
    //-----------------------------------------------------------------------------
    virtual int GetOffsetTime() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the response time of the stored model interface implementation.
    //!
    //! @return                             Returns 0 if error occured, otherwise
    //!                                     the respone time of the stored model
    //!                                     interface implementation
    //-----------------------------------------------------------------------------
    virtual int GetResponseTime() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the cycle time of the stored model interface implementation, i.e.
    //! the time after which it is rescheduled.
    //!
    //! @return                             Returns 0 if error occured, otherwise
    //!                                     the cycle time of the stored model
    //!                                     interface implementation
    //-----------------------------------------------------------------------------
    virtual int GetCycleTime() const = 0;

    //-----------------------------------------------------------------------------
    //! Set the provided model library as library to store.
    //!
    //! @param[in]     modelLibrary         Model library to set
    //! @return                             False if library is already set, true
    //!                                     otherwise
    //-----------------------------------------------------------------------------
    virtual bool SetModelLibrary(ModelLibrary* modelLibrary) = 0;

    //-----------------------------------------------------------------------------
    //! Releases this component from the stored library.
    //!
    //! @return                             Result of releasing this component
    //-----------------------------------------------------------------------------
    virtual bool ReleaseFromLibrary() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the stored model interface instance.
    //!
    //! @return                             Stored model interface instance
    //-----------------------------------------------------------------------------
    virtual ModelInterface* GetImplementation() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the stored ID.
    //!
    //! @return                             Stored ID
    //-----------------------------------------------------------------------------
    virtual std::string GetName() const = 0;
};

} // namespace SimulationSlave


