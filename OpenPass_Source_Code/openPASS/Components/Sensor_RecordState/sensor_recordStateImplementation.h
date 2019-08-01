/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
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

/*!
 * \brief records the state of the agent.
 *
 * This component is used to record the state of agent in every single timestep.
 *
 * \ingroup SensorRecordState
 */

//ToDo This module needs a major refactoring, when the new observation/logging concept is implemented. Until then it uses the first ObservationInterface of the ObservationNetwork.
class SensorRecordStateImplementation : public SensorInterface
{
public:
    const std::string COMPONENTNAME = "SensorRecordState";

    SensorRecordStateImplementation(
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

    SensorRecordStateImplementation(const SensorRecordStateImplementation&) = delete;
    SensorRecordStateImplementation(SensorRecordStateImplementation&&) = delete;
    SensorRecordStateImplementation& operator=(const SensorRecordStateImplementation&) = delete;
    SensorRecordStateImplementation& operator=(SensorRecordStateImplementation&&) = delete;
    virtual ~SensorRecordStateImplementation() = default;

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
    void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

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
    void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

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
    void Trigger(int time);

private:

    int agentId = 0;
    int timeMSec = 0;
    int indexLaneEgo{0};
    ObservationInterface* observerInstance {nullptr};

    /*!
     * \brief Translates secondaray covered lanes into a string.
     *
     * This function uses AgentInterface::GetSecondaryCoveredLanes and translates it into a string.
     *
     * @return      secondary lanes as string.
     */
    std::string SecondaryLanesToString();

    /*!
     * \brief Sends all output relevant agent variables to the observer.
     *
     * Sends all output relevant agent variables to the observer.
     * Which are later on logged.
     */
    void ObserveEgo();
};


