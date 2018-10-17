/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  DynamicsLateralImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include "DynamicsLateralImplementation.h"
#include "steeringSignal.h"
#include "DLateralToPRFSSignal.h"
#include "commonTools.h"

void DynamicsLateralImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        const std::shared_ptr<SteeringSignal const> signal = std::dynamic_pointer_cast<SteeringSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_desiredSteeringWheelAngle = signal->steeringWheelAngle;
    }

    else if (localLinkId == 1)
    {
        // from Algorithm_DecideOnLaneChange
        const std::shared_ptr<BoolSignal const> signal = std::dynamic_pointer_cast<BoolSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_alignToQueue = signal->value;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void DynamicsLateralImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try
        {
            data = std::make_shared<DLateralToPRfsSignal const>(out_steering_angle_degrees, out_steeringCurvature);
        }
        catch(const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void DynamicsLateralImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    if (in_alignToQueue)
    {
        _doInit = true; //initialises _steeringWheelangle in next timestep
        return;
    }

    _steeringWheelangle = _doInit? in_desiredSteeringWheelAngle * _steeringRatio:
                                   _steeringWheelangle + GetDeltaSteeringWheelAngle();
    _doInit = false;

    // convert steering wheel angle to steering angle of front wheels [degree]
    out_steering_angle_degrees = TrafficHelperFunctions::ValueInBounds(-_steeringMax, _steeringWheelangle, _steeringMax) / _steeringRatio;
    // calculate curvature (Ackermann model; reference point of yawing = rear axle!) [radiant]
    out_steeringCurvature = std::tan(DegreeToRadiant * out_steering_angle_degrees) / _wheelbase;
    // change of path coordinate since last time step [m]
    double ds = GetAgent()->GetVelocityAbsolute() * static_cast<double>(GetCycleTime()) * 0.001;
    // change of yaw angle due to ds and curvature [radiant]
    double dpsi = std::atan(out_steeringCurvature*ds);
    GetAgent()->SetYawVelocity(dpsi / (GetCycleTime() * 0.001));
    // new yaw angle in current time step [radiant]
    double psi = GetAgent()->GetYawAngle() + dpsi;
    GetAgent()->SetYawAngle(psi);
}

double DynamicsLateralImplementation::GetDeltaSteeringWheelAngle()
{
    double deficit = in_desiredSteeringWheelAngle - _steeringWheelangle;
    double maxDelta = (std::abs(deficit) >= AcuteDeficit ? SteeringSpeedMax : SteeringSpeedComfort) * GetCycleTime()/1000.0;
    return TrafficHelperFunctions::ValueInBounds(-maxDelta, deficit, maxDelta);
}
