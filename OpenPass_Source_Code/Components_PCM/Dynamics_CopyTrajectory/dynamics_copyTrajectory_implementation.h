/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef DYNAMICS_COPYTRAJECTORY_IMPLEMENTATION_H
#define DYNAMICS_COPYTRAJECTORY_IMPLEMENTATION_H

#include <vector>
#include "modelInterface.h"
#include "observationInterface.h"

/** \addtogroup Dynamics_CopyTrajectory
* @{
* \brief Implementation interface class for the module Dynamics_CopyTrajectory
*
*  This module uses a given trajectory to set the position of the agent.
*
* \section Dynamics_CopyTrajectory_Inputs Inputs
* none
*
* \section Dynamics_CopyTrajectory_InitInputs Init Inputs
* none
*
* \section Dynamics_CopyTrajectory_Outputs Outputs
* none
*
* \section Dynamics_CopyTrajectory_ExternalParameters External parameters
* name | meaning
* -----|---------
* _timeVec | time vector of trajector
* _xPosVec | x coordinate vector of trajectory
* _yPosVec | y coordinate vector of trajectory
* _velVec | velocity vector of trajectory
* _psiVec | y yaw angle vector of trajectory
*
* \section Dynamics_CopyTrajectory_ConfigParameters Parameters to be specified in agentConfiguration.xml
* type | id | meaning | corresponding external paramter
* -----|----|---------|----------------------------------
* IntVector | 0 | time vector of trajector | _timeVec
* DoubleVector | 1 | x coordinate vector of trajectory | _xPosVec
* DoubleVector | 2 | y coordinate vector of trajectory | _yPosVec
* DoubleVector | 3 | velocity vector of trajectory | _velVec
* DoubleVector | 4 | yaw angle vector of trajectory | _psiVec
*
* \section Dynamics_CopyTrajectory_InternalParameters Internal paramters
* none
*
* @} */

/*!
* \brief Implementation interface class for the module Dynamics_CopyTrajectory
*
* This module uses a given trajectory to set the position of the agent.
* \ingroup Dynamics_CopyTrajectory
*/
class Dynamics_CopyTrajectory_Implementation : public SensorInterface {
public:
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
                                           const std::map<int, ObservationInterface*> *evaluations,
                                           const CallbackInterface *callbacks,
                                           AgentInterface *agent);
    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~Dynamics_CopyTrajectory_Implementation() {}

    //-----------------------------------------------------------------------------
    //! Function is called by framework when another component delivers a signal over
    //! a channel to this component (scheduler calls update taks of other component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    //! @param[in]     data           Referenced signal (copied by sending component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    //-----------------------------------------------------------------------------
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

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
    unsigned int _counter;          //!< index, which waypoint has been given out already
    std::vector<int>    _timeVec;   //!< time vector of trajectory
    std::vector<double> _xPosVec;   //!< x coordinate vector of trajectory
    std::vector<double> _yPosVec;   //!< y coordinate vector of trajectory
    std::vector<double> _velVec;    //!< velocity vector of trajectory
    std::vector<double> _psiVec;    //!< yaw angle vector of trajectory
};

#endif // DYNAMICS_COPYTRAJECTORY_IMPLEMENTATION_H
