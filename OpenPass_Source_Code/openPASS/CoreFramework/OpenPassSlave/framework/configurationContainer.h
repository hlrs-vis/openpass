/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  ConfigurationContainer.h
*   \brief This file stores all configurations
*/
//-----------------------------------------------------------------------------

#pragma once

#include <unordered_map>

#include "slaveConfig.h"
#include "slaveConfigImporter.h"
#include "Interfaces/configurationContainerInterface.h"
#include "profilesImporter.h"
#include "scenery.h"
#include "sceneryImporter.h"
#include "systemConfigImporter.h"
#include "systemConfig.h"
#include "scenario.h"
#include "scenarioImporter.h"
#include "systemConfigImporter.h"
#include "vehicleModels.h"
#include "vehicleModelsImporter.h"
#include "configurationFiles.h"

namespace Configuration {

//-----------------------------------------------------------------------------
/** \brief This class imports and stores all configurations
*   \details
*/
//-----------------------------------------------------------------------------
class ConfigurationContainer : public ConfigurationContainerInterface
{
public:
    ConfigurationContainer(const ConfigurationFiles& configurationFiles) :
        configurationFiles{configurationFiles}
    {}

    virtual ~ConfigurationContainer() = default;

    /*!
    * \brief This functions imports all confiugrations
    *
    * \details This function imports all configurations and stores them.
    *
    * @return        true, if successful
    */
    bool ImportAllConfigurations() override;

    /*!
    * \brief Returns a pointer to the SystemConfigBlueprint
    *
    * @return        SystemConfigBlueprint pointer
    */
    std::shared_ptr<SystemConfigInterface> GetSystemConfigBlueprint() override;

    /*!
    * \brief Returns a pointer to the SlaveConfig
    *
    * @return        SlaveConfig pointer
    */
    SlaveConfigInterface* GetSlaveConfig() override;

    /*!
    * \brief Returns a pointer to the Profiles
    *
    * @return        Profiles pointer
    */
    ProfilesInterface* GetProfiles() override;

    /*!
    * \brief Returns a pointer to the Scenery
    *
    * @return        Scenery pointer
    */
    SceneryInterface* GetScenery() override;

    /*!
    * \brief Returns a pointer to the Scenario
    *
    * @return        Scenario pointer
    */
    ScenarioInterface* GetScenario() override;

    /*!
    * \brief Returns imported systemConfigs
    *
    * @return        systemConfigs map
    */
    std::map<std::string, std::shared_ptr<SystemConfigInterface>>& GetSystemConfigs() override;

    /*!
    * \brief Returns a pointer to the VehicleModels
    *
    * @return        VehicleModels pointer
    */
    VehicleModelsInterface* GetVehicleModels() override;

private:
    const ConfigurationFiles& configurationFiles;

    std::shared_ptr<SystemConfig> systemConfigBlueprint;
    SlaveConfig slaveConfig;
    Scenery scenery;
    Scenario scenario;
    std::map<std::string, std::shared_ptr<SystemConfigInterface>> systemConfigs;
    VehicleModels vehicleModels;
    Profiles profiles;
};

} //namespace Configuration
