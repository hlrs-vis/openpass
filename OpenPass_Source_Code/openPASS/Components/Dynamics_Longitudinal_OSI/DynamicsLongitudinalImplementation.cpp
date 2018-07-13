/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  DynamicsLongitudinalImplementation.cpp
//! @brief This file contains the implementation header file
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include <cassert>
#include <cmath>
#include "DynamicsLongitudinalImplementation.h"
#include "ALongitudinalDriverToDLongitudinalSignal.h"
#include "brakingSignal.h"

void DynamicsLongitudinalImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
        {

        // TODO get carParameters from other source

            // from AlgorithmLongitudinalDriver
            const std::shared_ptr<ALongitudinalDriverToDLongitudinalSignal const> signal = std::dynamic_pointer_cast<ALongitudinalDriverToDLongitudinalSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }
            in_accPedalPos = signal->accPedalPos;
            in_brakePedalPos = signal->brakePedalPos;
            in_carParameters = signal->carParameters;
            in_gear = signal->gear;
        }
        else if (localLinkId == 1)
        {
            const std::shared_ptr<BrakingSignal const> signal = std::dynamic_pointer_cast<BrakingSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }

            in_aVehicle = signal->acceleration;
        }
        else
        {
            const std::string msg = COMPONENTNAME + " invalid link";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
}

void DynamicsLongitudinalImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

double DynamicsLongitudinalImplementation::GetEngineSpeedByVelocity(double xVel, int gear, ALongitudinalDataProvider::CarParameters carParameters)
{
    return (xVel * carParameters.iDiffential * carParameters.gearRatio.at(gear) * 60)/(carParameters.rStat *_twoPi); // an dynamic wheel radius rDyn must actually be used here
}

double DynamicsLongitudinalImplementation::GetEngineMomentMax(double engineSpeed, ALongitudinalDataProvider::CarParameters carParameters)
{
    double torqueMax = carParameters.MEngMax; // initial value at max
    double speed = engineSpeed;

    bool isLowerSection = engineSpeed<carParameters.nEngMin+1000;
    bool isBeyondLowerSectionBorder = engineSpeed<carParameters.nEngMin;
    bool isUpperSection = engineSpeed>carParameters.nEngMax-1000;
    bool isBeyondUpperSectionBorder = engineSpeed>carParameters.nEngMax;


    if (isLowerSection)
    {
        if (isBeyondLowerSectionBorder) // not within limits
        {
               speed = carParameters.nEngMin;
         }
         torqueMax = (1000 - (speed - carParameters.nEngMin))*-0.5 + carParameters.MEngMax;
    }
    else if (isUpperSection)
    {
        double dragFactor = 1; // dont use drag

        if (isBeyondUpperSectionBorder)
        {
            speed = carParameters.nEngMax;
            dragFactor = -0.1; // use drag
        }

        torqueMax = ((speed - carParameters.nEngMax + 1000)*-0.2 + carParameters.MEngMax)*dragFactor;
    }

    return torqueMax;
}

double DynamicsLongitudinalImplementation::GetEngineMomentMin(double engineSpeed, ALongitudinalDataProvider::CarParameters carParameters)
{
    double dragMin = carParameters.MEngMin;

    bool isBeyondUpperSectionBorder = engineSpeed>carParameters.nEngMax;

    double engineMomentMax = GetEngineMomentMax(engineSpeed, carParameters);

    if (!isBeyondUpperSectionBorder)
        dragMin =  engineMomentMax * -0.1;
    else
        dragMin = engineMomentMax; // already drag value

    return dragMin;
}

double DynamicsLongitudinalImplementation::GetFrictionCoefficient()
{
    // Kurve
    // Straßenbeschaffenheit und Straßenbelag
    // Reifenbeschaffenheit
    // Gewicht des Fahrzeugs
    // Radaufstandskraft? carParameters.gewicht * _oneG/4
    // Anpressdruck
    // Fahrgeschwindigkeit
    // Profiltiefe
    // Wassertiefe
    // Schwerpunktverlagerung

    // double nue = GetAgent()->GetFrictionCoeff();

    return 1; // best case - no influence
}

double DynamicsLongitudinalImplementation::GetEngineMoment(double gasPedalPos, int gear, ALongitudinalDataProvider::CarParameters carParameters)
{
    double xVel = GetAgent()->GetVelocityAbsolute();

    double engineSpeedAtGear = GetEngineSpeedByVelocity(xVel, gear, carParameters);

    GetAgent()->SetEngineSpeed(engineSpeedAtGear);

    double max = GetEngineMomentMax(engineSpeedAtGear, carParameters);
    double maxAccAtGear = GetAccFromEngineMoment(xVel, max, gear, carParameters, GetCycleTime());

    GetAgent()->SetMaxAcceleration(maxAccAtGear*GetFrictionCoefficient());

    double min = GetEngineMomentMin(engineSpeedAtGear, carParameters);

    return (std::fabs(min) + max) * gasPedalPos + min;
}

double DynamicsLongitudinalImplementation::GetAccFromEngineMoment(double xVel, double engineMoment, int chosenGear, ALongitudinalDataProvider::CarParameters carParameters, int cycleTime)
{
    Q_UNUSED(xVel);
    Q_UNUSED(cycleTime);

    double wheelSetMoment = engineMoment * (carParameters.iDiffential * carParameters.gearRatio.at(chosenGear));
    double wheelSetForce = wheelSetMoment / carParameters.rStat;

//    double setAcc = wheelSetMoment/(carParameters.carMass * carParameters.rStat);
//    double nextSetVelocity = xVel + setAcc * cycleTime/1000.;
//    double meanVelocity = (xVel + nextSetVelocity)/2;
//    double forceResistanceAir = _rho/2 * carParameters.frontSurface * carParameters.dragCoefficient * std::pow(meanVelocity, 2.0);
//    double forceResistanceRolling = 0.01 * carParameters.carMass;

//    if (wheelSetForce==0)
//    {
//        forceResistanceRolling = -0.;
//    }

    double vehicleSetForce = wheelSetForce;// - forceResistanceRolling - forceResistanceAir;
    double acc = vehicleSetForce/(carParameters.carMass);

    return acc;
}

double DynamicsLongitudinalImplementation::GetAccVehicle(double gasPedalPos, double brakePedalPos, std::vector<int> gear, ALongitudinalDataProvider::CarParameters carParameters, bool output)
{
    double resultAcc = 0;

    double chosenGear = static_cast<double>(gear.at(1));
    double xVel = GetAgent()->GetVelocityAbsolute();
    // for testing
    bool brakeNotGas = false;

        if (brakePedalPos>0) // brake
    {
         resultAcc = brakePedalPos * _oneG * -1;


     /*   double forceResistanceAir = _rho/2 * carParameters.frontSurface * carParameters.dragCoefficient * std::pow(xVel,2.0); // approx.
        double forceResistanceRolling = 0.01 * carParameters.carMass;

        double resistanceSetForce = forceResistanceRolling + forceResistanceAir;
        double accResistance = resistanceSetForce/(carParameters.carMass);

        resultAcc -= accResistance;*/

        // TODO add maxDrag at engineSpeed to brake acc

        double maxDecel = GetAgent()->GetMaxDeceleration();
        resultAcc = std::fmax(maxDecel,resultAcc);

        brakeNotGas = true;

    }
    else // gas
    {
        double engineMoment = GetEngineMoment(gasPedalPos, static_cast<int>(chosenGear), carParameters);
        resultAcc =  GetAccFromEngineMoment(xVel, engineMoment, chosenGear, carParameters, GetCycleTime());
    }

    if (output&&brakeNotGas)
        std::cout << std::endl << "in_brakePedalPos " << in_brakePedalPos << " (" << in_gear.at(1) << ") accVehicle " << resultAcc;
    else if (output)
        std::cout << std::endl << "in_accPedalPos " << in_accPedalPos << " (" << in_gear.at(1) << ") accVehicle " << resultAcc;

   /*
    * // for testing
    * if (brakeNotGas)
        std::cout << std::endl << "-" << in_brakePedalPos << "; " << resultAcc << "; " << in_gear.at(1);
    else
        std::cout << std::endl << "" << in_accPedalPos  << "; " << resultAcc << "; " << in_gear.at(1);*/

    return resultAcc;
}

void DynamicsLongitudinalImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    // Set max deceleration with every trigger event to keep track of changing friction coefficient
    // TODO: use vehicle specific max brake acc to substitue _oneG

    double maxDecel = _oneG * GetFrictionCoefficient() * -1;
    GetAgent()->SetMaxDeceleration(maxDecel);

    double aLong;
    double v;
    double yawAngle = GetAgent()->GetYawAngle();

    _nCollisions = (int) GetAgent()->GetCollisionPartners().size();

    if(_nCollisions > 0)
    {
        aLong = 0.0;
        v = VLowerLimit;
    }
    else
    {
        // for testing purpose
        bool output = false;

        int gearFromAgent = GetAgent()->GetGear();

        double accVehicle = in_aVehicle; //0; TODO disable channel

        //TODO CK this overwrites all braking signals
        if(gearFromAgent!=-999) { // TODO check agent initialisation
            accVehicle = GetAccVehicle(in_accPedalPos, in_brakePedalPos, in_gear, in_carParameters, output);
        }

        aLong = accVehicle;
        v = GetAgent()->GetVelocityX() + aLong*GetCycleTime()/1000.;

        if(v < VLowerLimit)
        {
            aLong = 0.0;
            v = VLowerLimit;
        }
    }

    // set longitudinal properties
    GetAgent()->SetVelocityX(v);
    GetAgent()->SetAccelerationX(aLong);

    // change of path coordinate since last time step [m]
    double ds = v * static_cast<double>(GetCycleTime()) * 0.001;
    // change of inertial x-position due to ds and yaw [m]
    double dx = ds * std::cos(yawAngle);
    // change of inertial y-position due to ds and yaw [m]
    double dy = ds * std::sin(yawAngle);
    // new inertial x-position [m]
    double x = GetAgent()->GetPositionX() + dx;
    GetAgent()->SetPositionX(x);
    // new inertial y-position [m]
    double y = GetAgent()->GetPositionY() + dy;
    GetAgent()->SetPositionY(y);
}

std::string DynamicsLongitudinalImplementation::GetInfoString(double time)
{
    std::stringstream s;
    s << "Time " << time <<
         "ms, Agent " << GetAgent()->GetAgentId() <<
         ", x = " << GetAgent()->GetPositionX() <<
         ", y = " << GetAgent()->GetPositionY() <<
         ", v = " << GetAgent()->GetVelocityX() <<
         ", a = " << GetAgent()->GetAccelerationX() <<
         ", heading: " << GetAgent()->GetYawAngle() <<
         ", v_y_global = " << GetAgent()->GetVelocityX() * std::sin(GetAgent()->GetYawAngle()) <<
         ", accPedalPos = " << GetAgent()->GetEffAccelPedal() <<
         ", brakePedalPos = " << GetAgent()->GetEffBrakePedal() <<
         ", gear = " << GetAgent()->GetGear() << " von " << in_carParameters.maxGear;

    return s.str();
}

bool DynamicsLongitudinalImplementation::HasVToBeCorrectedDueToCollision()
{
    if (GetAgent()->GetCollisionPartners().size() == 0)
    {
        return false;
    }

    return true;
}
