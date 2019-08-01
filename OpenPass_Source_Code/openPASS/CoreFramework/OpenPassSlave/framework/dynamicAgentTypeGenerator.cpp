/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "dynamicAgentTypeGenerator.h"

DynamicAgentTypeGenerator::DynamicAgentTypeGenerator(SampledProfiles& sampledProfiles,
        DynamicParameters& dynamicParameters, std::shared_ptr<SystemConfigInterface> systemConfigBlueprint, ProfilesInterface* profiles,
        VehicleModelsInterface* vehicleModels) :
    sampledProfiles(sampledProfiles),
    dynamicParameters(dynamicParameters),
    systemConfigBlueprint(systemConfigBlueprint),
    profiles(profiles),
    vehicleModels(vehicleModels)
{
}

DynamicAgentTypeGenerator& DynamicAgentTypeGenerator::GatherBasicComponents()
{
    for (const auto& component : defaultComponents.basicComponentNames)
    {
        GatherComponent(component, agentBuildInformation.agentType);
    }

    return *this;
}

DynamicAgentTypeGenerator& DynamicAgentTypeGenerator::GatherDriverComponents()
{
    auto driverParameters = profiles->GetDriverProfiles().at(sampledProfiles.driverProfileName);

    if (driverParameters->GetParametersString().count("SensorDriverModule") > 0)
    {
        const auto& sensorDriverModuleName = driverParameters->GetParametersString().at("SensorDriverModule");
        GatherComponent(sensorDriverModuleName, agentBuildInformation.agentType);
    }
    else
    {
        GatherComponent("Sensor_Driver", agentBuildInformation.agentType);
    }

    if (driverParameters->GetParametersString().count("AlgorithmLateralModule") > 0)
    {
        const auto& algorithmLateralModuleName = driverParameters->GetParametersString().at("AlgorithmLateralModule");
        GatherComponent(algorithmLateralModuleName, agentBuildInformation.agentType);
    }
    else
    {
        GatherComponent("AlgorithmLateralDriver", agentBuildInformation.agentType);
    }

    if (driverParameters->GetParametersString().count("AlgorithmLongitudinalModule") > 0)
    {
        const auto& algorithmLongitudinalModuleName = driverParameters->GetParametersString().at("AlgorithmLongitudinalModule");
        GatherComponent(algorithmLongitudinalModuleName, agentBuildInformation.agentType);
    }
    else
    {
        GatherComponent("AlgorithmLongitudinalDriver", agentBuildInformation.agentType);
    }

    const auto& driverModule = driverParameters->GetParametersString().at("Type");

    if (driverParameters->GetParametersString().count("ParametersModule") > 0)
    {
        const auto& parametersModuleName = driverParameters->GetParametersString().at("ParametersModule");
        GatherComponentWithParameters(parametersModuleName, agentBuildInformation.agentType, driverParameters.get());
        GatherComponent(driverModule, agentBuildInformation.agentType);
    }
    else
    {
        GatherComponentWithParameters(driverModule, agentBuildInformation.agentType, driverParameters.get());
    }

    for (const auto& componentName : defaultComponents.driverComponentNames)
    {
        GatherComponent(componentName, agentBuildInformation.agentType);
    }

    return *this;
}

DynamicAgentTypeGenerator& DynamicAgentTypeGenerator::GatherVehicleComponents()
{
    std::shared_ptr<SimulationSlave::AgentType> agentType = agentBuildInformation.agentType;
    VehicleComponentProfileNames vehicleComponentProfileNames = sampledProfiles.vehicleComponentProfileNames;
    std::unordered_map<std::string, VehicleComponentProfiles> possibleVehicleComponentProfiles =
        profiles->GetVehicleComponentProfiles();

    if (!vehicleComponentProfileNames.empty())
    {
        for (std::string componentName : defaultComponents.vehicleComponentNames)
        {
            GatherComponent(componentName, agentType);
        }
    }

    for (auto vehicleComponentProfile : vehicleComponentProfileNames)
    {
        if (possibleVehicleComponentProfiles.count(vehicleComponentProfile.first) == 0)
        {
            throw std::logic_error("No vehicle component profile of type " + vehicleComponentProfile.first);
        }
        if (possibleVehicleComponentProfiles.at(vehicleComponentProfile.first).count(vehicleComponentProfile.second) == 0)
        {
            throw std::logic_error("No vehicle component profile of type " + vehicleComponentProfile.first + " with name " +
                                   vehicleComponentProfile.second);
        }
        auto parameters = possibleVehicleComponentProfiles.at(vehicleComponentProfile.first).at(
                              vehicleComponentProfile.second);

        if (profiles->GetVehicleProfiles().count(sampledProfiles.vehicleProfileName) == 0)
        {
            throw std::logic_error("No vehicle profile of type " + sampledProfiles.vehicleProfileName);
        }
        auto vehicleProfile = profiles->GetVehicleProfiles().at(sampledProfiles.vehicleProfileName);

        auto matchedComponent = std::find_if(vehicleProfile.vehicleComponents.begin(),
                                             vehicleProfile.vehicleComponents.end(), [vehicleComponentProfile](VehicleComponent curComponent)
        { return curComponent.type == vehicleComponentProfile.first; });

        for (auto link : matchedComponent->sensorLinks)
        {
            auto& parameterListItem = parameters->InitializeListItem("SensorLinks");
            parameterListItem.AddParameterInt("SensorId", link.sensorId);
            parameterListItem.AddParameterString("InputId", link.inputId);
        }

        GatherComponentWithParameters(vehicleComponentProfile.first, agentType, parameters.get());
    }

    return *this;
}

DynamicAgentTypeGenerator& DynamicAgentTypeGenerator::GatherSensors()
{
    const std::string sensorFusionModulName = "SensorFusion";

    GatherComponent(sensorFusionModulName, agentBuildInformation.agentType);

    int inputIdSensorFusion = systemConfigBlueprint->GetSystems().at(0)->GetComponents().at(sensorFusionModulName)->GetInputLinks().at(
                                  0);
    int sensorNumber = 0;
    auto vehicleProfile = profiles->GetVehicleProfiles().at(sampledProfiles.vehicleProfileName);

    const auto sensorProfilesFromConfig = profiles->GetSensorProfiles();

    for (const SensorParameter& sensor : vehicleProfile.sensors)
    {
        auto matchedSensorProfile = std::find_if(sensorProfilesFromConfig.begin(),
                                    sensorProfilesFromConfig.end(), [sensor](SensorProfile curProfile)
        { return curProfile.name == sensor.sensorProfile.name && curProfile.type == sensor.sensorProfile.type; });

        if (matchedSensorProfile == sensorProfilesFromConfig.end())
        {
            throw std::runtime_error("Could not find SensorProfile");
        }

        auto curParameters = std::make_shared<SimulationCommon::Parameters>(*matchedSensorProfile->parameters);
        systemConfigBlueprint->AddModelParameters(curParameters);

        curParameters->AddParameterString("Name", matchedSensorProfile->name);
        curParameters->AddParameterString("Type", matchedSensorProfile->type);
        curParameters->AddParameterInt("Id", sensor.id);
        curParameters->AddParameterDouble("Longitudinal", sensor.sensorPosition.longitudinal);
        curParameters->AddParameterDouble("Lateral", sensor.sensorPosition.lateral);
        curParameters->AddParameterDouble("Height", sensor.sensorPosition.height);
        curParameters->AddParameterDouble("Pitch", sensor.sensorPosition.pitch);
        curParameters->AddParameterDouble("Yaw", sensor.sensorPosition.yaw);
        curParameters->AddParameterDouble("Roll", sensor.sensorPosition.roll);
        curParameters->AddParameterDouble("Latency", dynamicParameters.sensorLatencies.at(sensor.id));

        const std::string uniqueSensorName =  "Sensor_" + std::to_string(sensor.id);
        GatherComponentWithParameters(uniqueSensorName, agentBuildInformation.agentType, curParameters.get(),
                                      "SensorObjectDetector", sensorNumber);

        if (sensorNumber > 0)
        {
            agentBuildInformation.agentType->GetComponents().at(sensorFusionModulName)->AddInputLink(sensorNumber,
                    inputIdSensorFusion + sensorNumber);
        }

        // clone sensor and set specific parameters
        SensorParameter sensorParameter(sensor);
        sensorParameter.sensorProfile.parameters = curParameters;
        agentBuildInformation.sensorParameters.push_back(sensorParameter);

        sensorNumber++;
    }

    return *this;
}

DynamicAgentTypeGenerator& DynamicAgentTypeGenerator::SetVehicleModelParameters()
{
    VehicleProfile vehicleProfile = profiles->GetVehicleProfiles().at(sampledProfiles.vehicleProfileName);
    agentBuildInformation.vehicleModelName = vehicleProfile.vehicleModel;
    agentBuildInformation.vehicleModelParameters = vehicleModels->GetVehicleModel(vehicleProfile.vehicleModel);

    return *this;
}

void DynamicAgentTypeGenerator::GatherComponent(const std::string componentName,
        std::shared_ptr<SimulationSlave::AgentType> agentType)
{
    auto componentType = std::make_shared<SimulationSlave::ComponentType>(*systemConfigBlueprint->GetSystems().at(
                             0)->GetComponents().at(componentName));

    for (const auto& componentOutput : componentType->GetOutputLinks())
    {
        agentType->AddChannel(componentOutput.second);
    }

    if (!agentType->AddComponent(componentType))
    {
        throw std::runtime_error("Could not add component to agentType");
    }
}

void DynamicAgentTypeGenerator::GatherComponentWithParameters(std::string componentName,
        std::shared_ptr<SimulationSlave::AgentType> agentType,
        ParameterInterface* parameters)
{
    GatherComponentWithParameters(componentName, agentType, parameters, componentName, 0);
}

void DynamicAgentTypeGenerator::GatherComponentWithParameters(std::string componentName,
        std::shared_ptr<SimulationSlave::AgentType> agentType,
        ParameterInterface* parameters, std::string componentNameInSystemConfigBlueprint, int channelOffset)
{
    auto componentTypeFromSystemConfigBlueprint = systemConfigBlueprint->GetSystems().at(0)->GetComponents().at(componentNameInSystemConfigBlueprint);

    auto componentType = std::make_shared<SimulationSlave::ComponentType>(componentName,
                         componentTypeFromSystemConfigBlueprint->GetInit(),
                         componentTypeFromSystemConfigBlueprint->GetPriority(),
                         componentTypeFromSystemConfigBlueprint->GetOffsetTime(),
                         componentTypeFromSystemConfigBlueprint->GetResponseTime(),
                         componentTypeFromSystemConfigBlueprint->GetCycleTime(),
                         componentTypeFromSystemConfigBlueprint->GetModelLibrary());

    for (const auto& componentOutput : componentTypeFromSystemConfigBlueprint->GetOutputLinks())
    {
        componentType->AddOutputLink(componentOutput.first, componentOutput.second + channelOffset);
        agentType->AddChannel(componentOutput.second + channelOffset);
    }
    for (const auto& componentInput : componentTypeFromSystemConfigBlueprint->GetInputLinks())
    {
        componentType->AddInputLink(componentInput.first, componentInput.second + channelOffset);
    }

    componentType->SetModelParameter(parameters);

    if (!agentType->AddComponent(componentType))
    {
        throw std::runtime_error("Could not add component to agentType");
    }
}

DynamicAgentTypeGenerator AgentBuildInformation::make(SampledProfiles& sampledProfiles,
        DynamicParameters& dynamicParameters,
        std::shared_ptr<SystemConfigInterface> systemConfigBlueprint,
        ProfilesInterface* profiles,
        VehicleModelsInterface* vehicleModels)
{
    return DynamicAgentTypeGenerator(sampledProfiles, dynamicParameters, systemConfigBlueprint, profiles, vehicleModels);
}
