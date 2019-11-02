/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#ifndef INIT_AGENT_IMPLEMENTATION_H
#define INIT_AGENT_IMPLEMENTATION_H

#include "modelInterface.h"
#include "observationInterface.h"
#include "pcm_trajectory.h"
#include "trajectorySignal.h"
#include "primitiveSignals.h"
#include "componentPorts.h"

/**
 * \addtogroup Components_PCM openPASS components pcm
 * @{
 * \addtogroup Init_Agent
 *
 * \brief Init Component to provide parameter of agent which are not accessible by an other component.
 *
 * \details This component provides parameter of the agent which are not accessible by an other
* component like an algorithm.
 *
 * @}
 */

/*!
 * \copydoc Init_Agent
 * \ingroup Init_Agent
 */
class Init_Agent_Implementation : public InitInterface
{
public:
    const std::string COMPONENTNAME = "Init_Agent";

    Init_Agent_Implementation(
        int componentId,
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
    Init_Agent_Implementation(const Init_Agent_Implementation &) = delete;
    Init_Agent_Implementation(Init_Agent_Implementation &&) = delete;
    Init_Agent_Implementation &operator=(const Init_Agent_Implementation &) = delete;
    Init_Agent_Implementation &operator=(Init_Agent_Implementation &&) = delete;
    virtual ~Init_Agent_Implementation() = default;

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

    std::map<int, ComponentPort *> outputPorts; //!< map for all OutputPort
    /** \addtogroup Init_Agent
     *  @{
     *      \name OutputPorts
     *      All output ports with PortId
     *      @{
     */
    OutputPort<TrajectorySignal, PCM_Trajectory> trajectory {100, &outputPorts}; //!< Trajectory
    OutputPort<DoubleSignal, double> weight {101, &outputPorts}; //!< weight of the agent in kg
    OutputPort<DoubleSignal, double> wheelbase {102, &outputPorts}; //!< wheelbase of the agent in m
    OutputPort<DoubleSignal, double> distanceToCOG {103, &outputPorts}; //!< distance to COG of the agent in m
    /**
     *      @}
     *  @}
     */
};

#endif // INIT_AGENT_IMPLEMENTATION_H
