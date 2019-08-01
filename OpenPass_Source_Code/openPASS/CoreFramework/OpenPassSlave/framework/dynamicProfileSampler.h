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
#include "Interfaces/samplerInterface.h"

class DynamicProfileSampler;

/*!
 * \brief The SampledProfiles struct constains the name of all profiles sampled by the DynamicProfileSampler.
 * With this the DynamicAgentTypeGenerator can generate an AgentType.
 *
 * An instance of this struct is aquired by calling the static make function and then sequentially calling
 * all sample function on the returned DynamicProfileSampler.
 */
struct SampledProfiles
{
    friend class DynamicProfileSampler;
    std::string driverProfileName;
    std::string vehicleProfileName;
    std::unordered_map<std::string, std::string> vehicleComponentProfileNames;

    /*!
     * \brief Creates a new DynamicProfileSampler which can be used to build an instance of this struct.
     * \param agentProfileName  name of the AgentProfile samplied by the AgentProfileSampler
     * \param sampler           sampler
     * \param profiles          imported profiles from the ProfilesCatalog
     * \return new instance of DynamicProfileSampler
     */
    static DynamicProfileSampler make(std::string agentProfileName, const SamplerInterface& sampler, ProfilesInterface* profiles);

private:
    SampledProfiles() = default;
};

/*!
 * \brief The DynamicProfileSampler class is responsible for sampling all profile names (except AgentProfile which is sampled by the AgentProfilesSampler)
 */
class DynamicProfileSampler
{
public:
    DynamicProfileSampler(std::string agentProfileName, const SamplerInterface& sampler, ProfilesInterface* profiles) :
        agentProfileName(agentProfileName),
        sampler(sampler),
        profiles(profiles)
    {
    }

    /*!
     * \brief Samples the driver profile name based on the agent profile name.
     * \return reference to itself
     */
    DynamicProfileSampler &SampleDriverProfile();

    /*!
     * \brief Samples the vehicle profile name based on the agent profile name.
     * \return reference to itself
     */
    DynamicProfileSampler &SampleVehicleProfile();

    /*!
     * \brief Samples all vehicle component profile names based on the vehicle profile name.
     * Therefore SampleVehicleProfile has to be called before this method.
     *
     * \return reference to itself
     */
    DynamicProfileSampler &SampleVehicleComponentProfiles();

    /*!
     * \brief Returns the generated SampledProfiles struct.
     */
    operator SampledProfiles&&()
    {
        return std::move(sampledProfiles);
    }

private:
    SampledProfiles sampledProfiles;
    std::string agentProfileName;
    const SamplerInterface& sampler;
    ProfilesInterface* profiles;
};


