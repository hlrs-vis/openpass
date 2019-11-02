/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  LimiterAccelerationVehicleComponentsImplementation.cpp
//! @brief This file contains the implementation of the header file
//-----------------------------------------------------------------------------

#include "limiterAccelerationVehicleComponentsImplementation.h"
#include <qglobal.h>

void LimiterAccelerationVehicleComponentsImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0) {
        const std::shared_ptr<ComponentStateSignalInterface const> stateSignal = std::dynamic_pointer_cast<ComponentStateSignalInterface const>(data);
        if (stateSignal->componentState == ComponentState::Acting)
        {
            const std::shared_ptr<AccelerationSignal const> signal = std::dynamic_pointer_cast<AccelerationSignal const>(data);
            if (!signal)
            {
                const std::string msg = GetComponentName() + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }

            incomingAcceleration = signal->acceleration;
        }
        componentState = stateSignal->componentState;
    }
    else if (localLinkId == 100)
    {
        // from ParametersAgent
        const std::shared_ptr<ParametersVehicleSignal const> signal = std::dynamic_pointer_cast<ParametersVehicleSignal const>(data);
        if (!signal)
        {
            const std::string msg = GetComponentName() + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
        vehicleModelParameters = signal->vehicleParameters;

        PrepareReferences();
    }
    else
    {
        const std::string msg = GetComponentName() + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void LimiterAccelerationVehicleComponentsImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try
        {
            data = std::make_shared<AccelerationSignal const>(componentState, outgoingAcceleration);
        }
        catch(const std::bad_alloc&)
        {
            const std::string msg = GetComponentName() + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }

    else
    {
        const std::string msg = GetComponentName() + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void LimiterAccelerationVehicleComponentsImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    const double &accelerationLimit = CalculateAccelerationLimit();
    const double &decelerationLimit = CalculateDecelerationLimit();

    outgoingAcceleration = std::max(std::min(incomingAcceleration, accelerationLimit), decelerationLimit);
}

double LimiterAccelerationVehicleComponentsImplementation::InterpolateEngineTorqueBasedOnSpeed(const double &engineSpeed)
{
    if(engineSpeedReferences.size() != engineTorqueReferences.size() || engineSpeedReferences.empty())
    {
        throw std::runtime_error("Interpolation requires same size for vectors and at least one element.");
    }

    if(engineSpeed <= engineSpeedReferences.front())
    {
        return engineTorqueReferences.front();
    }

    if(engineSpeed >= engineSpeedReferences.back())
    {
        return engineTorqueReferences.back();
    }

    for(size_t i = 1; i < engineSpeedReferences.size(); i++)
    {
        if(engineSpeedReferences.at(i) >= engineSpeed)
        {
            const double differenceBetweenEngineSpeedReferencePoints = engineSpeedReferences.at(i) - engineSpeedReferences.at(i-1);
            const double percentage = (engineSpeed - engineSpeedReferences.at(i-1)) / differenceBetweenEngineSpeedReferencePoints;

            const double differenceBetweenEngineTorqueReferencePoints = engineTorqueReferences.at(i) - engineTorqueReferences.at(i-1);

            const double interpolatedTorque = engineTorqueReferences.at(i-1) + (differenceBetweenEngineTorqueReferencePoints * percentage);

            return interpolatedTorque;
        }
    }

    throw std::runtime_error("Could not interpolate torque.");
}

std::vector<double> LimiterAccelerationVehicleComponentsImplementation::PrepareEngineTorqueVectorBasedOnGearRatios()
{
    std::vector<double> engineTorquesBasedOnGearRatios {};

    if(vehicleModelParameters.gearRatios.size() < 2)
    {
        throw std::runtime_error("At least on gear is required!");
    }

    for(size_t i = 1; i < vehicleModelParameters.gearRatios.size(); i++)
    {
        const double engineSpeedBasedOnGear = CalculateEngineSpeedBasedOnGear(GetAgent()->GetVelocity(), i);

        if(engineSpeedBasedOnGear > vehicleModelParameters.maximumEngineSpeed || engineSpeedBasedOnGear < vehicleModelParameters.minimumEngineSpeed)
        {
            continue;
        }

        const double interpolatedEngineTorque = InterpolateEngineTorqueBasedOnSpeed(engineSpeedBasedOnGear);

        double engineTorqueBasedOnGearRatio = interpolatedEngineTorque * vehicleModelParameters.gearRatios.at(i);
        engineTorquesBasedOnGearRatios.push_back(engineTorqueBasedOnGearRatio);
    }

    return engineTorquesBasedOnGearRatios;
}

double LimiterAccelerationVehicleComponentsImplementation::CalculateEngineSpeedBasedOnGear(const double &currentVelocity, const size_t &gear)
{
    const double engineSpeed = currentVelocity * vehicleModelParameters.axleRatio * vehicleModelParameters.gearRatios.at(gear) / (twoPI * vehicleModelParameters.staticWheelRadius) * 60.0;
    return engineSpeed;
}

void LimiterAccelerationVehicleComponentsImplementation::PrepareReferences()
{
    const double& maxEngineTorque = vehicleModelParameters.maximumEngineTorque;
    const double& maxEngineSpeed = vehicleModelParameters.maximumEngineSpeed;
    const double& minEngineSpeed = vehicleModelParameters.minimumEngineSpeed;

    engineTorqueReferences = {0.5 * maxEngineTorque,
                              1.0 * maxEngineTorque,
                              1.0 * maxEngineTorque,
                              1.0 * maxEngineTorque,
                              1.0 * maxEngineTorque / maxEngineSpeed * 5000.0};

    engineSpeedReferences = {minEngineSpeed,
                             1350,
                             4600,
                             5000,
                             maxEngineSpeed};
}

double LimiterAccelerationVehicleComponentsImplementation::CalculateAccelerationLimit()
{
    const double currentVelocity = GetAgent()->GetVelocity();

    const auto &engineTorquesBasedOnGearRatios = PrepareEngineTorqueVectorBasedOnGearRatios();

    if(engineTorquesBasedOnGearRatios.empty())
    {
        return oneG;
    }

    const double &engineTorqueBasedOnVelocity = *(std::max_element(engineTorquesBasedOnGearRatios.begin(), engineTorquesBasedOnGearRatios.end()));

    const double forceAtWheel = engineTorqueBasedOnVelocity * vehicleModelParameters.axleRatio / vehicleModelParameters.staticWheelRadius;
    const double forceRoll = vehicleModelParameters.weight * oneG * frictionCoefficient;
    const double forceAir = (airResistance / 2) * vehicleModelParameters.frontSurface * vehicleModelParameters.airDragCoefficient * std::pow(currentVelocity, 2);

    const double accelerationLimit = (forceAtWheel - forceRoll - forceAir) / vehicleModelParameters.weight;

    return accelerationLimit;
}

double LimiterAccelerationVehicleComponentsImplementation::CalculateDecelerationLimit()
{
    const double decelerationLimit = world->GetFriction() * (-oneG);

    return decelerationLimit;
}
