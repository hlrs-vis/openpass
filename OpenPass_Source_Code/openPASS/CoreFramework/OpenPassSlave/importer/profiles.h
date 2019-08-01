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
#include "Interfaces/profilesInterface.h"

class Profiles : public ProfilesInterface
{
public:
    virtual ~Profiles() override = default;

    virtual std::unordered_map<std::string, AgentProfile>& GetAgentProfiles() override;

    virtual DriverProfiles& GetDriverProfiles() override;

    virtual std::unordered_map<std::string, VehicleComponentProfiles>& GetVehicleComponentProfiles() override;

    virtual std::unordered_map<std::string, VehicleProfile>& GetVehicleProfiles() override;

    virtual StringProbabilities& GetDriverProbabilities(std::string agentProfileName) override;

    virtual StringProbabilities& GetVehicleProfileProbabilities(std::string agentProfileName) override;

    virtual std::list<SensorProfile>& GetSensorProfiles() override;

private:
    std::unordered_map<std::string, AgentProfile> agentProfiles {};
    std::unordered_map<std::string, VehicleProfile> vehicleProfiles {};
    DriverProfiles driverProfiles {};
    std::unordered_map<std::string, VehicleComponentProfiles> vehicleComponentProfiles {};
    std::list<SensorProfile> sensorProfiles {};
};
