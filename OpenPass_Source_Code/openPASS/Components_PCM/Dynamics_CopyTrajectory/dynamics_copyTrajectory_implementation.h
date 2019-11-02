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

#ifndef DYNAMICS_COPYTRAJECTORY_IMPLEMENTATION_H
#define DYNAMICS_COPYTRAJECTORY_IMPLEMENTATION_H

#include <vector>
#include "modelInterface.h"
#include "observationInterface.h"

/** \addtogroup Components_PCM openPASS components pcm
* @{
* \addtogroup Dynamics_CopyTrajectory
* \brief Dynamic component to follow a exactly a given trajectory.
*
* \details This module uses the given trajectory of the agent to set its position,
* orientation and velocity at every timestep.
*
* @} */

/*!
* \copydoc Dynamics_CopyTrajectory
* \ingroup Dynamics_CopyTrajectory
*/
class Dynamics_CopyTrajectory_Implementation : public DynamicsInterface
{
public:
    const std::string COMPONENTNAME = "Dynamics_CopyTrajectory";

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
    Dynamics_CopyTrajectory_Implementation(int componentId,
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
    virtual ~Dynamics_CopyTrajectory_Implementation() {}

    Dynamics_CopyTrajectory_Implementation(const Dynamics_CopyTrajectory_Implementation &) = delete;
    Dynamics_CopyTrajectory_Implementation(Dynamics_CopyTrajectory_Implementation &&) = delete;
    Dynamics_CopyTrajectory_Implementation &operator=(const Dynamics_CopyTrajectory_Implementation &) =
        delete;
    Dynamics_CopyTrajectory_Implementation &operator=(Dynamics_CopyTrajectory_Implementation &&) =
        delete;

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

    unsigned int counter;       //!< index, which waypoint has been given out already
    double timeStep;            //!< Time step as double in s

    const std::vector<int>    *timeVec = nullptr;   //!< time vector of trajectory
    const std::vector<double> *xPosVec = nullptr;   //!< x coordinate vector of trajectory
    const std::vector<double> *yPosVec = nullptr;   //!< y coordinate vector of trajectory
    const std::vector<double> *velVec = nullptr;    //!< velocity vector of trajectory
    const std::vector<double> *psiVec = nullptr;    //!< yaw angle vector of trajectory
};

#endif // DYNAMICS_COPYTRAJECTORY_IMPLEMENTATION_H
