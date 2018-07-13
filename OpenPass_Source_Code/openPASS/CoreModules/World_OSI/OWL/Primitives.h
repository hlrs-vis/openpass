/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*
* OSI     ^_^
* World  /ò,ó \
* Layer  |:::(|
* ========w=w===
*
******************************************************************************/

#pragma once

namespace OWL {
namespace Primitive {

struct Dimension
{
    double length;
    double width;
    double height;
};

struct AbsCoordinate
{
    double x;
    double y;
    double hdg;
};

struct AbsPosition
{
    double x;
    double y;
    double z;
};

struct AbsVelocity
{
    double vx;
    double vy;
    double vz;
};

struct AbsAcceleration
{
    double ax;
    double ay;
    double az;
};

struct AbsOrientation
{
    double yaw;
    double pitch;
    double roll;
};

struct AbsOrientationRate
{
    double yawRate;
    double pitchRate;
    double rollRate;
};

struct RoadCoordinate
{
    double s;
    double t;
    double yaw;
};

struct RoadVelocity
{
    double vs;
    double vt;
    double vz;
};

struct RoadAcceleration
{
    double as;
    double at;
    double az;
};

struct RoadOrientationRate
{
    double hdgRate;
};

struct LanePosition
{
    double v;
    double w;
};

struct LaneVelocity
{
    double vv;
    double vw;
    double vz;
};

struct LaneAcceleration
{
    double av;
    double aw;
    double az;
};

struct LaneOrientation
{
    double hdg;
};

struct LaneOrientationRate
{
    double hdgRate;
};

} // namespace Primitive
} // namespace OWL
