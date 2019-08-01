/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  SensorRecordStateImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include "sensor_recordStateImplementation.h"

SensorRecordStateImplementation::SensorRecordStateImplementation(
        std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        WorldInterface *world,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface*> *observations,
        const CallbackInterface *callbacks,
        AgentInterface *agent) :
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
        agent)
{
    agentId = GetAgent()->GetId();

    try
    {
        observerInstance = GetObservations()->at(0);
        if(observerInstance == nullptr)
        {
            throw std::runtime_error("");
        }
    }
    catch(...)
    {
        const std::string msg = COMPONENTNAME + " invalid observation module setup";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }

}

void SensorRecordStateImplementation::UpdateInput(int, const std::shared_ptr<SignalInterface const> &, int)
{
}

void SensorRecordStateImplementation::UpdateOutput(int ,std::shared_ptr<SignalInterface const> &, int)
{
}

void SensorRecordStateImplementation::Trigger(int time)
{
    timeMSec = time;

    auto collisionPartners = GetAgent()->GetCollisionPartners();
    indexLaneEgo = GetAgent()->GetMainLaneId();

    ObserveEgo();
}

void SensorRecordStateImplementation::ObserveEgo()
{
    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Trace,
                             "XPosition",
                             std::to_string(GetAgent()->GetPositionX()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Trace,
                             "YPosition",
                             std::to_string(GetAgent()->GetPositionY()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Trace,
                             "YawAngle",
                             std::to_string(GetAgent()->GetYaw()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Visualization,
                             "VelocityEgo",
                             std::to_string(GetAgent()->GetVelocity()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Visualization,
                             "AccelerationEgo",
                             std::to_string(GetAgent()->GetAcceleration()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Visualization,
                             "BrakeLight",
                             GetAgent()->GetBrakeLight() ? "1" : "0");

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Visualization,
                             "IndicatorState",
                             std::to_string(static_cast<int>(GetAgent()->GetIndicatorState())));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Visualization,
                             "LightStatus",
                             std::to_string(static_cast<int>(GetAgent()->GetLightState())));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::RoadPosition,
                             "PositionRoute",
                             std::to_string(GetAgent()->GetDistanceToStartOfRoad()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::RoadPosition,
                             "TCoordinate",
                             std::to_string(GetAgent()->GetPositionLateral()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::RoadPosition,
                             "Lane",
                             std::to_string(indexLaneEgo));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::RoadPosition,
                             "Road",
                             GetAgent()->GetRoadId());

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::RoadPosition,
                             "SecondaryLanes",
                             SecondaryLanesToString());

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Vehicle,
                             "YawRate",
                             std::to_string(GetAgent()->GetYawRate()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Vehicle,
                             "AccelerationPedalPosition",
                             std::to_string(GetAgent()->GetEffAccelPedal()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Vehicle,
                             "BrakePedalPosition",
                             std::to_string(GetAgent()->GetEffBrakePedal()));

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::Vehicle,
                             "Gear",
                             std::to_string(GetAgent()->GetGear()));

    int frontAgentId = -1;
    const auto roadId = GetAgent()->GetRoadId();
    const auto searchStartDistance = GetAgent()->GetDistanceToStartOfRoad(MeasurementPoint::Front);
    const auto* frontAgent = GetWorld()->GetNextAgentInLane(roadId, indexLaneEgo, searchStartDistance);

    if (frontAgent)
    {
        frontAgentId = frontAgent->GetId();
    }

    observerInstance->Insert(timeMSec,
                             agentId,
                             LoggingGroup::RoadPosition,
                             "VehicleInFront",
                             std::to_string(frontAgentId));
}

std::string SensorRecordStateImplementation::SecondaryLanesToString()
{
    std::list<int> secondaryList = GetAgent()->GetSecondaryCoveredLanes();
    std::string listOfSecondaryLanes{""};
    for (auto objectIt = secondaryList.cbegin(); objectIt != secondaryList.cend(); objectIt++)
    {
        listOfSecondaryLanes += std::to_string(*objectIt);

        if (std::next(objectIt) != secondaryList.cend())
        {
            listOfSecondaryLanes += ";";
        }
    }
    return listOfSecondaryLanes;
}
