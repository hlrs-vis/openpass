/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "componentStateInformation.h"

using namespace ComponentControl;

ComponentStateInformation::ComponentStateInformation(const ComponentType componentType,
                                                     const std::string &componentName,
                                                     const ComponentState currentState) :
    componentType(componentType),
    componentName(componentName),
    currentState(currentState)
{
    defaultState = ComponentState::Undefined;
}

ComponentType ComponentStateInformation::GetComponentType() const
{
    return componentType;
}

std::string ComponentStateInformation::GetComponentName() const
{
    return componentName;
}

ComponentState ComponentStateInformation::GetCurrentState() const
{
    return currentState;
}

void ComponentStateInformation::SetCurrentState(const ComponentState currentState)
{
    this->currentState = currentState;
}

void ComponentStateInformation::SetDesiredState(const ComponentState desiredState)
{
    this->desiredState = desiredState;
}

ComponentState ComponentStateInformation::GetMaxReachableState() const
{
    return maxReachableState;
}

void ComponentStateInformation::SetMaxReachableState(const ComponentState maxReachableState)
{
    this->maxReachableState = maxReachableState;
}

bool ComponentStateInformation::GetMaxReachableStateSetByEvent() const
{
    return maxReachableStateSetByEvent;
}

void ComponentStateInformation::SetMaxReachableStateSetByEvent(const bool maxReachableStateSetByEvent)
{
    this->maxReachableStateSetByEvent = maxReachableStateSetByEvent;
}

void ComponentStateInformation::AddStateCondition(std::shared_ptr<Condition> condition, const ComponentState state)
{
    std::pair<std::shared_ptr<Condition>,ComponentState> pair(std::move(condition), state);
    conditionalStates.push_back(std::move(pair));
}

const std::vector<std::pair<std::shared_ptr<Condition>, ComponentState>> &ComponentStateInformation::GetStateConditions() const
{
    return conditionalStates;
}

// vvvvv -- AdasComponentStateInformation -- vvvvv

AdasComponentStateInformation::AdasComponentStateInformation(const ComponentType componentType,
                                                             const std::string &componentName,
                                                             const ComponentState currentState,
                                                             const AdasType adasType) :
    ComponentStateInformation(componentType,
                              componentName,
                              currentState),
    adasType(adasType)
{
    SetMaxReachableState(DetermineDefaultMaximumComponentStateByType());
}

AdasType AdasComponentStateInformation::GetAdasType() const
{
    return adasType;
}

ComponentState AdasComponentStateInformation::DetermineDefaultMaximumComponentStateByType() const
{
    ComponentState maxState = ComponentState::Undefined;

    if (adasType == AdasType::Safety)
    {
        maxState = ComponentState::Acting;
    }
    else if (adasType == AdasType::Comfort)
    {
        maxState = ComponentState::Disabled;
    }

    return maxState;
}
