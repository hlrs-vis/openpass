/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
/**********************************************
***********************************************/

//! @file  AlgorithmAgentFollowingDriverModelImplementation.h
//! \brief This file contains the implementation header file.

#pragma once

#include "Interfaces/callbackInterface.h"
#include "Interfaces/modelInterface.h"
#include "Common/primitiveSignals.h"
#include "Components/Sensor_Driver/Signals/sensorDriverSignal.h"

class AgentInterface;
class StochasticsInterface;
class ObservationInterface;
class ParameterInterface;
class WorldInterface;

//! Calulaction parameters for the lateral acceleration wish [m/s].
struct LateralDynamicConstants
{
    //! Typical acceleration for a lane change [m/s].
    const double lateralAcceleration = 1.5;
    //! Typical lateral damping for a lane change [m/s].
    const double zeta = 1.0;
    //! aggressiveness of the controller for heading errors
    const double gainHeadingError = 7.5;
};

class AlgorithmAgentFollowingDriverModelImplementation : public SensorInterface
{
public:
    const std::string COMPONENTNAME = "AlgorithmAgentFollowingDriverModel";

    //! \brief Constructor.
    //!
    //! \param [in] componentId   Component ID
    //! \param [in] isInit        Component's init state
    //! \param [in] priority      Task priority level
    //! \param [in] offsetTime    Start time offset
    //! \param [in] responseTime  Update response time
    //! \param [in] cycleTime     Cycle time
    //! \param [in] stochastics   Stochastics instance
    //! \param [in] world         World interface
    //! \param [in] parameters    Paramaters
    //! \param [in] observations  Observation instance
    //! \param [in] callbacks     Callbacks
    //! \param [in] agent         Agent
    AlgorithmAgentFollowingDriverModelImplementation(
            std::string componentName,
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

    AlgorithmAgentFollowingDriverModelImplementation(const AlgorithmAgentFollowingDriverModelImplementation&) = delete;
    AlgorithmAgentFollowingDriverModelImplementation(AlgorithmAgentFollowingDriverModelImplementation&&) = delete;
    AlgorithmAgentFollowingDriverModelImplementation& operator=(const AlgorithmAgentFollowingDriverModelImplementation&) = delete;
    AlgorithmAgentFollowingDriverModelImplementation& operator=(AlgorithmAgentFollowingDriverModelImplementation&&) = delete;
    virtual ~AlgorithmAgentFollowingDriverModelImplementation();

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

    //----------------------------------------------------------------

private:

    OwnVehicleInformation ownVehicleInformation;

    GeometryInformation geometryInformation;

    SurroundingObjects surroundingObjects;

    //! component state for finely granulated evaluation of signal
    ComponentState componentState = ComponentState::Acting;

    //! parameters for the lateral acceleration wish [m/s]
    const LateralDynamicConstants lateralDynamicConstants;

    // constants from IDM paper
    //! desired speed
    double vWish = 120.0/3.6;
    //! free acceleration exponent characterizing how the acceleration decreases with velocity (1: linear, infinity: constant)
    double delta = 4.0;
    //! desired time gap between ego and front agent
    double tGapWish = 1.5;
    //! minimum distance between ego and front (used at slow speeds) also called jam distance
    double minDistance = 2.0;
    //! maximum acceleration in satisfactory way, not vehicle possible acceleration
    double maxAcceleration = 1.4;
    //! desired deceleration
    double decelerationWish = 2.0;

    //! The longitudinal velocity of the current vehicle [m/s].
    double out_longitudinal_speed = 0;
    //! The lateral velocity of the current vehicle [m/s].
    double out_lateral_speed = 0;
    //! The relative lateral position of the vehicle [m].
    double out_lateral_displacement = 0;
    //! The gain for lateral displacement error controller [-].
    double out_lateral_gain_displacement = 20.0;
    //! The damping constant of the lateral oscillation of the vehicle [].
    double out_lateral_damping = 0;
    //! The lateral oscillation frequency of the vehicle [1/s].
    double out_lateral_frequency = 0;
    //! The heading angle error of the vehicle [rad].
    double out_lateral_heading_error = 0;
    //! The gain for heading error controller [-].
    double out_lateral_gain_heading_error = 7.5;
    //! The curvature of the lane at vehicle's position [1/m].
    double out_curvature = 0;
    //! The width of the lane containing the vehicle [m].
    double out_laneWidth = 0;

    //! The longitudinal acceleration of the vehicle [m/s^2].
    double out_longitudinal_acc = 0;

    //! The state of the turning indicator [-].
    int out_indicatorState = static_cast<int>(IndicatorState::IndicatorState_Off);
    //! Activation of HornSwitch [-].
    bool out_hornSwitch = false;
    //! Activation of Headlight [-].
    bool out_headLight = false;
    //! Activation of Highbeam Light [-].
    bool out_highBeamLight = false;
    //! Activation of Flasher [-].
    bool out_flasher = false;
};
