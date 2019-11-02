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

#ifndef DYNAMICS_COLLISION_IMPLEMENTATION_H
#define DYNAMICS_COLLISION_IMPLEMENTATION_H

#include <vector>
#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "postCrashDynamic.h"
#include "componentPorts.h"

/**
* \addtogroup Components_PCM openPASS components pcm
* @{
* \addtogroup Dynamics_Collision
* \brief Dynamic component to model the dynamic of collision.
*
* \details This module uses the #PostCrashDynamic data of the \ref CollisionDetection_Impact module
* and overwrites the agents data with the dynamic data of the collision.
* Therefore this component has to have the lowest priority of all dynamic components in the agent equipment.
* In this case this component is always scheduled last and can overwrite any other changes of the agent an other
* dynamics component has made before.\n
*
* As soon as it receives a signal that a collision occured, it retrieves the latest #PostCrashDynamic data
* by calling the agentInterface function #AgentInterface::GetCollisionData. Based on this data the new dynamic
* is realized for this simulation step. The Realization of the #PostCrashDynamic is only made up once
* as long as the collision lasts.
*
* After the collision is over this component is ready for another collision and #PostCrashDynamic to realize.
*
*
* @} */

/*!
* \copydoc Dynamics_Collision
* \ingroup Dynamics_Collision
*/
class Dynamics_Collision_Implementation : public DynamicsInterface
{
public:
    const std::string COMPONENTNAME = "Dynamics_Collision";

    //-----------------------------------------------------------------------------
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
    //-----------------------------------------------------------------------------
    Dynamics_Collision_Implementation(int componentId,
                                      bool isInit,
                                      int priority,
                                      int offsetTime,
                                      int responseTime,
                                      int cycleTime,
                                      StochasticsInterface *stochastics,
                                      WorldInterface *world,
                                      const ParameterInterface *parameters,
                                      const std::map<int, ObservationInterface *> *evaluations,
                                      const CallbackInterface *callbacks,
                                      AgentInterface *agent);
    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~Dynamics_Collision_Implementation() {}

    Dynamics_Collision_Implementation(const Dynamics_Collision_Implementation &) = delete;
    Dynamics_Collision_Implementation(Dynamics_Collision_Implementation &&) = delete;
    Dynamics_Collision_Implementation &operator=(const Dynamics_Collision_Implementation &) = delete;
    Dynamics_Collision_Implementation &operator=(Dynamics_Collision_Implementation &&) = delete;

    //-----------------------------------------------------------------------------
    //! Function is called by framework when another component delivers a signal over
    //! a channel to this component (scheduler calls update taks of other component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    //! @param[in]     data           Referenced signal (copied by sending component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    //-----------------------------------------------------------------------------
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data,
                             int time);

    //-----------------------------------------------------------------------------
    //! Function is called by framework when this component has to deliver a signal over
    //! a channel to another component (scheduler calls update task of this component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    //! @param[out]    data           Referenced signal (copied by this component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    //-----------------------------------------------------------------------------
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    //-----------------------------------------------------------------------------
    //! Function is called by framework when the scheduler calls the trigger task
    //! of this component
    //!
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    //-----------------------------------------------------------------------------
    virtual void Trigger(int time);

private:

    std::map<int, ComponentPort *> inputPorts; //!< map for all InputPort
    /** \addtogroup Dynamics_Collision
     *  @{
     *      \name InputPorts
     *      All input ports with PortId
     *      @{
     */
    InputPort<BoolSignal, bool> collisionOccured {0, &inputPorts}; //!< current x-coordinate of agent
    /**
     *      @}
     *  @}
     */

    //! Specifies the current state of collision the agent is in
    enum class CollisionState
    {
        NOCOLLISION,
        COLLISIONIMPULS,
        COLLISION
    } collisionState = CollisionState::NOCOLLISION;

    //! current postCrashDynamic
    PostCrashDynamic *postCrashDynamic = nullptr;
};

#endif // DYNAMICS_COLLISION_IMPLEMENTATION_H
