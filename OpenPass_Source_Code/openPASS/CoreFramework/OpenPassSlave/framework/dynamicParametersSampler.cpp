/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "dynamicParametersSampler.h"
#include <algorithm>

DynamicParameterSampler::DynamicParameterSampler(const SamplerInterface& sampler,
                                                 std::string &vehicleProfileName,
                                                 std::unordered_map<std::string, VehicleProfile> &vehicleProfiles,
                                                 std::list<SensorProfile> &sensorProfiles) :
    sampler(sampler),
    vehicleProfileName(vehicleProfileName),
    vehicleProfiles(vehicleProfiles),
    sensorProfiles(sensorProfiles)
{
}

DynamicParameterSampler &DynamicParameterSampler::SampleSensorLatencies()
{
    for (const auto& sensor : vehicleProfiles.at(vehicleProfileName).sensors)
    {
        const auto& sensorProfile = std::find_if(sensorProfiles.cbegin(), sensorProfiles.cend(),
                                                 [sensor](const auto& sensorProfile){return sensorProfile.name == sensor.sensorProfile.name && sensorProfile.type == sensor.sensorProfile.type;});
        const auto& latencyParameters = sensorProfile->parameters->GetParametersNormalDistribution().at("Latency");
        double latency = sampler.RollForStochasticAttribute(latencyParameters.mean, latencyParameters.standardDeviation, latencyParameters.min, latencyParameters.max);
        dynamicParameter.sensorLatencies.insert({sensor.id, latency});
    }

    return *this;
}

DynamicParameterSampler DynamicParameters::make(const SamplerInterface &sampler, std::string& vehicleProfileName, std::unordered_map<std::string, VehicleProfile> &vehicleProfiles, std::list<SensorProfile> &sensorProfiles)
{
    return DynamicParameterSampler(sampler, vehicleProfileName, vehicleProfiles, sensorProfiles);
}

DynamicParameters DynamicParameters::empty()
{
    return DynamicParameters();
}
