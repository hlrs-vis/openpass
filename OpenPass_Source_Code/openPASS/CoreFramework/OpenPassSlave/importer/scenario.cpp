/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "scenario.h"
#include <algorithm>
#include <qglobal.h>

namespace Configuration {

const std::string& Scenario::GetVehicleCatalogPath()
{
    return vehicleCatalogPath;
}

void Scenario::SetVehicleCatalogPath(const std::string& catalogPath)
{
    this->vehicleCatalogPath = catalogPath;
}

const std::string& Scenario::GetPedestrianCatalogPath()
{
    return pedestrianCatalogPath;
}

void Scenario::SetPedestrianCatalogPath(const std::string& catalogPath)
{
    this->pedestrianCatalogPath = catalogPath;
}

const std::string& Scenario::GetSceneryPath()
{
    return sceneryPath;
}

void Scenario::SetSceneryPath(const std::string& sceneryPath)
{
    this->sceneryPath = sceneryPath;
}

void Scenario::SetEgoEntity(const ScenarioEntity& egoEntity)
{
    this->egoEntity = egoEntity;
}

void Scenario::AddScenarioEntity(const ScenarioEntity& entity)
{
    entities.push_back(entity);
}

void Scenario::AddScenarioGroupsByEntityNames(const std::map<std::string, std::list<std::string>> &groupDefinitions)
{
    std::vector<ScenarioEntity*> groupEntities;
    for (auto groupDefinition : groupDefinitions)
    {
        for (const auto &memberName : groupDefinition.second)
        {
            const auto groupEntityIterator = std::find_if(entities.begin(),
                                                          entities.end(),
                                                          [&memberName](const auto entity)
                                                          {
                                                            return entity.name == memberName;
                                                          });
            groupEntities.push_back(&(*groupEntityIterator));
        }

        scenarioGroups.insert({groupDefinition.first, groupEntities});
    }
}

const ScenarioEntity &Scenario::GetEgoEntity()
{
    return egoEntity;
}

const std::vector<ScenarioEntity*> &Scenario::GetScenarioEntities()
{
    try
    {
        return scenarioGroups.at("ScenarioAgents");
    }
    catch (const std::out_of_range& err)
    {
        Q_UNUSED(err);
        throw std::runtime_error("ScenarioAgents group not found.");
    }
}

const std::map<std::string, std::vector<ScenarioEntity*>> &Scenario::GetScenarioGroups()
{
    return scenarioGroups;
}

void Scenario::AddEventDetector(const std::string& eventDetectorType,
                                SimulationCommon::EventDetectorParameters eventDetectorParameters)
{
    eventDetectorParameters.SetType(eventDetectorType);
    eventDetectors.push_back(eventDetectorParameters);
}

void Scenario::AddManipulator(const std::string& manipulatorType,
                              SimulationCommon::ManipulatorParameters manipulatorParameters)
{
    manipulatorParameters.SetType(manipulatorType);
    manipulators.push_back(manipulatorParameters);
}

void Scenario::AddAction(std::shared_ptr<ScenarioActionInterface> action)
{
    actions.push_back(action);
}

std::list<SimulationCommon::EventDetectorParameters> Scenario::GetEventDetectors()
{
    return eventDetectors;
}

std::list<SimulationCommon::ManipulatorParameters> Scenario::GetManipulators()
{
    return manipulators;
}

std::list<std::shared_ptr<ScenarioActionInterface>> Scenario::GetActions() const
{
    return actions;
}

int Scenario::GetEndTime() const
{
    // we add plus one here to align with the "greater_than" rule
    // Time is parsed in seconds, but we use ms internally (* 1000)
    return static_cast<int>((std::rint(endTimeInSeconds * 1000))) + 1;
}

void Scenario::SetEndTime(const double endTime)
{
    this->endTimeInSeconds = endTime;
}

} // namespace SimulationSlave
