/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"

/** \addtogroup AlgorithmLateralDriver
* @{
* \brief models the lateral controller of the driver
*
* This component models the lateral controller of the driver. It calculates a
* steering wheel angle wish of the driver that tries to diminish the lateral
* offset to the current or target lane in a lane change. This wish depends
* also on the intensity of a lane change which is modelled with the model
* factors in_freqLateralOscillation and in_zeta.
*
* \section AlgorithmLateralDriver_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* in_LatDisplacement          | The current control deviation of lateral displacment in m.
* in_currCurvature          | The curvature of the road at the current position of the agent in 1/m.
* in_vLongEgo               | The longitudinal velocity of the  vehicle in m/s.
* in_vLatEgo                | The lateral velocity of the vehicle in m/s.
* in_freqLateralOscillation | The eigenfrequency of the lateral acceleration PT2-Controller in Hz.
* in_notifyCollision        | This flag states whether a collision has freshly occurred in the current time step.
* in_steeringRatio          | The steering ratio of the vehicle.
* in_steeringMax            | The maximum possible steering wheel angle in both directions in degree.
* in_wheelBase              | The wheel base of the vehicle in m.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0   | LateralDriverSignal                          | in_currCurvature, in_laneWidth, in_vLatEgo, in_vLongEgo, in_notifyCollision, in_freqLateralOscillation, in_zeta, in_LatDisplacement, in_isLaneChangeAborted
*
* \section AlgorithmLateralDriver_InitInputs Init Inputs
* Init input variables:
* name | meaning
* -----|------
* steeringRatio | Steering ratio.
* steeringMax   | Maximum steering angle [].
* wheelbase     | Wheelbase [m].
*
* Note that these variables are only used implicetly by the component without assigning the values to variable names.
*
* Init input channel IDs:
* Input Id | signal class | contained variables
* ------------|--------------|-------------
* 100 | IDriverToLateralSignal | used from signal: steeringRatio, steeringMax, wheelbase
*
* \section AlgorithmLateralDriver_Outputs Outputs
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
* \section AlgorithmLateralDriver_ExternalParameters External parameters
* none
*
* \section AlgorithmLateralDriver_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|------
* VLim          | 7.0  | The limit velocity below which the damping of the model is maximised to Zeta0 in m/s.
* Zeta0         | 1.0  | The default damping parameter for small velocities.
* Curvaturemax0 | 0.01 | The maximal considered curvature due to the steering of the driver for small velocities in 1/m.
* Curvaturemax  | 0.1  | The default maximal considered curvature due to the steering of the driver in 1/m.
* SmallV        | 1.0  | The limit velocity below which the maximal considered curvature is Curvaturemax0 in m/s.
* Curvaturemax2 | 0.04 | The maximal considered curvature due to the curvature of the street in 1/m.
*
*   @} */

/*!
* \brief models the lateral controller of the driver
*
* This class models the lateral controller of the driver. It calculates a
* steering wheel angle wish of the driver that tries to diminish the lateral
* offset to the current or target lane in a lane change. This wish depends
* also on the intensity of a lane change which is modelled with the model
* factors in_freqLateralOscillation and in_zeta.
*
* \ingroup AlgorithmLateralDriver
*/
class AlgorithmLateralDriverImplementation : public AlgorithmInterface
{
public:
    const std::string COMPONENTNAME = "AlgorithmLateralDriver";

    AlgorithmLateralDriverImplementation(
        int componentId,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface*> *observations,
        const CallbackInterface *callbacks,
        int agentId) :
        AlgorithmInterface(
            componentId,
            isInit,
            priority,
            offsetTime,
            responseTime,
            cycleTime,
            stochastics,
            parameters,
            observations,
            callbacks,
            agentId)
    {}
    AlgorithmLateralDriverImplementation(const AlgorithmLateralDriverImplementation&) = delete;
    AlgorithmLateralDriverImplementation(AlgorithmLateralDriverImplementation&&) = delete;
    AlgorithmLateralDriverImplementation& operator=(const AlgorithmLateralDriverImplementation&) = delete;
    AlgorithmLateralDriverImplementation& operator=(AlgorithmLateralDriverImplementation&&) = delete;
    virtual ~AlgorithmLateralDriverImplementation() = default;

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
    //-----------------------------------------------------------------------------
    /*!
    * \brief Returns the lateral acceleration wish.
    *
    * \details Returns the lateral acceleration wish.
    *
    * @param[in]    DeltaY  Lateral displacement.
    *
    * @return       lateral acceleration wish.
    */
    //-----------------------------------------------------------------------------
    double ALatWish(double DeltaW);

    //-----------------------------------------------------------------------------
    /*!
    * \brief Returns the desired curvature.
    *
    * \details Returns the desired curvature.
    *
    * @param[in]    accLatTarget         Desired acceleration.
    *
    * @return       Desired curvature.
    */
    //-----------------------------------------------------------------------------
    double CurvatureTarget(double accLatTarget);

    // --- module internal variables
    //  --- Inputs
    //! The displacement to the current or target lane in a lane change in m.
    double in_LatDisplacement;
    //! The heading angle error relative to the road in rad.
    double in_HeadingError;
    //! The curvature at the current position of the agent in 1/m.
    double in_currCurvature = 0;
    //! The velocity of the current vehicle in m/s.
    double in_vLongEgo = 0;
    //! The lateral velocity of the current vehicle in m/s.
    double in_vLatEgo = 0;
    //! The parameter that models the frequency of the driver steering his vehicle to oscillate around the centre of the road.
    double in_freqLateralOscillation = 0;
    //! The parameter that models the damping of the driver reducing the lateral deviation to the target lanes centre.
    double in_zeta = 0;
    //! The gain of the heading error controller.
    double in_GainDeltaPhi = 0.5;
    //! This flag states whether a collision has freshly occurred in the current time step.
    bool in_notifyCollision = false;

    //  --- Outputs
    //! The steering wheel angle wish of the driver in degree.
    double out_desiredSteeringWheelAngle{0};
    /** @} @} */

    //  --- Internal Parameters

    //! The limit velocity below which the damping of the model is maximised to Zeta0 in m/s.
    const double VLim = 7.0;
    //! The default damping parameter for small velocities.
    const double Zeta0 = 1.0;
    //! The maximal considered curvature due to the steering of the driver for small velocities in 1/m.
    const double Curvaturemax0 = 0.01;
    //!The default maximal considered curvature due to the steering of the driver in 1/m.
    const double Curvaturemax = 0.1;
    //! The limit velocity below which the maximal considered curvature is Curvaturemax0 in m/s.
    const double SmallV = 1.0;
    //! The maximal considered curvature due to the curvature of the street in 1/m.
    const double Curvaturemax2 = 0.04;
    //! The steering ratio of the vehicle.
    double in_steeringRatio = 10.7;
    //! The maximum steering wheel angle of the car in both directions in degree.
    double in_steeringMax = 180.0;
    //! The wheelbase of the car in m.
    double in_wheelBase = 2.89;
    //! Helper constant to convert radiant into degree.
    const double RadiantToDegree = 57.295779513082320876798154814105;
    /** @} @} */
};
