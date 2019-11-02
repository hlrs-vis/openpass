/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2019 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  sensor_driverDefinitions.h
//! @brief This file contains several classes for sensor driver purposes
//-----------------------------------------------------------------------------

#pragma once
#include "Common/globalDefinitions.h"

//! This struct is used to transport data of an object as seen be the driver
struct ObjectInformation
{
    //! Id of the object (default if not existing)
    int id {-1};
    //! false if there is no object in this position
    bool exist {false};
    //! true if stationary object, false if agent
    bool isStatic {false};
    //! Absolute velocity of the agent (default if object is not an agent)
    double velocity {-999.0};
    //! Acceleration of the agent (default if object is not an agent)
    double acceleration {-999.0};
    //! Heading relative to the street (default if not existing)
    double heading {-999.0};
    //! Length of object (default if not existing)
    double length {-999.0};
    //! Width of object (default if not existing)
    double width {-999.0};
    //! Height of object (default if not existing)
    double height {-999.0};
    //! Relative distance along the road (i.e in direction s) between own agent and object (default if not existing)
    double relativeLongitudinalDistance {-999.0};
    //! Relative distance at right angle to the road (i.e. in direction t) between own agent and object (default if not existing)
    double relativeLateralDistance {-999.0};
};

//! This struct is used to trasport data of a lane concerning traffic rules as seen by the driver
struct LaneInformationTrafficRules
{
    //! Vector of all traffic signs valid for this lane inside the visibility distance
    std::vector<CommonTrafficSign::Entity> trafficSigns;
};

//! This struct is used to transport data of a lane concerning its geometric features as seen by the driver
struct LaneInformationGeometry
{
    //! Wether there is a lane on this position
    bool exists {false};
    //! Curvature at current s position (default if not existing)
    double curvature {-999.0};
    //! Width at current s position (default if not existing)
    double width {-999.0};
    //! Distance from current position to the end of the lane or infinity if the end is farther away than the visibility distance
    double distanceToEndOfLane {-999.0};
};

//! Data about the lane to left (in driving direction) of the mainLane
struct OwnVehicleInformation
{
    //! Velocity of agent
    double velocity {-999.0};
    //! Acceleration of agent
    double acceleration {-999.0};
    //! t-coordinate
    double lateralPosition {-999.0};
    //! Heading relative to lane
    double heading {-999.0};
    //! Angle of the steering wheel
    double steeringWheelAngle {-999.0};
    //! Wether the agent touches multiple neighbouring lanes
    bool isCrossingLanes {false};
    //! Distance between the left front point and the left boundary of the lane it is in.
    double distanceToLaneBoundaryLeft {-999.0};
    //! Distance between the right front point and the right boundary of the lane it is in.
    double distanceToLaneBoundaryRight {-999.0};
    //! Wether this agent has collided with another object
    bool collision {false};
};

//! This struct contains data of the objects surrounding the own agent as seen by the driver.
//! For all cases mainLaneId is used as own lane
struct SurroundingObjects
{
    //! Next object in the same lane
    ObjectInformation objectFront;
    //! Closest object behind the agent in the same lane
    ObjectInformation objectRear;
    //! Next object in the lane to the left
    ObjectInformation objectFrontLeft;
    //! Closest object behind the agent in the lane to the left
    ObjectInformation objectRearLeft;
    //! Next object in the lane to the right
    ObjectInformation objectFrontRight;
    //! Closest object behind the agent in the lane to the right
    ObjectInformation objectRearRight;
};

//! This struct contains infrastructure information of surrounding lanes that are used to establish traffic rules
struct TrafficRuleInformation
{
    //! Data about the lane to left (in driving direction) of the mainLane
    LaneInformationTrafficRules laneLeft;
    //! Data about the lane the where the middle of the front of the agent is (i.e. mainLane)
    LaneInformationTrafficRules laneEgo;
    //! Data about the lane to right (in driving direction) of the mainLane
    LaneInformationTrafficRules laneRight;
};

//! This struct contains infrastructure information of surrounding lanes that describe its geometric features
struct GeometryInformation
{
    //! Current maximum visibility distance as specified by the world
    double visibilityDistance {-999.0};
    //! Data about the lane to left (in driving direction) of the mainLane
    LaneInformationGeometry laneLeft;
    //! Data about the lane the where the middle of the front of the agent is (i.e. mainLane)
    LaneInformationGeometry laneEgo;
    //! Data about the lane to right (in driving direction) of the mainLane
    LaneInformationGeometry laneRight;
};
