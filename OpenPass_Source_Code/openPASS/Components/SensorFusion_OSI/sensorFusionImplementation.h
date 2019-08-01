/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/** \addtogroup SensorFusion
* @{
* \brief This file models the SensorFusion.
*
* \details This file models the SensorFusion which can be part of an agent.
*          This module gets OSI SensorData of all sensors of the vehicle and forwards a combined
*          SensorData to the driver assistance systems.
*
* \section MODULENAME_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* sensorData | SensorData of a single sensor.
*
* Input channel IDs:
* Input Id | signal class | contained variables
* ----------|--------------|-------------
*  0 		| SensorDataSignal  | sensorData
*
* \section MODULENAME_Outputs Outputs
* Output variables:
* name | meaning
* -----|------
* out_sensorData | Combined SensorData from all sensors.
*
* Output channel IDs:
* Output Id | signal class | contained variables
* ----------|--------------|-------------
*  0 		| SensorDataSignal  | out_sensorData
*
* @} */

#pragma once

#include "Interfaces/modelInterface.h"
#include "Common/sensorDataSignal.h"
#include "osi/osi_sensordata.pb.h"

//-----------------------------------------------------------------------------
/** \brief This class is the SensorFusion module.
* 	\details This class contains all logic regarding the sensor fusion.
*
* 	\ingroup SensorFusion
*/
//-----------------------------------------------------------------------------
class SensorFusionImplementation : public UnrestrictedModelInterface
{
public:
    const std::string COMPONENTNAME = "SensorFusion";

    SensorFusionImplementation(
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

    SensorFusionImplementation(const SensorFusionImplementation&) = delete;
    SensorFusionImplementation(SensorFusionImplementation&&) = delete;
    SensorFusionImplementation& operator=(const SensorFusionImplementation&) = delete;
    SensorFusionImplementation& operator=(SensorFusionImplementation&&) = delete;
    virtual ~SensorFusionImplementation() = default;

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
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

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
    virtual void Trigger(int time);

private:
    int previousTimeStamp {0};
    osi3::SensorData out_sensorData;
};


