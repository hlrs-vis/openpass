/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef DYNAMICS_LONGITUDINAL_BASIC_IMPLEMENTATION_H
#define DYNAMICS_LONGITUDINAL_BASIC_IMPLEMENTATION_H

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"

//-----------------------------------------------------------------------------

/** \addtogroup Dynamics_Longitudinal_Basic
* @{
* \brief Calculate the next velocity and position of the agent in the next timestep
* based on the acceleration in longitudinal direction.
*
* This component gets a desired acceleration and calculates the position and velocity
* of the next timestep of the agent.
*
* \section Dynamics_Longitudinal_Basic_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* in_aVehicle     | The acceleration of the agent in m/s^2.
*
* Input channel IDs:
* Input ID | signal class | contained variables
* ------------|--------------|-------------
* 0 | DoubleSignal | in_aVehicle
*
* \section Dynamics_Longitudinal_Basic_InitInputs Init Inputs
* none
*
* \section Dynamics_Longitudinal_Basic_Outputs Outputs
* none
*
* \section Dynamics_Longitudinal_Basic_ExternalParameters External parameters
* none
*
* \section Dynamics_Longitudinal_Basic_ConfigParameters Parameters to be specified in agentConfiguration.xml
* none
*
* \section Dynamics_Longitudinal_Basic_InternalParameters Internal paramters
* name | value | meaning
* -----|-------|-------------
* _VLowerLimit | 0 | The minimal velocity of the agent in m/s.
*
* @} */

/*!
* \brief Calculate the next velocity and position of the agent in the next timestep based on the acceleration in longitudinal direction.
*
* This component gets a desired acceleration and calculates the position and velocity
* of the next timestep of the agent.
*
* \ingroup Dynamics_Longitudinal_Basic
*/
class Dynamics_Longitudinal_Basic_Implementation : public DynamicsInterface
{
public:
    const std::string COMPONENTNAME = "Dynamics_Longitudinal_Basic";

    Dynamics_Longitudinal_Basic_Implementation(
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
    Dynamics_Longitudinal_Basic_Implementation(const Dynamics_Longitudinal_Basic_Implementation&) = delete;
    Dynamics_Longitudinal_Basic_Implementation(Dynamics_Longitudinal_Basic_Implementation&&) = delete;
    Dynamics_Longitudinal_Basic_Implementation& operator=(const Dynamics_Longitudinal_Basic_Implementation&) = delete;
    Dynamics_Longitudinal_Basic_Implementation& operator=(Dynamics_Longitudinal_Basic_Implementation&&) = delete;
    virtual ~Dynamics_Longitudinal_Basic_Implementation() = default;

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
    // --- Inputs
    //! The acceleration of the agent in m/s^2.
    double in_aVehicle = 0;

    /**
        * \addtogroup Dynamics_Longitudinal_Basic
        * @{
        *
        *    @name Internal Parameters
        *    @{*/
    // --- Internal Parameter
    //! The minimal velocity of the agent in m/s.
    const double _VLowerLimit = 0;
    /**@}*/
        /**@}*/
};

#endif // DYNAMICS_LONGITUDINAL_BASIC_IMPLEMENTATION_H
