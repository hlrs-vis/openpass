/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018, 2019 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

/**
 * @defgroup module_tt Two-track vehicle model
 * The dynamics of the vehicle is modelled in 2 dimensions. In the sub items the principle function is described in order of execution.
 */
/** @addtogroup module_tt
 * Abbreviations:
 * - COG = center-of-gravity
 * - CS = coordinate system
*/

/**
 * @ingroup module_tt
 * @defgroup init_tt Initialization
*/
/**
 * @ingroup module_tt
 * @defgroup sim_step_00_tt Entry
*/
/**
 * @ingroup module_tt
 * @defgroup sim_step_10_tt Process
*/
/**
 * @ingroup module_tt
 * @defgroup sim_step_20_tt Output
*/

#include <memory>
#include <qglobal.h>
#include "dynamics_twotrack_implementation.h"
#include "dynamics_twotrack_local.h"
#include <QString>


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
    std::map<int, double> parameterMapDoubleExternal =
        GetParameters()->GetParametersDouble();
    foreach (auto &iterator, parameterMapDouble)
    {
        int id = iterator.first;
        parameterMapDouble.at(id)->SetValue(parameterMapDoubleExternal.at(id));
    }
    int id_torque = torqueBrakeMin.GetId();
    parameterMapDouble.at(id_torque)->SetValue(-std::fabs(parameterMapDoubleExternal.at(id_torque)));

    timeStep = (double)GetCycleTime() / 1000.0;

    yawVelocity = 0.0;
    yawAcceleration = 0.0;
    vehicle = new VehicleSimpleTT();

    /** @addtogroup init_tt
     * Define vehicle's body and engine characteristics:
     *  - total mass
     *  - power
     *  - maximum brake torque
    */
    vehicle->InitSetEngine(agent->GetWeight(), powerEngineMax.GetValue(), torqueBrakeMin.GetValue());

    /** @addtogroup init_tt
     * Define vehicle's geometry:
     *  - set the height of the COG
     *  - set the longitudinal position of the COG
     *  - set the wheelbase
     *  - set the track width
    */
    vehicle->InitSetGeometry(agent->GetWheelbase(), agent->GetWheelbase()/2.0 - agent->GetDistanceCOGtoFrontAxle(),
                             agent->GetTrackWidth(), 0.0);

    /** @addtogroup init_tt
     * Define vehicle's tire properties:
     *  - set initial velocity
     *  - set peak tire force
     *  - set force at full slide
     *  - set the slip corresponding to peak tire force
     *  - set the radius of the tire
     *  - set the road/tire friction coefficient
    */
    vehicle->InitSetTire(agent->GetVelocityX(),
                         forceTireMaxStatic.GetValue(), forceTireSlideStatic.GetValue(),
                         slipTireMax.GetValue(), radiusTire.GetValue(), agent->GetFrictionCoeff());

    std::vector<double> defaultBrake = {0.0, 0.0, 0.0, 0.0};
    if (!brakeSuperpose.SetDefaultValue(defaultBrake))
    {
        std::stringstream log;
        log << COMPONENTNAME << " Default brake superposition value set failed";
        LOG(CbkLogLevel::Error, log.str());
    }

    std::vector<double> defaultForceVertical = {
        vehicle->forceTireVerticalStatic[0],
        vehicle->forceTireVerticalStatic[1],
        vehicle->forceTireVerticalStatic[2],
        vehicle->forceTireVerticalStatic[3]};
    if (!forceWheelVertical.SetDefaultValue(defaultForceVertical))
    {
        std::stringstream log;
        log << COMPONENTNAME << " Default vertical force value set failed";
        LOG(CbkLogLevel::Error, log.str());
    }

    throttlePedal.SetDefaultValue(1.0);
    brakePedal.SetDefaultValue(0.0);
    angleTireFront.SetDefaultValue(0.0);

    LOGINFO("Constructing Dynamics_TwoTrack successful");
#ifdef QT_DEBUG
    logFile.setFileName(QString().sprintf("dataLog_TT_%d.csv", agent->GetAgentId()));
    if(logFile.exists())
    {
        logFile.remove();
    }
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    logStream = new QTextStream(&logFile);
    *logStream << QString().sprintf("time;agent;position_X;position_Y;velocity_X;velocity_Y\n");
    clockCount = 0;
#endif
}

Dynamics_TwoTrack_Implementation::~Dynamics_TwoTrack_Implementation()
{
    delete (vehicle);
#ifdef QT_DEBUG
    logFile.close();
#endif
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

void Dynamics_TwoTrack_Implementation::Trigger(int time)
{

    Q_UNUSED(time);

    /** @addtogroup sim_step_00_tt
     * Record intentions of the driver or assistant systems:
     *  - throttle pedal
     *  - brake pedal
     *  - steering wheel
    */
    GetAgent()->SetAccelerationIntention(throttlePedal.GetValue());
    GetAgent()->SetDecelerationIntention(brakePedal.GetValue());
    GetAgent()->SetAngleIntention(angleTireFront.GetValue());

    /** @addtogroup sim_step_00_tt
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

    LOGINFO(QString().sprintf("time %d, agent %d: positionCar (%.4f, %.2f), velocityCar (%.4f, %.2f)", time, GetAgent()->GetAgentId(),
                                  positionCar.x, positionCar.y, velocityCar.x, velocityCar.y).toStdString());

    /** @addtogroup sim_step_10_tt
     * Apply acceleration/deceleration intentions:
     *  - calculate tire torques due to engine
     *  - calculate tire torques due to braking by
     *      - driver (pedal state)
     *      - assistant systems (selective single wheel braking)
    */
    vehicle->DriveTrain(Saturate(throttlePedal.GetValue(), 0.0, 1.0),
                        Saturate(brakePedal.GetValue(), 0.0, 1.0),
                        brakeSuperpose.GetValue());

    /** @addtogroup sim_step_10_tt
     * Apply tire forces at the tire/road interface:
     *  - calculate longitudinal tire slips and forces
     *  - calculate lateral tire slips and forces
     *  - calculate friction forces
    */
    vehicle->ForceLocal(timeStep, angleTireFront.GetValue(), forceWheelVertical.GetValue());

    /** @addtogroup sim_step_10_tt
     * Combine local tire forces to a global force at the vehicle's body:
     *  - total longitudinal force
     *  - total lateral force
     *  - air drag
     *  - total rotational momentum
    */
    vehicle->ForceGlobal();

    /** @addtogroup sim_step_20_tt
     * Perform a translational Euler step:
     *  - calculate next vehicle's position from prevoius velocity values
     *  - calculate new velocity from previous acceleration values
     *  - calculate new acceleration from global forces
    */
    NextStateTranslation();

    /** @addtogroup sim_step_20_tt
     * Perform a rotational Euler step:
     *  - calculate vehicle's orientation from previous rotational velocity
     *  - calculate vehicle's rotational velocity from previous rotational acceleration
     *  - calculate vehicle's rotational acceleration from the total rotational momentum
    */
    NextStateRotation();

    /** @addtogroup sim_step_20_tt
     * Write actual vehicle's state:
     *  - global position (cartesian coordinates)
     *  - direction of vehicle's longitudinal axes (angle in polar coordinates)
     *  - vehicle's longitudinal and lateral velocity in vehicle's CS
     *  - vehicle's rotational velociy
     *  - vehicle's longitudinal and lateral acceleration in vehicle's CS
     *  - vehicle's rotational acceleration
     *  - inertia forces on vehicle's COG
    */
    NextStateSet();

#ifdef QT_DEBUG
    *logStream << QString().sprintf("%d;%d;%.4f;%.2f;%.4f;%.2f\n", clockCount, GetAgent()->GetAgentId(),
                                    positionCar.x, positionCar.y, velocityCar.x, velocityCar.y);
    clockCount += timeStep_ms;
#endif

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

    // preserve directions of velocity and acceleration
    velocityCar.Rotate(yawAngle);
    accelerationCar.Rotate(yawAngle);

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

    // reassign directions of velocity and acceleration
    velocityCar.Rotate(-yawAngle);
    accelerationCar.Rotate(-yawAngle);

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

    // update outputs
    std::vector<double> forceInert = {-vehicle->forceTotalXY.x, -vehicle->forceTotalXY.y};
    forceGlobalInertia.SetValue(forceInert);

}

