/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
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
#include "Common/sensorDataSignal.h"

/** \addtogroup Algorithm_Lateral
* @{
* \brief models the lateral controller of the driver
*
* This component models the lateral controller of the driver. It calculates a
* steering wheel angle wish of the driver that tries to diminish the lateral
* offset to the current or target lane in a lane change. This wish depends
* also on the intensity of a lane change.
*
* \section Algorithm_Lateral_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* in_LatDisplacement        | The current control deviation of lateral displacment in m.
* in_lateralDeviation       | The lateral deviation from the lane change or swerving trajectory [m].
* in_headingError           | The heading error regarding the lane change or swerving trajectory [rad].
* in_gainLateralDeviation   | Gain for lateral deviation control in AlgorithmLateralDriver [rad/s^2].
* in_gainHeadingError       | Gain for heading error control in AlgorithmLateralDriver [1/s].
* in_kappaSet               | Set value for trajectory curvature [1/m].
* velocity                  | Current velocity
* steeringWheelAngle        | Current steering wheel angle
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0 | LateralSignal | laneWidth, lateralDeviation, gainLateralDeviation, headingError, gainHeadingError, kappaSet
* 101 | SensorDriverSignal | used from signal: velocity, steeringWheelAngle
*
* \section Algorithm_Lateral_InitInputs Init Inputs
* Init input variables:
* name | meaning
* -----|------
* in_steeringRatio          | The steering ratio of the vehicle.
* in_steeringMax            | The maximum possible steering wheel angle in both directions in degree.
* in_wheelBase              | The wheel base of the vehicle in m.
*
* Note that these variables are only used implicetly by the component without assigning the values to variable names.
*
* Init input channel IDs:
* Input Id | signal class | contained variables
* ------------|--------------|-------------
* 100 | ParametersVehicleSignal | used from signal: steeringRatio, steeringMax, wheelbase
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
* \section Algorithm_Lateral_ExternalParameters External parameters
* none
*
* \section Algorithm_Lateral_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|------.
*
*   @} */

/*!
* \brief models the lateral controller of the driver
*
* This class models the lateral controller of the driver. It calculates a
* steering wheel angle wish of the driver that tries to diminish the lateral
* offset to the current or target lane in a lane change. This wish depends
* also on the intensity of a lane change.
*
* \ingroup Algorithm_LateralDriver
*/
class AlgorithmLateralImplementation : public AlgorithmInterface
{
public:
    const std::string COMPONENTNAME = "AlgorithmLateral";

    AlgorithmLateralImplementation(
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
    {}
    AlgorithmLateralImplementation(const AlgorithmLateralImplementation&) = delete;
    AlgorithmLateralImplementation(AlgorithmLateralImplementation&&) = delete;
    AlgorithmLateralImplementation& operator=(const AlgorithmLateralImplementation&) = delete;
    AlgorithmLateralImplementation& operator=(AlgorithmLateralImplementation&&) = delete;
    virtual ~AlgorithmLateralImplementation() = default;

    /*!
    * \brief Update Inputs
    *
    * Function is called by framework when another component delivers a signal over
    * a channel to this component (scheduler calls update taks of other component).
    *
    * Refer to module description for input channels and input ids.
    *
    * @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    * @param[in]     data           Referenced signal (copied by sending component)
    * @param[in]     time           Current scheduling time
    */
    void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

    /*!
    * \brief Update outputs.
    *
    * Function is called by framework when this Component.has to deliver a signal over
    * a channel to another component (scheduler calls update task of this component).
    *
    * Refer to module description for output channels and output ids.
    *
    * @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    * @param[out]    data           Referenced signal (copied by this component)
    * @param[in]     time           Current scheduling time
    */
    void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    /*!
    * \brief Process data within component.
    *
    * Function is called by framework when the scheduler calls the trigger task
    * of this component
    *
    * @param[in]     time           Current scheduling time
    */
    void Trigger(int time);

private:
    // --- module internal functions

    // --- module internal variables
    //  --- Inputs
    //! Current lateral deviation regarding trajectory [m].
    double in_lateralDeviation = 0;
    //! Gain for lateral deviation controller [-].
    double in_gainLateralDeviation = 20.0;
    //! Current heading error regarding trajectory [rad].
    double in_headingError = 0;
    //! Gain for heading error controller [-].
    double in_gainHeadingError = 7.5;
    //! Set value for trajectory curvature [1/m].
    double in_kappaSet = 0;
    //! current velocity
    double velocity = 0.0;
    //! current angle of the steering wheel
    double steeringWheelAngle = 0.0;

    //  --- Outputs
    //! The steering wheel angle wish of the driver in degree.
    double out_desiredSteeringWheelAngle{0};
    /** @} @} */

    //  --- Internal Parameters

    //! The steering ratio of the vehicle.
    double in_steeringRatio = 10.7;
    //! The maximum steering wheel angle of the car in both directions in degree.
    double in_steeringMax = 180.0;
    //! The wheelbase of the car in m.
    double in_wheelBase = 2.89;
    //! Helper constant to convert radiant into degree.
    const double RadiantToDegree = 57.295779513082320876798154814105;
    /** @} @} */

    bool isActive{false};

    //! Previous scheduling time (for calculation of cycle time lenght).
    int timeLast{-100};
};
