/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef ACTION_SCOPELOGGER_IMPLEMENTATION_H
#define ACTION_SCOPELOGGER_IMPLEMENTATION_H

#include <QFile>
#include <QDir>
#include <math.h>
#include <cassert>
#include <fstream>
#include <QTextStream>
#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "componentPorts.h"

/**
 * \addtogroup Components openPASS components
 * @{
 * \addtogroup Action_ScopeLogger
 *
 * \brief Action component for logging agent's scope
 *
 * \details This module stores several information provided by input signals in a .csv file
 * (position, velocity, driver wishes etc.).
 *
 * @}
 */

/*!
 * \copydoc Action_ScopeLogger
 * \ingroup Action_ScopeLogger
 */
class Action_ScopeLogger_Implementation : public ActionInterface
{
public:
    const std::string COMPONENTNAME = "Action_ScopeLogger";

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
    Action_ScopeLogger_Implementation(int componentId,
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
    virtual ~Action_ScopeLogger_Implementation();

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
    //-----------------------------------------------------------------------------
    //! Save all Data of one time Step
    //! @param[in]  time             current time step
    //!
    //! @return
    void SaveTimeStepData(int time);

    std::map<int, ComponentPort *> inputPorts; //!< map for all InputPort
    /** \addtogroup Action_ScopeLogger
     *  @{
     *      \name InputPort
     *      All input ports with PortId
     *      @{
     */
    InputPort<DoubleSignal, double> positionX {0, &inputPorts}; //!< current x-coordinate of agent
    InputPort<DoubleSignal, double> positionY {1, &inputPorts}; //!< current y-coordinate of agent
    InputPort<DoubleSignal, double> yawAngle {2, &inputPorts}; //!< current yaw angle of agent
    InputPort<DoubleSignal, double> velocityX {3, &inputPorts}; //!< current velocity of agent in straight direction of his local coordinate system
    InputPort<DoubleSignal, double> gasPedal {4, &inputPorts}; //!< throttle/gas pedal in [0,1] required to follow the trajectory
    InputPort<DoubleSignal, double> brakePedal {5, &inputPorts}; //!< brake pedal in [0,1] required to follow the trajectory
    InputPort<DoubleSignal, double> frontWheelAngle {6, &inputPorts}; //!< angle of the front wheels required to follow the trajectory
    InputPort<BoolSignal, bool> collisionOccured {7, &inputPorts}; //!< true if collision occured
    /**
     *      @}
     *  @}
     */

    std::map<int, externalParameter<std::string>*> parameterMapString;
    /** \addtogroup Action_ScopeLogger
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in agentConfiguration file.
     *      @{
     */
    externalParameter<std::string> resultFolderName {0, &parameterMapString }; //!< foldername for the result file
    externalParameter<std::string> tempFileName {1, &parameterMapString };       //!< file name for the temporary file
    externalParameter<std::string> fileNamePrefix {2, &parameterMapString };   //!< file name prefix
    externalParameter<std::string> fileNameSuffix {3, &parameterMapString }; //!< file name suffix
    /**
     *      @}
     *  @}
     */

    //! full path name of result file
    std::string fullResultFilePath;
    //! full name of result file
    std::string fullFileName;

    //! time vector
    std::vector<int> timeVector;
    //! map with agent data
    std::map<int, std::vector<std::pair<QString, QVariant>>> agentData;
};

#endif // ACTION_SCOPELOGGER_IMPLEMENTATION_H
