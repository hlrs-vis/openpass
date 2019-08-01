/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <unordered_map>

#include "Interfaces/agentTypeInterface.h"
#include "Interfaces/slaveConfigInterface.h"
#include "Interfaces/profilesInterface.h"
#include "Interfaces/sceneryInterface.h"
#include "Interfaces/scenarioInterface.h"
#include "Interfaces/systemConfigInterface.h"
#include "Interfaces/vehicleModelsInterface.h"

class ConfigurationContainerInterface
{
public:
    ConfigurationContainerInterface() = default;
    virtual ~ConfigurationContainerInterface() = default;

    virtual bool ImportAllConfigurations() = 0;

    //Getters
    virtual std::shared_ptr<SystemConfigInterface> GetSystemConfigBlueprint() = 0;
    virtual SlaveConfigInterface* GetSlaveConfig() = 0;
    virtual ProfilesInterface* GetProfiles() = 0;
    virtual SceneryInterface* GetScenery() = 0;
    virtual ScenarioInterface* GetScenario() = 0;
    virtual std::map<std::string, std::shared_ptr<SystemConfigInterface>>& GetSystemConfigs() = 0;
    virtual VehicleModelsInterface* GetVehicleModels() = 0;
};
