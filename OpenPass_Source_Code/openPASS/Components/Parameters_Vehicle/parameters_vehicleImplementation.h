/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/primitiveSignals.h"

/** \addtogroup Parameters_Vehicle
* @{
* \brief defines and distributes parameters (constants and stochasitc variables) for all components of an agent
*
* This component defines parameters for all other components of an agent. It includes constants and stochastic
* variables, which are divided into driver and vehicle parameters in the components output data structs.
*
* \section Parameters_Vehicle_Inputs Inputs
* none
*
* \section Parameters_Vehicle_InitInputs Init Inputs
* none
*
* \section ParametersAgent_Outputs Outputs
* **Output variables:**
*
* Information in output data struct out_vehicleParameters
* name | meaning
* ---|---
* airDragCoefficient                                  | The air drag coefficient of the vehicle
* axleRatio                                           | The ratio of the axle gear
* decelerationFromPowertrainDrag                      | The deceleration caused by the overall powertrain drag torque in m/s2
* distanceReferencePointToFrontAxle                   | The distance between the vehicle coordinate system's reference point (rear axle) and the front axle in m
* distanceReferencePointToLeadingEdge                 | The distance between the vehicle coordinate system's reference point (rear axle) and the front bumper in m
* frictionCoeff                                       | The friction coefficient between road and the vehicles tires
* frontSurface                                        | The projected front surface of the vehicle in m2
* gearRatios                                          | The ratios of all gears in the gearbox (no reverse gear)
* height                                              | The maximum height of the vehicle in m
* heightCOG                                           | The height of the center of gravity above ground in m
* length                                              | The maximum length of the vehicle in m
* maxCurvature                                        | The maximum curavture the vehicle is able to drive in 1/m
* maximumEngineSpeed                                  | The maximum engine speed in 1/min
* maximumEngineTorque                                 | The maximum torque of the engine in Nm
* maximumSteeringWheelAngleAmplitude                  | The maximum amplitude of the steering wheel angle in degree
* maxVelocity                                         | The maximum velocity of the vehicle in m/s
* minimumEngineSpeed                                  | The idle speed of the engine in 1/min
* minimumEngineTorque                                 | The drag torque of the engine in Nm
* momentInertiaPitch                                  | The moment of inertia along the vehicle's lateral axes in kgm2
* momentInertiaRoll                                   | The moment of inertia along the vehicle's longtudinal axes in kgm2
* momentInertiaYaw                                    | The moment of inertia along the vehicle's vertical axes in kgm2
* numberOfGears                                       | The number of gears in the gearbox (no reverse gear)
* staticWheelRadius                                   | The static wheel radius in m
* steeringRatio                                       | The ratio of the steering gear
* trackwidth                                          | The trackwidth of the vehicle in m (for all axles)
* vehicleType                                         | The type of the driver's vehicle
* weight                                              | The overall mass of the vehicle in kg
* wheelbase                                           | The wheelbase of the vehicle in m
* width                                               | The maximum width of the vehicle in m
*
* **Output channel IDs:**
* Output Id | signal class | contained variables
* ------------|--------------|-------------
*  1 | VehicleParameterSignal                 | out_vehicleParameters
*/

/*!
 * \brief defines constants or creates stochastic values that are used in more than one module
 *
 * \ingroup Parameters_Vehicle
 */
class ParametersVehicleImplementation : public SensorInterface
{
public:
    const std::string COMPONENTNAME = "ParametersVehicle";

    ParametersVehicleImplementation(
        std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface* stochastics,
        WorldInterface* world,
        const ParameterInterface* parameters,
        const std::map<int, ObservationInterface*>* observations,
        const CallbackInterface* callbacks,
        AgentInterface* agent);
    ParametersVehicleImplementation(const ParametersVehicleImplementation&) = delete;
    ParametersVehicleImplementation(ParametersVehicleImplementation&&) = delete;
    ParametersVehicleImplementation& operator=(const ParametersVehicleImplementation&) = delete;
    ParametersVehicleImplementation& operator=(ParametersVehicleImplementation&&) = delete;

    virtual ~ParametersVehicleImplementation() = default;

    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const>& data, int time);

    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const>& data, int time);

    virtual void Trigger(int time);

protected:

    /*!
    * \brief Starts the initialization sequence of the class instance - mostly parsing inputs
    */
    virtual void Initialize();

    //! data struct for vehicle parameters
    VehicleModelParameters out_vehicleParameters;

private:

    //! Flag that indicates the need to initialize - e.g parsing input informations
    bool initialisation = false;

    const CallbackInterface* callbacks;
};
