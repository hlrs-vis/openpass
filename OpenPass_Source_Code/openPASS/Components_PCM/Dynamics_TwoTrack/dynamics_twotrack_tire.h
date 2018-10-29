/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
    double GetRollFriction(const double velTireX);
    void Rescale(const double forceZ_update);

private:

    double forceZ_static;
    double forceZ;

    double forcePeak_static;
    double forceSat_static;
    double slipPeak;
    double slipSat;
    double forcePeak;
    double forceSat;

    const double frictionRoll = 0.01;
    const double stiffnessRoll = 0.3;
    const double velocityLimit = 0.27; // ca. 1 km/h
    const double s_slide = 0.4;

};

#endif // TIRE_H
