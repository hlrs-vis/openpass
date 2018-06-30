/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include <math.h>
#include "dynamics_twotrack_vehicle.h"
#include "dynamics_twotrack_local.h"
#include <cmath>
#include <QFile>

#define MAXMOMENT 300.0


VehicleSimpleTT::VehicleSimpleTT()
{
    forceTotalXY.Scale(0.0);
    momentTotalZ = 0.0;
    tires.resize(NUMBER_TIRES);

}

VehicleSimpleTT::~VehicleSimpleTT()
{
    for (int i = 0; i < NUMBER_TIRES; ++i) {
        delete tires[i];
    }
}

void VehicleSimpleTT::InitSetEngine(double weight, double P_engine, double T_brakeLimit)
{
    powerEngineMax = P_engine;
    torqueBrakeMin = - std::fabs(T_brakeLimit);
    massTotal = weight;

}

void VehicleSimpleTT::InitSetGeometry(double h_COG, double x_COG,
                                      double y_wheelbase, double y_track)
{

    heightCOG = h_COG;
    yawVelocity = 0.0;

    wheelBase = y_wheelbase;

    positionTire[0].x = x_COG;
    positionTire[1].x = positionTire[0].x;
    positionTire[2].x = positionTire[0].x - y_wheelbase;
    positionTire[3].x = positionTire[2].x;

    positionTire[0].y = y_track / 2.0;
    positionTire[1].y = -positionTire[0].y;
    positionTire[2].y = y_track / 2.0;
    positionTire[3].y = -positionTire[2].y;

    forceTireVerticalStatic[0] = massTotal*accelVerticalEarth / 2.0 * x_COG / y_wheelbase;
    forceTireVerticalStatic[1] = forceTireVerticalStatic[0];
    forceTireVerticalStatic[2] = massTotal*accelVerticalEarth / 2.0 * (y_wheelbase - x_COG) / y_wheelbase;
    forceTireVerticalStatic[3] = forceTireVerticalStatic[2];

}

void VehicleSimpleTT::InitSetTire(double vel, double F_max, double F_slide, double s_max,
                                  double r_tire, double frictionScaleRoll)
{
    for (int i = 0; i < NUMBER_TIRES; ++i) {
        tires[i] = new Tire(forceTireVerticalStatic[i], F_max, F_slide, s_max, r_tire, frictionScaleRoll);
        rotationVelocityTireX[i] = vel / r_tire;
        rotationVelocityGradTireX[i] = 0.0;
    }
}

void VehicleSimpleTT::SetVelocity(const double v_abs, const double slideAngle, const double w)
{
    velocityCar.x = v_abs; // trajectory CS
    velocityCar.y = 0.0; // trajectory CS
    angleSlide = slideAngle; // non-ISO
    yawVelocity = w;
}

void VehicleSimpleTT::DriveTrain(double throttlePedal, double brakePedal)
{

    double torqueEngineMax;
    double rotVelMean = 0.5 * (rotationVelocityTireX[2] + rotationVelocityTireX[3]);
    if (!qFuzzyIsNull(rotVelMean)) {
        torqueEngineMax = powerEngineMax / rotVelMean;
    } else {
        torqueEngineMax = powerEngineMax / 0.001;
    }

    torqueEngineMax = Saturate(torqueEngineMax, 0.0, MAXMOMENT);

    for (int i = 0; i < NUMBER_TIRES; ++i) {

        // tire torque
        torqueTireX[i] = brakePedal * torqueBrakeMin;

        if (i > 1) { // RWD with open differential
            torqueTireX[i] += throttlePedal * torqueEngineMax / 2.0;
        }

    }
}

void VehicleSimpleTT::ForceLocal(double timeStep, double angleTireFront)
{

    double angleTire[NUMBER_TIRES];
    angleTire[0] = angleTireFront + anglePreSet;
    angleTire[1] = angleTireFront - anglePreSet;
    angleTire[2] = -anglePreSet;
    angleTire[3] = anglePreSet;

    Tire *tire_tmp;
    Common::Vector2d velocityTire(0.0, 0.0);
    double rotVelNew, forceAbs;

    // slips + forces
    velocityCar.Rotate(-angleSlide); // car coordinates
    for (int i = 0; i < NUMBER_TIRES; ++i) {

        tire_tmp = tires[i];
        slipTire[i].Scale(0.0);

        // global rotation of the tire
        velocityTire = positionTire[i];
        velocityTire.Rotate(M_PI / 2.0);
        velocityTire.Scale(yawVelocity);

        // translation superposition
        velocityTire.Add(velocityCar);
        velocityTire.Rotate(-angleTire[i]); // tire CS

        // rotational inertia
        torqueTireX[i] -= tire_tmp->inertia * rotationVelocityGradTireX[i];

        // longitudinal slip
        slipTire[i].x = tire_tmp->GetLongSlip(torqueTireX[i], velocityTire.x);

        // lateral slip
        slipTire[i].y = tire_tmp->CalcSlipY(slipTire[i].x, velocityTire.x, velocityTire.y); // non-ISO

        // local tire force
        forceAbs = tire_tmp->GetForce(slipTire[i].Length());
        forceTire[i] = slipTire[i]; // tire CS
        forceTire[i].Norm();
        forceTire[i].Scale(forceAbs);

        // roll friction
        bool posForce = forceTire[i].x > 0.0 ? true : false;
        forceTire[i].x += tire_tmp->GetRollFriction(rotationVelocityTireX[i]*tire_tmp->radius, 0.0);
        if ((forceTire[i].x<0.0 && posForce) || (forceTire[i].x>0.0 && !posForce)){
            forceTire[i].x = 0.0;
        }

        forceTire[i].Rotate(angleTire[i]); // car's CS

        // local plane momentum (around z-axis)
        momentTireZ[i] = positionTire[i].Cross(forceTire[i]);

        // rotational velocity
        rotVelNew = velocityTire.x / (1-slipTire[i].x) / tire_tmp->radius;

        // memorize rotation velocity derivative for inertia torque
        rotationVelocityGradTireX[i] = (rotVelNew - rotationVelocityTireX[i]) / timeStep;

        // memorize rotation velocity
        rotationVelocityTireX[i] = rotVelNew;

    }
    velocityCar.Rotate(angleSlide); // traj. CS

}

void VehicleSimpleTT::ForceGlobal()
{

    forceTotalXY.Scale(0.0);
    momentTotalZ = 0.0;
    for (int i = 0; i < NUMBER_TIRES; ++i) {
        // total force
        forceTotalXY.Add(forceTire[i]);

        // total yaw momentum
        momentTotalZ += momentTireZ[i];
    }

    // air drag
    double forceAirDrag = -0.5 * densityAir * coeffDrag * areaFace * velocityCar.Length() *
            velocityCar.Length();

    forceTotalXY.Rotate(angleSlide); // traj. CS
    forceTotalXY.x += forceAirDrag; // traj. CS
    forceTotalXY.Rotate(-angleSlide); // car CS

}
