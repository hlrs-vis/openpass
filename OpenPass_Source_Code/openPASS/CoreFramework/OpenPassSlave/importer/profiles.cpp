/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "profiles.h"

std::unordered_map<std::string, AgentProfile>& Profiles::GetAgentProfiles()
{
    return agentProfiles;
}

DriverProfiles& Profiles::GetDriverProfiles()
{
    return driverProfiles;
}

std::unordered_map<std::string, VehicleComponentProfiles>& Profiles::GetVehicleComponentProfiles()
{
    return vehicleComponentProfiles;
}

std::unordered_map<std::string, VehicleProfile>& Profiles::GetVehicleProfiles()
{
    return vehicleProfiles;
}

StringProbabilities& Profiles::GetDriverProbabilities(std::string agentProfileName)
{
    try
    {
        return agentProfiles.at(agentProfileName).driverProfiles;
    }
    catch (const std::out_of_range&)
    {
        throw std::runtime_error("Cannot retrieve driver probabilities. Unknown agent profile " + agentProfileName);
    }
}

StringProbabilities& Profiles::GetVehicleProfileProbabilities(std::string agentProfileName)
{
    try
    {
        return agentProfiles.at(agentProfileName).vehicleProfiles;
    }
    catch (const std::out_of_range&)
    {
        throw std::runtime_error("Cannot retrieve vehicle probabilities. Unknown agent profile " + agentProfileName);
    }
}

std::list<SensorProfile>& Profiles::GetSensorProfiles()
{
    return sensorProfiles;
}
