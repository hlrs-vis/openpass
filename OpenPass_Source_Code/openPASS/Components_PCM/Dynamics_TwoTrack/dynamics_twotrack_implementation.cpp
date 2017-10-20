/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <memory>
#include <qglobal.h>
#include "dynamics_twotrack_implementation.h"
#include "dynamics_twotrack_local.h"
#define numberTires 4
#define NO_YAW_RATE

Dynamics_TwoTrack_Implementation::Dynamics_TwoTrack_Implementation(
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
        AgentInterface *agent) :
    DynamicsInterface(
        componentId,
        isInit,
        priority,
        offsetTime,
        responseTime,
        cycleTime,
        stochastics,
        world,
        parameters,
        observations,
        callbacks,
        agent)
{
    std::map<int, double> doubleParamMap = GetParameters()->GetParametersDouble();
    timeStep = (double)GetCycleTime() / 1000.0;

    yawVelocity = 0.0;
    yawAcceleration = 0.0;

    radiusTire = doubleParamMap[0]; //0.3
    forceTireMaxStatic = doubleParamMap[1]; //5000;
    forceTireSlideStatic = doubleParamMap[2]; //3000;
    slipTireMax = doubleParamMap[3]; //0.1;
    powerEngineMax = doubleParamMap[4]; //100000; // DIESEL POWER
    torqueBrakeMin = - std::fabs(doubleParamMap[5]); //-10000;

    vehicle = new VehicleSimpleTT();
    vehicle->InitSetEngine(agent->GetWeight(), powerEngineMax, torqueBrakeMin);
    vehicle->InitSetGeometry(agent->GetHeightCOG(), agent->GetDistanceCOGtoFrontAxle(),
                             agent->GetWheelbase(), agent->GetTrackWidth());
    vehicle->InitSetTire(agent->GetVelocityX(),
                         forceTireMaxStatic, forceTireSlideStatic,
                         slipTireMax, radiusTire, agent->GetFrictionCoeff());
}

Dynamics_TwoTrack_Implementation::~Dynamics_TwoTrack_Implementation()
{
    delete (vehicle);
}

void Dynamics_TwoTrack_Implementation::UpdateInput(int localLinkId,
                                                   const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateInput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = inputPorts.at(localLinkId)->SetSignalValue(data);

    if (success) {
        log << COMPONENTNAME << " UpdateInput successful";
        LOG(CbkLogLevel::Debug, log.str());
    } else {
        log << COMPONENTNAME << " UpdateInput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

void Dynamics_TwoTrack_Implementation::UpdateOutput(int localLinkId,
                                                    std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void Dynamics_TwoTrack_Implementation::Trigger(int time)
{

    Q_UNUSED(time);

    ReadPreviousState();

    // transfer to trajectory CS
    angleSlide = - std::atan2(velocityCar.y, velocityCar.x); // non-ISO
    vehicle->SetVelocity(velocityCar.Length(), angleSlide, yawVelocity);

    // drive
    vehicle->DriveTrain(Saturate(throttlePedal.GetValue(), 0.0, 1.0),
                        Saturate(brakePedal.GetValue(), 0.0, 1.0));

    // road
    vehicle->ForceLocal(timeStep, angleTireFront.GetValue());

    // sum up
    vehicle->ForceGlobal();

    // Set next state
    SetNextState();

}

void Dynamics_TwoTrack_Implementation::ReadPreviousState()
{
    // actual state
    positionCar.x = GetAgent()->GetPositionX(); // global CS
    positionCar.y = GetAgent()->GetPositionY(); // global CS
    yawAngle = GetAgent()->GetYawAngle(); // global CS

    velocityCar.x = GetAgent()->GetVelocityX(); // car's CS
    velocityCar.y = GetAgent()->GetVelocityY(); // car's CS
    yawVelocity = GetAgent()->GetYawVelocity();

    accelerationCar.x = GetAgent()->GetAccelerationX(); // car's CS
    accelerationCar.y = GetAgent()->GetAccelerationY(); // car's CS
    yawAcceleration = GetAgent()->GetYawAcceleration();

}

void Dynamics_TwoTrack_Implementation::SetNextState()
{

    // update geometry
    velocityCar.Rotate(angleSlide + yawAngle);
    //velocityCar.Rotate(yawAngle);
    GetAgent()->SetPositionX( positionCar.x + timeStep * velocityCar.x );
    GetAgent()->SetPositionY( positionCar.y + timeStep * velocityCar.y );
    GetAgent()->SetYawAngle( yawAngle + timeStep * yawVelocity);

    // update velocity
    velocityCar.Rotate(- angleSlide - yawAngle);
    //velocityCar.Rotate(- yawAngle);
    GetAgent()->SetVelocityX( velocityCar.x + timeStep * accelerationCar.x);
    GetAgent()->SetVelocityY( velocityCar.y + timeStep * accelerationCar.y);
    GetAgent()->SetYawVelocity(yawVelocity + yawAcceleration * timeStep);

    // update forces
    // transfer to trajectory and/or world CS ??? -> NOPE !!!
    GetAgent()->SetAccelerationX( vehicle->forceTotalXY.x / GetAgent()->GetWeight() );
    GetAgent()->SetAccelerationY( vehicle->forceTotalXY.y / GetAgent()->GetWeight() );
    GetAgent()->SetYawAcceleration(vehicle->momentTotalZ / GetAgent()->GetMomentInertiaYaw());
}
