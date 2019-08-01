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

#include <vector>
#include <memory>
#include <sstream>
#include <qglobal.h>

#include "algorithm_trajectoryFollower_implementation.h"

#include "pid_controller.h"
#include "trajectoryfollowing.h"

using std::endl;

Algorithm_TrajectoryFollower_Implementation::Algorithm_TrajectoryFollower_Implementation(
    int componentId,
    bool isInit,
    int priority,
    int offsetTime,
    int responseTime,
    int cycleTime,
    StochasticsInterface *stochastics,
    const ParameterInterface *parameters,
    const std::map<int, ObservationInterface *> *observations,
    const CallbackInterface *callbacks,
    int agentId) :
    AlgorithmInterface(componentId,
                       isInit,
                       priority,
                       offsetTime,
                       responseTime,
                       cycleTime,
                       stochastics,
                       parameters,
                       observations,
                       callbacks,
                       agentId)
{
    std::stringstream log;
    log << "Constructing " << COMPONENTNAME;
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    std::map<int, double> parameterMapDoubleExternal = GetParameters()->GetParametersDouble();
    foreach (auto &iterator, parameterMapDouble)
    {
        int id = iterator.first;
        parameterMapDouble.at(id)->SetValue(parameterMapDoubleExternal.at(id));
    }

    log << "Construction of " << COMPONENTNAME << " successful";
    LOG(CbkLogLevel::Debug, log.str());
}

bool Algorithm_TrajectoryFollower_Implementation::Init()
{
    carStats = new CarStatistics( weight.GetValue(),
                                  wheelbase.GetValue(),
                                  tireCoefficient1.GetValue(),
                                  tireCoefficient1.GetValue(),
                                  distanceToCOG.GetValue(),
                                  frontWheelAngleLimit.GetValue()
                                );

    brakeController = new PIDController( brake_P.GetValue(),
                                         brake_I.GetValue(),
                                         brake_D.GetValue(),
                                         integratorMin, integratorMax, ignoreIntegratorThreshold);

    gasController = new PIDController( gas_P.GetValue(),
                                       gas_I.GetValue(),
                                       gas_D.GetValue(),
                                       integratorMin, integratorMax, ignoreIntegratorThreshold );

    ControlData startControl( 0, // frontWheelAngle0
                              0, // brakePedal0
                              0  // throttlePedal0
                            );

    PositionData startPosition( Common::Vector2d( positionX.GetValue(),
                                                  positionY.GetValue() ),
                                yawAngle.GetValue(),
                                velocityX.GetValue() );

    State startState( startPosition, startControl );

    if ( carStats != nullptr && brakeController != nullptr && gasController != nullptr )
    {
        trajectoryController = new TrajectoryFollowingControl( *carStats,
                                                               startState,
                                                               *gasController,
                                                               *brakeController,
                                                               lookAheadTime.GetValue()
                                                             );
    }
    else
    {
        // should not be reached
        trajectoryController = nullptr;
        return false;
    }

    waypoints = ReadWayPointData( trajectory.GetValue().GetTimeVec(), // WayPointsTime
                                  trajectory.GetValue().GetXPosVec(), // WayPointsX
                                  trajectory.GetValue().GetYPosVec(), // WayPointsY
                                  trajectory.GetValue().GetUVelVec()  // WayPointsVelocity
                                );

    if ( !waypoints )
    {
        std::stringstream log;
        log << "setting of waypoints failed - array lengths are not equal";
        LOG(CbkLogLevel::Warning, log.str());
        log.str(std::string());

        if ( trajectoryController )
        {
            delete trajectoryController;
            trajectoryController = nullptr; //prevents further execution of trigger
        }
        return false;
    }

    trajectoryController->setWaypoints( *waypoints );

    return true;
}

Algorithm_TrajectoryFollower_Implementation::~Algorithm_TrajectoryFollower_Implementation()
{
    std::stringstream log;
    log << "Destroying " << COMPONENTNAME;
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    delete waypoints;
    waypoints = nullptr;
    delete carStats;
    carStats = nullptr;
    delete gasController;
    gasController = nullptr;
    delete brakeController;
    brakeController = nullptr;
    delete trajectoryController;
    trajectoryController = nullptr;

    log << "Destruction of TrajectoryFollower successful";
    LOG(CbkLogLevel::Debug, log.str());

}

void Algorithm_TrajectoryFollower_Implementation::UpdateInput(int localLinkId,
                                                              const std::shared_ptr<SignalInterface const> &data,
                                                              int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateInput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = inputPorts.at(localLinkId)->SetSignalValue(data);

    if (success)
    {
        log << COMPONENTNAME << " UpdateInput successful";
        LOG(CbkLogLevel::Debug, log.str());
    }
    else
    {
        log << COMPONENTNAME << " UpdateInput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

void Algorithm_TrajectoryFollower_Implementation::UpdateOutput(int localLinkId,
                                                               std::shared_ptr<SignalInterface const> &data,
                                                               int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateOutput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = outputPorts.at(localLinkId)->GetSignalValue(data);

    if (success)
    {
        log << COMPONENTNAME << " UpdateOutput successful";
        LOG(CbkLogLevel::Debug, log.str());
    }
    else
    {
        log << COMPONENTNAME << " UpdateOutput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

void Algorithm_TrajectoryFollower_Implementation::Trigger(int time)
{
    Q_UNUSED(time);
    std::stringstream log;

    if (!initialized)
    {
        //set default signal values and allocate computational data structures on heap
        bool success;

        try
        {
            success = Init();
        }
        catch (...)
        {
            success = false;
        }

        if (!success)
        {
            log << "setting of parameters failed";
            log << "Construction of TrajectoryFollower failed - computation object is null";
            LOG(CbkLogLevel::Warning, log.str());
        }
        else
        {
            initialized = true;
        }
    }

    if (!initialized)
    {
        return;
    }


    if (trajectoryController == nullptr)
    {
        log << COMPONENTNAME << " Trigger not callable";
        LOG(CbkLogLevel::Error, log.str());
        return;
    }

    log << COMPONENTNAME << " Trigger";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    //transfer all relevant input signals into the computation structure
    PositionData position( positionX.GetValue(),
                           positionY.GetValue(),
                           yawAngle.GetValue(),
                           velocityX.GetValue() );

    // setup controller
    trajectoryController->setCurrentPositionData(position);

    log << "TrajectoryFollower (AgentID " << GetAgentId() << ") Input" << std::endl
        << " PositionX= " << position.position.x
        << " PositionY= " << position.position.y
        << " YawAngle= " << position.angle
        << " Velocity= " << position.velocity << std::endl
        << " TargetWaypoint: Index=" << trajectoryController->getCurrentWayPointIndex()
        << " time= " << trajectoryController->getCurrentWayPointData().time
        << " X= " << trajectoryController->getCurrentWayPoint().x
        << " Y= " << trajectoryController->getCurrentWayPoint().y
        << " Velocity= " << trajectoryController->getCurrentWayPointData().velocity << std::endl
        << " LastWaypoint: Index=" << trajectoryController->getPreviousWayPointIndex()
        << " time= " << trajectoryController->getPreviousWayPointData().time
        << " X= " << trajectoryController->getPreviousWayPoint().x
        << " Y= " << trajectoryController->getPreviousWayPoint().y
        << " Velocity= " << trajectoryController->getPreviousWayPointData().velocity;
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    // perform the control
    ControlData control = trajectoryController->computeRequiredControl();

    log << "TrajectoryFollower (AgentID " << GetAgentId() << ") Output" << std::endl
        << " FrontWheelAngle= " << control.frontWheelAngle
        << " ThrottlePedal= " << control.gas
        << " BrakePedal= " << control.brake;
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    // create new output signals
    requiredFrontWheelAngle.SetValue(control.frontWheelAngle);
    requiredThrottlePedal.SetValue(control.gas);
    requiredBrakePedal.SetValue(control.brake);

    log << "TrajectoryFollower::Trigger successful";
    LOG(CbkLogLevel::Debug, log.str());

    return;
}

std::vector<WaypointData> *Algorithm_TrajectoryFollower_Implementation::ReadWayPointData(
    const std::vector<int> *Time,
    const std::vector<double> *X,
    const std::vector<double> *Y,
    const std::vector<double> *Velocity)
{
    if (Time == nullptr || X == nullptr || Y == nullptr || Velocity == nullptr)
    {
        return nullptr;
    }
    unsigned int n = X->size();
    if ( n == 0 || n != Y->size() || n != Velocity->size() || n != Time->size() )
    {
        return nullptr;
    }

    std::vector<WaypointData> *waypoints = new std::vector<WaypointData>(n);
    const std::vector<int> &TimeRef = *Time;
    const std::vector<double> &XRef = *X;
    const std::vector<double> &YRef = *Y;
    const std::vector<double> &VelocityRef = *Velocity;
    std::vector<WaypointData> &waypointsRef = *waypoints;

    for (unsigned int i = 0; i < n; ++i)
    {
        waypointsRef[i].time = TimeRef[i];
        waypointsRef[i].position.x = XRef[i];
        waypointsRef[i].position.y = YRef[i];
        waypointsRef[i].velocity = VelocityRef[i];
    }

    return waypoints;
}
