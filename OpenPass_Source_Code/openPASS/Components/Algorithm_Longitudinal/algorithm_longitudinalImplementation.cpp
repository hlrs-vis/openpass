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
/** @file  algorithm_longitudinalImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include <cassert>
#include "algorithm_longitudinalImplementation.h"
#include "Common/longitudinalSignal.h"
#include "Common/accelerationSignal.h"
#include "Common/parametersVehicleSignal.h"
#include "globalDefinitions.h"

void AlgorithmLongitudinalImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        // from PrioritizerLongitudinalVehicleComponents

        const std::shared_ptr<ComponentStateSignalInterface const> stateSignal = std::dynamic_pointer_cast<ComponentStateSignalInterface const>(data);
        if(stateSignal->componentState == ComponentState::Acting)
        {
            const std::shared_ptr<AccelerationSignal const> signal = std::dynamic_pointer_cast<AccelerationSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }
            componentState = ComponentState::Acting;
            accelerationWish = signal->acceleration;
        }
        else
        {
            componentState = ComponentState::Disabled;
        }
        initializedAccelerationInput = true;
    }
    else if (localLinkId == 1)
    {
        if (!initializedVehicleModelParameters) // load initial constant parameters
        {
            // once from ParametersAgent
            const std::shared_ptr<ParametersVehicleSignal const> signal = std::dynamic_pointer_cast<ParametersVehicleSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }

            vehicleModelParameters = signal->vehicleParameters;
            initializedVehicleModelParameters = true;
        }
    }
    else if (localLinkId == 3 || localLinkId == 4)
    {
        // from SensorDriver
        const std::shared_ptr<SensorDriverSignal const> signal = std::dynamic_pointer_cast<SensorDriverSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        currentVelocity = signal->GetOwnVehicleInformation().velocity;
        initializedSensorDriverData = true;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmLongitudinalImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try {
            data = std::make_shared<LongitudinalSignal const>(
                        componentState,
                        out_accPedalPos,
                        out_brakePedalPos,
                        out_gear);
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

void AlgorithmLongitudinalImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    if(initializedVehicleModelParameters &&
       initializedAccelerationInput &&
       initializedSensorDriverData)
    {
        CalculatePedalPositionAndGear();        // calculate gear and pedal positions
    }
    else {
        const std::string msg = COMPONENTNAME + " missing input";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmLongitudinalImplementation::CalculatePedalPositionAndGear()
{
    if (currentVelocity == 0 && accelerationWish == 0)
    {
            out_accPedalPos = 0;
            out_brakePedalPos = 0;
            out_gear = 0;
    }
    else
    {
        AlgorithmLongitudinalCalculations calculations{currentVelocity,
                    accelerationWish,
                    vehicleModelParameters};
        calculations.CalculateGearAndEngineSpeed();
        calculations.CalculatePedalPositions();

        out_gear = calculations.GetGear();
        out_accPedalPos = calculations.GetAcceleratorPedalPosition();
        out_brakePedalPos = calculations.GetBrakePedalPosition();
    }
}
