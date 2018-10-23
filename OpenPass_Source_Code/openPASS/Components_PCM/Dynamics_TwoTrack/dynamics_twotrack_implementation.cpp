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

/**
 * @defgroup module_tt 2-dimensional two-track vehicle model
 * The dynamics of the vehicle is modelled in 2 dimensions. In the sub items the principle function is described in order of execution.
 * Abbreviations:
 * - COG = center-of-gravity
 * - CS = coordinate system
*/

/**
 * @ingroup module_tt
 * @defgroup init Initialization
*/
/**
 * @ingroup module_tt
 * @defgroup sim_step_00 Simulation step entry
*/
/**
 * @ingroup module_tt
 * @defgroup sim_step_10 Simulation step process
*/
/**
 * @ingroup module_tt
 * @defgroup sim_step_20 Simulation step output
*/


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

    /** @addtogroup init
     * Define vehicle's body and engine characteristics:
     *  - total mass
     *  - power
     *  - maximum brake torque
    */
    vehicle->InitSetEngine(agent->GetWeight(), powerEngineMax, torqueBrakeMin);

    /** @addtogroup init
     * Define vehicle's geometry:
     *  - set the height of the COG
     *  - set the longitudinal position of the COG
     *  - set the wheelbase
     *  - set the track width
    */
    vehicle->InitSetGeometry(agent->GetHeightCOG(), agent->GetDistanceCOGtoFrontAxle(),
                             agent->GetWheelbase(), agent->GetTrackWidth());

    /** @addtogroup init
     * Define vehicle's tire properties:
     *  - set initial velicity
     *  - set peak tire force
     *  - set force at full slide
     *  - set the slip corresponding to peak tire force
     *  - set the radius of the tire
     *  - set the road/tire friction coefficient
    */
    vehicle->InitSetTire(agent->GetVelocityX(),
                         forceTireMaxStatic, forceTireSlideStatic,
                         slipTireMax, radiusTire, agent->GetFrictionCoeff());

    std::vector<double> defaultBrake = {0.0, 0.0, 0.0, 0.0};
    if (!brakeSuperpose.SetDefaultValue(defaultBrake))
    {
        std::stringstream log;
        log << COMPONENTNAME << " Default value set failed";
        LOG(CbkLogLevel::Error, log.str());
    }

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

    if (success) // no need for the brake superpose signal
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

    /** @addtogroup sim_step_00
     * Record intentions of the driver or assistant systems:
     *  - throttle pedal
     *  - brake pedal
     *  - steering wheel
    */
    GetAgent()->SetAccelerationIntention(throttlePedal.GetValue());
    GetAgent()->SetDecelerationIntention(brakePedal.GetValue());
    GetAgent()->SetAngleIntention(angleTireFront.GetValue());

    /** @addtogroup sim_step_00
     * Read and update previous vehicle's state:
     *  - global position (cartesian coordinates)
     *  - direction of vehicle's longitudinal axes (angle in polar coordinates)
     *  - vehicle's longitudinal and lateral velocity in vehicle's CS
     *  - vehicle's rotational velociy
     *  - vehicle's longitudinal and lateral acceleration in vehicle's CS
     *  - vehicle's rotational acceleration
    */
    ReadPreviousState();
    vehicle->SetVelocity(velocityCar, yawVelocity);

    /** @addtogroup sim_step_10
     * Apply acceleration/deceleration intentions:
     *  - calculate tire torques due to engine
     *  - calculate tire torques due to braking by
     *      - driver (pedal state)
     *      - assistant systems (selective single wheel braking)
    */
    vehicle->DriveTrain(Saturate(throttlePedal.GetValue(), 0.0, 1.0),
                        Saturate(brakePedal.GetValue(), 0.0, 1.0),
                        brakeSuperpose.GetValue());

    /** @addtogroup sim_step_10
     * Apply tire forces at the tire/road interface:
     *  - calculate longitudinal tire slips and forces
     *  - calculate lateral tire slips and forces
     *  - calculate friction forces
    */
    vehicle->ForceLocal(timeStep, angleTireFront.GetValue());

    /** @addtogroup sim_step_10
     * Combine local tire forces to a global force at the vehicle's body:
     *  - total longitudinal force
     *  - total lateral force
     *  - air drag
     *  - total rotational momentum
    */
    vehicle->ForceGlobal();

    /** @addtogroup sim_step_20
     * Perform a translational Euler step:
     *  - calculate next vehicle's position from prevoius velocity values
     *  - calculate new velocity from previous acceleration values
     *  - calculate new acceleration from global forces
    */
    NextStateTranslation();

    /** @addtogroup sim_step_20
     * Perform a rotational Euler step:
     *  - calculate vehicle's orientation from previous rotational velocity
     *  - calculate vehicle's rotational velocity from previous rotational acceleration
     *  - calculate vehicle's rotational acceleration from the total rotational momentum
    */
    NextStateRotation();

    /** @addtogroup sim_step_20
     * Write actual vehicle's state:
     *  - global position (cartesian coordinates)
     *  - direction of vehicle's longitudinal axes (angle in polar coordinates)
     *  - vehicle's longitudinal and lateral velocity in vehicle's CS
     *  - vehicle's rotational velociy
     *  - vehicle's longitudinal and lateral acceleration in vehicle's CS
     *  - vehicle's rotational acceleration
    */
    NextStateSet();

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

void Dynamics_TwoTrack_Implementation::NextStateTranslation()
{

    // update position (constant velocity step)
    velocityCar.Rotate(yawAngle); // global CS
    positionCar =  positionCar + velocityCar*timeStep;
    velocityCar.Rotate(- yawAngle); // vehicle CS


    // update velocity
    Vector2d velocityCarNew = velocityCar + accelerationCar*timeStep;

    // update acceleration
    accelerationCar = vehicle->forceTotalXY * (1 / GetAgent()->GetWeight() );

    // correct velocity and acceleration for zero-crossing
    if (velocityCarNew.x*velocityCar.x<0.0)
    {
        velocityCar.x = 0.0;
        accelerationCar.x = 0.0;
    }
    else
    {
        velocityCar.x = velocityCarNew.x;
    }
    if (velocityCarNew.y*velocityCar.y<0.0)
    {
        velocityCar.y = 0.0;
        accelerationCar.y = 0.0;
    }
    else
    {
        velocityCar.y = velocityCarNew.y;
    }


}

void Dynamics_TwoTrack_Implementation::NextStateRotation()
{

    // update yaw angle
    yawAngle = yawAngle + timeStep * yawVelocity;

    // update yaw velocity
    double yawVelocityNew = yawVelocity + yawAcceleration * timeStep;

    // update acceleration
    yawAcceleration = vehicle->momentTotalZ / GetAgent()->GetMomentInertiaYaw();

    // correct velocity and acceleration for zero-crossing
    if (yawVelocityNew*yawVelocity<0.0)
    {
        yawVelocity = 0.0;
        yawAcceleration = 0.0;
    }
    else
    {
        yawVelocity = yawVelocityNew;
    }


}

void Dynamics_TwoTrack_Implementation::NextStateSet()
{

    // update position (constant acceleration step)
    GetAgent()->SetPositionX( positionCar.x );
    GetAgent()->SetPositionY( positionCar.y );
    GetAgent()->SetYawAngle( yawAngle );

    // update velocity
    GetAgent()->SetVelocityX( velocityCar.x );
    GetAgent()->SetVelocityY( velocityCar.y );
    GetAgent()->SetYawVelocity( yawVelocity );

    // update forces
    // transfer to trajectory and/or world CS ??? -> NOPE !!!
    GetAgent()->SetAccelerationX( accelerationCar.x  );
    GetAgent()->SetAccelerationY( accelerationCar.y  );
    GetAgent()->SetYawAcceleration( yawAcceleration );
}

