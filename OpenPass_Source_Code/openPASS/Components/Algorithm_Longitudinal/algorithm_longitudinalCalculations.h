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
/** @file  algorithm_longitudinalCalculations.h
* @brief This file contains the calculations
*
* This class contains the data calculations.
* */
//-----------------------------------------------------------------------------

#pragma once

#include <vector>
#include <cmath>

#include "Common/globalDefinitions.h"

//! \brief This class does all the calculations in the Algorithm_Longitudinal module.
//!
//! Based on the current velocity of the agent and a desired acceleration, this class calculates the
//! gear and the pedal positions required to achieve this acceleration. The AlgorithmLongitudinal constructs
//! an instance of this class with the parameters, then calls the calculate functions and then acquires
//! the results be calling the respective getter.
//!
//! \ingroup Algorithm_Longitudinal

class AlgorithmLongitudinalCalculations
{
public:

    //!
    //! \param velocity                 current velocity of the agent
    //! \param accelerationWish         desired acceleration (can be negative)
    //! \param vehicleModelParameters   parameters of the vehicle model
    //!
    AlgorithmLongitudinalCalculations(double velocity, double accelerationWish, const VehicleModelParameters &vehicleModelParameters);

    //!
    //! \brief Calculates the necessary gear and engine to achieve the desired acceleration at the current velocity
    //!
    void CalculateGearAndEngineSpeed();

    //!
    //! \brief Calculates the necessary accelerator pedal and brake pedal position to achieve desired acceleration
    //! depending on the gear. Therefor CalculateGearAndEngineSpeed needs to be called first.
    //!
    void CalculatePedalPositions();

    //!
    //! \return Calculated position of brake pedal in percent
    //!
    double GetBrakePedalPosition() const;

    //!
    //! \return Calculated position of accelerator pedal in percent
    //!
    double GetAcceleratorPedalPosition() const;

    //!
    //! \return Calculated engine speed
    //!
    double GetEngineSpeed() const;

    //!
    //! \return Calculated gear
    //!
    int GetGear() const;

    //!
    //! \brief Calculates the acceleration that will result from the engineTorque at the given gear
    //!
    double GetAccFromEngineTorque(double engineTorque, int chosenGear);

    //!
    //! \brief Calculates the engine speed required to drive with the given velocity at the specified gear
    //!
    double GetEngineSpeedByVelocity(double velocity, int gear);

    //!
    //! \brief Checks wether the engineSpeed and acceleration/torque can be achieved by the engine
    //!
    bool isWithinEngineLimits(int gear, double engineSpeed, double acceleration);
    inline bool isEngineSpeedWithinEngineLimits(double engineSpeed);
    bool isTorqueWithinEngineLimits(double torque, double engineSpeed);

    //!
    //! \brief Returns the maximum possible engineTorque for the given engineSpeed
    //!
    double GetEngineTorqueMin(double engineSpeed);

    //!
    //! \brief Returns the minimum engineTorque (i.e. maximum drag) for the given engineSpeed
    //!
    double GetEngineTorqueMax(double engineSpeed);

    //!
    //! \brief Calculates the engine torque required to achieve the given acceleration at the specified gear
    //!
    double GetEngineTorqueAtGear(int gear, double acceleration);

protected:
    //Input
    double velocity{0.0};
    double accelerationWish{0.0};
    const VehicleModelParameters &vehicleModelParameters;

    //Output
    int gear{1};
    double engineSpeed{0.0};
    double brakePedalPosition{0.0};
    double acceleratorPedalPosition{0.0};
};


