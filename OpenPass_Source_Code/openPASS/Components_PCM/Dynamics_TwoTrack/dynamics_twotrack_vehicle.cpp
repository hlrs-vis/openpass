/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "dynamics_twotrack_vehicle.h"
#include "dynamics_twotrack_local.h"
#include <cmath>
#include <QFile>


VehicleSimpleTT::VehicleSimpleTT()
{
    forceTotalXY.Scale(0.0);
    momentTotalZ = 0.0;
    tires.resize(NUMBER_OF_WHEELS);

}

VehicleSimpleTT::~VehicleSimpleTT()
{
    for (int i = 0; i < NUMBER_OF_WHEELS; ++i)
    {
        delete tires[i];
    }
}

void VehicleSimpleTT::InitSetEngine(double weight, double P_engine, double T_brakeLimit)
{
    powerEngineLimit = std::fabs(P_engine);
    torqueBrakeLimit = std::fabs(T_brakeLimit);
    massTotal = weight;
}

void VehicleSimpleTT::InitSetGeometry(double x_wheelbase, double x_COG,
                                      double y_track, double y_COG)
{

    yawVelocity = 0.0;

    positionTire[0].x = x_wheelbase/2.0 - x_COG; // > 0
    positionTire[1].x = positionTire[0].x; // > 0
    positionTire[2].x = -x_wheelbase/2.0 - x_COG; // < 0
    positionTire[3].x = positionTire[2].x; // < 0

    positionTire[0].y = y_track/2.0 - y_COG; // > 0
    positionTire[1].y = -y_track/2.0 - y_COG; // < 0
    positionTire[2].y = positionTire[0].y; // > 0
    positionTire[3].y = positionTire[1].y; // < 0

    double massFront = -massTotal * positionTire[2].x / x_wheelbase;
    double massRear = massTotal * positionTire[0].x / x_wheelbase;

    forceTireVerticalStatic[0] = -accelVerticalEarth * massFront * positionTire[1].y / y_track;
    forceTireVerticalStatic[1] = accelVerticalEarth * massFront * positionTire[0].y / y_track;
    forceTireVerticalStatic[2] = -accelVerticalEarth * massRear * positionTire[3].y / y_track;
    forceTireVerticalStatic[3] = accelVerticalEarth * massRear * positionTire[2].y / y_track;

    // RWD
    torqueTireXthrottle[0] = 0.0;
    torqueTireXthrottle[1] = 0.0;

}

void VehicleSimpleTT::InitSetTire(double vel, double F_max, double F_slide, double s_max,
                                  double r_tire, double frictionScaleRoll)
{
    for (int i = 0; i < NUMBER_OF_WHEELS; ++i)
    {
        tires[i] = new Tire(forceTireVerticalStatic[i], F_max, F_slide, s_max, r_tire, frictionScaleRoll);
        rotationVelocityTireX[i] = vel / r_tire;
        rotationVelocityGradTireX[i] = 0.0;
    }
}

void VehicleSimpleTT::SetVelocity(Common::Vector2d velocityCars, const double w)
{
    velocityCar = velocityCars; // car CS
    yawVelocity = w;
}

void VehicleSimpleTT::DriveTrain(double throttlePedal, double brakePedal,
                                 std::vector<double> brakeSuperpose)
{

    double torqueEngineMax;
    double rotVelMean = 0.5 * (rotationVelocityTireX[2] + rotationVelocityTireX[3]);
    if (!qFuzzyIsNull(rotVelMean))
    {
        torqueEngineMax = powerEngineLimit / rotVelMean;
    }
    else
    {
        torqueEngineMax = powerEngineLimit / 0.001;
    }

    torqueEngineMax = Saturate(torqueEngineMax, 0.0, torqueEngineLimit);
    double brakePedalMod;
    for (int i = 0; i < NUMBER_OF_WHEELS; ++i)
    {

        // brake balance
        if (i < 2)
        {
            brakePedalMod = brakeBalance * 2.0 * brakePedal;
        }
        else
        {
            brakePedalMod = (1.0 - brakeBalance) * 2.0 * brakePedal;
        }
        brakePedalMod += brakeSuperpose[i];

        // tire torque
        torqueTireXbrake[i] = Saturate(brakePedalMod, 0.0, 1.0) * torqueBrakeLimit;

        if (i > 1)   // RWD with open differential
        {
            torqueTireXthrottle[i] = throttlePedal * torqueEngineMax / 2.0;
        }

    }
}

void VehicleSimpleTT::ForceLocal(double timeStep, double angleTireFront, std::vector<double> forceVertical)
{

    double angleTire[NUMBER_OF_WHEELS];
    angleTire[0] = angleTireFront + anglePreSet;
    angleTire[1] = angleTireFront - anglePreSet;
    angleTire[2] = -anglePreSet;
    angleTire[3] = anglePreSet;

    Tire *tire_tmp;
    Common::Vector2d velocityTire(0.0, 0.0);
    double rotVelNew, forceAbs, torqueTireSum;

    // slips + forces
    for (int i = 0; i < NUMBER_OF_WHEELS; ++i)
    {

        tire_tmp = tires[i];
        tire_tmp->Rescale(forceVertical[i]); // here goes the delta_F_z scaling
        slipTire[i].Scale(0.0);

        // global rotation of the tire
        velocityTire = positionTire[i];
        velocityTire.Rotate(M_PI / 2.0);
        velocityTire.Scale(yawVelocity);

        // translation superposition
        velocityTire.Add(velocityCar);
        velocityTire.Rotate(-angleTire[i]); // tire CS

        // rotational inertia
        //torqueTireX[i] -= tire_tmp->inertia * rotationVelocityGradTireX[i];

        if (qFuzzyIsNull(velocityTire.x))
        {
            torqueTireSum = 0.0;
        }
        else if (velocityTire.x < 0.0)
        {
            torqueTireSum = torqueTireXbrake[i];
        }
        else
        {
            torqueTireSum = - torqueTireXbrake[i];
        }
        torqueTireSum += torqueTireXthrottle[i];

        // longitudinal slip
        slipTire[i].x = tire_tmp->GetLongSlip(torqueTireSum);

        // lateral slip
        slipTire[i].y = tire_tmp->CalcSlipY(slipTire[i].x, velocityTire.x, velocityTire.y); // non-ISO

        // local tire force
        forceAbs = tire_tmp->GetForce(slipTire[i].Length());
        forceTire[i] = slipTire[i]; // tire CS
        forceTire[i].Norm();
        forceTire[i].Scale(forceAbs);

        // roll friction
        bool posForce = forceTire[i].x > 0.0;
        forceTire[i].x += tire_tmp->GetRollFriction(velocityTire.x);
        if ((forceTire[i].x < 0.0 && posForce) || (forceTire[i].x > 0.0 && !posForce))
        {
            forceTire[i].x = 0.0;
        }

        forceTire[i].Rotate(angleTire[i]); // car's CS

        // local plane momentum (around z-axis)
        momentTireZ[i] = positionTire[i].Cross(forceTire[i]);

        // rotational velocity
        rotVelNew = velocityTire.x / (1 - slipTire[i].x) / tire_tmp->radius;

        // memorize rotation velocity derivative for inertia torque
        rotationVelocityGradTireX[i] = (rotVelNew - rotationVelocityTireX[i]) / timeStep;

        // memorize rotation velocity
        rotationVelocityTireX[i] = rotVelNew;

    }

}

void VehicleSimpleTT::ForceGlobal()
{

    forceTotalXY.Scale(0.0);
    momentTotalZ = 0.0;
    for (int i = 0; i < NUMBER_OF_WHEELS; ++i)
    {
        // total force
        forceTotalXY.Add(forceTire[i]);

        // total yaw momentum
        momentTotalZ += momentTireZ[i];
    }

    // air drag
    double forceAirDrag = -0.5 * densityAir * coeffDrag * areaFace * velocityCar.Length() * velocityCar.Length();
    double angleSlide = velocityCar.Angle(); // ISO

    forceTotalXY.Rotate(-angleSlide); // traj. CS
    forceTotalXY.Add(forceAirDrag); // traj. CS
    forceTotalXY.Rotate(angleSlide); // car CS

}
