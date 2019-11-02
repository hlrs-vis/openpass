/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/*!
 * \addtogroup Dynamics_TrajectoryFollower
 * @{
 * \brief makes an agent strictly follow a predefined path
 *
 * The path is defined in a file that is loaded in the importer into a map.
 * The module simply reads the coordinates defined in the map every step and forces the agent to move to that position.
 * The vehicle is in this mode very simplified (there is no driver, no logic or any intelligence). It's basically a "playback" of the trajectory.
 *
* \section Dynamics_TrajectoryFollower_Inputs Inputs
* Input variables:
* name | meaning
* -----|------
* inputAcceleration | acceleration from some vehicle component, if any
* velocity | current velocity
* maxReachableState | max ComponentState for this module is given by ComponentController
*
* Input channel IDs:
* | Input ID   | signal class              | contained variables               |
* |------------|---------------------------|-----------------------------------|
* | 0          | LateralSignal             | componentState                    |
* | 1          | AccelerationSignal        | inputAcceleration, componentState |
* | 83         | CompCtrlToAgentCompSignal | maxReachableState                 |
*
* * \section Dynamics_TrajectoryFollower_Outputs Outputs
* Output variables:
* name                       | meaning
* ---------------------------|------
* acceleration               | Acceleration of the current agent coresponding to the pedalpositions [m/s²]
* velocity                   | The driven curvature of the car coresponding to the steering wheel angle based on Ackermann [radiant]
* positionX                  | new inertial x-position [m]
* positionY                  | new inertial y-position [m]
* yaw                        | new yaw angle in current time step [radiant]
* yawRate                    | change of yaw angle due to ds and curvature [radiant]
* steeringWheelAngle         | new angle of the steering wheel angle [degree]
* travelDistance             | distance traveled by the agent during this timestep [m]
*
*
* Output channel IDs:
* | Output Id                  | signal class                              | contained variables                                                                            |
* |----------------------------|-------------------------------------------|------------------------------------------------------------------------------------------------|
* | 0                          | DynamicsSignal                            | acceleration, velocity, positionX, positionY, yaw, yawRate, steeringWheelAngle, travelDistance |
* | 83                         | AgentCompToCompCtrlSignal | componentType, componentName, componentState                                                   |
 * @} */

#pragma once

#include "Interfaces/modelInterface.h"
#include "globalDefinitions.h"
#include "CoreFramework/OpenPassSlave/importer/trajectory.h"

#include "Common/lateralSignal.h"
#include "Common/dynamicsSignal.h"
#include "Common/accelerationSignal.h"
#include "Common/compCtrlToAgentCompSignal.h"
#include "Common/agentCompToCompCtrlSignal.h"
#include "Common/agentBasedEvent.h"

/*!
 * \brief Makes an agent strictly follow a predefined path.
 *
 * \ingroup Dynamics_TrajectoryFollower
 */
class TrajectoryFollowerCommonBase : public UnrestrictedModelInterface
{
public:
    const std::string COMPONENTNAME = "Dynamics_TrajectoryFollower";

    TrajectoryFollowerCommonBase(std::string componentName,
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
                                 AgentInterface *agent);

    TrajectoryFollowerCommonBase(const TrajectoryFollowerCommonBase&) = delete;
    TrajectoryFollowerCommonBase(TrajectoryFollowerCommonBase&&) = delete;
    TrajectoryFollowerCommonBase& operator=(const TrajectoryFollowerCommonBase&) = delete;
    TrajectoryFollowerCommonBase& operator=(TrajectoryFollowerCommonBase&&) = delete;
    virtual ~TrajectoryFollowerCommonBase() = default;

    /*!
    * \brief Update Inputs
    *
    * Function is called by framework when another component delivers a signal over
    * a channel to this component (scheduler calls update taks of other component).
    *
    * Refer to module description for input channels and input ids.
    *
    * @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    * @param[in]     data           Referenced signal (copied by sending component)
    * @param[in]     time           Current scheduling time
    */
    void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time);

    /*!
    * \brief Update outputs.
    *
    * Function is called by framework when this component has to deliver a signal over
    * a channel to another component (scheduler calls update task of this component).
    *
    * Refer to module description for output channels and output ids.
    *
    * @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    * @param[out]    data           Referenced signal (copied by this component)
    * @param[in]     time           Current scheduling time
    */
    void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    /*!
    * \brief Process data within component.
    *
    * Function is called by framework when the scheduler calls the trigger task
    * of this component
    *
    * @param[in]     time           Current scheduling time
    */
    virtual void Trigger(int time) = 0;

    //only for unit tests
    Position GetLastWorldPosition();
    double GetLastVelocity();
    double GetLastAcceleration();

protected:
    bool initialization {true};

    Position currentWorldPosition;
    double currentVelocity {0.0};
    double currentAcceleration {0.0};
    double currentYawRate {0.0};
    double distance {0.0};

    Position lastWorldPosition;
    int lastCoordinateTimestamp {0};
    double lastVelocity {0.0};

    bool inputAccelerationActive {false};
    double inputAcceleration {0.0};

    double cycleTimeInSeconds {0.0};

    bool enforceTrajectory{false};
    bool automaticDeactivation{false};

    int currentTime{0};

    void HandleCompCtrlSignalInput(const std::shared_ptr<SignalInterface const> &data);
    void HandleCompCtrlSignalOutput(std::shared_ptr<SignalInterface const> &data);

    void ThrowCouldNotInstantiateSignalError();
    void ThrowInvalidSignalTypeError();

    ComponentState GetState() const;
    void UpdateState(const ComponentState newState);
private:
    ComponentState componentState {ComponentState::Disabled};
    bool canBeActivated{true};

    void ParseParameters(const ParameterInterface *parameters);
};
