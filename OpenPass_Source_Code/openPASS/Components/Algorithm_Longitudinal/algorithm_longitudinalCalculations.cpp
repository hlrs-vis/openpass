/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  algorithm_longitudinalCalculations.cpp */
//-----------------------------------------------------------------------------

#include <cassert>
#include <array>
#include <limits>
#include <map>
#include <iostream>
#include "algorithm_longitudinalCalculations.h"

AlgorithmLongitudinalCalculations::AlgorithmLongitudinalCalculations(double xVel,
                                                                     double in_aVehicle,
                                                                     const VehicleModelParameters &vehicleModelParameters) :
    velocity(xVel),
    accelerationWish(in_aVehicle),
    vehicleModelParameters(vehicleModelParameters)
{
}

double AlgorithmLongitudinalCalculations::GetBrakePedalPosition() const
{
    return brakePedalPosition;
}

double AlgorithmLongitudinalCalculations::GetAcceleratorPedalPosition() const
{
    return acceleratorPedalPosition;
}

double AlgorithmLongitudinalCalculations::GetEngineSpeed() const
{
    return engineSpeed;
}

int AlgorithmLongitudinalCalculations::GetGear() const
{
    return gear;
}

double AlgorithmLongitudinalCalculations::GetEngineTorqueAtGear(int gear, double acceleration)
{
    if (acceleration == 0 || gear == 0)
    {
        return 0;
    }

    if (gear > vehicleModelParameters.numberOfGears || gear < 0)
    {
        throw std::runtime_error("Gear in AlgorithmLongitudinal is invalid");
    }

    double wheelSetTorque = vehicleModelParameters.weight * vehicleModelParameters.staticWheelRadius * acceleration;
    double engineTorqueAtGear = wheelSetTorque / (vehicleModelParameters.axleRatio * vehicleModelParameters.gearRatios.at(gear));

    return engineTorqueAtGear;
}

double AlgorithmLongitudinalCalculations::GetEngineSpeedByVelocity(double xVel, int gear)
{
    return (xVel * vehicleModelParameters.axleRatio * vehicleModelParameters.gearRatios.at(gear) * 60) / (vehicleModelParameters.staticWheelRadius * 2.0 * M_PI); // an dynamic wheel radius rDyn must actually be used here
}

void AlgorithmLongitudinalCalculations::CalculateGearAndEngineSpeed()
{
    std::map<double, int> nEngineSet;
    std::map<double, std::tuple<int, double, double>> minDeltaAccWheelBased;

    for (int gear = 1; gear <= vehicleModelParameters.numberOfGears; ++gear)
    {
        double engineSpeed = GetEngineSpeedByVelocity(velocity, gear);
        double limitWheelAcc;
        double accDelta;

        if (accelerationWish >= 0)
        {
            double MMax = GetEngineTorqueMax(engineSpeed);
            limitWheelAcc = GetAccFromEngineTorque(MMax, gear);

            if(accelerationWish == 0)
                accDelta = 0;
            else
                accDelta = std::fabs(accelerationWish - limitWheelAcc);
        }
        else
        {
            double MMin = GetEngineTorqueMin(engineSpeed);
            limitWheelAcc = GetAccFromEngineTorque(MMin, gear);
            accDelta = std::fabs(accelerationWish - limitWheelAcc);
        }

        nEngineSet[engineSpeed] = gear;
        minDeltaAccWheelBased[accDelta] = {gear, engineSpeed, limitWheelAcc};

    }

    bool foundGear = false;

    for (const auto& [engineSpeedResult, gearResult] : nEngineSet)
    {
        if (isWithinEngineLimits(gearResult, engineSpeedResult, accelerationWish)) //&&isChangeOfGearPossibleNow
        {
              gear = gearResult;
              engineSpeed = engineSpeedResult;

              foundGear = true;
        }
        else
            if(foundGear) // leaving possible range
            {
                return;
            }
    }

    if(foundGear)
    {
        return;
    }

    // take lowest delta for gear and engineSpeed
    auto val = minDeltaAccWheelBased.begin()->second;

    // trim wish acceleration to possible value
    gear = std::get<0>(val);
    engineSpeed = std::get<1>(val);
    accelerationWish = std::min(accelerationWish, std::get<2>(val));
}


bool AlgorithmLongitudinalCalculations::isWithinEngineLimits(int gear, double engineSpeed, double acceleration)
{
    if(!isEngineSpeedWithinEngineLimits(engineSpeed))
    {
        return false;
    }

    double currentWishTorque = GetEngineTorqueAtGear(gear, acceleration);

    return isTorqueWithinEngineLimits(currentWishTorque, engineSpeed);
}

bool AlgorithmLongitudinalCalculations::isTorqueWithinEngineLimits(double torque, double engineSpeed)
{
    double currentMEngMax = GetEngineTorqueMax(engineSpeed);

    return torque <= currentMEngMax;
}

inline bool AlgorithmLongitudinalCalculations::isEngineSpeedWithinEngineLimits(double engineSpeed)
{
    return (engineSpeed >= vehicleModelParameters.minimumEngineSpeed && engineSpeed <= vehicleModelParameters.maximumEngineSpeed);
}

double AlgorithmLongitudinalCalculations::GetEngineTorqueMax(double engineSpeed)
{
    double torqueMax = vehicleModelParameters.maximumEngineTorque; // initial value at max
    double speed = engineSpeed;

    bool isLowerSection = engineSpeed < vehicleModelParameters.minimumEngineSpeed + 1000;
    bool isBeyondLowerSectionBorder = engineSpeed < vehicleModelParameters.minimumEngineSpeed;
    bool isUpperSection = engineSpeed > vehicleModelParameters.maximumEngineSpeed - 1000;
    bool isBeyondUpperSectionBorder = engineSpeed > vehicleModelParameters.maximumEngineSpeed;


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

double AlgorithmLongitudinalCalculations::GetEngineTorqueMin(double engineSpeed)
{
    return GetEngineTorqueMax(engineSpeed) * -.1;
}

double AlgorithmLongitudinalCalculations::GetAccFromEngineTorque(double engineTorque, int chosenGear)
{
    double wheelSetTorque = engineTorque * (vehicleModelParameters.axleRatio * vehicleModelParameters.gearRatios.at(chosenGear));
    double wheelSetForce = wheelSetTorque / vehicleModelParameters.staticWheelRadius;
    return wheelSetForce / vehicleModelParameters.weight;
}

void AlgorithmLongitudinalCalculations::CalculatePedalPositions()
{
    constexpr double oneG = 9.81;

    if (accelerationWish < 0) // speed shall be reduced with drag or brake
    {

        double engineTorque = GetEngineTorqueAtGear(gear, accelerationWish);
        double MDragMax = GetEngineTorqueMin(engineSpeed);

        if (engineTorque < MDragMax)
        { // brake

            // calculate acceleration of MDragMax and substract
            // this from in_aVehicle since brake and drag work in parallel while clutch is closed
            double accMDragMax = GetAccFromEngineTorque(MDragMax, gear);

            acceleratorPedalPosition = 0.0;
            brakePedalPosition = std::min(-(accelerationWish - accMDragMax) / oneG, 1.0);
            return;
        }
    }

    // cases of acceleration and drag => use engine here

    double MDragMax = GetEngineTorqueMin(engineSpeed);
    double MTorqueMax = GetEngineTorqueMax(engineSpeed);

    double wishTorque = GetEngineTorqueAtGear(gear, accelerationWish);

    acceleratorPedalPosition = std::min((wishTorque - MDragMax) / (MTorqueMax - MDragMax), 1.0);
    brakePedalPosition = 0.0;
}
