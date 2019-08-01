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

#ifndef ALGORITHM_TRAJECTORYFOLLOWER_IMPLEMENTATION_H
#define ALGORITHM_TRAJECTORYFOLLOWER_IMPLEMENTATION_H

#include <vector>
#include <map>

#include "modelInterface.h"
#include "primitiveSignals.h"
#include "pcm_trajectory.h"
#include "trajectorySignal.h"
#include "componentPorts.h"
#include "vector2d.h"

#include "trajectoryfollowing.h"

/**
 * \addtogroup Components_PCM openPASS components pcm
 * @{
 * \addtogroup Algorithm_TrajectoryFollower
 *
 * \brief Algorithm component to follow a given trajectory.
 *
 * \details This class implements an AlgorithmInterface.\n
 * It provides an Algorithm to follow a given trajectory. \n
 * It gets the position, velocity and yaw angle of an agent and calculates the required
 * gas pedal, brake pedal and front wheel angle to follow a given trajectory. \n
 * It also retrieves a signal if a collision occured.
 * In this case the #requiredThrottlePedal is set to 0,
 * the #requiredBrakePedal is set to 1
 * and the #requiredFrontWheelAngle stays the same for the rest
 * of the simulation.
 *
 * @}
 */

/*!
 * \copydoc Algorithm_TrajectoryFollower
 * \ingroup Algorithm_TrajectoryFollower
 */
class Algorithm_TrajectoryFollower_Implementation : public AlgorithmInterface
{
public:
    const std::string COMPONENTNAME = "Algorithm_TrajectoryFollower";

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
    Algorithm_TrajectoryFollower_Implementation(int componentId,
                                                bool isInit,
                                                int priority,
                                                int offsetTime,
                                                int responseTime,
                                                int cycleTime,
                                                StochasticsInterface *stochastics,
                                                const ParameterInterface *parameters,
                                                const std::map<int, ObservationInterface *> *observations,
                                                const CallbackInterface *callbacks,
                                                int agentId);
    //! Destructor
    virtual ~Algorithm_TrajectoryFollower_Implementation();

    Algorithm_TrajectoryFollower_Implementation(const Algorithm_TrajectoryFollower_Implementation &) =
        delete;
    Algorithm_TrajectoryFollower_Implementation(Algorithm_TrajectoryFollower_Implementation &&) =
        delete;
    Algorithm_TrajectoryFollower_Implementation &operator=(const
                                                           Algorithm_TrajectoryFollower_Implementation &) = delete;
    Algorithm_TrajectoryFollower_Implementation &operator=(Algorithm_TrajectoryFollower_Implementation
                                                           &&) = delete;

    //! Function is called by framework when another component delivers a signal over
    //! a channel to this component (scheduler calls update taks of other component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    //! @param[in]     data           Referenced signal (copied by sending component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data,
                             int time);

    //! Function is called by framework when this component has to deliver a signal over
    //! a channel to another component (scheduler calls update task of this component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    //! @param[out]    data           Referenced signal (copied by this component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    //! Function is called by framework when the scheduler calls the trigger task
    //! of this component
    //!
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    virtual void Trigger(int time);

private:

    //! Function to initialize all internal objects.
    //!
    //! @return                       True on success
    bool Init();

    //! Function to read in trajectory data given seperately
    //!
    //! @return                       Vector of waypoints comprising all trajectory data
    std::vector<WaypointData> *ReadWayPointData(const std::vector<int> *Time,
                                                const std::vector<double> *X,
                                                const std::vector<double> *Y,
                                                const std::vector<double> *Velocity);

    std::map<int, ComponentPort *> inputPorts; //!< map for all InputPort
    /** \addtogroup Algorithm_TrajectoryFollower
     *  @{
     *      \name InputPorts
     *      All input ports with PortId
     *      @{
     */
    InputPort<DoubleSignal, double> positionX {0, &inputPorts}; //!< current x-coordinate of agent
    InputPort<DoubleSignal, double> positionY {1, &inputPorts}; //!< current y-coordinate of agent
    InputPort<DoubleSignal, double> yawAngle {2, &inputPorts}; //!< current yaw angle of agent
    InputPort<DoubleSignal, double> velocityX {3, &inputPorts}; //!< current velocity of agent in straight direction of his local coordinate system
    InputPort<TrajectorySignal, PCM_Trajectory> trajectory {100, &inputPorts}; //!< given trajectory to follow
    InputPort<DoubleSignal, double> weight {101, &inputPorts}; //!< weight of the agent in kg
    InputPort<DoubleSignal, double> wheelbase {102, &inputPorts}; //!< wheelbase of the agent in m
    InputPort<DoubleSignal, double> distanceToCOG {103, &inputPorts}; //!< distance to COG of the agent in m
    /**
     *      @}
     *  @}
     */

    std::map<int, ComponentPort *> outputPorts; //!< map for all OutputPort
    /** \addtogroup Algorithm_TrajectoryFollower
     *  @{
     *      \name OutputPorts
     *      All output ports with PortId
     *      @{
     */
    OutputPort<DoubleSignal, double> requiredThrottlePedal {0, &outputPorts}; //!< throttle/gas pedal in [0,1] required to follow the trajectory
    OutputPort<DoubleSignal, double> requiredBrakePedal {1, &outputPorts}; //!< brake pedal in [0,1] required to follow the trajectory
    OutputPort<DoubleSignal, double> requiredFrontWheelAngle {2, &outputPorts}; //!< angle of the front wheels required to follow the trajectory
    /**
     *      @}
     *  @}
     */

    std::map<int, externalParameter<double>*> parameterMapDouble;
    /** \addtogroup Algorithm_TrajectoryFollower
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in agentConfiguration file.
     *      @{
     */
    externalParameter<double> tireCoefficient1 {0, &parameterMapDouble };       //!< first tire coefficient
    externalParameter<double> tireCoefficient2 {1, &parameterMapDouble };       //!< second tire coefficient
    externalParameter<double> frontWheelAngleLimit {2, &parameterMapDouble };   //!< maximum front wheel angle
    externalParameter<double> brake_P {3, &parameterMapDouble };                //!< P of brake pid controller
    externalParameter<double> brake_I {4, &parameterMapDouble };                //!< I of brake pid controller
    externalParameter<double> brake_D {5, &parameterMapDouble };                //!< D of brake pid controller
    externalParameter<double> gas_P {6, &parameterMapDouble };                  //!< P of gas pid controller
    externalParameter<double> gas_I {7, &parameterMapDouble };                  //!< I of gas pid controller
    externalParameter<double> gas_D {8, &parameterMapDouble };                  //!< D of gas pid controller
    externalParameter<double> lookAheadTime {9, &parameterMapDouble };          //!< time to look ahead, relevant for calculating the front wheel angle
    /**
     *      @}
     *  @}
     */

    /** \addtogroup Algorithm_TrajectoryFollower
     *  @{
     *      \name Internal Parameter
     *      Parameter which are set internally.
     *      @{
     */
    const double integratorMin = -10;           //!< parameter for the PID control algorithms
    const double integratorMax = 10;            //!< parameter for the PID control algorithms
    const double ignoreIntegratorThreshold = 0; //!< parameter for the PID control algorithms
    /**
     *      @}
     *  @}
     */

    //local computation objects
    std::vector<WaypointData> *waypoints =
        nullptr;      //!< vector of waypoints that should be followed
    CarStatistics *carStats =
        nullptr;      //!< class in which all information on the vehicle are stored
    PIDController *gasController =
        nullptr;      //!< class containing the PID-controller for the gas
    PIDController *brakeController =
        nullptr;      //!< class containing the PID-controller for the brake
    TrajectoryFollowingControl *trajectoryController =
        nullptr;      //!< class containing the actual algorithms for the trajectory following
    bool initialized = false;
    //!< true if all data structures are initialized
};

#endif // ALGORITHM_TRAJECTORYFOLLOWER_IMPLEMENTATION_H
