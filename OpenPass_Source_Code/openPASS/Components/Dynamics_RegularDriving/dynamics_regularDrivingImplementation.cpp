/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
//-----------------------------------------------------------------------------
//! @file  dynamics_regularDrivingImplementation.cpp
//! @brief This file contains the implementation header file
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include <cassert>
#include <cmath>
#include "dynamics_regularDrivingImplementation.h"
#include "Common/longitudinalSignal.h"
#include "commonTools.h"
#include "Common/accelerationSignal.h"
#include "Common/steeringSignal.h"
#include "Common/parametersVehicleSignal.h"
#include "globalDefinitions.h"

void DynamicsRegularDrivingImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        const std::shared_ptr<ComponentStateSignalInterface const> stateSignal = std::dynamic_pointer_cast<ComponentStateSignalInterface const>(data);
        if(stateSignal->componentState == ComponentState::Acting)
        {
            const std::shared_ptr<LongitudinalSignal const> signal = std::dynamic_pointer_cast<LongitudinalSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }
            in_accPedalPos = signal->accPedalPos;
            in_brakePedalPos = signal->brakePedalPos;
            ApplyPedalPositionLimits();
            in_gear = signal->gear;
            ApplyGearLimit();
        }
    }
    else if (localLinkId == 1)
    {
        const std::shared_ptr<ComponentStateSignalInterface const> stateSignal = std::dynamic_pointer_cast<ComponentStateSignalInterface const>(data);
        if(stateSignal->componentState == ComponentState::Acting)
        {
            const std::shared_ptr<SteeringSignal const> signal = std::dynamic_pointer_cast<SteeringSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }
            in_steeringWheelAngle = signal->steeringWheelAngle;
        }
    }
    else if (localLinkId == 100)
    {
        // from ParametersAgent
        const std::shared_ptr<ParametersVehicleSignal const> signal = std::dynamic_pointer_cast<ParametersVehicleSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        vehicleModelParameters = signal->vehicleParameters;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void DynamicsRegularDrivingImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try {
            data = std::make_shared<DynamicsSignal const>(dynamicsSignal);
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

void DynamicsRegularDrivingImplementation::ApplyGearLimit()
{
    in_gear = std::min(std::max(in_gear, 0), static_cast<int>(vehicleModelParameters.numberOfGears));
}

void DynamicsRegularDrivingImplementation::ApplyPedalPositionLimits()
{
    in_accPedalPos = std::min(std::max(in_accPedalPos, 0.0), 1.0);
    in_brakePedalPos = std::min(std::max(in_brakePedalPos, 0.0), 1.0);
}

double DynamicsRegularDrivingImplementation::GetEngineSpeedByVelocity(double xVel, int gear)
{
    return (xVel * vehicleModelParameters.axleRatio * vehicleModelParameters.gearRatios.at(gear) * 60) / (vehicleModelParameters.staticWheelRadius *_twoPi); // an dynamic wheel radius rDyn must actually be used here
}

double DynamicsRegularDrivingImplementation::GetEngineMomentMax(double engineSpeed)
{
    double torqueMax = vehicleModelParameters.maximumEngineTorque; // initial value at max
    double speed = engineSpeed;

    bool isLowerSection = engineSpeed<vehicleModelParameters.minimumEngineSpeed+1000;
    bool isBeyondLowerSectionBorder = engineSpeed<vehicleModelParameters.minimumEngineSpeed;
    bool isUpperSection = engineSpeed>vehicleModelParameters.maximumEngineSpeed-1000;
    bool isBeyondUpperSectionBorder = engineSpeed>vehicleModelParameters.maximumEngineSpeed;


    if (isLowerSection)
    {
        if (isBeyondLowerSectionBorder) // not within limits
        {
            speed = vehicleModelParameters.minimumEngineSpeed;
        }
        torqueMax = (1000 - (speed - vehicleModelParameters.minimumEngineSpeed)) * -0.1 + vehicleModelParameters.maximumEngineTorque;
    }
    else if (isUpperSection)
    {
        if (isBeyondUpperSectionBorder)
        {
            speed = vehicleModelParameters.maximumEngineSpeed;
        }

        torqueMax = (speed - vehicleModelParameters.maximumEngineSpeed + 1000) * -0.04 + vehicleModelParameters.maximumEngineTorque;
    }
    return torqueMax;
}

double DynamicsRegularDrivingImplementation::GetEngineMomentMin(double engineSpeed)
{
    return GetEngineMomentMax(engineSpeed) * -.1;
}

double DynamicsRegularDrivingImplementation::GetFrictionCoefficient()
{
    return 1;
}

double DynamicsRegularDrivingImplementation::GetEngineMoment(double gasPedalPos, int gear)
{
    double xVel = GetAgent()->GetVelocity();

    double engineSpeedAtGear = GetEngineSpeedByVelocity(xVel, gear);

    GetAgent()->SetEngineSpeed(engineSpeedAtGear);

    double max = GetEngineMomentMax(engineSpeedAtGear);
    double maxAccAtGear = GetAccFromEngineMoment(xVel, max, gear, GetCycleTime());

    GetAgent()->SetMaxAcceleration(maxAccAtGear*GetFrictionCoefficient());

    double min = GetEngineMomentMin(engineSpeedAtGear);

    return (std::fabs(min) + max) * gasPedalPos + min;
}

double DynamicsRegularDrivingImplementation::GetAccFromEngineMoment(double xVel, double engineMoment, int chosenGear, int cycleTime)
{
    Q_UNUSED(xVel);
    Q_UNUSED(cycleTime);

    double wheelSetMoment = engineMoment * (vehicleModelParameters.axleRatio * vehicleModelParameters.gearRatios.at(chosenGear));
    double wheelSetForce = wheelSetMoment / vehicleModelParameters.staticWheelRadius;

    double vehicleSetForce = wheelSetForce;
    double acc = vehicleSetForce/(vehicleModelParameters.weight);

    return acc;
}

double DynamicsRegularDrivingImplementation::GetAccVehicle(double accPedalPos, double brakePedalPos, int gear, int time)
{
    double resultAcc = 0;

    double xVel = GetAgent()->GetVelocity();

    if (brakePedalPos > 0.)  // Brake
    {
        double accelerationDueToPedal {brakePedalPos * _oneG * -1.};
        double engineSpeed {GetEngineSpeedByVelocity(xVel, gear)};
        double engineDrag {GetEngineMomentMin(engineSpeed)};
        double accelerationDueToDrag {GetAccFromEngineMoment(xVel, engineDrag, gear, 100)};

        if (accelerationDueToPedal > 0. || accelerationDueToDrag > 0.)
        {
            throw std::runtime_error("DynamicsRegularDrivingImplementation - Wrong sign for acceleration!");
        }

        resultAcc = accelerationDueToPedal + accelerationDueToDrag;

        double maxDecel = GetAgent()->GetMaxDeceleration();
        resultAcc = std::fmax(maxDecel,resultAcc);
    }
    else  // Gas
    {
        double engineMoment = GetEngineMoment(accPedalPos, gear);
        observation->Insert(time, GetAgent()->GetId(), LoggingGroup::Vehicle, "EngineMoment", std::to_string(engineMoment));
        resultAcc = GetAccFromEngineMoment(xVel, engineMoment, gear, GetCycleTime());
    }

    return resultAcc;
}

void DynamicsRegularDrivingImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    AgentInterface *agent = GetAgent();

    //Lateral behavior
    double maxDecel = _oneG * GetFrictionCoefficient() * -1;
    agent->SetMaxDeceleration(maxDecel);

    double v;
    double yawAngle = agent->GetYaw();

    double accVehicle = GetAccVehicle(in_accPedalPos, in_brakePedalPos, in_gear, time);

    v = agent->GetVelocity() + accVehicle*GetCycleTime()/1000.;

    if(v < VLowerLimit)
    {
        accVehicle = 0.0;
        v = VLowerLimit;
    }

    // change of path coordinate since last time step [m]
    double ds = v * GetCycleTime() * 0.001;
    // change of inertial x-position due to ds and yaw [m]
    double dx = ds * std::cos(yawAngle);
    // change of inertial y-position due to ds and yaw [m]
    double dy = ds * std::sin(yawAngle);
    // new inertial x-position [m]
    double x = agent->GetPositionX() + dx;
    // new inertial y-position [m]
    double y = agent->GetPositionY() + dy;

    dynamicsSignal.acceleration = accVehicle;
    dynamicsSignal.velocity = v;
    dynamicsSignal.positionX = x;
    dynamicsSignal.positionY = y;
    dynamicsSignal.travelDistance = ds;

    // convert steering wheel angle to steering angle of front wheels [degree]
    double steering_angle_degrees = TrafficHelperFunctions::ValueInBounds(-vehicleModelParameters.maximumSteeringWheelAngleAmplitude, in_steeringWheelAngle, vehicleModelParameters.maximumSteeringWheelAngleAmplitude) / vehicleModelParameters.steeringRatio;
    dynamicsSignal.steeringWheelAngle = steering_angle_degrees;
    observation->Insert(time, GetAgent()->GetId(), LoggingGroup::Vehicle, "SteeringAngle", std::to_string(steering_angle_degrees));
    // calculate curvature (Ackermann model; reference point of yawing = rear axle!) [radiant]
    double steeringCurvature = std::tan(DegreeToRadiant * steering_angle_degrees) / vehicleModelParameters.wheelbase;
    // change of yaw angle due to ds and curvature [radiant]
    double dpsi = std::atan(steeringCurvature*ds);
    dynamicsSignal.yawRate = dpsi / (GetCycleTime() * 0.001);
    // new yaw angle in current time step [radiant]
    double psi = agent->GetYaw() + dpsi;
    dynamicsSignal.yaw = psi;
}
