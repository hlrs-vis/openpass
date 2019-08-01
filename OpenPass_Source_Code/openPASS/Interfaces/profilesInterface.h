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

#include "parameterInterface.h"
#include <memory>
#include <unordered_map>
#include <list>

using StringProbabilities = std::unordered_map<std::string, double>;
using DriverProfiles = std::unordered_map<std::string, std::shared_ptr<ParameterInterface>>;
using VehicleComponentProfiles = std::unordered_map<std::string, std::shared_ptr<ParameterInterface>>;

enum class AgentProfileType
{
    Static,
    Dynamic
};

struct AgentProfile
{
    //Dynamic profile
    StringProbabilities driverProfiles {};
    StringProbabilities vehicleProfiles {};

    //Static profile
    std::string systemConfigFile;
    int systemId;
    std::string vehicleModel;

    AgentProfileType type;
};

struct SensorPosition
{
    std::string name {};
    double longitudinal {};
    double lateral {};
    double height {};
    double pitch {};
    double yaw {};
    double roll {};
};

struct SensorProfile
{
    std::string name;
    std::string type;
    std::shared_ptr<ParameterInterface> parameters;
};

struct SensorParameter
{
    int id;
    SensorPosition sensorPosition;
    SensorProfile sensorProfile;
};

struct SensorLink
{
    int sensorId {};
    std::string inputId {};
};

struct VehicleComponent
{
    std::string type {};
    StringProbabilities componentProfiles {};
    std::list<SensorLink> sensorLinks {};
};


struct VehicleProfile
{
    std::string vehicleModel {};
    std::list<VehicleComponent> vehicleComponents {};
    std::list<SensorParameter> sensors {};
};


//-----------------------------------------------------------------------------
//! Interface provides access to the profiles catalog
//-----------------------------------------------------------------------------
class ProfilesInterface
{
public:
    virtual ~ProfilesInterface() = default;

    /*!
    * \brief Returns a pointer to the agentProfiles
    *
    * @return        agentProfiles
    */
    virtual std::unordered_map<std::string, AgentProfile>& GetAgentProfiles() = 0;

    /*!
    * \brief Returns a pointer to the driverProfiles
    *
    * @return        driverProfiles
    */
    virtual DriverProfiles& GetDriverProfiles() = 0;

    /*!
    * \brief Returns a pointer to the map of vehicle component profiles
    *
    * @return        vehicleComponentProfiles
    */
    virtual std::unordered_map<std::string, VehicleComponentProfiles>& GetVehicleComponentProfiles() = 0;

    /*!
    * \brief Returns a pointer to the map of vehicle profiles
    *
    * @return        vehicleProfiles
    */
    virtual std::unordered_map<std::string, VehicleProfile>& GetVehicleProfiles() = 0;

    /*!
    * \brief Returns the driver profile probabilities of an agentProfile
    *
    * @param[in]    agentProfileName        Name of the agentProfile from which the probabilities are requested
    * @return       probality map for driver profiles
    */
    virtual StringProbabilities& GetDriverProbabilities(std::string agentProfileName) = 0;

    /*!
    * \brief Returns the vehicle profile probabilities of an agentProfile
    *
    * @param[in]    agentProfileName        Name of the agentProfile from which the probabilities are requested
    * @return       probality map for vehicle profiles
    */
    virtual StringProbabilities& GetVehicleProfileProbabilities(std::string agentProfileName) = 0;

    /*!
    * \brief Returns a pointer to the sensorProfiles
    *
    * @return        sensorProfiles
    */
    virtual std::list<SensorProfile>& GetSensorProfiles() = 0;
};
