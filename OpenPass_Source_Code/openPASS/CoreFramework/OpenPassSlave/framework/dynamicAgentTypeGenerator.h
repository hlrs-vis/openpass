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
#include "vehicleModels.h"
#include "agentType.h"
#include "componentType.h"
#include "dynamicProfileSampler.h"
#include "systemConfig.h"
#include "dynamicParametersSampler.h"

/*!
 * \brief Defines the default components that all agents need
 */
struct DefaultComponents
{
    // Basic components that are always needed
    const std::vector<std::string> basicComponentNames
    {
        "AgentUpdater",
        "ComponentController",
        "Dynamics_Collision",
        "Dynamics_RegularDriving",
        "Parameters_Vehicle",
        "PrioritizerDynamics",
        "PrioritizerLongitudinal",
        "PrioritizerSteering",
        "Sensor_RecordState"
    };

    //Components that are needed, if at least one driver is present
    const std::vector<std::string> driverComponentNames
    {
        "Action_LongitudinalDriver",
        "Action_SecondaryDriverTasks",
        "PrioritizerTurningIndicator"
    };

    //Components that are needed, if at least one vehicle component is present
    const std::vector<std::string> vehicleComponentNames
    {
        "Algorithm_LateralVehicleComponents",
        "Algorithm_LongitudinalVehicleComponents",
        "PrioritizerAccelerationVehicleComponents",
        "PrioritizerLateralVehicleComponents",
        "LimiterAccelerationVehicleComponents"
    };
};

class DynamicAgentTypeGenerator;

/*!
 * \brief The AgentBuildInformation struct contains all information that the AgentBlueprintProvider needs to
 * fill out the AgentBlueprint for a dynamic AgentProfile.
 *
 * An instance of this struct is aquired by calling the static make function and then sequentially calling all
 * building function on the returned DynamicAgentTypeGenerator
 */
struct AgentBuildInformation
{
    friend class DynamicAgentTypeGenerator;
    std::shared_ptr<SimulationSlave::AgentType> agentType = std::make_shared<SimulationSlave::AgentType>();
    std::string vehicleModelName;
    VehicleModelParameters vehicleModelParameters;
    std::vector<SensorParameter> sensorParameters;

    /*!
     * \brief Creates a new DynamicAgentTypeGenerator which can be used to build an instance of this struct.
     *
     * \param sampledProfiles         Profile names sampled by the DynamicProfilesSampler
     * \param dynamicParameters       Parameters samplied by the DynamicParametersSampler
     * \param systemConfigBlueprint   Imported SystemConfigBlueprint
     * \param profiles                Imported profiles from the ProfilesCatalog
     * \param vehicleModels           Imported vehicleModels from the VehiclesModelCatalog
     *
     * \return new instance of DynamicAgentTypeGenerator
     */
    static DynamicAgentTypeGenerator make(SampledProfiles& sampledProfiles, DynamicParameters& dynamicParameters, std::shared_ptr<SystemConfigInterface> systemConfigBlueprint, ProfilesInterface* profiles, VehicleModelsInterface* vehicleModels);

private:
    /*! Private constructor to be called by the DynamicAgentTypeGenerator
     */
    AgentBuildInformation() = default;
};

/*!
 * \brief The DynamicAgentTypeGenerator class is used to build the AgentBuildInformation struct in particular the AgentType
 *
 * This class is responsible for generating the AgentType for building an agent in the AgentFactory in the case of a dynamic AgentProfile.
 * This AgentType contains the Driver, all Sensors and VehicleComponents that are definied by the sampled profiles as well as the defaultComponents,
 * that are always needed. The DynamicAgentTypeGenerator also stores the information about the channels between these components in the AgentType.
 */
class DynamicAgentTypeGenerator
{
public:
    DynamicAgentTypeGenerator(SampledProfiles& sampledProfiles, DynamicParameters& dynamicParameters, std::shared_ptr<SystemConfigInterface> systemConfigBlueprint, ProfilesInterface* profiles, VehicleModelsInterface* vehicleModels);

    /*!
     * \brief Returns the generated AgentBuildInformation;
     */
    operator AgentBuildInformation&&()
    {
        return std::move(agentBuildInformation);
    }

    /*!
     * \brief Creates a new ComponentType for all basic components defined in DefaultComponents.basicComponents and stores them in the AgentType.
     * Also collects there output channels
     *
     * \return reference to itself
     */
    DynamicAgentTypeGenerator& GatherBasicComponents();

    /*!
     * \brief Creates a new ComponentType for all default driver components defined in DefaultComponents.basicComponents, the ParametersAgents and
     * the driver module itself and stores them in the AgentType. Also collects there output channels
     *
     * \return reference to itself
     */
    DynamicAgentTypeGenerator& GatherDriverComponents();

    /*!
     * \brief Creates a new ComponentType for all default vehicle components defined in DefaultComponents.basicComponents and
     * the sampled vehicle components module and stores them in the AgentType. Also collects there output channels
     *
     * \return reference to itself
     */
    DynamicAgentTypeGenerator& GatherVehicleComponents();

    /*!
     * \brief Creates a new ComponentType for all sensor defined by the VehicleModel and the SensorFusion module if needed
     * and stores them in the AgentType. Also collects there output channels
     *
     * \return reference to itself
     */
    DynamicAgentTypeGenerator& GatherSensors();

    /*!
     * \brief Sets the VehicleModelParameters in the AgentBuildInformation depending on the vehicle model name.
     * \return  reference to itself
     */
    DynamicAgentTypeGenerator& SetVehicleModelParameters();

    /*!
    * \brief Gathers a component and adds it to the AgentType
    *
    * \details  Retrieves the information of a component from the app config and adds the ComponentType to the AgentType.
    * The parameter set is read from the app config specified by the parameter set name
    *
    * @param[in]    componentName               Name of the component
    * @param[out]   agentType                   AgentType where the results get stored
    */
    void GatherComponent(const std::string componentName,
                         std::shared_ptr<SimulationSlave::AgentType> agentType);

    /*!
    * \brief Gathers a component with specified parameter set and adds it to the AgentType
    *
    * \details  Retrieves the information of a component from the app config and adds the ComponentType to the AgentType.
    * The parameter set is explictly specified.
    *
    * @param[in]    componentName               Name of the component
    * @param[out]   agentType                   AgentType where the results get stored
    * @param[in]    parameters                  Pointer to a specific parameters set.
    */
    void GatherComponentWithParameters(std::string componentName, std::shared_ptr<SimulationSlave::AgentType> agentType, ParameterInterface *parameters);

    /*!
    * \brief Gathers a component and adds it to the AgentType
    *
    * \details  Retrieves the information of a component from the app config and adds the ComponentType to the AgentType.
    * The parameter set is explictly specified.
    *
    * @param[in]    componentName               Name of the component
    * @param[out]   agentType                   AgentType where the results get stored
    * @param[in]    parameters                  Pointer to a specific parameters set.
    * @param[in]    componentNameInSystemConfigBlueprint    Name of the component in the app config, different if different from componentName (currently used for sensors)
    * @param[in]    channelIncrease             Increase in the ids of the output channels (currently used for sensors)
    */
    void GatherComponentWithParameters(std::string componentName, std::shared_ptr<SimulationSlave::AgentType> agentType, ParameterInterface *parameters,
                                                                                  std::string componentNameInSystemConfigBlueprint, int channelOffset);

    /*!
    * \brief Samples the latency of a sensor
    *
    * \details  Samples the latency of a sensor based on the parameters provided.
    *
    * @param[in]    curParameters               Parameter interface containing the mean value, standard deviation, minimum and maximum
    * @return       latency in s
    */
    double SampleSensorLatency(std::shared_ptr<ParameterInterface> curParameters);
private:
    AgentBuildInformation agentBuildInformation;

    SampledProfiles& sampledProfiles;
    DynamicParameters& dynamicParameters;
    std::shared_ptr<SystemConfigInterface> systemConfigBlueprint;
    ProfilesInterface* profiles;
    VehicleModelsInterface* vehicleModels;
    DefaultComponents defaultComponents;
};
