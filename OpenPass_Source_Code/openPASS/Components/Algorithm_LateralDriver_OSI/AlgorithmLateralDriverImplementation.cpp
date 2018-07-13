/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  AlgorithmLateralDriverImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>

#include "AlgorithmLateralDriverImplementation.h"

#include "commonTools.h"
#include "lateralDriverSignal.h"
#include "steeringSignal.h"

void AlgorithmLateralDriverImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        // from driver model
        const std::shared_ptr<LateralDriverSignal const> signal = std::dynamic_pointer_cast<LateralDriverSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_currCurvature = signal->currCurvature;
        in_vLatEgo = signal->vLatEgo;
        in_vLongEgo = signal->vLongEgo;
        in_notifyCollision = signal->notifyCollision;
        in_freqLateralOscillation = signal->freqLateralOscillation;
        in_zeta = signal->zeta;
        in_LatDisplacement = signal->LatDisplacement;
        in_GainDeltaPhi = signal->GainDeltaPhi;
        in_HeadingError = signal->DeltaPhi;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmLateralDriverImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        try
        {
            data = std::make_shared<SteeringSignal const>(GetComponentId(), ComponentState::Acting, out_desiredSteeringWheelAngle);
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

void AlgorithmLateralDriverImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    // cotroller for lateral displacement
    // ----------------------------------
    // control deviation in lateral displacment
    double DeltaW = in_LatDisplacement;
    // calculate lateral acceleration wish and apply PT2-controller behaviour
    double AccLatTarget = ALatWish(DeltaW);
    // calculate curvature resulting from lateral acceleration wish
    double DesiredCurvature = CurvatureTarget(AccLatTarget);
    // calculate needed steering wheel angle to produce this curvature (Ackermann model; reference point of yawing = rear axle!)
    double deltaHDeltaW = in_steeringRatio * std::atan(DesiredCurvature*in_wheelBase) * RadiantToDegree;

    // controller for heading angle error
    // ----------------------------------
    double deltaHDeltaPhi = in_GainDeltaPhi * in_steeringRatio * in_wheelBase / in_vLongEgo * in_HeadingError * RadiantToDegree;

    // total steering wheel angle
    // --------------------------
    double deltaH = deltaHDeltaW + deltaHDeltaPhi;
    out_desiredSteeringWheelAngle = TrafficHelperFunctions::ValueInBounds(-in_steeringMax, deltaH, in_steeringMax);
}

double AlgorithmLateralDriverImplementation::ALatWish(double DeltaW)
{
    return -in_freqLateralOscillation * (2*(in_vLongEgo > VLim ? in_zeta : Zeta0)*in_vLatEgo - in_freqLateralOscillation*DeltaW);
}

double AlgorithmLateralDriverImplementation::CurvatureTarget(double accLatTarget)
{
    // curvatureterm1 represents the controller part
    double curvatureterm1;

    if (std::abs(accLatTarget) <= 0.001)
        curvatureterm1 = 0.0;
    else if (in_vLongEgo <= SmallV)
        curvatureterm1 = Curvaturemax0 * accLatTarget / std::abs(accLatTarget);
    else
        curvatureterm1 = TrafficHelperFunctions::ValueInBounds(-Curvaturemax, accLatTarget / (in_vLongEgo * in_vLongEgo), Curvaturemax);

    return curvatureterm1 + std::min(Curvaturemax2, in_currCurvature);
}
