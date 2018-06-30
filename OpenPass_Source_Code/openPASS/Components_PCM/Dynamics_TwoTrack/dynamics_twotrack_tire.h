/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef TIRE_H
#define TIRE_H

//! Static tire model based on TMEASY by Rill et al.
class Tire
{
public:

    Tire();
    Tire(const double F_ref, const double F_max, const double F_slide, const double s_max,
         const double r, const double mu_scale);

    virtual ~Tire() = default;

    double radius;
    const double inertia = 1.2;

    double GetForce(const double);
    double GetLongSlip(const double tq);
    double CalcSlipY(double slipX, double vx, double vy);
    double GetRollFriction(const double velTireX, const double F_add);

private:

    double forceZ_static;

    double forcePeak;
    double forceSat;
    double slipPeak;
    double slipSat;

    const double frictionRoll = 0.01;
    const double stiffnessRoll = 0.3;
    const double velocityLimit = 0.27; // ca. 1 km/h

};

#endif // TIRE_H
