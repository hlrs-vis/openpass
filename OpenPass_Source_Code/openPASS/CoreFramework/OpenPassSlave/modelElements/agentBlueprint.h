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

//-----------------------------------------------------------------------------
/** \file  AgentBlueprint.h
*   \brief This file stores all information which is necessary to instantiate an agent
*/
//-----------------------------------------------------------------------------

#pragma once

#include "agentType.h"
#include "Interfaces/agentBlueprintInterface.h"
#include "spawnItemParameter.h"

//-----------------------------------------------------------------------------
/** \brief This class stores all information which is required to instantiate an agent.
*   \details The information stored include physical spawn variable as well as components the agent consists out of.
*
*/
//-----------------------------------------------------------------------------
class AgentBlueprint : public AgentBlueprintInterface
{
public:
    AgentBlueprint();
    virtual ~AgentBlueprint();

    /*!
    * \brief Sets the vehicle component profile names
    *
    * @param[in]     vehicleComponentProfileNames
    */
    virtual void SetVehicleComponentProfileNames(VehicleComponentProfileNames vehicleComponentProfileNames);

    /*!
    * \brief Sets the agent category
    *
    * @param[in]     agentCategory
    */
    virtual void SetAgentCategory(AgentCategory agentCategory);

    /*!
    * \brief Sets the agent profile name
    *
    * @param[in]     agentProfileName
    */
    virtual void SetAgentProfileName(std::string agentProfileName);

    /*!
    * \brief Sets the vehicle profile name
    *
    * @param[in]     vehicleProfileName
    */
    virtual void SetVehicleProfileName(std::string vehicleProfileName);

    /*!
    * \brief Sets the vehicle model name
    *
    * @param[in]     vehicleModelName
    */
    virtual void SetVehicleModelName(std::string vehicleModelName);

    /*!
    * \brief Sets the vehicle model parameter
    *
    * @param[in]     vehicleModelParameters
    */
    virtual void SetVehicleModelParameters(VehicleModelParameters vehicleModelParameters);

    /*!
    * \brief Sets the driver profile name
    *
    * @param[in]     driverProfileName
    */
    virtual void SetDriverProfileName(std::string driverProfileName);

    /*!
    * \brief Sets the spawn parameter
    *
    * @param[in]     spawnParameter
    */
    virtual void SetSpawnParameter(SpawnParameter spawnParameter);

    /*!
    * \brief Sets the minimum speed goal
    *
    * @param[in]     speedGoalMin
    */
    virtual void SetSpeedGoalMin(double speedGoalMin);

    /*!
    * \brief Sets the object name
    *
    * @param[in]     objectName
    */
    virtual void SetObjectName(std::string objectName);

    /*!
    * \brief Adds a sensor to the vehicle model parameters
    *
    * @param[in]     name           Name of the sensor
    * @param[in]     parameters     Parameters of the sensor
    */
    virtual void AddSensor(SensorParameter parameters);

    /*!
    * \brief Returns the agent category
    *
    * @return     agentCategory
    */
    virtual AgentCategory               GetAgentCategory();

    /*!
    * \brief Returns the agent profile name
    *
    * @return     agentProfileName
    */
    virtual std::string                 GetAgentProfileName();

    /*!
    * \brief Returns the vehicle profile name
    *
    * @return     vehicleProfileName
    */
    virtual std::string                 GetVehicleProfileName();

    /*!
    * \brief Returns the vehicle model name
    *
    * @return     vehicleModelName
    */
    virtual std::string                 GetVehicleModelName();

    /*!
    * \brief Returns the driver profile name
    *
    * @return     driverProfileName
    */
    virtual std::string                 GetDriverProfileName();

    /*!
    * \brief Returns the object name
    *
    * @return     objectName
    */
    virtual std::string                 GetObjectName();

    /*!
    * \brief Returns the vehicle model parameter
    *
    * @return     vehicleModelParameters
    */
    virtual VehicleModelParameters       GetVehicleModelParameters();

    virtual std::list<SensorParameter>  GetSensorParameters();

    /*!
    * \brief Returns the vehicle components profile names
    *
    * @return     vehicleComponentProfileNames
    */
    virtual VehicleComponentProfileNames            GetVehicleComponentProfileNames();

    virtual void SetAgentType(std::shared_ptr<SimulationSlave::AgentTypeInterface> agentType);

    /*!
    * \brief Returns the agent type as pointer
    *
    * @return     agentType
    */
    virtual SimulationSlave::AgentTypeInterface& GetAgentType();

    /*!
    * \brief Returns the spawn parameter as reference
    *
    * @return     spawnParameter
    */
    virtual SpawnParameter&             GetSpawnParameter();

    /*!
    * \brief Returns the minimum speed goal
    *
    * @return     speedGoalMin
    */
    virtual double                      GetSpeedGoalMin();

    /*!
    * \brief Resets the entire AgentBlueprint
    */
    virtual void Reset();

private:
    AgentCategory agentCategory {AgentCategory::Common};
    std::string agentProfileName = "";
    std::string vehicleProfileName = "";
    std::string vehicleModelName = "";
    std::string driverProfileName = "";
    std::string objectName = "";
    VehicleComponentProfileNames vehicleComponentProfileNames;

    SpawnParameter spawnParameter;
    VehicleModelParameters vehicleModelParameters;
    std::list<SensorParameter> sensorParameters;

    std::shared_ptr<SimulationSlave::AgentTypeInterface> agentType {nullptr};
    double speedGoalMin = 30.0 / 3.6;
};


