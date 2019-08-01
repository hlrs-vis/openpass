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

//! @file  trajectoryfollowing.h
//! @brief This file contains helper classes used for trajectory following
//!        as well as the class definition of the trajectory following logic.

#ifndef TRAJECTORYFOLLOWING_CONTROL_H
#define TRAJECTORYFOLLOWING_CONTROL_H

#include <vector>
#include <iostream>
#include <math.h>

#include "vector2d.h"
#include "pid_controller.h"
#include "lowpass.h"
#include "PCM_Data/pcm_trajectory.h"

const double GRAVITY = 9.81;                    //!< gravity constant G in m/s^2
const double PI  = 3.141592653589793238463;     //!< the circle number \pi

//! Class in which all information on the vehicle are stored
class CarStatistics
{
public:
    double weight;                  //!< weight of the vehicle
    double wheelbase;               //!< distance between front and rear axle
    double tireCoefficient1;        //!< factor to calculate the cornering stiffness of the vehicle
    double tireCoefficient2;        //!< factor to calculate the cornering stiffness of the vehicle
    double distanceFrontAxleToCOG;  //!< distance between front axle and Center Of Gravity (COG)
    double distanceRearAxleToCOG;   //!< distance between rear axle and Center Of Gravity (COG)
    double frontWheelAngleLimit;    //!< the maximum possible angle of the front wheels

    // derived Parameters
    double NormalForceFront;        //!< normal force on the front axel due to gravity
    double NormalForceRear;         //!< normal force on the rear axel due to gravity
    double CorneringStiffnessCoefficientFront;  //!<< coefficient for the front cornering stiffness of the vehicle
    double CorneringStiffnessCoefficientRear;   //!<< coefficient for the rear cornering stiffness of the vehicle

    //! Standard constructor
    CarStatistics():
        weight(1), wheelbase(1), tireCoefficient1(1), tireCoefficient2(1),
        distanceFrontAxleToCOG(1), distanceRearAxleToCOG(1), frontWheelAngleLimit(PI / 2.)
    {
        calculateDerivedParameters();
    }

    //! Constructor
    //!
    //! @param[in]     weight                   weight of the vehicle
    //! @param[in]     wheelbase                distance between front and rear axle
    //! @param[in]     tireCoefficient1         factor to calculate the cornering stiffness of the vehicle
    //! @param[in]     tireCoefficient2         factor to calculate the cornering stiffness of the vehicle
    //! @param[in]     distanceFrontAxleToCOG   distance between front axle and Center Of Gravity (COG)
    //! @param[in]     frontWheelAngleLimit     the maximum possible angle of the front wheels
    CarStatistics(double weight, double wheelbase, double tireCoefficient1,
                  double tireCoefficient2, double distanceFrontAxleToCOG,
                  double frontWheelAngleLimit):
        weight(weight), wheelbase(wheelbase), tireCoefficient1(tireCoefficient1),
        tireCoefficient2(tireCoefficient2), distanceFrontAxleToCOG(distanceFrontAxleToCOG),
        distanceRearAxleToCOG(wheelbase - distanceFrontAxleToCOG),
        frontWheelAngleLimit(frontWheelAngleLimit)
    {
        calculateDerivedParameters();
    }

    // setting some value implies a change of the derived parameters

    //! Function setting the weight and recalculating the derived parameters
    //!
    //! @param[in]     value            weight of the vehicle
    void setWeight(double value)
    {
        weight = value;
        calculateDerivedParameters();
    }

    //! Function setting the wheelbase and recalculating the derived parameters
    //!
    //! @param[in]     value            distance between front and rear axle
    void setWheelbase(double value)
    {
        wheelbase = value;
        calculateDerivedParameters();
    }

    //! Function setting the first tire coefficient and recalculating the derived parameters
    //!
    //! @param[in]     value            new first tire coefficient
    void setTireCoefficient1(double value)
    {
        tireCoefficient1 = value;
        calculateDerivedParameters();
    }

    //! Function setting the second tire coefficient and recalculating the derived parameters
    //!
    //! @param[in]     value            new second tire coefficient
    void setTireCoefficient2(double value)
    {
        tireCoefficient2 = value;
        calculateDerivedParameters();
    }

    //! Function setting the distance front axle to COG
    //!
    //! @param[in]     value            new distance between front axle and COG
    void setDistanceFrontAxleToCOG(double value)
    {
        distanceFrontAxleToCOG = value;
        calculateDerivedParameters();
    }

    //! Function setting the distance rear axle to COG
    //!
    //! @param[in]     value            new distance between rear axle and COG
    void setDistanceRearAxleToCOG(double value)
    {
        distanceRearAxleToCOG = value;
        calculateDerivedParameters();
    }

    //! Function setting the limit of the front wheel angle
    //!
    //! @param[in]     value            new limit
    void setFrontWheelAngleLimit(double value)
    {
        frontWheelAngleLimit = value;
        calculateDerivedParameters();
    }

private:

    //! Function calculating the derived parameters

    void calculateDerivedParameters()
    {
        NormalForceFront = distanceRearAxleToCOG * weight * GRAVITY / wheelbase;
        NormalForceRear = distanceFrontAxleToCOG * weight * GRAVITY / wheelbase;

        CorneringStiffnessCoefficientFront =
            tireCoefficient1 * NormalForceFront * NormalForceFront / 2 +
            tireCoefficient2 * NormalForceFront;

        CorneringStiffnessCoefficientRear =
            tireCoefficient1 * NormalForceRear * NormalForceRear / 2 +
            tireCoefficient2 * NormalForceRear;
    }
};

//! Lightweight structure comprising all information on a way point within a trajectory
struct WaypointData
{
    Common::Vector2d position;         //!< position (x- and y-coordinates)
    double velocity;        //!< velocity
    double time;            //!< timestamp

    //! Standard constructor
    WaypointData():
        position(), velocity(0), time(0)
    {}

    //! Constructor
    //!
    //! @param[in]     x            x-coordinate
    //! @param[in]     y            y-coordinate
    //! @param[in]     velocity
    //! @param[in]     time
    WaypointData(double x, double y, double velocity, double time):
        position(x, y), velocity(velocity), time(time)
    {}
};

//! Lightweight structure comprising all information on positioning a vehicle
struct PositionData
{
    Common::Vector2d position;         //!< 2d vector with x- and y-coordinates
    double angle;           //!< angle
    double velocity;        //!< velocity

    //! Standard constructor
    PositionData():
        position(), angle(0), velocity(0)
    {}

    //! Constructor
    //!
    //! @param[in]     position         2d vector with x- and y-coordinates
    //! @param[in]     angle
    //! @param[in]     velocity
    PositionData(Common::Vector2d position, double angle, double velocity):
        position(position), angle(angle), velocity(velocity)
    {}

    //! Constructor
    //!
    //! @param[in]     x                x-coordinate
    //! @param[in]     y                y-coordinate
    //! @param[in]     angle
    //! @param[in]     velocity
    PositionData(double x, double y, double angle, double velocity):
        angle(angle), velocity(velocity)
    {
        position.x = x;
        position.y = y;
    }
};

//! Lightweight structure comprising all information on the control of the vehicle
struct ControlData
{
    double frontWheelAngle;         //!< the front wheel angle calculated by the control
    double brake;                   //!< the brake calculated by the control
    double gas;                     //!< the gas calculated by the control

    //! Standard constructor
    ControlData():
        frontWheelAngle(0), brake(0), gas(0)
    {}

    //! Constructor
    //!
    //! @param[in]     frontWheelAngle      calculated angle for the front wheel
    //! @param[in]     brake                calculated brake value
    //! @param[in]     gas                  calculated gas value
    ControlData(double frontWheelAngle, double brake, double gas):
        frontWheelAngle(frontWheelAngle), brake(brake), gas(gas)
    {}
};

//! Lightweight structure comprising the position and the control data
struct State
{
    PositionData positionData;      //!< structure containing the position information
    ControlData controlData;        //!< structure containing the control information

    //! Standard constructor
    State():
        positionData(), controlData()
    {}

    //! Constructor
    //!
    //! @param[in]     position         structure with the position information
    //! @param[in]     control          structure with the control information
    State(PositionData position, ControlData control):
        positionData(position), controlData(control)
    {}
};

//! class where the actual control is done
class TrajectoryFollowingControl
{
public:

    //! Constructor
    //!
    //! @param[in]     waypoints        vector with the waypoints that the vehicle should approximate
    //! @param[in]     CarStats         class with information on the vehicle
    //! @param[in]     startState       structure with the position and control data
    //! @param[in]     GasPID           PID-control algorithm for the gas pedal
    //! @param[in]     BrakePID         PID-control algorithm for the brake pedal
    //! @param[in]     lookAheadTime    time in s how long the algorithm looks ahead
    TrajectoryFollowingControl(std::vector<WaypointData> &waypoints,
                               CarStatistics &CarStats, State &startState,
                               PIDController &GasPID, PIDController &BrakePID,
                               double lookAheadTime);

    //! Constructor that just sets a dummy waypoint at ((0,0),0,0), waypoints are to be set later
    //!
    //! @param[in]     CarStats         class with information on the vehicle
    //! @param[in]     startState       structure with the position and control data
    //! @param[in]     GasPID           PID-control algorithm for the gas pedal
    //! @param[in]     BrakePID         PID-control algorithm for the brake pedal
    //! @param[in]     lookAheadTime    time in s how long the algorithm looks ahead
    TrajectoryFollowingControl(CarStatistics &CarStats, State &startState,
                               PIDController &GasPID, PIDController &BrakePID,
                               double lookAheadTime);

    //! Function setting the current position
    //!
    //! @param[in]     Position     2d vector with x- and y-coordinates
    void setCurrentPosition(Common::Vector2d Position)
    {
        CurrentState_.positionData.position = Position;
    }

    //! Function setting the current yaw angle of the vehicle
    //!
    //! @param[in]     angle
    void setCurrentAngle(double angle)
    {
        CurrentState_.positionData.angle = angle;
    }

    //! Function setting the whole position data
    //!
    //! @param[in]     position        according struct
    void setCurrentPositionData(PositionData position)
    {
        CurrentState_.positionData = position;
    }

    //! Function setting the whole control data
    //!
    //! @param[in]     control          according struct
    void setCurrentControl(ControlData control)
    {
        CurrentState_.controlData = control;
    }

    //! Function setting the whole state including position and control data
    //!
    //! @param[in]     state            according struct
    void setCurrentState(State state)
    {
        CurrentState_ = state;
    }

    //! Function setting the front wheel angle
    //!
    //! @param[in]     value            new front wheel angle
    void setCurrentFrontWheelAngle(double value)
    {
        CurrentState_.controlData.frontWheelAngle = value;
    }

    //! Function setting the gas pedal
    //!
    //! @param[in]     value            new gas pedal
    void setCurrentGasPedal(double value)
    {
        CurrentState_.controlData.gas = value;
    }

    //! Function setting the brake pedal
    //!
    //! @param[in]     value            new brake pedal
    void setCurrentBrakePedal(double value)
    {
        CurrentState_.controlData.brake = value;
    }

    //! Function setting the look ahead time
    //!
    //! @param[in]     value            new look ahead time
    void setLookAheadTime(double value)
    {
        lookAheadTime_ = value;
    }

    //! Function returning the position of the waypoint that is currently aimed at
    //!
    //! @return                         position of the waypoint that is currently aimed at
    Common::Vector2d getCurrentWayPoint()
    {
        return waypoints_[currentWayPointIndex_].position;
    }

    //! Function returning the whole waypoint that is currently aimed at
    //!
    //! @return                         whole informaton on waypoint that is currently aimed at
    WaypointData getCurrentWayPointData()
    {
        return waypoints_[currentWayPointIndex_];
    }

    //! Function returning the position of the just passed waypoint
    //!
    //! @return                         position of the just passed waypoint
    Common::Vector2d getPreviousWayPoint()
    {
        return waypoints_[previousWayPointIndex_].position;
    }

    //! Function returning the whole previous waypoint
    //!
    //! @return                         whole informaton on the just passed waypoint
    WaypointData getPreviousWayPointData()
    {
        return waypoints_[previousWayPointIndex_];
    }

    //! Function returning the index of the waypoint that is currently aimed at
    //!
    //! @return                         index of the waypoint that is currently aimed at
    int getCurrentWayPointIndex()
    {
        return currentWayPointIndex_;
    }

    //! Function returning the index of the just passed waypoint
    //!
    //! @return                         index of the just passed waypoint
    int getPreviousWayPointIndex()
    {
        return previousWayPointIndex_;
    }

    //! Function calculating the lateral control
    //!
    //! @return                         the required wheel angle
    double lateralControl();

    //! Function calculating the gas control
    //!
    //! @return                         the required gas value
    double gasControl();

    //! Function calculating the brake control
    //!
    //! @return                         the required brake value
    double brakeControl();

    //! Function calling all controls and setting the controlData
    ControlData computeRequiredControl();

    //! Function setting the way points
    //!
    //! @param[in]     Waypoints        vector of the waypoints
    bool setWaypoints(std::vector<WaypointData> &Waypoints);

    //! Function setting the way points
    //!
    //! @param[in]     X            vector of the x-coordinates
    //! @param[in]     Y            vector of the y-coordinates
    //! @param[in]     Velocity     vector of the velocities
    //! @param[in]     Time         vector of the time
    bool setWaypoints(std::vector<double> &X, std::vector<double> &Y,
                      std::vector<double> &Velocity, std::vector<double> &Time);

private:
    int numWayPoints_;                              //!< size of the waypoint-vector
    std::vector<WaypointData> waypoints_;           //!< waypoints that should be targeted at

    CarStatistics &CarStats_;                       //!< struct with all information on the vehicle
    State CurrentState_;                            //!< struct comprising the position and the control data
    PIDController &GasPID_, &BrakePID_;             //!< control algorithms for the brake and gas pedal
    LowPassFilter
    generalLowPassFilter;             //!< filter for filtering the results of the brake and gas

    double lookAheadTime_;                          //!< how far does the algorithm plan in advance
    int currentWayPointIndex_;                      //!< current target waypoint
    int previousWayPointIndex_;                     //!< waypoint previously passed
    std::vector<double>
    distancesBetweenConsecutiveWayPoints;     //!< distances of consecutive way points

    //! Function calculating the all distances between consecutive way points
    //!
    //! @return                         vector of distances
    std::vector<double> computeDistancesBetweenConsecutiveWayPoints();

    //! Function calculating how many indices the look ahead time corresponds to
    //!
    //! @param[in]     totalDistance        distance that is looked ahead (v*t)
    //! @param[in]     distances            vector of distances between consecutive way points
    //! @param[in]     fromWayPointIndex    index of the 'starting' waypoint
    //! @return                             index surplus
    int computeNumberOfLookaheadPoints(double totalDistance,
                                       const std::vector<double> &distances,
                                       int fromWayPointIndex);

    //! Function calculating the index of the waypoint ahead that should be targeted at
    //!
    //! @param[in]     position             position of the vehicle
    //! @param[in]     direction            direction of the vehicle
    //! @param[in]     fromIndex            index of the 'starting' waypoint
    //! @param[in]     numPointsLookAhead   how many indices are looked ahead
    //! @return                             corresponding index
    int findClosestWayPointAheadIndex(Common::Vector2d &position, Common::Vector2d &direction,
                                      int fromIndex, int numPointsLookAhead);

};

#endif // TRAJECTORYFOLLOWING_CONTROL_H
