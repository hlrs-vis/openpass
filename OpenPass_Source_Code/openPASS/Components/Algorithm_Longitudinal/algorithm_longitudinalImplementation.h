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

//! @file algorithm_longitudinalImplementation.h


/** \addtogroup Algorithm_Longitudinal
* @{

* \brief models the longitudinal controller of the driver.
*
* This component models the longitudinal controller of the driver / some ADAS.
* It calculates the positions of the acceleration pedal and the brake pedal,
* as well as the chosen gear and models the actuation of the pedals.
*
* \section Algorithm_Longitudinal_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* accelerationWish | wish acceleration
* velocity | current velocity
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0 | AccelerationSignal | accelerationWish
* 101 | SensorDriverSignal | used from signal: velocity
*
* \section Algorithm_Longitudinal_InitInputs Init Inputs
* Init input variables:
* name | meaning
* -----|------
* vehicleModelParameters | VehicleModelParameters
*
* Init input channel IDs:
* Input Id | signal class | contained variables
* ------------|--------------|-------------
* 100 | ParametersVehicleSignal | vehicleModelParameters
*
* \section Algorithm_Lateral_Outputs Outputs
* Output variables:
* name | meaning
* -----|------
* out_desiredSteeringWheelAngle | The steering wheel angle wish of the driver in degree.
*
* Output channel IDs:
* Output Id | signal class | contained variables
* ------------|--------------|-------------
* 0 | SteeringSignal | out_desiredSteeringWheelAngle
*
* \section Algorithm_Longitudinal_ExternalParameters External parameters
* none
*
* \section Algorithm_Longitudinal_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|------.
*
*   @} */

#pragma once

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/primitiveSignals.h"
#include "algorithm_longitudinalCalculations.h"
#include "Components/Sensor_Driver/Signals/sensorDriverSignal.h"

/*! \addtogroup Algorithm_Longitudinal
* @{
* \brief models the longitudinal controller of the driver.
*
* This component models the longitudinal controller of the driver / some ADAS.
* It calculates the positions of the acceleration pedal and the brake pedal,
* as well as the chosen gear and models the actuation of the pedals.
*
*/
class AlgorithmLongitudinalImplementation : public AlgorithmInterface
{
public:

     //! Name of the current component
    const std::string COMPONENTNAME = "AlgorithmLongitudinal";

    AlgorithmLongitudinalImplementation(
        std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface*> *observations,
        const CallbackInterface *callbacks,
        AgentInterface *agent) :
        AlgorithmInterface(
            componentName,
            isInit,
            priority,
            offsetTime,
            responseTime,
            cycleTime,
            stochastics,
            parameters,
            observations,
            callbacks,
            agent)
    {
    }
    AlgorithmLongitudinalImplementation(const AlgorithmLongitudinalImplementation&) = delete;
    AlgorithmLongitudinalImplementation(AlgorithmLongitudinalImplementation&&) = delete;
    AlgorithmLongitudinalImplementation& operator=(const AlgorithmLongitudinalImplementation&) = delete;
    AlgorithmLongitudinalImplementation& operator=(AlgorithmLongitudinalImplementation&&) = delete;

    virtual ~AlgorithmLongitudinalImplementation() = default;

    void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time) override;

    void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time) override;

    void Trigger(int time) override;

private:

    //! \brief Calculate the pedal positions and gear
    void CalculatePedalPositionAndGear();

    // --- Inputs

    ComponentState componentState {ComponentState::Armed};

    //! initialize module with sended prioritzer data
    bool initializedAccelerationInput{false};

    //! initialize module with sended vehicle parameters sended from ParametersAgent
    bool initializedVehicleModelParameters{false};

    //! initialize module with sended parameters sended from SensoDriver
    bool initializedSensorDriverData{false};

    //! The wish acceleration of the agent in m/s^2.
    double accelerationWish{0.0};

    //! current agent velocity
    double currentVelocity{0.0};

    //  --- Outputs

    //! Position of the accelerator pedal position in percent.
    double out_accPedalPos{0.0};
    //! Position of the brake pedal position in percent.
    double out_brakePedalPos{0.0};
    //! Currently choosen gear.
    int out_gear{0};

    //  --- Init Inputs

    //! contains: double carMass; double rDyn and more;
    VehicleModelParameters vehicleModelParameters;

};
