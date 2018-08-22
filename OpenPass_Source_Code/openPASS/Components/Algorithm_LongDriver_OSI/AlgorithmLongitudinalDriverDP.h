/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  AlgorithmLongitudinalDriverDataprovider.h
* @brief This file contains the calculations
*
* This class contains the data calculations.
* Singleton such that data calculations is generated only once and not seperately
* for each instance.
* */
//-----------------------------------------------------------------------------

#pragma once

#include <vector>

//! \brief provide data and calculate driver's action parameters.
//!
//! \ingroup AlgorithmLongitudinalDataProvider

class ALongitudinalDataProvider
{
public:

    ALongitudinalDataProvider(const ALongitudinalDataProvider&) = delete;
    ALongitudinalDataProvider(ALongitudinalDataProvider&&) = delete;
    ALongitudinalDataProvider& operator=(const ALongitudinalDataProvider&) = delete;
    ALongitudinalDataProvider& operator=(ALongitudinalDataProvider&&) = delete;

    virtual ~ALongitudinalDataProvider() = default;

    // singleton access
    static ALongitudinalDataProvider& Instance()
    {
        static ALongitudinalDataProvider instance;
        return instance;
    }

    //! \brief Constant Parameters, that are special to the car and
    //! have an effect on the calculations
    class CarParameters
    {
        public:

            // constant factor of a chosen gear that reduces engine turns to
            // wheel turns at fixed differential
            std::vector<double> baseRatio = {0, 4.35, 2.496, 1.665, 1.230, 1.0, 0.851};

            // constructors
            CarParameters():
                iDiffential(3.0),
                rStat(0.318),
                carMass(1671.0),
                nEngMin(900),
                nEngMax(8200),
                MEngMin(-54),
                MEngMax(540),
                maxGear(6),
                gearRatio(baseRatio),
                frontSurface(1.93),
                dragCoefficient(0.3)
            {}

            CarParameters(double iDiffential,
                          double rStat,
                          double carMass,
                          double nEngMin,
                          double nEngMax,
                          double MEngMin,
                          double MEngMax,
                          int maxGear,
                          std::vector<double> gearRatio,
                          double frontSurface,
                          double dragCoefficient):
                iDiffential(iDiffential),
                rStat(rStat),
                carMass(carMass),
                nEngMin(nEngMin),
                nEngMax(nEngMax),
                MEngMin(MEngMin),
                MEngMax(MEngMax),
                maxGear(maxGear),
                gearRatio(gearRatio),
                frontSurface(frontSurface),
                dragCoefficient(dragCoefficient)
            {}

            // constant ratio of differential
            double iDiffential;
            // static radius of the wheels  in m
            double rStat;
            // mass of the drivers car + driver + guests and luggage in kg
            double carMass;
            // minimum and maximum of engine turns per minute
            double nEngMin; // ca. Lehrlaufdrehzahl
            double nEngMax; // Otto-Motor, Diesel nur bis 5500?
            // minimum and maximum of engine torque (force) Nm
            double MEngMin;
            double MEngMax;
            // highest gear the car offers
            int maxGear;
            // ratios of the gears
            std::vector<double> gearRatio;
            // projected area of the car exposed to air resistance in m²
            double frontSurface;
            // CW value
            double dragCoefficient;
    };


    std::vector<double> PedalPosition(double xVel, int lastGear, double* lastEngineSpeed, double in_aVehicle, CarParameters carParameters, bool output);

    std::vector<double> GetBestGear(double xVel, double in_aVehicle, CarParameters carParameters, bool output);

    // helper functions to adjust values after calculation
    double ApplyFirstOrderTransientBehaviour(double oldVal, double currentVal);
    double ApplyClosedLoopController(double xVel, double lastXVel, double lastIn_aVehicle, double cycleTime, double pedalPosOLC);
    double ClosedLoopController(double xVel, double lastXVel, double lastIn_aVehicle, double cycleTime);
    double BoundPedalPosition(double pedalPosition);


private:

    ALongitudinalDataProvider(){} // avoid access of constructor of singleton outside class

    // double PI
    double _twoPi = 2 * 3.1415926535;

    // helper value to approximate value from discrete to continious function
    double _tOne = 0.055;

    // value of earth gravity
    double _oneG = 9.81;

    double GetAccFromEngineTorque(double engineTorque, int chosenGear, CarParameters carParameters);

    double GetWheelSetTorque(double acc, CarParameters carParameters);

    double GetEngineSpeedByVelocity(double xVel, int gear, CarParameters carParameters);

    bool isEngineSpeedWithinEngineLimits(double engineSpeed, CarParameters carParameters);
    bool isTorqueWithinEngineLimits(double moment, double engineSpeed, CarParameters carParameters, bool output);
    bool isWithinEngineLimits(double in_aVehicle, int gear, double engineSpeed, CarParameters carParameters, bool output);

    double GetEngineTorqueMin(double engineSpeed, CarParameters carParameters, bool output);
    double GetEngineTorqueMax(double engineSpeed, CarParameters carParameters, bool output);

    double GetEngineTorqueAtGear(int gear, double acc, CarParameters carParameters);
};


