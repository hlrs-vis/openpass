/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "agentBlueprintProvider.h"
#include "systemConfigImporter.h"
#include "dynamicProfileSampler.h"
#include "dynamicParametersSampler.h"

AgentBlueprintProvider::AgentBlueprintProvider(ConfigurationContainerInterface *configurationContainer, const SamplerInterface& sampler) :
    agentProfileSampler(configurationContainer, sampler),
    sampler(sampler),
    profiles(configurationContainer->GetProfiles()),
    agentProfiles(configurationContainer->GetProfiles()->GetAgentProfiles()),
    vehicleModels(configurationContainer->GetVehicleModels()),
    systemConfigBlueprint(configurationContainer->GetSystemConfigBlueprint()),
    systemConfigs(configurationContainer->GetSystemConfigs())
{
}

bool AgentBlueprintProvider::SampleAgent(AgentBlueprintInterface &agentBlueprint, LaneCategory laneCategory, unsigned int scenarioAgentIterator)
{
    agentProfileSampler.SampleAgentProfileName(agentBlueprint, laneCategory, scenarioAgentIterator);
    auto agentProfile = agentProfiles.at(agentBlueprint.GetAgentProfileName());
    if (agentProfile.type == AgentProfileType::Dynamic)
    {
        SampledProfiles sampledProfiles = SampledProfiles::make(agentBlueprint.GetAgentProfileName(), sampler, profiles)
                .SampleDriverProfile()
                .SampleVehicleProfile()
                .SampleVehicleComponentProfiles();
        DynamicParameters dynamicParameters = DynamicParameters::make(sampler, sampledProfiles.vehicleProfileName, profiles->GetVehicleProfiles(), profiles->GetSensorProfiles())
                .SampleSensorLatencies();
        AgentBuildInformation agentBuildInformation = AgentBuildInformation::make(sampledProfiles, dynamicParameters, systemConfigBlueprint, profiles, vehicleModels)
                .SetVehicleModelParameters()
                .GatherBasicComponents()
                .GatherDriverComponents()
                .GatherVehicleComponents()
                .GatherSensors();
        return GenerateDynamicAgentBlueprint(agentBlueprint, agentBuildInformation, sampledProfiles.driverProfileName);
    }
    else
    {
        return GenerateStaticAgentBlueprint(agentBlueprint, agentProfile.systemConfigFile, agentProfile.systemId, agentProfile.vehicleModel);
    }
}

bool AgentBlueprintProvider::GenerateDynamicAgentBlueprint(AgentBlueprintInterface &agentBlueprint, AgentBuildInformation agentBuildInformation, std::string &driverProfileName)
{
    agentBlueprint.SetVehicleModelName(agentBuildInformation.vehicleModelName);
    agentBlueprint.SetVehicleModelParameters(agentBuildInformation.vehicleModelParameters);
    agentBlueprint.SetDriverProfileName(driverProfileName);
    agentBlueprint.SetAgentType(agentBuildInformation.agentType);

    for(const auto& sensor : agentBuildInformation.sensorParameters)
    {
        agentBlueprint.AddSensor(sensor);
    }
    return true;
}

bool AgentBlueprintProvider::GenerateStaticAgentBlueprint(AgentBlueprintInterface &agentBlueprint, std::string systemConfigName, int systemId, std::string vehicleModelName)
{
    try
    {
        auto& systems = systemConfigs.at(systemConfigName)->GetSystems();
        agentBlueprint.SetAgentType(systems.at(systemId));
    }
    catch (const std::out_of_range& e)
    {
        LOG_INTERN(LogLevel::Error) << "No system for specified id found in imported systemConfig: " << e.what();
        return false;
    }

    agentBlueprint.SetVehicleModelName(vehicleModelName);
    auto vehicleModelParameters = vehicleModels->GetVehicleModel(vehicleModelName);
    agentBlueprint.SetVehicleModelParameters(vehicleModelParameters);

    return true;
}
