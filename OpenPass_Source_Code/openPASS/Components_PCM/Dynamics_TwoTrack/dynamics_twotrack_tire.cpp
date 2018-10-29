/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "dynamics_twotrack_tire.h"
#include "dynamics_twotrack_local.h"
#include <cmath>
#include <QtGlobal>

Tire::Tire(): radius(1.0), forceZ_static(-100.0), forcePeak_static(100.0), forceSat_static (50.0), slipPeak(0.1)
{
    Rescale(forceZ_static);
}

Tire::Tire(const double F_ref, const double F_max, const double F_slide, const double s_max,
           const double r, const double mu_scale):
    radius (r),
    forceZ_static (F_ref)
{
    // implicite roll friction scaling
    forcePeak_static  = F_max*mu_scale;
    forceSat_static  = F_slide*mu_scale;
    slipPeak = s_max*mu_scale;
    slipSat = s_slide*mu_scale;
    Rescale(forceZ_static);
}

double Tire::GetForce(const double slip)
{

    double slipAbs = std::fabs(slip);
    double force;
    double slipAbsNorm = Saturate(slipAbs, 0.0, 1.0) / slipPeak;

    if (qFuzzyIsNull(slip)) { // make it easy
        return 0.0;
    } else if (slipAbsNorm <= 1.0) { // adhesion
        force = forcePeak *
                stiffnessRoll * slipAbsNorm /
                ( 1.0 + slipAbsNorm * ( slipAbsNorm + stiffnessRoll - 2.0 ) );
    } else if (slipAbs < slipSat) { // semi-slide
        double slipSlideForceNorm = slipSat / slipPeak;
        double slipNormRatio = ( slipAbsNorm - 1.0 ) / ( slipSlideForceNorm - 1.0 );

        force = forcePeak *
                ( 1.0 -
                  ( 1.0 - forceSat / forcePeak ) *
                  slipNormRatio * slipNormRatio *
                  ( 3.0 - 2.0 * slipNormRatio ) );
    } else { // slide
        force = forceSat;
    }
    return (slip > 0.0) ? force : -force;

}

double Tire::GetLongSlip(const double torque)
{
    double force = torque / radius;
    double forceAbs = std::fabs(force);

    if (( qFuzzyIsNull(force) ))
    {
        return 0.0;
    } else if ( forceAbs <= forcePeak ) { // moderate force in adhesion (slip limited)
        double p_2 = 0.5 * ( stiffnessRoll * ( 1.0 - forcePeak / forceAbs ) - 2.0 );
        double slip = slipPeak * ( -p_2 - std::sqrt( p_2 * p_2 - 1.0 ) );
        return force > 0.0 ? slip : -slip;
    } else { // slide
        //return force > 0.0 ? 1.0 : -1.0;
        return force > 0.0 ? slipSat : -slipSat;
    }
}

double Tire::CalcSlipY(double slipX, double vx, double vy)
{
    if (qFuzzyIsNull(vy) || (qAbs(vx)<velocityLimit && qAbs(vy)<velocityLimit)) {
        return 0.0;
    } else if (qFuzzyIsNull(vx)) {
        return Saturate(-vy, -1.0, 1.0); // non-ISO
    } else {
        return Saturate((std::fabs(slipX) - 1) * vy / std::fabs(vx), -1.0, 1.0); // non-ISO
    }
}

double Tire::GetRollFriction(const double velTireX)
{
    double forceFriction = forceZ * frictionRoll;

    if (velTireX < 0.0)
    {
        forceFriction *= -1.0;
    }
    if (std::fabs(velTireX) < velocityLimit)
    {
        forceFriction *= (velTireX/velocityLimit);
    }

    return forceFriction;
}

void Tire::Rescale(const double forceZ_update)
{

    forceZ = forceZ_update;
    double scaling = Saturate(forceZ/forceZ_static, 0.1, 2.0);

    forcePeak = forcePeak_static*scaling;
    forceSat = forceSat_static*scaling;
}
