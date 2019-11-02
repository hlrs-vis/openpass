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

#ifndef SENSOR_COLLISION_IMPLEMENTATION_H
#define SENSOR_COLLISION_IMPLEMENTATION_H

#include <QFile>
#include <QDir>
#include <math.h>
#include <cassert>
#include <fstream>
#include <QTextStream>
#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "postCrashDynamic.h"
#include "componentPorts.h"

/**
 * \addtogroup Components_PCM openPASS components pcm
 * @{
 * \addtogroup Sensor_Collision
 *
 * \brief Sensor component to sense a collision
 *
 * \details This module provides information about whether a collision occured.\n
 * At every timestep it checks the AgentInterface function #AgentInterface::GetCollisionPartners()
 * whether the current count of collisions has changed. In this case a collision occured. \n
 * To maintain a specific time the agents can penetrate eachother, the output signal collisionOccured
 * stays false till the #penetrationTime is over or if the collision is already over.
 * After that condition the component signals that a collision occured to other components
 * as long as the collision is still detected.
 *
 * \image html Collision_Integrated.png "State machine of collision handling"
 *
 * @}
 */

/*!
 * \copydoc Sensor_Collision
 * \ingroup Sensor_Collision
 */
class Sensor_Collision_Implementation : public SensorInterface
{
public:
    const std::string COMPONENTNAME = "Sensor_Collision";

    //! Constructor
    //!
    //! @param[in]     componentId    Corresponds to "id" of "Component"
    //! @param[in]     isInit         Corresponds to "init" of "Component"
    //! @param[in]     priority       Corresponds to "priority" of "Component"
    //! @param[in]     offsetTime     Corresponds to "offsetTime" of "Component"
    //! @param[in]     responseTime   Corresponds to "responseTime" of "Component"
    //! @param[in]     cycleTime      Corresponds to "cycleTime" of "Component"
    //! @param[in]     stochastics    Pointer to the stochastics class loaded by the framework
    //! @param[in]     world          Pointer to the world
    //! @param[in]     parameters     Pointer to the parameters of the module
    //! @param[in]     evaluations    Pointer to the evaluations of the module
    //! @param[in]     callbacks      Pointer to the callbacks
    //! @param[in]     agent          Pointer to the agent in which the module is situated
    Sensor_Collision_Implementation(int componentId,
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

    //! Destructor
    virtual ~Sensor_Collision_Implementation();

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

    std::map<int, ComponentPort *> outputPorts; //!< map for all OutputPort
    /** \addtogroup Sensor_Collision
     *  @{
     *      \name OutputPorts
     *      All output ports with PortId
     *      @{
     */
    OutputPort<BoolSignal, bool> collisionOccured {0, &outputPorts}; //!< true if collision occured
    /**
     *      @}
     *  @}
     */

    std::map<int, externalParameter<int>*> parameterMapInt;
    /** \addtogroup Sensor_Collision
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in agentConfiguration file.
     *      @{
     */
    externalParameter<int> penetrationTime {0, &parameterMapInt }; //!< offset time after a collision occured and is set as output
    /**
     *      @}
     *  @}
     */

    //! time of first collision
    int timeOfFirstCollision;

    //! Stores th number of collisionData of the previous time step
    int previousNumberOfCollisionData = 0;

    //! Specifies the current state of collision the agent is in
    enum class CollisionState
    {
        NOCOLLISION,
        COLLISIONPENETRATING,
        COLLISION
    } collisionState = CollisionState::NOCOLLISION;
};

#endif // SENSOR_COLLISION_IMPLEMENTATION_H
