/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  SensorDriverImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>

#include "sensor_driverImplementation.h"
#include "Signals/sensorDriverSignal.h"

SensorDriverImplementation::SensorDriverImplementation(
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
        agent),
    sensorDriverCalculations(agent)
{}

void SensorDriverImplementation::UpdateInput(int localLinkId,
                                             const std::shared_ptr<const SignalInterface> &data,
                                             int time)
{
    Q_UNUSED(time);
    Q_UNUSED(localLinkId);
    Q_UNUSED(data)
}

void SensorDriverImplementation::UpdateOutput(int localLinkId,
                                              std::shared_ptr<const SignalInterface> &data,
                                              int time)
{
    Q_UNUSED(time);

    try
    {
        switch(localLinkId)
        {
        case 0:
            // driver sensor data
            data = std::make_shared<SensorDriverSignal const>(ownVehicleInformation, trafficRuleInformation, geometryInformation, surroundingObjects);
            break;
        default:
            const std::string msg = COMPONENTNAME + " invalid link";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    catch(const std::bad_alloc&)
    {
        const std::string msg = COMPONENTNAME + " could not instantiate signal";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void SensorDriverImplementation::Trigger(int time)
{
    Q_UNUSED(time);
    GetOwnVehicleInformation();
    GetTrafficRuleInformation();
    GetGeometryInformation();
    GetSurroundingObjectsInformation();
}

void SensorDriverImplementation::GetOwnVehicleInformation()
{
    ownVehicleInformation.velocity                      = GetAgent()->GetVelocity(VelocityScope::Absolute);
    ownVehicleInformation.acceleration                  = GetAgent()->GetAcceleration();
    ownVehicleInformation.lateralPosition               = GetAgent()->GetPositionLateral();
    ownVehicleInformation.heading                       = GetAgent()->GetRelativeYaw();
    ownVehicleInformation.steeringWheelAngle            = GetAgent()->GetSteeringWheelAngle();
    ownVehicleInformation.isCrossingLanes               = GetAgent()->IsCrossingLanes();
    ownVehicleInformation.distanceToLaneBoundaryLeft    = GetAgent()->GetLaneRemainder(Side::Left);
    ownVehicleInformation.distanceToLaneBoundaryRight   = GetAgent()->GetLaneRemainder(Side::Right);
    ownVehicleInformation.collision                     = GetAgent()->GetCollisionPartners().size() > 0;
}

void SensorDriverImplementation::GetTrafficRuleInformation()
{
    trafficRuleInformation.laneEgo    = GetTrafficRuleLaneInformationEgo();
    trafficRuleInformation.laneLeft   = GetTrafficRuleLaneInformationLeft();
    trafficRuleInformation.laneRight  = GetTrafficRuleLaneInformationRight();
}

LaneInformationTrafficRules SensorDriverImplementation::GetTrafficRuleLaneInformationEgo()
{
    LaneInformationTrafficRules laneInformation;
    const double visibilityDistance = GetWorld()->GetVisibilityDistance();

    laneInformation.trafficSigns            = GetAgent()->GetTrafficSignsInRange(visibilityDistance);

    return laneInformation;
}

LaneInformationTrafficRules SensorDriverImplementation::GetTrafficRuleLaneInformationLeft()
{
    LaneInformationTrafficRules laneInformation;
    const int relativeLaneId = 1;
    const double visibilityDistance = GetWorld()->GetVisibilityDistance();

    laneInformation.trafficSigns            = GetAgent()->GetTrafficSignsInRange(visibilityDistance, relativeLaneId);

    return laneInformation;
}

LaneInformationTrafficRules SensorDriverImplementation::GetTrafficRuleLaneInformationRight()
{
    LaneInformationTrafficRules laneInformation;
    const int relativeLaneId = -1;
    const double visibilityDistance = GetWorld()->GetVisibilityDistance();

    laneInformation.trafficSigns            = GetAgent()->GetTrafficSignsInRange(visibilityDistance, relativeLaneId);

    return laneInformation;
}

void SensorDriverImplementation::GetGeometryInformation()
{
    geometryInformation.visibilityDistance            = GetWorld()->GetVisibilityDistance();
    geometryInformation.laneEgo    = GetGeometryLaneInformationEgo();
    geometryInformation.laneLeft   = GetGeometryLaneInformationLeft();
    geometryInformation.laneRight  = GetGeometryLaneInformationRight();
}

LaneInformationGeometry SensorDriverImplementation::GetGeometryLaneInformationEgo()
{
    LaneInformationGeometry laneInformation;
    const double visibilityDistance = GetWorld()->GetVisibilityDistance();

    laneInformation.exists                  = true;     // Ego lane must exist by definition, or else vehicle would have despawned by now. Information not necessary atm!
    laneInformation.curvature               = GetAgent()->GetLaneCurvature();
    laneInformation.width                   = GetAgent()->GetLaneWidth();
    laneInformation.distanceToEndOfLane     = GetAgent()->GetDistanceToEndOfLane(visibilityDistance);

    return laneInformation;
}

LaneInformationGeometry SensorDriverImplementation::GetGeometryLaneInformationLeft()
{
    LaneInformationGeometry laneInformation;
    const int relativeLaneId = 1;
    const double visibilityDistance = GetWorld()->GetVisibilityDistance();

    laneInformation.exists                  = GetAgent()->ExistsLaneLeft();
    laneInformation.curvature               = GetAgent()->GetLaneCurvature(relativeLaneId);
    laneInformation.width                   = GetAgent()->GetLaneWidth(relativeLaneId);
    laneInformation.distanceToEndOfLane     = GetAgent()->GetDistanceToEndOfLane(visibilityDistance, relativeLaneId);

    return laneInformation;
}

LaneInformationGeometry SensorDriverImplementation::GetGeometryLaneInformationRight()
{
    LaneInformationGeometry laneInformation;
    const int relativeLaneId = -1;
    const double visibilityDistance = GetWorld()->GetVisibilityDistance();

    laneInformation.exists                  = GetAgent()->ExistsLaneRight();
    laneInformation.curvature               = GetAgent()->GetLaneCurvature(relativeLaneId);
    laneInformation.width                   = GetAgent()->GetLaneWidth(relativeLaneId);
    laneInformation.distanceToEndOfLane     = GetAgent()->GetDistanceToEndOfLane(visibilityDistance, relativeLaneId);

    return laneInformation;
}

void SensorDriverImplementation::GetSurroundingObjectsInformation()
{
    const double visibilityDistance = GetWorld()->GetVisibilityDistance();

    surroundingObjects.objectFront = GetOtherObjectInformation(GetAgent()->GetObjectInFront(visibilityDistance));
    surroundingObjects.objectRear = GetOtherObjectInformation(GetAgent()->GetObjectBehind(visibilityDistance));
    surroundingObjects.objectFrontLeft = GetOtherObjectInformation(GetAgent()->GetObjectInFront(visibilityDistance, 1));
    surroundingObjects.objectRearLeft = GetOtherObjectInformation(GetAgent()->GetObjectBehind(visibilityDistance, 1));
    surroundingObjects.objectFrontRight = GetOtherObjectInformation(GetAgent()->GetObjectInFront(visibilityDistance, -1));
    surroundingObjects.objectRearRight = GetOtherObjectInformation(GetAgent()->GetObjectBehind(visibilityDistance, -1));
}

ObjectInformation SensorDriverImplementation::GetOtherObjectInformation(const WorldObjectInterface* surroundingObject)
{
    ObjectInformation objectInformation;

    auto surroundingVehicle = dynamic_cast<const AgentInterface*>(surroundingObject);

    if (!surroundingObject)
    {
        objectInformation.exist = false;
    }
    else
    {
        objectInformation.exist = true;
        objectInformation.id = surroundingObject->GetId();
        objectInformation.relativeLongitudinalDistance = GetAgent()->GetDistanceToObject(surroundingObject);
        objectInformation.heading = surroundingObject->GetYaw();
        objectInformation.length = surroundingObject->GetLength();
        objectInformation.width = surroundingObject->GetWidth();
        objectInformation.height = surroundingObject->GetHeight();
        if (!surroundingVehicle)
        {
            objectInformation.isStatic = true;
        }
        else
        {
            objectInformation.isStatic = false;
            objectInformation.heading = surroundingVehicle->GetRelativeYaw();
            objectInformation.velocity = surroundingVehicle->GetVelocity();
            objectInformation.acceleration = surroundingVehicle->GetAcceleration();
            objectInformation.relativeLateralDistance = sensorDriverCalculations.GetLateralDistanceToObject(surroundingVehicle);
        }
    }

    return objectInformation;
}
