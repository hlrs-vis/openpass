/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  algorithm_lateralImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include "algorithm_lateralImplementation.h"
#include "Common/steeringSignal.h"
#include "Common/parametersVehicleSignal.h"
#include "Components/Sensor_Driver/Signals/sensorDriverSignal.h"
#include "Common/lateralSignal.h"
#include "Common/commonTools.h"
#include "globalDefinitions.h"

void AlgorithmLateralImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        // from PrioritizerLateralVehicleComponent or Driver
        const std::shared_ptr<ComponentStateSignalInterface const> stateSignal = std::dynamic_pointer_cast<ComponentStateSignalInterface const>(data);
        if(stateSignal->componentState == ComponentState::Acting)
        {
            isActive = true;
            const std::shared_ptr<LateralSignal const> signal = std::dynamic_pointer_cast<LateralSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }
            in_lateralDeviation = signal->lateralDeviation;
            in_gainLateralDeviation = signal->gainLateralDeviation;
            in_headingError = signal->headingError;
            in_gainHeadingError = signal->gainHeadingError;
            in_kappaSet = signal->kappaSet;
        }
        else
        {
            isActive = false;
        }
    }
    else if (localLinkId == 100)
    {
        // from ParametersAgent
        const std::shared_ptr<ParametersVehicleSignal const> signal = std::dynamic_pointer_cast<ParametersVehicleSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_steeringRatio = signal->vehicleParameters.steeringRatio;
        in_steeringMax = signal->vehicleParameters.maximumSteeringWheelAngleAmplitude;
        in_wheelBase = signal->vehicleParameters.wheelbase;
    }
    else if (localLinkId == 101 || localLinkId == 102)
    {
        // from SensorDriver
        const std::shared_ptr<SensorDriverSignal const> signal = std::dynamic_pointer_cast<SensorDriverSignal const>(data);

        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

         velocity = signal->GetOwnVehicleInformation().velocity;
         steeringWheelAngle = signal->GetOwnVehicleInformation().steeringWheelAngle;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmLateralImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try
        {
            if (isActive)
            {
                data = std::make_shared<SteeringSignal const>(ComponentState::Acting, out_desiredSteeringWheelAngle);
            }
            else
            {
                data = std::make_shared<SteeringSignal const>(ComponentState::Disabled, 0.0);
            }
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

void AlgorithmLateralImplementation::Trigger(int time)
{
    // Time step length
    double dt{(time - timeLast) * 0.001};

    // Scale gains to current velocity. Linear interpolation between 0 and default values at 200km/h.
    double velocityFactor = 3.6 / 200. * velocity;
    in_gainLateralDeviation *= velocityFactor;
    in_gainHeadingError *= velocityFactor;

    // Controller for lateral deviation
    double deltaHLateralDeviation = in_gainLateralDeviation
            * in_steeringRatio * in_wheelBase / (velocity * velocity)
            * in_lateralDeviation * RadiantToDegree;

    // Controller for heading angle error
    double deltaHHeadingError = in_gainHeadingError
            * in_steeringRatio * in_wheelBase / velocity
            * in_headingError * RadiantToDegree;

    // Controller for curvature
    double deltaHkappa = std::atan(in_kappaSet * in_wheelBase)
            * in_steeringRatio * RadiantToDegree;

    // Total steering wheel angle
    double deltaH = deltaHLateralDeviation + deltaHHeadingError + deltaHkappa;

    // Limit steering wheel velocity. Human limit set to 160°/s.
    if (std::fabs(deltaH - steeringWheelAngle) > 160. * dt)
    {
        deltaH = deltaH / std::fabs(deltaH) * 160. * dt + steeringWheelAngle;
    }

    // Calculate lateral position of next time step
    double lateralDeviationNext = std::sin(deltaH / in_steeringRatio / RadiantToDegree) * velocity * dt + in_lateralDeviation;
    if (in_lateralDeviation * lateralDeviationNext < 0.)  // negative --> sign changed
    {
        deltaH = std::asin(in_lateralDeviation / (velocity * dt)) * in_steeringRatio * RadiantToDegree;
    }

    out_desiredSteeringWheelAngle = TrafficHelperFunctions::ValueInBounds(-in_steeringMax, deltaH, in_steeringMax);

    timeLast = time;
}
