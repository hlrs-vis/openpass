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
#include <unordered_map>
#include "Interfaces/samplerInterface.h"
#include "Interfaces/profilesInterface.h"

class DynamicParameterSampler;

struct DynamicParameters
{
    friend DynamicParameterSampler;
    std::unordered_map<int, double> sensorLatencies;

    static DynamicParameterSampler make(const SamplerInterface& sampler, std::string &vehicleProfileName, std::unordered_map<std::string, VehicleProfile>& vehicleProfiles, std::list<SensorProfile>& sensorProfiles);

    //For testing;
    static DynamicParameters empty();
private:
    DynamicParameters() = default;
};

class DynamicParameterSampler
{
public:
    DynamicParameterSampler(const SamplerInterface& sampler, std::string &vehicleProfileName, std::unordered_map<std::string, VehicleProfile>& vehicleProfiles, std::list<SensorProfile>& sensorProfiles);

    operator DynamicParameters&&()
    {
        return std::move(dynamicParameter);
    }

    DynamicParameterSampler& SampleSensorLatencies();

private:
    DynamicParameters dynamicParameter;
    const SamplerInterface& sampler;
    std::string& vehicleProfileName;
    std::unordered_map<std::string, VehicleProfile>& vehicleProfiles;
    std::list<SensorProfile>& sensorProfiles;
};
