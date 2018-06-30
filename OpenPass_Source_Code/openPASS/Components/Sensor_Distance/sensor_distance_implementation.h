/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef SENSOR_DISTANCE_IMPLEMENTATION_H
#define SENSOR_DISTANCE_IMPLEMENTATION_H

#include <math.h>
#include "modelInterface.h"
#include "observationInterface.h"

/*!
 * \addtogroup Components_Basic openPASS components basic
 * @{
 * \addtogroup Sensor_Distance
 * @{
 * \brief basic module to sense the current state of an agent
 *
 * This module provides information about the distance of the agent to the agent in front
 * and the current velocity of the agent.
 *
 * \section Sensor_Distance_Inputs Inputs
 * none
 *
 * \section Sensor_Distance_InitInputs Init Inputs
 * none
 *
 * \section Sensor_Distance_Outputs Outputs
 * Output variables:
 * name | meaning
 * -----|------
 * distance2NextAgent             | The distance between this agent and agent in front
 * out_agentVelocity              | The velocity of the agent
 *
 * Output channel IDs:
 * Output Id | signal class | contained variables
 * ------------|--------------|-------------
 * 0 | DoubleSignal | distance2NextAgent
 * 1 | DoubleSignal | out_agentVelocity
 *
 * \section Sensor_Distance_ExternalParameters External parameters
 * none
 *
 * \section Sensor_Distance_ConfigParameters Parameters to be specified in agentConfiguration.xml
 * none
 *
 * \section Sensor_Distance_InternalParameters Internal paramters
 * none
 *
 * @}
 * @} */

/*!
* \brief This module is a basic module to sense the current state of an agent.
*
* This module provides information about the distance of the agent to the agent in front
* and the current velocity of the agent.
*
* \ingroup Sensor_Distance
*/
class Sensor_Distance_Implementation : public SensorInterface
{
public:
    const std::string COMPONENTNAME = "Sensor_Distance";

    Sensor_Distance_Implementation(int componentId,
                                   bool isInit,
                                   int priority,
                                   int offsetTime,
                                   int responseTime,
                                   int cycleTime,
                                   StochasticsInterface *stochastics,
                                   WorldInterface *world,
                                   const ParameterInterface *parameters,
                                   const std::map<int, ObservationInterface *> *observations,
                                   const CallbackInterface *callbacks,
                                   AgentInterface *agent);
    virtual ~Sensor_Distance_Implementation() = default;

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
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data,
                             int time);

    /*!
     * \brief Update outputs.
     *
     * Function is called by framework when this component has to deliver a signal over
     * a channel to another component (scheduler calls update task of this component).
     *
     * Refer to module description for output channels and output ids.
     *
     * \param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
     * \param[out]    data           Referenced signal (copied by this component)
     * \param[in]     time           Current scheduling time
     */
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

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

private:
    //! The distance to the next agent in front.
    double out_distanceToNextAgent = 0;
    //! The velocity of the agent.
    double out_agentVelocity = 0;
};

#endif // SENSOR_DISTANCE_IMPLEMENTATION_H
