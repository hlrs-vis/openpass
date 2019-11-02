/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
//-----------------------------------------------------------------------------
//! @file  Scenario.h
//! @brief This file contains all information for the choosen scenario
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include "Common/globalDefinitions.h"
#include "Interfaces/scenarioInterface.h"
#include "Interfaces/scenarioActionInterface.h"

namespace Configuration {

class Scenario : public ScenarioInterface
{
public:
    Scenario() = default;
    Scenario(const Scenario&) = delete;
    Scenario(Scenario&&) = delete;
    Scenario& operator=(const Scenario&) = delete;
    Scenario& operator=(Scenario&&) = delete;
    virtual ~Scenario() override = default;

    /*!
     * \brief Getter for vehicle catalog path
     *
     * \return The relative file path to the OpenSCENARIO vehicle catalog in use
     */
    const std::string& GetVehicleCatalogPath() override;

    /*!
     * \brief Setter for vehicle catalog path
     *
     * \param[in]   catalogPath     Relative file path to the OpenSCENARIO vehicle catalog to use
     */
    void SetVehicleCatalogPath(const std::string& catalogPath) override;

    /*!
     * \brief Getter for pedestrian catalog path
     *
     * \return The relative file path to the OpenSCENARIO pedestrian catalog in use
     */
    const std::string& GetPedestrianCatalogPath() override;

    /*!
     * \brief Setter for pedestrian catalog path
     *
     * \param[in]   catalogPath     Relative file path to the OpenSCENARIO pedestrian catalog to use
     */
    void SetPedestrianCatalogPath(const std::string& catalogPath) override;

    /*!
     * \brief Getter for scenery path
     *
     * \return The relative file path to the OpenDRIVE scenery in use
     */
    const std::string& GetSceneryPath() override;

    /*!
     * \brief Setter for scenery path
     *
     * \param[in]   sceneryPath     Relative file path to the OpenDRIVE scenery to use
     */
    void SetSceneryPath(const std::string& sceneryPath) override;

    void SetEgoEntity(const ScenarioEntity& egoEntity) override;
    void AddScenarioEntity(const ScenarioEntity& entity) override;
    void AddScenarioGroupsByEntityNames(const std::map<std::string, std::list<std::string>> &groupDefinitions) override;

    const ScenarioEntity &GetEgoEntity() override;
    const std::vector<ScenarioEntity*> &GetScenarioEntities() override;
    const std::map<std::string, std::vector<ScenarioEntity*>> &GetScenarioGroups() override;

    void AddEventDetector(const std::string& eventDetectorType, SimulationCommon::EventDetectorParameters eventDetectorParameters) override;
    void AddManipulator(const std::string& manipulatorType, SimulationCommon::ManipulatorParameters manipulatorParameters) override;
    void AddAction(std::shared_ptr<ScenarioActionInterface> action) override;

    std::string GetEventDetectorLibraryName();
    std::list<SimulationCommon::EventDetectorParameters> GetEventDetectors() override;

    std::string GetManipulatorLibraryName();
    std::list<SimulationCommon::ManipulatorParameters> GetManipulators() override;
    std::list<std::shared_ptr<ScenarioActionInterface>> GetActions() const override;

    int GetEndTime() const override;
    void SetEndTime(const double endTime) override;

private:
    ScenarioEntity egoEntity;
    std::vector<ScenarioEntity> entities;
    std::map<std::string, std::vector<ScenarioEntity*>> scenarioGroups;

    std::list<SimulationCommon::EventDetectorParameters> eventDetectors;
    std::list<SimulationCommon::ManipulatorParameters> manipulators;
    std::list<std::shared_ptr<ScenarioActionInterface>> actions;

    std::string vehicleCatalogPath;     //!< The path of the vehicle catalog (relative to Scenario.xosc)
    std::string pedestrianCatalogPath;  //!< The path of the pedestrian catalog (relative to Scenario.xosc)
    std::string sceneryPath;            //!< The path of the scenery file (relative to Scenario.xosc)

    double endTimeInSeconds; //!< The simulationTime at which the simulation should end
};


} // namespace SimulationSlave


