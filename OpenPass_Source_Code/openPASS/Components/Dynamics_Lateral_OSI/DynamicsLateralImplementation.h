/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

/** \addtogroup DynamicsLateral
* @{
* \brief  models the operation of the steering wheel by the driver and the reaction of the vehicle on this
*
* This component models the operation of the steering wheel by the driver and
* the reaction of the vehicle on this. It calculates the yaw angle that
* results from the steering wheel angle wish of the driver.
*
*\section DynamicsLateral_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* in_desiredSteeringWheelAngle | The steering wheel angle wish of the driver in degree.
* in_alignToQueue = false      | This flag states whether the vehicle should be reset due to getting into a traffic jam during a lane change.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0   | SteeringSignal            | in_desiredSteeringWheelAngle
* 1   | BoolSignal                | in_alignToQueue
*
* \section DynamicsLateral_InitInputs Init Inputs
* Init input variables:
* name | meaning
* -----|------
* _steeringRatio               | The steering ratio transforming the steering wheel angles to the front wheel angles (from Init_Driver).
* _steeringMax                 | The maximal possible angle of the front wheels, in degree.
* _wheelbase                   | The wheelbase of the car in m.
*
*
* Init input channel IDs:
* Input Id | signal class | contained variables
* ------------|--------------|-------------
* 100 | IDriverToLateralSignal | _steeringRatio, _wheelbase, _steeringMax
*
* \section DynamicsLateral_Outputs Outputs
* Output variables:
* name | meaning
* -----|------
* out_steering_angle_degrees | The steering wheel angle in degree.
* out_steeringCurvature      | The steering curvature in degree.
*
* Output channel IDs:
* Output Id | signal class | contained variables
* ------------|--------------|-------------
* 0 | DLateralToPRfsSignal | out_steering_angle_degrees, out_steeringCurvature
*
* \section DynamicsLateral_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|---------
* SteeringSpeedMax     | 1000.0 | The maximal possible velocity of turning the steering wheel in degree/s.
* SteeringSpeedComfort | 180.0  | The normal velocity of turning the steering wheel in degree/s.
* AcuteDeficit         | 60.0   | The limit difference between wished steering wheel angle and current steering wheel angle above which the wheel will be turned with maximal velocity.
*
*   @} */

#pragma once

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"

/*!
 * \brief models the operation of the steering wheel by the driver and the reaction of the vehicle on this
 *
 * \ingroup DynamicsLateral
 */
class DynamicsLateralImplementation : public DynamicsInterface
{
public:
    const std::string COMPONENTNAME = "DynamicsLateral";

    DynamicsLateralImplementation(
        int componentId,
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
        AgentInterface *agent) :
        DynamicsInterface(
            componentId,
            isInit,
            priority,
            offsetTime,
            responseTime,
            cycleTime,
            stochastics,
            world,
            parameters,
            observations,
            callbacks,
            agent)
    {}
    DynamicsLateralImplementation(const DynamicsLateralImplementation&) = delete;
    DynamicsLateralImplementation(DynamicsLateralImplementation&&) = delete;
    DynamicsLateralImplementation& operator=(const DynamicsLateralImplementation&) = delete;
    DynamicsLateralImplementation& operator=(DynamicsLateralImplementation&&) = delete;
    virtual ~DynamicsLateralImplementation() = default;

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
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

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
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    /*!
    * \brief Process data within component.
    *
    * Function is called by framework when the scheduler calls the trigger task
    * of this component
    *
    * @param[in]     time           Current scheduling time (unused)
    */
    virtual void Trigger(int time);

private:
    // module internal functions
    /*!
    * \brief Returns the change of the steering wheel in degrees.
    *
    * Considers comfort and maximum change rate of the steering wheel to calculate the steering wheel angle, which is used to reach a desired position.
    *
    * @return     Change of the steering wheel in degrees.
    */
    double GetDeltaSteeringWheelAngle();

    // module internal variables
    bool _doInit = true;
    double _steeringWheelangle = 0;

    unsigned int _nCollisionsObserved = 0;

    // --- Inputs
    //! The steering wheel angle wish of the driver in degree.
    double in_desiredSteeringWheelAngle = 0;
    //! This flag states whether the vehicle should be reset due to getting into a traffic jam during a lane change.
    bool in_alignToQueue = false;

    // --- Outputs
    //! The steering wheel angle in degree.
    double out_steering_angle_degrees = 0;
    //! The steering curvature in degree.
    double out_steeringCurvature = 0;

    // --- Init Inputs
    //! The steering ratio transforming the steering wheel angles to the front wheel angles (from Init_Driver).
    double _steeringRatio = 10.7;
    //! The maximal possible angle of the front wheels, in degree.
    double _steeringMax = 180.0;
    //! The wheelbase of the car in m.
    double _wheelbase = 2.89;

    // from parameters

    // --- Internal Parameters
    //! The maximal possible velocity of turning the steering wheel in degrees/s.
    double SteeringSpeedMax = 1000.0;
    //! The normal velocity of turning the steering wheel in degrees/s.
    double SteeringSpeedComfort = 180.0;
    //! The limit difference between wished steering wheel angle and current steering wheel angle above which the wheel will be turned with maximal velocity.
    double AcuteDeficit = 60.0;

    // from Constants
    const double DegreeToRadiant = 0.01745329251994329576923690768489;
};


