/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "stateManager.h"

using namespace ComponentControl;

void StateManager::AddComponent(const int localLinkId,
                                std::shared_ptr<ComponentStateInformation> componentStateInformation)
{
    // If the component is already registered, throw an error
    if (vehicleComponentStateInformations.find(localLinkId) != vehicleComponentStateInformations.cend())
    {
        throw std::invalid_argument("ComponentController - StateManager: AddComponent() failed. A component has already been registered to local link id "
                                    + std::to_string(localLinkId));
    }

    // If the component is not yet registered, register it
    vehicleComponentStateInformations.insert({localLinkId, componentStateInformation});
}

std::shared_ptr<ComponentStateInformation> StateManager::GetComponent(const int localLinkId) const
{
    auto vehicleComponentIter = vehicleComponentStateInformations.find(localLinkId);

    if (vehicleComponentIter != vehicleComponentStateInformations.cend())
    {
        return vehicleComponentIter->second;
    }

    throw std::out_of_range("Specified Link Id is not registered in ComponentController");
}

ComponentState StateManager::GetMaxReachableStateOfComponentAtLocalLinkId(const int localLinkId) const
{
    const auto componentAtLocalLinkId = GetComponent(localLinkId);
    return componentAtLocalLinkId->GetMaxReachableState();
}

int StateManager::GetComponentLocalLinkIdByName(const std::string& componentName) const
{
    for (const auto& localLinkIdToComponentStateInformation : vehicleComponentStateInformations)
    {
        const auto componentStateInformation = localLinkIdToComponentStateInformation.second;

        if (componentStateInformation->GetComponentName() == componentName)
        {
            return localLinkIdToComponentStateInformation.first;
        }
    }

    throw std::out_of_range("Statemanager could not find the localLinkId for " + componentName + ".");
}

void StateManager::UpdateComponentCurrentState(const int localLinkId, const ComponentState currentState)
{
    auto vehicleComponentStateInformationIterator = vehicleComponentStateInformations.find(localLinkId);

    if (vehicleComponentStateInformationIterator != vehicleComponentStateInformations.end())
    {
        vehicleComponentStateInformationIterator->second->SetCurrentState(currentState);
    }
}

void StateManager::UpdateComponentDesiredState(const int localLinkId, const ComponentState desiredState)
{
    auto vehicleComponentStateInformationIterator = vehicleComponentStateInformations.find(localLinkId);

    if (vehicleComponentStateInformationIterator != vehicleComponentStateInformations.end())
    {
        vehicleComponentStateInformationIterator->second->SetDesiredState(desiredState);
    }
}

void StateManager::UpdateComponentMaxReachableState(const int localLinkId, const ComponentState maxReachableState)
{
    auto vehicleComponentStateInformationIterator = vehicleComponentStateInformations.find(localLinkId);

    if (vehicleComponentStateInformationIterator != vehicleComponentStateInformations.end())
    {
        vehicleComponentStateInformationIterator->second->SetMaxReachableState(maxReachableState);
    }
}

void StateManager::FlagComponentMaxReachableStateSetByEvent(const int localLinkId)
{
    auto vehicleComponentStateInformationIterator = vehicleComponentStateInformations.find(localLinkId);

    if (vehicleComponentStateInformationIterator != vehicleComponentStateInformations.end())
    {
        vehicleComponentStateInformationIterator->second->SetMaxReachableStateSetByEvent(true);
    }
}

void StateManager::UpdateMaxReachableStatesForRegisteredComponents(const
        std::list<std::shared_ptr<ComponentChangeEvent const>>& componentStateChangeEventListFilteredByAgent)
{
    for (const auto stateChangeEvent : componentStateChangeEventListFilteredByAgent)
    {
        try
        {
            const auto localLinkId = GetComponentLocalLinkIdByName(stateChangeEvent->GetComponentName());
            UpdateComponentMaxReachableState(localLinkId, ComponentStateMapping.at(stateChangeEvent->GetGoalStateName()));
            FlagComponentMaxReachableStateSetByEvent(localLinkId);
        }
        catch (const std::out_of_range& error)
        {
            const std::string errorMessage = error.what();
            const std::string warning = errorMessage + "The event will be ignored.";
            LOG(CbkLogLevel::Warning, warning);
        }
    }

    const auto componentNameToTypeAndStateMap = GetVehicleComponentNamesToTypeAndStateMap();
    for (auto& vehicleComponentLocalLinkIdToStateInformationPair : vehicleComponentStateInformations)
    {
        auto& vehicleComponentStateInformation = vehicleComponentLocalLinkIdToStateInformationPair.second;
        // only update max reachable state if it was not previously set by an event
        if (!(vehicleComponentStateInformation->GetMaxReachableStateSetByEvent()))
        {
            const auto& conditionalStates = vehicleComponentStateInformation->GetStateConditions();
            ComponentState calculatedMaxReachableState = ComponentState::Undefined;

            // try to cast vehicleComponentStateInformation to AdasComponentStateInformation type
            // if successful, calculate max reachable state with appropriate function call
            std::shared_ptr<AdasComponentStateInformation> adasComponentStateInformation =
                std::dynamic_pointer_cast<AdasComponentStateInformation>(vehicleComponentStateInformation);
            if (adasComponentStateInformation)
            {
                calculatedMaxReachableState = adasComponentStateInformation->DetermineDefaultMaximumComponentStateByType();
            }

            for (const auto& conditionalState : conditionalStates)
            {
                if (conditionalState.first->IsFullfilled(componentNameToTypeAndStateMap))
                {
                    calculatedMaxReachableState = conditionalState.second;
                    break;
                }
            }
            vehicleComponentStateInformation->SetMaxReachableState(calculatedMaxReachableState);
        }
    }
}

bool StateManager::LocalLinkIdIsRegistered(const int localLinkId) const
{
    return (vehicleComponentStateInformations.find(localLinkId) != vehicleComponentStateInformations.end());
}

std::map<std::string, std::pair<ComponentType, ComponentState>>
        StateManager::GetVehicleComponentNamesToTypeAndStateMap()
{
    std::map<std::string, std::pair<ComponentType, ComponentState>> vehicleComponentStates;

    for (const auto& vehicleComponentStateInformationIterator : vehicleComponentStateInformations)
    {
        const auto& vehicleComponentStateInformation = vehicleComponentStateInformationIterator.second;
        vehicleComponentStates.insert({ vehicleComponentStateInformation->GetComponentName(), { vehicleComponentStateInformation->GetComponentType(), vehicleComponentStateInformation->GetCurrentState() } });
    }

    return vehicleComponentStates;
}
