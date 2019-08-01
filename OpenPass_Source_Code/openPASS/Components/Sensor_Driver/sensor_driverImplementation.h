/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "Signals/sensor_driverDefinitions.h"
#include "Common/primitiveSignals.h"
#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "sensor_driverCalculations.h"

/** \addtogroup Sensor_Driver
* @{
* \brief Collect and distribute Data from the WorldInterface and AgentInterface concerning the own vehicle, traffic rule information, lane geometry and surrounding objects.
*
* This component creates a data struct to store the world and agent data of the own vehicle, traffic rule
* information, lane geometry and surrounding vehicles.
* Own vehicle data examples are ID, velocity, heading angle, yaw angle, ... for the ego vehicle.
* Traffic rule information are traffic signs for each lane.
* Lane geometry is visibility distance and lane specifics like lane curvature, lane width, ...
* Surrounding object data (vehicles or statics) is for example distance, velocity ... for different positions around the agent.
*
* \section SD_Inputs Inputs
* name | meaning
* ----------        | -----------
* agent             | This interface provides access to agent parameters, properties, attributes and dynamic states
* callbacks         | Interface for callbacks to framework
* componentId       | Id of the component
* cycleTime         | Cycle time of this components trigger task [ms]
* data              | Referenced signal (copied by sending component)
* isInit            | Query whether the component was just initialized
* localLinkId       | Corresponds to "id" of "ComponentInput"
* observations      | Interface which has to be provided by observation modules
* offsetTime        | Offset time of the component
* parameters        | Interface provides access to the configuration parameters
* priority          | Priority of the component
* responseTime      | Response time of the component
* stochastics       | Provides access to the stochastics functionality of the framework
* time              | Current scheduling time
* world             | Provides access to world representation
*
* \section SD_Outputs Outputs
* name | meaning
* -|-
* none
*
* Output channel IDs:
* Output Id     | signal class              | contained variables
* --------------|---------------------------|-------------
* 0             | SensorDriverSignal        | ownVehicleInformation, trafficRuleInformation, geometryInformation, surroundingObjects
*
*
* @} */

class SensorDriverImplementation : public SensorInterface
{
public:
    //! \brief Constructor.
    //! \param [in] componentName   Name of the component
    //! \param [in] isInit          Query whether the component was just initialized
    //! \param [in] priority        Priority of the component
    //! \param [in] offsetTime      Offset time of the component
    //! \param [in] responseTime    Response time of the component
    //! \param [in] cycleTime       Cycle time of this components trigger task [ms]
    //! \param [in] stochastics     Provides access to the stochastics functionality of the framework
    //! \param [in] world           Provides access to world representation
    //! \param [in] parameters      Interface provides access to the configuration parameters
    //! \param [in] observations    Interface which has to be provided by observation modules
    //! \param [in] callbacks       Interface for callbacks to framework
    //! \param [in] agent           This interface provides access to agent parameters, properties, attributes and dynamic states
    SensorDriverImplementation(std::string componentName,
            bool isInit,
            int priority,
            int offsetTime,
            int responseTime,
            int cycleTime,
            StochasticsInterface *stochastics,
            WorldInterface *world,
            const ParameterInterface *parameters,
            const std::map<int, ObservationInterface*> *observations,
            const CallbackInterface *callbacks,
            AgentInterface *agent);

    SensorDriverImplementation(const SensorDriverImplementation&) = delete;
    SensorDriverImplementation(SensorDriverImplementation&&) = delete;
    SensorDriverImplementation& operator=(const SensorDriverImplementation&) = delete;
    SensorDriverImplementation& operator=(SensorDriverImplementation&&) = delete;

    virtual ~SensorDriverImplementation() = default;

    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    virtual void Trigger(int time);

    //! Name of the component for debug output
    const std::string COMPONENTNAME = "SensorDriver";

private:

    //! \brief Get sensor data concerning the own vehicle.
    virtual void GetOwnVehicleInformation();

    //! \brief Get sensor data containing traffic rule information.
    virtual void GetTrafficRuleInformation();

    //! \brief Get traffic rule sensor data from the ego lane.
    LaneInformationTrafficRules GetTrafficRuleLaneInformationEgo();

    //! \brief Get traffic rule sensor data from the lane left of ego.
    LaneInformationTrafficRules GetTrafficRuleLaneInformationLeft();

    //! \brief Get traffic rule sensor data from the lane right of ego.
    LaneInformationTrafficRules GetTrafficRuleLaneInformationRight();

    //! \brief Get lane geometry sensor data.
    virtual void GetGeometryInformation();

    //! \brief Get lane geometry sensor data from the ego lane.
    LaneInformationGeometry GetGeometryLaneInformationEgo();

    //! \brief Get lane geometry sensor data from the lane left of ego.
    LaneInformationGeometry GetGeometryLaneInformationLeft();

    //! \brief Get lane geometry sensor data from the lane right of ego.
    LaneInformationGeometry GetGeometryLaneInformationRight();

    //! \brief Get sensor data of surrounding objects.
    virtual void GetSurroundingObjectsInformation();

    //! \brief Get information of one object.
    virtual ObjectInformation GetOtherObjectInformation(const WorldObjectInterface *surroundingObject);

    SensorDriverCalculations sensorDriverCalculations;

    //! \brief Struct for all sensor data concerning the own vehicle
    OwnVehicleInformation ownVehicleInformation;
    //! \brief Struct for all sensor data concerning traffic rule information
    TrafficRuleInformation trafficRuleInformation;
    //! \brief Struct for all sensor data concerning lane geometry information
    GeometryInformation geometryInformation;
    //! \brief Struct for all sensor data concerning surrounding objects
    SurroundingObjects surroundingObjects;
};
