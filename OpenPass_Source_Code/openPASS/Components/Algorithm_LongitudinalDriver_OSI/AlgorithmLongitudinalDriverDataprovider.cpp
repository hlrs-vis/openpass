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
/** @file  AlgorithmLongitudinalDriverDataprovider.cpp */
//-----------------------------------------------------------------------------

#include <cassert>
#include <cmath>
#include <array>
#include <limits>
#include <map>
#include <iostream>
#include "AlgorithmLongitudinalDriverDataprovider.h"


double ALongitudinalDataProvider::ClosedLoopController(double xVel, double lastXVel, double lastIn_aVehicle, double cycleTime){

    double proportionalFactor = 0.5; // [s/m] empirisch ermittelter Wert
    double lastVelocityWish = lastIn_aVehicle * cycleTime/1000 + lastXVel;

    // velocity delta of former setpoint and actual value
    double deltaVel = lastVelocityWish - xVel;
    return proportionalFactor * deltaVel;
}

double ALongitudinalDataProvider::ApplyClosedLoopController(double xVel, double lastXVel, double lastIn_aVehicle, double cycleTime, double pedalPosOLC)
{
    double pedalPosCLC = ClosedLoopController(xVel, lastXVel, lastIn_aVehicle, cycleTime);
    return pedalPosOLC + pedalPosCLC;
}

double ALongitudinalDataProvider::BoundPedalPosition(double pedalPosition)
{
    // cut value
    if (pedalPosition>1.0)
        return 1.0;
    else if (pedalPosition<-1.0)
        return -1.0;

    return pedalPosition; // no cut necessary
}

double ALongitudinalDataProvider::GetWheelSetTorque(double acc, CarParameters carParameters)
{
    return carParameters.carMass * carParameters.rStat * acc;
}

double ALongitudinalDataProvider::GetEngineTorqueAtGear(int gear, double acc, CarParameters carParameters)
{
    if (acc==0)
        return 0;

    if (gear>carParameters.maxGear||gear<0)
        gear = 0; // not defined or not transfered to wheel

    if (gear==0)
        return 0;

    double wheelSetTorque = GetWheelSetTorque(acc, carParameters) ;
    double engineTorqueAtGear = wheelSetTorque/(carParameters.iDiffential*carParameters.gearRatio.at(gear));

    return engineTorqueAtGear;
}

double ALongitudinalDataProvider::GetEngineSpeedByVelocity(double xVel, int gear, CarParameters carParameters)
{
    return (xVel * carParameters.iDiffential * carParameters.gearRatio.at(gear) * 60)/(carParameters.rStat *_twoPi); // an dynamic wheel radius rDyn must actually be used here
}

std::vector<double> ALongitudinalDataProvider::GetBestGear(double xVel, double in_aVehicle, CarParameters carParameters, bool output)
{ // result: gear, engineSpeed

    if (output)
        std::cout << std::endl << "Get Gear: ... for IN_ACC " << in_aVehicle;

    // std::cout << std::endl << "xVel km/h " << xVel *3.6;

    std::vector<double> result = {0., 0.};
    std::map<double, int> nEngineSet;
    std::map<double, std::vector<double>> minDeltaAccWheelBased;

    for (int gear = 1; gear <= carParameters.maxGear;++gear)
    {
        double engineSpeed =  GetEngineSpeedByVelocity(xVel, gear, carParameters);
        double limitWheelAcc;
        double accDelta;

        if (in_aVehicle>=0){

            double MMax = GetEngineTorqueMax(engineSpeed, carParameters, output);
            limitWheelAcc = GetAccFromEngineTorque(MMax, gear, carParameters);

            if(in_aVehicle==0)
                accDelta = 0;
            else
                accDelta = std::fabs(in_aVehicle - limitWheelAcc);
        }
        else
        {
            double MMin = GetEngineTorqueMin(engineSpeed, carParameters, output);
            limitWheelAcc = GetAccFromEngineTorque(MMin, gear, carParameters);
            accDelta = std::fabs(in_aVehicle - limitWheelAcc);
        }

        nEngineSet[engineSpeed] = gear;
        minDeltaAccWheelBased[accDelta] = {static_cast<double>(gear), engineSpeed, limitWheelAcc};

        if (output)
        {
            std::cout << std::endl << "[" << engineSpeed << "] = " << gear;
            if (gear==carParameters.maxGear)
                std::cout << std::endl;
        }
    }

    bool foundGear = false;

    for (std::map<double, int>::iterator it=nEngineSet.begin(); it!=nEngineSet.end(); ++it)
    {
        double engineSpeed =  it->first;
        int gearResult = it->second;

        bool isNextChoiceWithinEngineLimits = isWithinEngineLimits(in_aVehicle, gearResult, engineSpeed, carParameters, output);

        if (output)
            std::cout << std::endl << "engLim: " << isNextChoiceWithinEngineLimits ;

        if (isNextChoiceWithinEngineLimits) //&&isChangeOfGearPossibleNow
        {
              result.at(0) = static_cast<double>(gearResult);
              result.at(1) = engineSpeed;

              foundGear = true;
        }
        else
            if(foundGear) // leaving possible range
            {
                if (output)
                    std::cout << std::endl << "gear: " << result.at(0)<< std::endl;
                return result;
            }
    }



    // take lowest delta for gear and engineSpeed
    std::vector<double> val = minDeltaAccWheelBased.begin()->second;

    // trim wish acceleration to possible value
    in_aVehicle = val.at(2);

    if (output)
        std::cout << std::endl << "OutOfEngineRange-CASE new gear: " << val.at(0);

    return {val.at(0), val.at(1)};
}


bool ALongitudinalDataProvider::isWithinEngineLimits(double in_aVehicle, int gear, double engineSpeed, CarParameters carParameters, bool output)
{
    if(isEngineSpeedWithinEngineLimits(engineSpeed, carParameters))
    {
        // std::cout << std::endl << "in_aVeh -- " << in_aVehicle;
        // std::cout << std::endl << "gear -- " << gear;
        // std::cout << std::endl << "engineSpeed OK -- " << gear;

        double currentWishTorque = 0;

        if(in_aVehicle!=0)
            currentWishTorque = GetEngineTorqueAtGear(gear, in_aVehicle, carParameters);


        bool isWishTorqueWithinLimits = isTorqueWithinEngineLimits(currentWishTorque, engineSpeed, carParameters, output);

        if (isWishTorqueWithinLimits)
            return true;
    }
    return false;
}

bool ALongitudinalDataProvider::isTorqueWithinEngineLimits(double moment, double engineSpeed, CarParameters carParameters, bool output)
{
    if (moment==0)
        return true;

    double currentMEngMax = GetEngineTorqueMax(engineSpeed, carParameters, output);
    double currentMEngMin = GetEngineTorqueMin(engineSpeed, carParameters, output);

    // std::cout << std::endl << "moment " << moment;
    // std::cout << std::endl << "currentMEngMax " << currentMEngMax;
    // std::cout << std::endl << "currentMEngMin " << currentMEngMin;

    if (moment<currentMEngMin||moment>currentMEngMax){
       return false;
    }
    return true;
}

bool ALongitudinalDataProvider::isEngineSpeedWithinEngineLimits(double engineSpeed, CarParameters carParameters)
{
    if (engineSpeed<carParameters.nEngMin||engineSpeed>carParameters.nEngMax){
       return false;
    }
    return true;
}

double ALongitudinalDataProvider::GetEngineTorqueMax(double engineSpeed, CarParameters carParameters, bool output)
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
               if (output)
                   std::cout << std::endl << " engine warning - min limit of engine speed exceeded";
               speed = carParameters.nEngMin;
         }
         torqueMax = (1000 - (speed - carParameters.nEngMin))*-0.5 + carParameters.MEngMax;
    }
    else if (isUpperSection)
    {
        double dragFactor = 1; // dont use drag

        if (isBeyondUpperSectionBorder)
        {
            if (output)
                std::cout << std::endl << " engine warning - max limit of engine speed exceeded";

            speed = carParameters.nEngMax;
            dragFactor = -0.1; // use drag

        }
        torqueMax = ((speed - carParameters.nEngMax + 1000)*-0.2 + carParameters.MEngMax)*dragFactor;
    }

    return torqueMax;
}

double ALongitudinalDataProvider::GetEngineTorqueMin(double engineSpeed, CarParameters carParameters, bool output)
{
    double dragMin = carParameters.MEngMin;

    bool isBeyondUpperSectionBorder = engineSpeed>carParameters.nEngMax;

    double engineTorqueMax = GetEngineTorqueMax(engineSpeed, carParameters, output);

    if (!isBeyondUpperSectionBorder)
        dragMin =  engineTorqueMax * -0.1;
    else
        dragMin = engineTorqueMax; // already drag value

    return dragMin;
}

double ALongitudinalDataProvider::ApplyFirstOrderTransientBehaviour(double oldVal, double currentVal)
{
    double betaVal = _tOne / (1+_tOne);
    double alphaVal = 1 - betaVal;

    return alphaVal * currentVal + betaVal * oldVal;
}

double ALongitudinalDataProvider::GetAccFromEngineTorque(double engineTorque, int chosenGear, CarParameters carParameters)
{
    double wheelSetTorque = engineTorque * (carParameters.iDiffential * carParameters.gearRatio.at(chosenGear));
    double wheelSetForce = wheelSetTorque / carParameters.rStat;
    return wheelSetForce/(carParameters.carMass);
}

std::vector<double> ALongitudinalDataProvider::PedalPosition(double xVel, int lastGear, double* lastEngineSpeed, double in_aVehicle, CarParameters carParameters, bool output)
{   // return gear and pedal position

    // set initial values
    double pedalPosition = 0;
    double engineSpeed = *lastEngineSpeed;
    double chosenGear = static_cast<double>(lastGear);
    double MDragMax = carParameters.MEngMin;
    double MTorqueMax =  carParameters.MEngMax;


    if (in_aVehicle<0) // speed shall be reduced with drag or brake
    {
        double engineTorque = GetEngineTorqueAtGear(chosenGear, in_aVehicle, carParameters);
        MDragMax = GetEngineTorqueMin(engineSpeed, carParameters, output);

        if (engineTorque < MDragMax){ // brake

            if (output)
                std::cout << "BRAKE PEDAL USE" << std::endl;

            // calculate acceleration of MDragMax and substract
            // this from in_aVehicle since brake and drag work in parallel while clutch is closed
            // TODO: this will not take effect if clutch is active
            double accMDragMax = GetAccFromEngineTorque(MDragMax, chosenGear, carParameters);

            pedalPosition = (in_aVehicle+accMDragMax) /_oneG;

            // TODO what happens to engine speed though braking
            // *lastEngineSpeed = engineSpeed;

            return {chosenGear, pedalPosition};
        }
    }

   if (output && in_aVehicle==0)
         std::cout << std::endl << "--> Orig_KEEPVEL" << std::endl;
   else if (in_aVehicle!=0) // cases of acceleration and drag => use engine here
   {
       if (output && in_aVehicle<0)
           std::cout << std::endl << "--> Orig_DRAG" << std::endl;
       else if (output)
           std::cout << std::endl << "--> Orig_GAS" << std::endl;

       // calculate engineSpeed at optimal gear
       std::vector<double> gearAndEngineSpeed = GetBestGear(xVel, in_aVehicle, carParameters, output);

       // set and save newly calculated values
       engineSpeed = gearAndEngineSpeed.at(1);
       chosenGear = gearAndEngineSpeed.at(0);

       if (output)
            std::cout << std::endl << "lastEngineSpeed " << *lastEngineSpeed;

       // save newly calculated engineSpeed for next use
       *lastEngineSpeed = engineSpeed;


       if (output)
       {
            std::cout << std::endl << "newEngineSpeed " << engineSpeed;
            std::cout << std::endl << "nextLastEngineSpeed " << *lastEngineSpeed << std::endl;
       }
   }

   MDragMax = GetEngineTorqueMin(engineSpeed, carParameters, output);
   MTorqueMax = GetEngineTorqueMax(engineSpeed, carParameters, output);

   double wishTorque = GetEngineTorqueAtGear(chosenGear, in_aVehicle, carParameters); // Momentenanforderung im ausgewaehlten Gang
   // TODO Runterschalten, wenn wishMoment größer als MEngMax

   pedalPosition = (wishTorque - MDragMax)/(MTorqueMax - MDragMax);

   if (output)
   {
       std::cout << std::endl << "EngineTorqueAtGear (" << chosenGear << ", " << in_aVehicle << ", ...) = " << wishTorque << std::endl;

       std::cout << std::endl << "engineSpeed:             " << engineSpeed;
       std::cout << std::endl << "chosenGear:              " << chosenGear << std::endl;
       std::cout << std::endl << "save lastEngineSpeed as: " << *lastEngineSpeed << std::endl;
       std::cout << std::endl << "max drag torque:         " << MDragMax;
       std::cout << std::endl << "max torque:       " << MTorqueMax << std::endl;
    }

    return {chosenGear, pedalPosition};
}
