/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "dynamicProfileSampler.h"

DynamicProfileSampler& DynamicProfileSampler::SampleDriverProfile()
{
    auto probabilities = profiles->GetDriverProbabilities(agentProfileName);
    sampledProfiles.driverProfileName = sampler.SampleStringProbability(probabilities);
    return *this;
}

DynamicProfileSampler& DynamicProfileSampler::SampleVehicleProfile()
{
    auto probabilities = profiles->GetVehicleProfileProbabilities(agentProfileName);
    sampledProfiles.vehicleProfileName = sampler.SampleStringProbability(probabilities);
    return *this;
}

DynamicProfileSampler& DynamicProfileSampler::SampleVehicleComponentProfiles()
{
    VehicleProfile vehicleProfile = profiles->GetVehicleProfiles().at(sampledProfiles.vehicleProfileName);
    for (VehicleComponent vehicleComponentInProfile : vehicleProfile.vehicleComponents)
    {
        std::string vehicleComponentName = sampler.SampleStringProbability(vehicleComponentInProfile.componentProfiles);
        if (vehicleComponentName != "")
        {
            sampledProfiles.vehicleComponentProfileNames.insert(std::make_pair<std::string&, std::string&>
                    (vehicleComponentInProfile.type,
                     vehicleComponentName));
        }
    }
    return *this;
}

DynamicProfileSampler SampledProfiles::make(
    std::string agentProfileName,
    const SamplerInterface& sampler,
    ProfilesInterface* profiles)
{
    return DynamicProfileSampler(agentProfileName, sampler, profiles);
}
