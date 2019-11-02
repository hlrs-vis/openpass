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

#ifndef ALGORITHM_SELECTOR_IMPLEMENTATION_H
#define ALGORITHM_SELECTOR_IMPLEMENTATION_H

#include <QApplication>
#include <QFile>
#include <QDir>
#include <math.h>
#include <cassert>
#include <fstream>
#include <QTextStream>
#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "vectorSignals.h"
#include "componentPorts.h"
#include "selector.h"

/**
 * \addtogroup Components_PCM openPASS components pcm
 * @{
 * \addtogroup Algorithm_Selector
 *
 * \brief Algorithm component to select a resulting Output depending on a given Input.
 *
 * \details This components selects a resulting throttle, brake, steering and brake superpose
 * depending on the input values of other components.
 *
 * The resulting Output is based on priorities:
 * 1. collision occured - set throttle = 0, brake = 1, steering = current steering in collision moment
 * 2. evasive steering active - set throttle = 0, brake = 0, steering = evasiveSteering angle
 * 3. brake assist active - set throttle = 0, brake = brakeAssistBrake, steering = driverSteering
 * 4. lane assist active - set throttle = 0, brake = 0, steering = 0, brakeSuperpose = laneAssistBrakeSuperpose
 * 5. any system was active - set throttle = 0, brake = 0, steering = 0;
 * 6. no system was / is active - set throttle = driverThrottle, brake = driverBrake, steering = driverSteering
 *
 * After a collision occured the selector selects the output of the initial collision
 * state for the rest of the simulation.
 *
 * @}
 */

/*!
 * \copydoc Algorithm_Selector
 * \ingroup Algorithm_Selector
 */
class Algorithm_Selector_Implementation : public AlgorithmInterface
{
public:
    const std::string COMPONENTNAME = "Algorithm_Selector";

    //! Constructor
    //!
    //! @param[in]     componentId    Corresponds to "id" of "Component"
    //! @param[in]     isInit         Corresponds to "init" of "Component"
    //! @param[in]     priority       Corresponds to "priority" of "Component"
    //! @param[in]     offsetTime     Corresponds to "offsetTime" of "Component"
    //! @param[in]     responseTime   Corresponds to "responseTime" of "Component"
    //! @param[in]     cycleTime      Corresponds to "cycleTime" of "Component"
    //! @param[in]     stochastics    Pointer to the stochastics class loaded by the framework
    //! @param[in]     parameters     Pointer to the parameters of the module
    //! @param[in]     evaluations    Pointer to the evaluations of the module
    //! @param[in]     callbacks      Pointer to the callbacks
    Algorithm_Selector_Implementation(int componentId,
                                      bool isInit,
                                      int priority,
                                      int offsetTime,
                                      int responseTime,
                                      int cycleTime,
                                      StochasticsInterface *stochastics,
                                      const ParameterInterface *parameters,
                                      const std::map<int, ObservationInterface *> *observations,
                                      const CallbackInterface *callbacks, int agentId);
    virtual ~Algorithm_Selector_Implementation();

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

    std::map<int, ComponentPort *> inputPorts; //!< map for all InputPort
    /** \addtogroup Algorithm_Selector
     *  @{
     *      \name InputPort
     *      All input ports with PortId
     *      @{
     */
    InputPort<DoubleSignal, double> driverThrottle {0, &inputPorts}; //!< driver throttle [0..1]
    InputPort<DoubleSignal, double> driverBrake {1, &inputPorts}; //!< driver brake [0..1]
    InputPort<DoubleSignal, double> driverSteering {2, &inputPorts}; //!< driver steering

    InputPort<BoolSignal, bool> brakeAssistActive {3, &inputPorts}; //!< is brake assist system active
    InputPort<DoubleSignal, double> brakeAssistBrake {4, &inputPorts}; //!< brake of brake assist [0..1]

    InputPort<BoolSignal, bool> laneAssistActive {5, &inputPorts}; //!< is lane assist system active
    InputPort<SignalVectorDouble, std::vector<double>> laneAssistBrakeSuperpose {6, &inputPorts}; //!< brake superpose of line assist

    InputPort<BoolSignal, bool> evasiveSteeringtActive {7, &inputPorts}; //!< is evasive steering system active
    InputPort<DoubleSignal, double> evasiveSteeringAngle {8, &inputPorts}; //!< steering angle of evasive steering system

    InputPort<BoolSignal, bool> collisionOccured {9, &inputPorts}; //!< is a collision occured
    /**
      *      @}
      *  @}
      */

    std::map<int, ComponentPort *> outputPorts; //!< map for all OutputPort
    /** \addtogroup Algorithm_Selector
     *  @{
     *      \name OutputPorts
     *      All output ports with PortId
     *      @{
     */
    OutputPort<DoubleSignal, double> resultingThrottle {0, &outputPorts}; //!< resulting throttle
    OutputPort<DoubleSignal, double> resultingBrake {1, &outputPorts}; //!< resulting brake
    OutputPort<DoubleSignal, double> resultingSteering {2, &outputPorts}; //!< resulting steering
    OutputPort<SignalVectorDouble, std::vector<double>> resultingBrakeSuperpose {3, &outputPorts}; //!< resulting brake superpose
    /**
     *      @}
     *  @}
     */

    /**
     *    \name Internal objects
     *    @{
    */
    //! Selector object
    Selector selector;
    /**
     *    @}
    */
};

#endif // ALGORITHM_SELECTOR_IMPLEMENTATION_H
