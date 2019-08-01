/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "agentBlueprint.h"
#include <assert.h>

AgentBlueprint::AgentBlueprint()
{
}

AgentBlueprint::~AgentBlueprint()
{
}

void AgentBlueprint::SetVehicleComponentProfileNames(VehicleComponentProfileNames vehicleComponentProfileNames)
{
    this->vehicleComponentProfileNames = vehicleComponentProfileNames;
}

void AgentBlueprint::SetAgentCategory(AgentCategory agentCategory)
{
    this->agentCategory = agentCategory;
}

void AgentBlueprint::SetAgentProfileName(std::string agentTypeName)
{
    this->agentProfileName = agentTypeName;
}

void AgentBlueprint::SetVehicleProfileName(std::string vehicleProfileName)
{
    this->vehicleProfileName = vehicleProfileName;
}

void AgentBlueprint::SetVehicleModelName(std::string vehicleModelName)
{
    this->vehicleModelName = vehicleModelName;
}

void AgentBlueprint::SetVehicleModelParameters(VehicleModelParameters vehicleModelParameters)
{
    this->vehicleModelParameters = vehicleModelParameters;
}

void AgentBlueprint::SetDriverProfileName(std::string driverProfileName)
{
    this->driverProfileName = driverProfileName;
}

void AgentBlueprint::SetSpawnParameter(SpawnParameter spawnParameter)
{
    this->spawnParameter = spawnParameter;
}

void AgentBlueprint::SetSpeedGoalMin(double speedGoalMin)
{
    this->speedGoalMin = speedGoalMin;
}

void AgentBlueprint::SetObjectName(std::string objectName)
{
    this->objectName = objectName;
}

void AgentBlueprint::AddSensor(SensorParameter parameters)
{
    sensorParameters.push_back(parameters);
}

AgentCategory AgentBlueprint::GetAgentCategory()
{
    return agentCategory;
}

std::string AgentBlueprint::GetAgentProfileName()
{
    return agentProfileName;
}

std::string AgentBlueprint::GetVehicleProfileName()
{
    return vehicleProfileName;
}

std::string AgentBlueprint::GetVehicleModelName()
{
    return vehicleModelName;
}

std::string AgentBlueprint::GetObjectName()
{
    return objectName;
}

std::string AgentBlueprint::GetDriverProfileName()
{
    return driverProfileName;
}

VehicleModelParameters AgentBlueprint::GetVehicleModelParameters()
{
    return vehicleModelParameters;
}

std::list<SensorParameter> AgentBlueprint::GetSensorParameters()
{
    return sensorParameters;
}

VehicleComponentProfileNames AgentBlueprint::GetVehicleComponentProfileNames()
{
    return vehicleComponentProfileNames;
}

void AgentBlueprint::SetAgentType(std::shared_ptr<SimulationSlave::AgentTypeInterface> agentType)
{
    this->agentType = agentType;
}


SimulationSlave::AgentTypeInterface& AgentBlueprint::GetAgentType()
{
    assert(agentType.get());
    return *agentType.get();
}

SpawnParameter& AgentBlueprint::GetSpawnParameter()
{
    return spawnParameter;
}

/*
SimulationSlave::SpawnItemParameter& AgentBlueprint::GetSpawnItemParameter()
{
    return spawnItemParameter;
}
*/

double AgentBlueprint::GetSpeedGoalMin()
{
    return speedGoalMin;
}

void AgentBlueprint::Reset()
{
    agentCategory = AgentCategory::Common;
    agentProfileName = "";
    vehicleProfileName = "";
    driverProfileName = "";

    spawnParameter = SpawnParameter{};
    vehicleModelParameters = VehicleModelParameters{};
    agentType.reset();

    sensorParameters.clear();

    speedGoalMin = 30.0 / 3.6;
}


