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
//! \file  ComponentStateInformation.h
//! \brief This file contains all functions for class ComponentStateInformation
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include <utility>
#include <vector>

#include "componentControllerCommon.h"
#include "condition.h"

namespace ComponentControl {
/*!
 * \brief The ComponentStateConfiguration class handles the dependencies of a single component
 *
 * The ComponentStateConfiguration consist of an ordered list of conditional states and a default state.
 * The conditions of the list are evaluated from top to bottom and the first fullfilled condition defines
 * the maxReachableState of the component. If no condition is fullfilled then the defaultState is the
 * maxReachableState.
 */
class ComponentStateInformation
{
public:
    ComponentStateInformation(const ComponentType componentType,
                              const std::string &componentName,
                              const ComponentState currentState);

    ComponentStateInformation(const ComponentStateInformation&) = default;
    ComponentStateInformation(ComponentStateInformation&&) = default;
    ComponentStateInformation& operator=(const ComponentStateInformation&) = default;
    ComponentStateInformation& operator=(ComponentStateInformation&&) = default;    
    virtual ~ComponentStateInformation() = default;

    ComponentStateInformation() = delete;

    /*!
     * \brief GetComponentType Gets the component type of the contained component information
     * \return Gets the component type of the contained component information as a ComponentType
     */
    ComponentType GetComponentType() const;

    /*!
     * \brief GetComponentName gets the component name of the contained component information
     * \return Gets the component name of the contained component information as a std::string
     */
    std::string GetComponentName() const;

    /*!
     * \brief GetCurrentState gets the current state of the contained component information
     * \return Gets the current state of the contained component information as a ComponentState
     */
    ComponentState GetCurrentState() const;

    /*!
     * \brief SetCurrentState sets the current state of the contained component information
     * \param currentState The new current state of the contained component information as a ComponentState
     */
    void SetCurrentState(const ComponentState currentState);

    /*!
     * \brief SetDesiredState sets the desired state of the contained component information
     * \param desiredState The new desired state of the contained component information as a ComponentState
     */
    void SetDesiredState(const ComponentState desiredState);

    /*!
     * \brief GetMaxReachableState gets the max reachable state of the contained component information
     * \return the max reachable state of the contained component information
     */
    ComponentState GetMaxReachableState() const;
    /*!
     * \brief SetMaxReachableState sets the max reachable state of the contained component information
     * \param the new max reachable state of the contained component information
     */
    void SetMaxReachableState(const ComponentState maxReachableState);

    /*!
     * \brief GetMaxReachableStateSetByEvent gets the flag indicating whether the max reachable state was set by an event
     * \return the flag indicating whether the max reachable state was set by an event
     */
    bool GetMaxReachableStateSetByEvent() const;
    /*!
     * \brief SetMaxReachableStateSetByEvent sets the flag indicating whether the max reachable state was set by an event
     * \param the new value for the flag indicating whether the max reachable state was set by an event
     */
    void SetMaxReachableStateSetByEvent(const bool maxReachableStateSetByEvent);

    /*!
     * \brief Adds a conditional state to the bottom of the list of conditional states.
     * \param condition - the condition for which the state should be selected
     * \param state - the state which should be selected, given the condition
     */
    void AddStateCondition(std::shared_ptr<Condition> condition, const ComponentState state);

    /*!
     * \brief GetStateConditions gets a const ref to the std::vector containing pairs of Conditions to ComponentStates
     *        which define max reachable states given fulfillment of conditions
     * \return a const ref to a std::vector containing pairs of conditions and their related states
     */
    const std::vector<std::pair<std::shared_ptr<Condition>, ComponentState>> &GetStateConditions() const;

private:
    const ComponentType componentType;
    const std::string componentName;

    bool maxReachableStateSetByEvent {false};
    ComponentState defaultState {ComponentState::Undefined};
    ComponentState currentState {ComponentState::Undefined};
    ComponentState desiredState {ComponentState::Undefined};
    ComponentState maxReachableState {ComponentState::Undefined};
    std::vector<std::pair<std::shared_ptr<Condition>, ComponentState>> conditionalStates{};
};

class AdasComponentStateInformation : public ComponentStateInformation
{
public:
    AdasComponentStateInformation(const ComponentType componentType,
                                  const std::string &componentName,
                                  const ComponentState currentState,
                                  const AdasType adasType);
    AdasComponentStateInformation(const AdasComponentStateInformation&) = default;
    AdasComponentStateInformation(AdasComponentStateInformation&&) = default;
    AdasComponentStateInformation& operator=(const AdasComponentStateInformation&) = default;
    AdasComponentStateInformation& operator=(AdasComponentStateInformation&&) = default;
    ~AdasComponentStateInformation() = default;

    AdasComponentStateInformation() = delete;

    /*!
     * \brief GetAdasType gets the adas type of the contained component information
     * \return Gets the ADAS type of the contained component information as an AdasType
     */
    AdasType GetAdasType() const;

    /*!
     * \brief DetermineDefaultMaximumComponentStateByType determines the default max reachable state by adas type of this component
     * \return the default max reachable component state for the adas type of this component
     */
    ComponentState DetermineDefaultMaximumComponentStateByType() const;
private:
    AdasType adasType;
};

} // namespace ComponentControl
