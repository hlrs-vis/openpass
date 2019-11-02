/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018, 2019 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#ifndef DYNAMICS_TWOTRACK_IMPLEMENTATION_H
#define DYNAMICS_TWOTRACK_IMPLEMENTATION_H

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "vectorSignals.h"
#include "vector2d.h"
#include "componentPorts.h"
#include "dynamics_twotrack_vehicle.h"

#ifdef QT_DEBUG
#include <QFile>
#include <QTextStream>
#endif

using namespace Common;

/**
 * \addtogroup Components_PCM openPASS components pcm
 * @{
 * \addtogroup Dynamics_TwoTrack
 * \brief Dynamic component to model the dynamic of a two track vehicle.
 *
 * \details Simple open-loop two-track model.
 *
 * @}
 */

/*!
 * \copydoc Dynamics_TwoTrack
 * \ingroup Dynamics_TwoTrack
 */
class Dynamics_TwoTrack_Implementation : public DynamicsInterface
{
public:
    const std::string COMPONENTNAME = "Dynamics_TwoTrack";

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
    Dynamics_TwoTrack_Implementation(
        int componentId,
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
    Dynamics_TwoTrack_Implementation(const Dynamics_TwoTrack_Implementation &) = delete;
    Dynamics_TwoTrack_Implementation(Dynamics_TwoTrack_Implementation &&) = delete;
    Dynamics_TwoTrack_Implementation &operator=(const Dynamics_TwoTrack_Implementation &) = delete;
    Dynamics_TwoTrack_Implementation &operator=(Dynamics_TwoTrack_Implementation &&) = delete;

    //! Destructor
    ~Dynamics_TwoTrack_Implementation();

    //! Function is called by framework when another component delivers a signal over
    //! a channel to this component (scheduler calls update taks of other component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    //! @param[in]     data           Referenced signal (copied by sending component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

    //! Function is called by framework when this component has to deliver a signal over
    //! a channel to another component (scheduler calls update task of this component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    //! @param[out]    data           Referenced signal (copied by this component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    //! Function is called by framework when the scheduler calls the trigger task
    //! of this component
    //!
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    void Trigger(int time);

private:

    std::map<int, externalParameter<double>*> parameterMapDouble;
    /** \addtogroup Dynamics_Two_Track
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in agentConfiguration file.
     *      @{
     */
    externalParameter<double> radiusTire {0, &parameterMapDouble }; //!<
    externalParameter<double> forceTireMaxStatic {1, &parameterMapDouble }; //!<
    externalParameter<double> forceTireSlideStatic {2, &parameterMapDouble }; //!<
    externalParameter<double> slipTireMax {3, &parameterMapDouble }; //!<
    externalParameter<double> powerEngineMax {4, &parameterMapDouble }; //!<
    externalParameter<double> torqueBrakeMin {5, &parameterMapDouble }; //!<
    /**
     *      @}
     *  @}
     */


    std::map<int, ComponentPort *> inputPorts; //!< map for all InputPort
    /** \addtogroup Dynamics_TwoTrack
     *  @{
     *      \name InputPorts
     *      All input ports with PortId
     *      @{
     */
    InputPort<DoubleSignal, double> throttlePedal {0, &inputPorts}; //!< State of the gas pedal [0...1]
    InputPort<DoubleSignal, double> brakePedal {1, &inputPorts}; //!< State of the brake pedal [0...1]
    InputPort<DoubleSignal, double> angleTireFront {2, &inputPorts}; //!< Mean pointing angle of the front tires in car coordinate system [rad]
    InputPort<SignalVectorDouble, std::vector<double>> brakeSuperpose {3, &inputPorts}; //!< Brake superposition (e.g. correction of the trajectory by lane assist)
    InputPort<SignalVectorDouble, std::vector<double>> forceWheelVertical {4, &inputPorts}; //!< Vertical force on the wheels
    /**
     *      @}
     *  @}
     */

    std::map<int, ComponentPort *> outputPorts; //!< map for all OutputPort
    /** \addtogroup Dynamics_TwoTrack
     *  @{
     *      \name OutputPorts
     *      All output ports with PortId
     *      @{
     */
    OutputPort<SignalVectorDouble, std::vector<double>> forceGlobalInertia {0, &outputPorts}; //!< inertia force on vehicle's CoG
    /**
     *      @}
     *  @}
     */

    /** \addtogroup Dynamics_TwoTrack
     *  @{
     *    \name Internal states
     *    @{
    */
    //! Time step as double in s
    double timeStep;
    //! Yaw angle
    double yawAngle;
    //! Car's position
    Common::Vector2d positionCar;
    //! Yaw rate
    double yawVelocity;
    //! Car's velocity
    Common::Vector2d velocityCar;
    //! Yaw acceleration
    double yawAcceleration;
    //! Car's acceleration
    Common::Vector2d accelerationCar;
    /**
     *    @}
     *  @}
    */

    /** \addtogroup Dynamics_TwoTrack
     *  @{
     *    \name Internal objects
     *    @{
    */
    //! Vehicle in trajectory CS
    VehicleSimpleTT *vehicle;
    /**
     *    @}
     *  @}
    */

    //! Update data on agent's actual position, velocity and acceleration
    void ReadPreviousState();

    //! Calculate next position, translation velocity and translation acceleration of the agent
    void NextStateTranslation();

    //! Calculate next yaw angle, rotation velocity and rotation acceleration of the agent
    void NextStateRotation();

    //! Write next position, velocity and acceleration of the agent
    void NextStateSet();

#ifdef QT_DEBUG
    const int timeStep_ms = 10; // default time step [ms]
    int clockCount;
    QFile logFile;
    QTextStream* logStream;
#endif

};

#endif // DYNAMICS_TWOTRACK_IMPLEMENTATION_H
