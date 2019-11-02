/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <utility>
#include <memory>
#include "Interfaces/callbackInterface.h"
#include "componentControllerCommon.h"
#include "componentStateInformation.h"
#include "Common/componentStateChangeEvent.h"

namespace ComponentControl {

/*!
 * \brief The StateManager class handles the dependencies of all components
 */
class StateManager
{
public:
    StateManager(const CallbackInterface *callbacks):
        callbacks(callbacks)
    {
    }

    /*!
     * \brief Adds a new component which has dependencies to other components
     *
     * \param name      name of the component
     * \param config    dependencies of the component
     * \return true if there is not already a component of this name, false otherwise
     */
    void AddComponent(const int localLinkId, std::shared_ptr<ComponentStateInformation> componentStateInformation);

    /*!
     * \brief GetComponent gets a registered component by its local link id
     * \param localLinkId   the localLinkId representing the local channel to which the desired component was registered
     * \return The information regarding the component as a ComponentStateInformation
     */
    std::shared_ptr<ComponentStateInformation> GetComponent(const int localLinkId) const;

    /*!
     * \brief GetMaxReachableStateOfComponentAtLocalLinkId gets the current max reachable state for a component registered at localLinkId
     * \param localLinkId the id for the interesting component
     * \return the current max reachable state of the component as a ComponentState type
     */
    ComponentState GetMaxReachableStateOfComponentAtLocalLinkId(const int localLinkId) const;

    /*!
     * \brief GetComponentLocalLinkIdByName gets the localLinkId registered for a specified component name
     * \param componentName the name of the registered component
     * \return The localLinkId for the component
     */
    int GetComponentLocalLinkIdByName(const std::string &componentName) const;

    /*!
     * \brief UpdateComponentCurrentState updates the current state of the component as represented in the ComponentController
     * \param localLinkId the localLinkId of the component
     * \param currentState the new state of the component
     */
    void UpdateComponentCurrentState(const int localLinkId, const ComponentState currentState);

    /*!
     * \brief UpdateComponentDesiredState updates the desired state of the component as represented in the ComponentController
     * \param localLinkId the localLinkId of the component
     * \param desiredState the new desired state of the component
     */
    void UpdateComponentDesiredState(const int localLinkId, ComponentState desiredState);

    /*!
     * \brief UpdateComponentMaxReachableState updates the max reachable state of the component as represented in the ComponentController
     * \param localLinkId the localLinkId of the component
     * \param maxReachableState the new maxReachableState of the component
     */
    void UpdateComponentMaxReachableState(const int localLinkId, ComponentState maxReachableState);

    /*!
     * \brief GetVehicleComponentNameToTypesAndStatesMap
     * \return a std::map of component names to a pair containing their respective ADAS type and ComponentState as represented in the ComponentController
     */
    std::map<std::string, std::pair<ComponentType, ComponentState>> GetVehicleComponentNamesToTypeAndStateMap();

    /*!
     * \brief LocalLinkIdIsRegistered
     * \param localLinkId the localLinkId to test for registration
     * \return whether or not the localLinkId is registered
     */
    bool LocalLinkIdIsRegistered(const int localLinkId) const;

    /*!
     * \brief FlagComponentMaxReachableStateSetByEvent sets the flag noting that the component's max reachable state was set by an event
     * \param localLinkId the localLinkId of the component for which the flag shoule be set
     */
    void FlagComponentMaxReachableStateSetByEvent(const int localLinkId);

    /*!
     * \brief UpdateMaxReachableStatesForRegisteredComponents Calculates and updates the max reachable states for the registered components using the provided
     *        event list (filtered to include only ComponentChangeEvents and only for the agentId of the ComponentController owning the statemanager) and registered
     *        Condition-state pairs
     * \param componentStateChangeEventListFilteredByAgent the event list from the event network, filtered to include only those events related to the agentId
     *        of the controlling ComponentController and only including ComponentChangeEvents
     */
    void UpdateMaxReachableStatesForRegisteredComponents(const std::list<std::shared_ptr<ComponentChangeEvent const>> &componentStateChangeEventListFilteredByAgent);
private:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message) const
    {
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

    std::map<int, std::shared_ptr<ComponentStateInformation>> vehicleComponentStateInformations;

    const CallbackInterface *callbacks;
};

} // namespace ComponentControl
