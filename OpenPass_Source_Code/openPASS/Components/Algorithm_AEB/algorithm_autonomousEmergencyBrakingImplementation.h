/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
/** \addtogroup Algorithm_AEB
* @{
* \brief This module represents a simple autonomous emergency braking system (AEB).
*
* This module represents a simple autonomous emergency braking system (AEB). If the predicted time to collision (TTC)
* with an other object detected by a sensor is below a certain treshold it brake with a constant deceleration. The module
* deactivates again if the TTC is above 1.5 times the treshold
*
* \section inputs Inputs
* name | meaning
* -----|---------
* detectedMovingObjects | moving objects detected by a sensor
* detectedStationaryObjects | stationary objects detected by a sensor
*
* \section outputs Outputs
* name | meaning
* -----|---------
* activeAcceleration | current acceleration of this module
* componentState | current state of this module
*
* @} */

#pragma once

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/primitiveSignals.h"
#include "Common/accelerationSignal.h"
#include "agentBasedEvent.h"
#include <vector>
#include "Common/sensorDataSignal.h"
#include "osi/osi_sensordata.pb.h"

class AlgorithmAutonomousEmergencyBrakingImplementation : public AlgorithmInterface
{
public:
    const std::string COMPONENTNAME = "Algorithm_AutonomousEmergencyBraking";

    AlgorithmAutonomousEmergencyBrakingImplementation(
        std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface* stochastics,
        const ParameterInterface* parameters,
        const std::map<int, ObservationInterface*>* observations,
        const CallbackInterface* callbacks,
        AgentInterface* agent);
    AlgorithmAutonomousEmergencyBrakingImplementation(const AlgorithmAutonomousEmergencyBrakingImplementation&) = delete;
    AlgorithmAutonomousEmergencyBrakingImplementation(AlgorithmAutonomousEmergencyBrakingImplementation&&) = delete;
    AlgorithmAutonomousEmergencyBrakingImplementation& operator=(const AlgorithmAutonomousEmergencyBrakingImplementation&) =
        delete;
    AlgorithmAutonomousEmergencyBrakingImplementation& operator=(AlgorithmAutonomousEmergencyBrakingImplementation&&) =
        delete;
    virtual ~AlgorithmAutonomousEmergencyBrakingImplementation() = default;

    /*!
     * \brief Update Inputs
     *
     * Function is called by framework when another component delivers a signal over
     * a channel to this component (scheduler calls update taks of other component).
     *
     * Refer to module description for input channels and input ids.
     *
     * \param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
     * \param[in]     data           Referenced signal (copied by sending component)
     * \param[in]     time           Current scheduling time
     */
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const>& data, int time);

    /*!
     * \brief Update outputs.
     *
     * Function is called by framework when this Component.has to deliver a signal over
     * a channel to another component (scheduler calls update task of this component).
     *
     * Refer to module description for output channels and output ids.
     *
     * \param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
     * \param[out]    data           Referenced signal (copied by this component)
     * \param[in]     time           Current scheduling time
     */
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const>& data, int time);

    /*!
     * \brief Process data within component.
     *
     * Function is called by framework when the scheduler calls the trigger task
     * of this component.
     *
     * Refer to module description for information about the module's task.
     *
     * \param[in]     time           Current scheduling time
     */
    virtual void Trigger(int time);

    //for testing
    double GetAcceleration()
    {
        return activeAcceleration;
    }

private:
    /*!
     * ------------------------------------------------------------------------
     * \brief Reads the information of ttc, acceleration from the
     *        ParameterInterface and stores them in the stageParameters struct.
     *
     * \param parameters ParameterInterface containing braking information
     * ------------------------------------------------------------------------
     */
    void ParseParameters(const ParameterInterface* parameters);

    /*!
     * \brief Calculates the minimim ttc (time to collision) of all objects the linked sensors
     * have detected
     *
     * \return minimum ttc
     */
    double CalculateTTC();

    /*!
     * \brief Calculates the ttc between a moving object and the own agent
     * \param baseMoving osi BaseMoving of other object
     * \return ttc
     */
    double CalculateObjectTTC(const osi3::BaseMoving& baseMoving);

    /*!
     * \brief Calculates the ttc between a stationary object and the own agent
     * \param baseStationary osi BaseStationary of other object
     * \return ttc
     */
    double CalculateObjectTTC(const osi3::BaseStationary& baseStationary);

    /*!
     * \brief ShouldBeActivated Determines if the Autonomous Emergency Braking system
     *        should be activated
     *
     * \param[in] ttc the time to crash for which activation of the AEB is to be determined
     *
     * \returns true if AEB should be activated, false otherwise
     */
    bool ShouldBeActivated(double ttc) const;
    /*!
     * \brief ShouldBeDeactivated Determines if the Autonomous Emergency Braking system
     *        should be deactivated
     *
     * \param[in] ttc the time to crash for which deactivation of the AEB is to be determined
     *
     * \returns true if AEB should be deactivated, false otherwise
     */
    bool ShouldBeDeactivated(double ttc) const;

    /*!
     * \brief Sets the current acceleration of the system
     *
     * The acceleration is determined by the currently active stage. After a
     * stage gets activated the acceralation moves to the defined acceleration of
     * this stage by a parametrized gradient.
     *
     * \param time  current timeStep
     */
    void UpdateAcceleration(const int time);

    std::vector<int> sensors; ///!< Collection of sensor input ids

    double collisionDetectionLongitudinalBoundary {0.0}; ///!< Additional length added to the vehicle boundary when checking for collision detection
    double collisionDetectionLateralBoundary {0.0}; ///!< Additional width added to the vehicle boundary when checking for collision detection

    ObservationInterface* observer = nullptr; ///!< Observer containing the eventnetwork into which (de-)activation events are inserted
    std::vector<osi3::DetectedMovingObject> detectedMovingObjects; ///!< Collection of moving objects detected by connected sensors
    std::vector<osi3::DetectedStationaryObject> detectedStationaryObjects; ///!< Collection of stationary objects detected by connected sensors

    ComponentState componentState = ComponentState::Disabled; ///!< Current state of the AEB component

    double ttcBrake{0.0}; ///!< The minimum Time-To-Collision before the AEB component activates
    double brakingAcceleration{0.0}; ///!< The acceleration provided by the AEB component when activated (should be negative)
    double activeAcceleration {0.0}; ///!< The current acceleration actively provided by the AEB component (should be 0.0 when off)
};
