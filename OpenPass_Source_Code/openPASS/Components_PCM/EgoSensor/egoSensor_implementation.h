/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef EGOSENSOR_IMPLEMENTATION_H
#define EGOSENSOR_IMPLEMENTATION_H

#include <math.h>
#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "componentPorts.h"

/**
 * \addtogroup Components openPASS components
 * @{
 * \addtogroup EgoSensor
 *
 * \brief Sensor component to sense the current state of an agent
 *
 * \details This module provides information about the the position and several physical parameter of the agent.
 *
 * @}
 */

/*!
 * \copydoc EgoSensor
 * \ingroup EgoSensor
 */
class EgoSensor_Implementation : public SensorInterface
{
public:
    const std::string COMPONENTNAME = "EgoSensor";

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
    EgoSensor_Implementation(int componentId,
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
    virtual ~EgoSensor_Implementation() {}

    EgoSensor_Implementation(const EgoSensor_Implementation &) = delete;
    EgoSensor_Implementation(EgoSensor_Implementation &&) = delete;
    EgoSensor_Implementation &operator=(const EgoSensor_Implementation &) = delete;
    EgoSensor_Implementation &operator=(EgoSensor_Implementation &&) = delete;


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

    //-----------------------------------------------------------------------------
    //! This function is called only one time in the trigger task.
    //! It's used to initialize several internal parameter in the first step.
    //!
    //! @return                       True on success
    //-----------------------------------------------------------------------------
    bool initAfterInitsAreSet();

    std::map<int, ComponentPort *> outputPorts; //!< map for all OutputPort
    /** \addtogroup EgoSensor
     *  @{
     *      \name OutputPorts
     *      All output ports with PortId
     *      @{
     */
    OutputPort<DoubleSignal, double> positionX {0, &outputPorts}; //!< position in X
    OutputPort<DoubleSignal, double> positionY {1, &outputPorts}; //!< position in Y
    OutputPort<DoubleSignal, double> velocityX {2, &outputPorts}; //!< velocity in X
    OutputPort<DoubleSignal, double> velocityY {3, &outputPorts}; //!< velocity in Y
    OutputPort<DoubleSignal, double> accelerationX {4, &outputPorts}; //!< acceleration in X
    OutputPort<DoubleSignal, double> accelerationY {5, &outputPorts}; //!< acceleration in Y
    OutputPort<DoubleSignal, double> maximalXCoordinate {6, &outputPorts}; //!< maximal X coordinate
    OutputPort<DoubleSignal, double> minimalXCoordinate {7, &outputPorts}; //!< minimal X coordinate
    OutputPort<DoubleSignal, double> maximalYCoordinate {8, &outputPorts}; //!< maximal Y coordinate
    OutputPort<DoubleSignal, double> minimalYCoordinate {9, &outputPorts}; //!< minimal Y coordinate
    OutputPort<DoubleSignal, double> yawAngle {10, &outputPorts}; //!< yaw angle
    OutputPort<DoubleSignal, double> width {11, &outputPorts}; //!< width of agent
    OutputPort<DoubleSignal, double> length {12, &outputPorts}; //!< length of agent
    OutputPort<DoubleSignal, double> weight {13, &outputPorts}; //!< weight of agent
    OutputPort<DoubleSignal, double> wheelbase {14, &outputPorts}; //!< wheelbase of agent
    OutputPort<DoubleSignal, double> trackWidth {15, &outputPorts}; //!< track width of agent
    OutputPort<DoubleSignal, double> distanceCOGtoLeadingEdge {16, &outputPorts}; //!< distance of COG of agent to the leading edge
    OutputPort<DoubleSignal, double> distanceCOGtoFrontAxle {17, &outputPorts}; //!< distance of COG of agent to the front axle
    OutputPort<DoubleSignal, double> heightCOG {18, &outputPorts}; //!< height of COG of agent
    OutputPort<DoubleSignal, double> momentInertiaYaw {19, &outputPorts}; //!< moment of inertia yaw
    OutputPort<DoubleSignal, double> momentInertiaPitch {20, &outputPorts}; //!< moment of inertia pitch
    OutputPort<DoubleSignal, double> momentInertiaRoll {21, &outputPorts}; //!< moment of inertia roll
    OutputPort<DoubleSignal, double> frictionCoeff {22, &outputPorts}; //!< friction coefficient
    /**
     *      @}
     *  @}
     */

    //! Indicates if initAfterInitsAreSet() has been called.
    bool isInitDone;
};

#endif // EGOSENSOR_IMPLEMENTATION_H
