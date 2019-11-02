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

/** \addtogroup Agent_Updater
* @{
* \brief This module gets the final values for the dynamic parameters of the agent and updates the agent accordingly
*
* \section AgentUpdater_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* acceleration | Acceleration of the agent in m/s.
* velocity | Velocity of the agent in m/s.
* positionX | X position of the agent.
* positionY | Y position of the agent.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0 | DynamicsSignal  | acceleration, velocity, positionX, positionY
*
* @} */

#pragma once

#include "Interfaces/modelInterface.h"
#include "Interfaces/observationInterface.h"
#include "Common/dynamicsSignal.h"

/**
* \brief The class which is responsible for updating the agent in each time step.
*
* \ingroup AgentUpdater
*/
class AgentUpdaterImplementation : public UnrestrictedModelInterface
{
public:
    const std::string COMPONENTNAME = "AgentUpdater";

    AgentUpdaterImplementation(
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
        AgentInterface *agent) :
        UnrestrictedModelInterface(
            componentName,
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
    AgentUpdaterImplementation(const AgentUpdaterImplementation&) = delete;
    AgentUpdaterImplementation(AgentUpdaterImplementation&&) = delete;
    AgentUpdaterImplementation& operator=(const AgentUpdaterImplementation&) = delete;
    AgentUpdaterImplementation& operator=(AgentUpdaterImplementation&&) = delete;
    virtual ~AgentUpdaterImplementation() = default;

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

private:
    double acceleration {0.0};
    double velocity {0.0};
    double positionX {0.0};
    double positionY {0.0};
    double yaw {0.0};
    double yawRate {0.0};
    double steeringWheelAngle {0.0};
    double travelDistance {0.0};
};
