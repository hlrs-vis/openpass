/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*               2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <cassert>
#include <algorithm>
#include "AgentAdapter.h"
#include "Common/globalDefinitions.h"

namespace loc = World::Localization;

AgentAdapter::AgentAdapter(WorldInterface* world,
                           const CallbackInterface* callbacks,
                           World::Localization::Cache& localizationCache) :
    WorldObjectAdapter{static_cast<OWL::Interfaces::WorldData*>(world->GetWorldData())->AddMovingObject(static_cast<void*>(this))},
    world{world},
    callbacks{callbacks},
    worldData{static_cast<OWL::Interfaces::WorldData*>(world->GetWorldData())},
    locator{baseTrafficObject, *worldData, localizationCache}
{
}

AgentAdapter::~AgentAdapter()
{
}

bool AgentAdapter::InitAgentParameter(int id,
                                      int spawnTime,
                                      AgentBlueprintInterface* agentBlueprint)
{
    VehicleModelParameters vehicleModelParameters = agentBlueprint->GetVehicleModelParameters();
    this->vehicleModelParameters = vehicleModelParameters;

    this->vehicleModelType = agentBlueprint->GetVehicleModelName();
    this->driverProfileName = agentBlueprint->GetDriverProfileName();
    this->vehicleType = vehicleModelParameters.vehicleType;
    this->id = id;
    this->agentCategory = agentBlueprint->GetAgentCategory();
    this->agentTypeName = agentBlueprint->GetAgentProfileName();
    this->objectName = agentBlueprint->GetObjectName();
    this->speedGoalMin = agentBlueprint->GetSpeedGoalMin();
    this->spawnTime = spawnTime;

    if (vehicleType != AgentVehicleType::Car &&
            vehicleType != AgentVehicleType::Truck &&
            vehicleType != AgentVehicleType::Motorbike &&
            vehicleType != AgentVehicleType::Bicycle &&
            vehicleType != AgentVehicleType::Pedestrian)
    {
        LOG(CbkLogLevel::Error, "undefined traffic object type");
        return false;
    }

    // set default values
    GetBaseTrafficObject().SetHeight(1.0);
    GetBaseTrafficObject().SetZ(0.0);
    GetBaseTrafficObject().SetPitch(0.0);
    GetBaseTrafficObject().SetRoll(0.0);


    UpdateWidth(vehicleModelParameters.width);
    UpdateLength(vehicleModelParameters.length);
    UpdateDistanceReferencePointToFrontAxle(vehicleModelParameters.distanceReferencePointToFrontAxle);
    UpdateDistanceReferencePointToLeadingEdge(vehicleModelParameters.distanceReferencePointToLeadingEdge);
    UpdateYaw(agentBlueprint->GetSpawnParameter().yawAngle); //Yaw is needed to calculate velocity
    UpdatePositionX(agentBlueprint->GetSpawnParameter().positionX);
    UpdatePositionY(agentBlueprint->GetSpawnParameter().positionY);
    UpdateAccelPedal(0.);
    UpdateBrakePedal(0.);
    UpdateVelocity(agentBlueprint->GetSpawnParameter().velocity);
    UpdateAcceleration(agentBlueprint->GetSpawnParameter().acceleration);
    UpdateGear(agentBlueprint->GetSpawnParameter().gear);
    UpdateWeight(vehicleModelParameters.weight);
    UpdateHeightCOG(vehicleModelParameters.heightCOG);
    UpdateWheelbase(vehicleModelParameters.wheelbase);
    UpdateFrictionCoeff(vehicleModelParameters.frictionCoeff);
    UpdateTrackWidth(vehicleModelParameters.trackwidth);
    UpdateMomentInertiaRoll(vehicleModelParameters.momentInertiaRoll);
    UpdateMomentInertiaPitch(vehicleModelParameters.momentInertiaPitch);
    UpdateMomentInertiaYaw(vehicleModelParameters.momentInertiaYaw);
    UpdateMaxVelocity(vehicleModelParameters.maxVelocity);
    UpdateMaxCurvature(vehicleModelParameters.maxCurvature);

    SetSensorParameters(agentBlueprint->GetSensorParameters());

    // spawn tasks are executed before any other task types within current scheduling time
    // other task types will have a consistent view of the world
    // calculate initial position
    Locate();

    return true;
}

bool AgentAdapter::Update()
{
    if (GetVelocity() == 0.0)
    {
        return true;
    }
    boundingBoxNeedsUpdate = true;

    return Locate();
}

void AgentAdapter::SetBrakeLight(bool brakeLightStatus)
{
    GetBaseTrafficObject().SetBrakeLightState(brakeLightStatus);
}

bool AgentAdapter::GetBrakeLight() const
{
    return GetBaseTrafficObject().GetBrakeLightState();
}

bool AgentAdapter::Locate()
{
    // reset on-demand values
    remainders.clear();
    boundaryPoints.clear();

    locateResult = locator.Locate(GetBoundingBox2D(), GetLength() + GetWidth());
    if (!locateResult.isLocalizable)
    {
        return false;
    }

    GetBaseTrafficObject().SetRoadCoordinate(locateResult.globalRoadPosition.roadPosition);
    return true;
}

void AgentAdapter::Unlocate()
{
    if (GetVelocity() != 0.0)
    {
        locator.Unlocate();
    }
}

AgentCategory AgentAdapter::GetAgentCategory() const
{
    return agentCategory;
}

std::string AgentAdapter::GetAgentTypeName() const
{
    return agentTypeName;
}

void AgentAdapter::SetIndicatorState(IndicatorState indicatorState)
{
    GetBaseTrafficObject().SetIndicatorState(indicatorState);
}

IndicatorState AgentAdapter::GetIndicatorState() const
{
    return GetBaseTrafficObject().GetIndicatorState();
}

std::string AgentAdapter::GetRoadId(MeasurementPoint mp) const
{
    if (mp == MeasurementPoint::Front)
    {
        return locateResult.mainRoadId;
    }
    if (mp == MeasurementPoint::Reference)
    {
        return locateResult.globalRoadPosition.roadId;
    }
    if (mp == MeasurementPoint::Rear)
    {
        throw std::invalid_argument("measurement point not valid");
    }

    return {};
}

int AgentAdapter::GetMainLaneId(MeasurementPoint mp) const
{
    if (mp == MeasurementPoint::Front)
    {
        return locateResult.mainLaneId;
    }
    if (mp == MeasurementPoint::Reference)
    {
        return locateResult.globalRoadPosition.laneId;
    }
    if (mp == MeasurementPoint::Rear)
    {
        throw std::invalid_argument("measurement point not valid");
    }

    return -999;
}

int AgentAdapter::GetLaneIdLeft() const
{
    return locateResult.globalRoadPosition.laneId + 1;
}

int AgentAdapter::GetLaneIdRight() const
{
    return locateResult.globalRoadPosition.laneId - 1;
}

int AgentAdapter::GetLaneIdFromRelative(int relativeLaneId) const
{
    switch (relativeLaneId)
    {
    case 0:
        return GetMainLaneId();
    case 1:
        return GetLaneIdLeft();
    case -1:
        return GetLaneIdRight();
    default:
        throw std::invalid_argument("relativeLaneId not supported");
    }
}

int AgentAdapter::GetNumberOfLanes()
{
    return world->GetNumberOfLanes(GetRoadId(), GetDistanceToStartOfRoad());
}

bool AgentAdapter::IsAgentInWorld() const
{
    return locateResult.isLocalizable;
}

void AgentAdapter::SetPosition(Position pos)
{
    SetPositionX(pos.xPos);
    SetPositionY(pos.yPos);
    SetYaw(pos.yawAngle);
}

double AgentAdapter::GetDistanceToStartOfRoad(MeasurementPoint mp) const
{
    if (mp == MeasurementPoint::Front)
    {
        return GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd);
    }
    if (mp == MeasurementPoint::Rear)
    {
        return GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadStart);
    }
    if (mp == MeasurementPoint::Reference)
    {
        auto value =  GetRoadPosition().s;
        return value;
    }

    throw std::invalid_argument("measurement point not within valid bounds");
}

double AgentAdapter::GetVelocity(VelocityScope velocityScope) const
{
    if (velocityScope == VelocityScope::Absolute)
    {
        return GetBaseTrafficObject().GetAbsVelocityDouble();
    }
    if (velocityScope == VelocityScope::Lateral)
    {
        return GetBaseTrafficObject().GetAbsVelocityDouble() * std::sin(GetBaseTrafficObject().GetRoadCoordinate().hdg);
    }
    if (velocityScope == VelocityScope::Longitudinal)
    {
        return GetBaseTrafficObject().GetAbsVelocityDouble() * std::cos(GetBaseTrafficObject().GetRoadCoordinate().hdg);
    }
    if (velocityScope == VelocityScope::DirectionX)
    {
        return GetBaseTrafficObject().GetAbsVelocity().vx;
    }
    if (velocityScope == VelocityScope::DirectionY)
    {
        return GetBaseTrafficObject().GetAbsVelocity().vy;
    }

    throw std::invalid_argument("velocity scope not within valid bounds");
}

double AgentAdapter::GetLaneWidth(int relativeLane, double distance) const
{
    return world->GetLaneWidth(GetRoadId(), GetLaneIdFromRelative(relativeLane), GetRoadPosition().s + distance);
}

double AgentAdapter::GetLaneWidthRightDrivingAndStopLane()
{
    if (!IsLaneDrivingLane(GetLaneIdRight()) && !IsLaneStopLane(GetLaneIdRight()))
    {
        return 0.0;
    }
    return world->GetLaneWidth(GetRoadId(), GetLaneIdRight(), GetDistanceToStartOfRoad());
}

double AgentAdapter::GetLaneCurvature(int relativeLane, double distance)
{
    return world->GetLaneCurvature(GetRoadId(), GetLaneIdFromRelative(relativeLane), GetRoadPosition().s + distance);
}

bool AgentAdapter::IsEgoAgent() const
{
    return agentCategory == AgentCategory::Ego;
}

WorldObjectInterface* AgentAdapter::GetObjectInFront(double previewDistance, int relativeLaneId) const
{
    return world->GetNextObjectInLane(GetRoadId(), GetLaneIdFromRelative(relativeLaneId), GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd), previewDistance);
}

WorldObjectInterface *AgentAdapter::GetObjectBehind(double previewDistance, int relativeLaneId) const
{
    return world->GetClosestObjectInUpstream(GetRoadId(), GetLaneIdFromRelative(relativeLaneId), GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd),  previewDistance);
}

TrafficObjectInterface* AgentAdapter::GetTrafficObjectInFront(int laneId) const
{
    return world->GetNextTrafficObjectInLane(GetRoadId(), laneId,
            GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd));
}

AgentInterface* AgentAdapter::GetAgentInFront(int laneId) const
{
    return world->GetNextAgentInLane(GetRoadId(), laneId, GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd));
}

std::vector<const TrafficObjectInterface*> AgentAdapter::GetAllTrafficObjectsInFront() const
{
    std::vector<const TrafficObjectInterface*> frontTrafficObjects;
    for (auto laneId : locateResult.frontLaneIds)
    {
        const auto trafficObjectInFront = GetTrafficObjectInFront(laneId);
        if (trafficObjectInFront)
        {
            frontTrafficObjects.push_back(trafficObjectInFront);
        }
    }
    return frontTrafficObjects;
}

std::vector<const WorldObjectInterface*> AgentAdapter::GetAllWorldObjectsInFront() const
{
    std::vector<const WorldObjectInterface*> frontObjects;
    for (auto laneId : locateResult.frontLaneIds)
    {
        const auto objectInFront = GetObjectInFront(laneId);
        if (objectInFront)
        {
            frontObjects.push_back(objectInFront);
        }
    }
    return frontObjects;
}

std::vector<AgentInterface*> AgentAdapter::GetAllAgentsInFront() const
{
    std::vector<AgentInterface*> frontAgents;
    for (auto laneId : locateResult.frontLaneIds)
    {
        auto agentInFront = GetAgentInFront(laneId);
        if (agentInFront)
        {
            frontAgents.push_back(agentInFront);
        }
    }
    return frontAgents;
}

const AgentInterface* AgentAdapter::GetAgentBehind(int laneId) const
{
    return world->GetClosestAgentInUpstream(GetRoadId(), laneId,
                                            GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadStart));
}

double AgentAdapter::GetDistanceToObject(const WorldObjectInterface* otherObject) const
{
    if (otherObject == nullptr)
    {
        return INFINITY;
    }

    double distanceIfOtherObjectIsInFront = otherObject->GetDistanceToStartOfRoad(MeasurementPoint::Rear) -
                                           GetDistanceToStartOfRoad(MeasurementPoint::Front);
    double distanceIfOtherObjectIsBehind = otherObject->GetDistanceToStartOfRoad(MeasurementPoint::Front) -
                                          GetDistanceToStartOfRoad(MeasurementPoint::Rear);

    //other object is in front
    if (distanceIfOtherObjectIsInFront >= 0)
    {
        return distanceIfOtherObjectIsInFront;
    }
    //other object is behind
    else
        if (distanceIfOtherObjectIsBehind < 0)
        {
            return distanceIfOtherObjectIsBehind;
        }
        else
        {
            return 0;
        }
}

void AgentAdapter::UpdateCollision(std::pair<ObjectTypeOSI, int> collisionPartner)
{
    auto findIter = std::find_if(collisionPartners.begin(), collisionPartners.end(),
                                 [collisionPartner](const std::pair<ObjectTypeOSI, int>& storedCollisionPartner)
    {
        return collisionPartner == storedCollisionPartner;
    });
    if (findIter == collisionPartners.end())
    {
        collisionPartners.push_back(collisionPartner);
    }
}

const WorldObjectInterface* AgentAdapter::GetNearestObjectInVector(std::vector<const WorldObjectInterface*> objects)
{
    const WorldObjectInterface* nearestObject = nullptr;

    double minDistance = INFINITY;
    double currentDistance;

    for (const WorldObjectInterface* object : objects)
    {
        currentDistance = GetDistanceToObject(object);
        if (currentDistance < minDistance)
        {
            minDistance = currentDistance;
            nearestObject = object;
        }
    }
    return nearestObject;
}

bool AgentAdapter::IsLeavingWorld() const
{
    return locateResult.isLeavingWorld;
}

bool AgentAdapter::IsCrossingLanes() const
{
    return locateResult.isCrossingLanes;
}

std::vector<CommonTrafficSign::Entity> AgentAdapter::GetTrafficSignsInRange(double searchDistance, int relativeLane) const
{
    assert(relativeLane != (int)INFINITY);
    return world->GetTrafficSignsInRange(GetRoadId(), GetLaneIdFromRelative(relativeLane), GetDistanceToStartOfRoad(), searchDistance);
}

bool AgentAdapter::GetHeadLight() const
{
    return GetBaseTrafficObject().GetHeadLight();
}

bool AgentAdapter::GetHighBeamLight() const
{
    return GetBaseTrafficObject().GetHighBeamLight();
}

double AgentAdapter::GetDistanceToFrontTrafficObject() const
{
    std::vector<const TrafficObjectInterface*> trafficObjectsInFront = GetAllTrafficObjectsInFront();
    double minDist = INFINITY;

    for (auto trafficObjectInFront : trafficObjectsInFront)
    {
        double currDist = trafficObjectInFront->GetDistanceToStartOfRoad() - GetDistanceToStartOfRoad();
        minDist = std::min(minDist, currDist);
    }

    return minDist;
}

const Obstruction AgentAdapter::GetObstruction(const WorldObjectInterface& worldObject) const
{
    auto leftGap = GetLaneRemainder(Side::Left);
    auto rightGap = GetLaneRemainder(Side::Right);

    if (leftGap == 0.0 && rightGap == 0.0)
    {
        return Obstruction::Invalid();
    }

    double distanceLeft = 0.0;
    double distanceRight = 0.0;

    if (leftGap > 0.0)
    {
        auto lateralDistance = world->GetLateralDistance(GetBoundaryPoint(Side::Right),
                               worldObject.GetBoundaryPoint(Side::Left));
        if (std::get<0>(lateralDistance))
        {
            distanceLeft = std::get<1>(lateralDistance);
        }
        else
        {
            return Obstruction::Invalid();
        }
    }

    if (rightGap > 0.0)
    {
        auto lateralDistance = world->GetLateralDistance(GetBoundaryPoint(Side::Left),
                               worldObject.GetBoundaryPoint(Side::Right));
        if (std::get<0>(lateralDistance))
        {
            distanceRight = std::get<1>(lateralDistance);
        }
        else
        {
            return Obstruction::Invalid();
        }
    }

    return {distanceLeft, distanceRight};
}

bool AgentAdapter::ExistsLaneLeft() const
{
    return world->ExistsLaneLeft(GetRoadId(), GetMainLaneId(),
                                 GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd));
}

bool AgentAdapter::ExistsLaneRight() const
{
    return world->ExistsLaneRight(GetRoadId(), GetMainLaneId(),
                                  GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd));
}

bool AgentAdapter::IsLaneStopLane(int laneId, double distance)
{
    const auto& availableStopLanes = world->GetStopLanesAtDistance(GetRoadId(), GetDistanceToStartOfRoad() + distance);
    for (int lane : availableStopLanes)
    {
        if (lane == laneId)
        {
            return true;
        }
    }
    return false;
}

bool AgentAdapter::IsLaneDrivingLane(int laneId, double distance)
{
    const auto& availableDrivingLanes = world->GetDrivingLanesAtDistance(GetRoadId(), GetDistanceToStartOfRoad() + distance);
    for (int lane : availableDrivingLanes)
    {
        if (lane == laneId)
        {
            return true;
        }
    }
    return false;
}

bool AgentAdapter::IsLaneExitLane(int laneId, double distance)
{
    const auto& availableExitLanes = world->GetExitLanesAtDistance(GetRoadId(), GetDistanceToStartOfRoad() + distance);
    for (int lane : availableExitLanes)
    {
        if (lane == laneId)
        {
            return true;
        }
    }
    return false;
}

bool AgentAdapter::IsLaneRamp(int laneId, double distance)
{
    const auto& availableRamps = world->GetRampsAtDistance(GetRoadId(), GetDistanceToStartOfRoad() + distance);
    for (int lane : availableRamps)
    {
        if (lane == laneId)
        {
            return true;
        }
    }
    return false;
}

double AgentAdapter::GetDistanceToEndOfLane(double sightDistance, int relativeLane) const
{
    return world->GetDistanceToEndOfLane(GetRoadId(), GetLaneIdFromRelative(relativeLane),
                                         GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd),
                                         sightDistance);
}

std::vector<const WorldObjectInterface*> AgentAdapter::GetObjectsInRange(int relativeLane, double backwardsRange,
        double forwardRange, MeasurementPoint mp) const
{
    return world->GetObjectsInRange(GetRoadId(), GetLaneIdFromRelative(relativeLane), GetDistanceToStartOfRoad(mp) - backwardsRange,
                                    GetDistanceToStartOfRoad(mp) + forwardRange);
}

std::vector<const AgentInterface*> AgentAdapter::GetAgentsInRange(int relativeLane, double backwardsRange,
        double forwardRange, MeasurementPoint mp) const
{
    return world->GetAgentsInRange(GetRoadId(), GetLaneIdFromRelative(relativeLane), GetDistanceToStartOfRoad(mp) - backwardsRange,
                                   GetDistanceToStartOfRoad(mp) + forwardRange);
}

std::vector<const AgentInterface*> AgentAdapter::GetAgentsInRangeAbsolute(int laneId, double minDistance,
        double maxDistance) const
{
    return world->GetAgentsInRange(GetRoadId(), laneId, minDistance, maxDistance);
}

double AgentAdapter::GetDistanceToEndOfRamp(int laneId, double sightDistance) const
{

    return world->GetDistanceToEndOfRamp(GetRoadId(), laneId,
                                         GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd),
                                         sightDistance);
}

double AgentAdapter::GetPositionLateral() const
{
    return GetRoadPosition().t;
}

double AgentAdapter::GetLaneDirection() const
{
    return world->GetLaneDirection(GetRoadId(), GetMainLaneId(), GetRoadPosition().s);
}

void AgentAdapter::Unregister() const
{
    worldData->RemoveMovingObjectById(GetBaseTrafficObject().GetId());
}

double AgentAdapter::GetLaneRemainder(Side side) const
{
    if (remainders.empty())
    {
        // Update only on request
        remainders = locator.GetLaneRemainders();
    }

    assert(!remainders.empty());
    return side == Side::Left ? remainders.begin()->left : remainders.rbegin()->right;
}

GlobalRoadPosition AgentAdapter::GetBoundaryPoint(Side side) const
{
    if (boundaryPoints.empty())
    {
        // Update only on request
        boundaryPoints = locator.GetBoundaryPoints();
    }
    assert(!boundaryPoints.empty());
    return side == Side::Left ? *boundaryPoints.begin() : *boundaryPoints.rbegin();
}

double AgentAdapter::GetDistanceToEndOfExit(int laneID, double sightDistance) const
{
    return world->GetDistanceToEndOfExit(GetRoadId(), laneID,
                                         GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd),
                                         sightDistance);
}

LightState AgentAdapter::GetLightState() const
{

    if (GetFlasher())
    {
        return LightState::Flash;
    }
    if (GetHighBeamLight())
    {
        return LightState::HighBeam;
    }
    if (GetHeadLight())
    {
        return LightState::LowBeam;
    }

    return LightState::Off;
}

std::list<int> AgentAdapter::GetSecondaryCoveredLanes()
{
    return locateResult.touchedLaneIds;
}
