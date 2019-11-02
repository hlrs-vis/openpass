/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/** \addtogroup Dynamics_Collision
* @{
* \brief This modules calculates all dynaimcs of the agent in case of a collision
*
* This module becomes active after the agent was involved in a collision.
* Once it is active the agent decelerates with a constant deceleration of -10.0 m/s^2 in a linear trajectory until it comes to a stop.
* At the moment of the collision the velocities and the yaw angle of the ego and the collision partner are adjusted based on a inelastic collision algorithm.
* If the agent collides with a static object, it immediately comes to a fullstop.
*
* \section Dynamics_Collision_Outputs Outputs
* Output variables:
* name | meaning
* -----|------
* dynamicsSignal| Signal containing all dynamics information
*
* Output channel IDs:
* Output ID | signal class | contained variables
* ------------|--------------|-------------
* 0 |  DynamicsSignal | dynamicsSignal
*
* @} */

#pragma once

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/dynamicsSignal.h"

/**
* \brief
*
* \ingroup DynamicsCollision
*/
class DynamicsCollisionImplementation : public UnrestrictedModelInterface
{
public:
    const std::string COMPONENTNAME = "DynamicsCollision";

    DynamicsCollisionImplementation(
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
    DynamicsCollisionImplementation(const DynamicsCollisionImplementation&) = delete;
    DynamicsCollisionImplementation(DynamicsCollisionImplementation&&) = delete;
    DynamicsCollisionImplementation& operator=(const DynamicsCollisionImplementation&) = delete;
    DynamicsCollisionImplementation& operator=(DynamicsCollisionImplementation&&) = delete;
    virtual ~DynamicsCollisionImplementation() = default;

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
    * Function is called by framework when this component has to deliver a signal over
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
    * @param[in]     time           Current scheduling time
    */
    virtual void Trigger(int time);

    //for testing
    double GetVelocity ()
    {
        return velocity;
    }

    //for testing
    double GetMovingDirection ()
    {
        return movingDirection;
    }

private:
    DynamicsSignal dynamicsSignal;

    double velocity {};
    double movingDirection {};
    unsigned int numberOfCollisionPartners = 0;
    bool isActive = false;
};
