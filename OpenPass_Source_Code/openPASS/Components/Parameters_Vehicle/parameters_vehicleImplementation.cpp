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
/** \file  parameters_vehicleImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>

#include "parameters_vehicleImplementation.h"
#include "Common/parametersVehicleSignal.h"

ParametersVehicleImplementation::ParametersVehicleImplementation(
    std::string componentName,
    bool isInit,
    int priority,
    int offsetTime,
    int responseTime,
    int cycleTime,
    StochasticsInterface* stochastics,
    WorldInterface* world,
    const ParameterInterface* parameters,
    const std::map<int, ObservationInterface*>* observations,
    const CallbackInterface* callbacks,
    AgentInterface* agent) :
    SensorInterface(
        componentName,
        isInit,
        priority,
        offsetTime,
        responseTime,
        cycleTime,
        stochastics,
        world,
        parameters,
        observations,
        callbacks,
        agent),
    callbacks(callbacks)
{
    initialisation = true;
}

void ParametersVehicleImplementation::Initialize()
{
    initialisation = false;

    out_vehicleParameters = GetAgent()->GetVehicleModelParameters();
}

void ParametersVehicleImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const>& data,
        int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void ParametersVehicleImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const>& data,
        int time)
{
    Q_UNUSED(time);

    try
    {
        switch (localLinkId)
        {
            case 1:
                // vehicle parameters
                data = std::make_shared<ParametersVehicleSignal const>(out_vehicleParameters);
                break;

            default:
                const std::string msg = COMPONENTNAME + " invalid link";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
        }
    }
    catch (const std::bad_alloc&)
    {
        const std::string msg = COMPONENTNAME + " could not instantiate signal";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void ParametersVehicleImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    if (initialisation)
    {
        Initialize();
    }


    // send infos to observer
    ObservationInterface* observer;
    try
    {
        observer = GetObservations()->at(0);
        if (observer == nullptr) { throw std::runtime_error(""); }
    }
    catch (...)
    {
        const std::string msg = COMPONENTNAME + " invalid observation module setup";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }

    //TODO CK Move into a specific init task of observer
    if (GetAgent()->GetAgentCategory() == AgentCategory::Ego)
    {
        // notify observer of being ego-vehicle
        observer->GatherFollowers();
    }
    else
        if (time > 0)
        {
            // only agents that are spawned not initially should call this function
            observer->InformObserverOnSpawn(GetAgent());
        }
}
