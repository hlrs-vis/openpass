/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  AlgorithmAEBmplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include <QCoreApplication>
#include <limits>

#include "algorithm_autonomousEmergencyBrakingImplementation.h"
#include "sensorFusionQuery.h"
#include "boundingBoxCalculation.h"

AlgorithmAutonomousEmergencyBrakingImplementation::AlgorithmAutonomousEmergencyBrakingImplementation(
    std::string componentName,
    bool isInit,
    int priority,
    int offsetTime,
    int responseTime,
    int cycleTime,
    StochasticsInterface* stochastics,
    const ParameterInterface* parameters,
    const std::map<int, ObservationInterface*>* observations,
    const CallbackInterface* callbacks,
    AgentInterface* agent) :
    AlgorithmInterface(
        componentName,
        isInit,
        priority,
        offsetTime,
        responseTime,
        cycleTime,
        stochastics,
        parameters,
        observations,
        callbacks,
        agent)
{
    try
    {
        ParseParameters(parameters);
    }
    catch (...)
    {
        const std::string msg = COMPONENTNAME + " could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }

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
}

void AlgorithmAutonomousEmergencyBrakingImplementation::ParseParameters(const ParameterInterface* parameters)
{
    ttcBrake = parameters->GetParametersDouble().at("TTC");
    brakingAcceleration = parameters->GetParametersDouble().at("Acceleration");
    collisionDetectionLongitudinalBoundary = parameters->GetParametersDouble().at("CollisionDetectionLongitudinalBoundary");
    collisionDetectionLateralBoundary = parameters->GetParametersDouble().at("CollisionDetectionLateralBoundary");

    const auto& sensorList = parameters->GetParameterLists().at("SensorLinks");
    for (const auto& sensorLink : sensorList)
    {
        if (sensorLink->GetParametersString().at("InputId") == "Camera")
        {
            sensors.push_back(sensorLink->GetParametersInt().at("SensorId"));
        }
    }
}

void AlgorithmAutonomousEmergencyBrakingImplementation::UpdateInput(int localLinkId,
        const std::shared_ptr<SignalInterface const>& data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " (component " << GetComponentName() << ", agent " << GetAgent()->GetId() <<
        ", input data for local link " << localLinkId << ": ";
    LOG(CbkLogLevel::Debug, log.str());

    //from SensorFusion
    if (localLinkId == 0)
    {
        const std::shared_ptr<SensorDataSignal const> signal = std::dynamic_pointer_cast<SensorDataSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        auto sensorData = signal->sensorData;
        detectedMovingObjects = SensorFusionHelperFunctions::RetrieveMovingObjectsBySensorId(sensors, sensorData);
        detectedStationaryObjects = SensorFusionHelperFunctions::RetrieveStationaryObjectsBySensorId(sensors, sensorData);
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmAutonomousEmergencyBrakingImplementation::UpdateOutput(int localLinkId,
        std::shared_ptr<SignalInterface const>& data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        try
        {
            data = std::make_shared<AccelerationSignal const>(componentState, activeAcceleration);
        }
        catch (const std::bad_alloc&)
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

void AlgorithmAutonomousEmergencyBrakingImplementation::Trigger(int time)
{
    const auto ttc = CalculateTTC();
    if (componentState == ComponentState::Disabled && ShouldBeActivated(ttc))
    {
        componentState = ComponentState::Acting;
        UpdateAcceleration(time);
    }
    else if(componentState == ComponentState::Acting && ShouldBeDeactivated(ttc))
    {
        componentState = ComponentState::Disabled;
        UpdateAcceleration(time);
    }
}

bool AlgorithmAutonomousEmergencyBrakingImplementation::ShouldBeActivated(const double ttc) const
{
    return ttc < ttcBrake;
}

bool AlgorithmAutonomousEmergencyBrakingImplementation::ShouldBeDeactivated(const double ttc) const
{
    return ttc > (ttcBrake * 1.5);
}

double AlgorithmAutonomousEmergencyBrakingImplementation::CalculateObjectTTC(const osi3::BaseMoving& baseMoving)
{
    BoundingBoxCalculation bbcalculation(GetAgent(), collisionDetectionLongitudinalBoundary,
                                         collisionDetectionLateralBoundary);
    double ttc = 0.0;

    while (ttc <= (ttcBrake * 1.5))
    {
        polygon_t objectBoundingBox = bbcalculation.CalculateBoundingBox(ttc, baseMoving);
        polygon_t ownBoundingBox = bbcalculation.CalculateOwnBoundingBox(ttc);

        if (bg::intersects(ownBoundingBox, objectBoundingBox))
        {
            return ttc;
        }

        ttc += GetCycleTime() / 1000.0;
    }

    return std::numeric_limits<double>::max();
}

double AlgorithmAutonomousEmergencyBrakingImplementation::CalculateObjectTTC(const osi3::BaseStationary& baseStationary)
{
    BoundingBoxCalculation bbcalculation(GetAgent(), collisionDetectionLongitudinalBoundary,
                                         collisionDetectionLateralBoundary);
    double ttc = GetCycleTime() / 1000.0;

    polygon_t objectBoundingBox = bbcalculation.CalculateBoundingBox(baseStationary);
    while (!ShouldBeActivated(ttc))
    {
        polygon_t ownBoundingBox = bbcalculation.CalculateOwnBoundingBox(ttc);

        if (bg::intersects(ownBoundingBox, objectBoundingBox))
        {
            return ttc;
        }

        ttc += GetCycleTime() / 1000.0;
    }

    return std::numeric_limits<double>::max();
}

double AlgorithmAutonomousEmergencyBrakingImplementation::CalculateTTC()
{
    double ttc = std::numeric_limits<double>::max();
    for (const auto& detectedObject : detectedMovingObjects)
    {
        double objectTtc = CalculateObjectTTC(detectedObject.base());
        if (objectTtc < ttc)
        {
            ttc = objectTtc;
        }
    }
    for (const auto& detectedObject : detectedStationaryObjects)
    {
        double objectTtc = CalculateObjectTTC(detectedObject.base());
        if (objectTtc < ttc)
        {
            ttc = objectTtc;
        }
    }

    return ttc;
}

void AlgorithmAutonomousEmergencyBrakingImplementation::UpdateAcceleration(const int time)
{
    std::shared_ptr<AgentBasedEvent> event;

    if(componentState == ComponentState::Acting && activeAcceleration != brakingAcceleration)
    {
        activeAcceleration = brakingAcceleration;
        event = std::make_shared<AgentBasedEvent>(time,
                                           COMPONENTNAME,
                                           "",
                                           EventDefinitions::EventType::AEBActive,
                                           GetAgent()->GetId());
    }
    else if (componentState == ComponentState::Disabled && activeAcceleration != 0.0)
    {
        activeAcceleration = 0.0;
        event = std::make_shared<AgentBasedEvent>(time,
                                           COMPONENTNAME,
                                           "",
                                           EventDefinitions::EventType::AEBInactive,
                                           GetAgent()->GetId());
    }

    if(event.get())
    {
        observer->InsertEvent(event);
    }
}
