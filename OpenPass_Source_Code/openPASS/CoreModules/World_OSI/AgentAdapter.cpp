/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <cassert>
#include <algorithm>

#include "AgentAdapter.h"
#include "globalDefinitions.h"
#include "worldInterface.h"
#include "WorldData.h"

namespace loc = World::Localization;

AgentAdapter::AgentAdapter(WorldInterface *world, const CallbackInterface *callbacks, World::Localization::Cache& localizationCache) :
    world{world},
    worldData{static_cast<OWL::WorldData*>(world->GetWorldData())},
    callbacks{callbacks},
    locator{static_cast<OWL::WorldData*>(world->GetWorldData())->GetRoads(), localizationCache}
{
}

AgentAdapter::~AgentAdapter()
{
    if(carInfo) {
        delete carInfo;
        carInfo = nullptr;
    }
}

const polygon_t& AgentAdapter::GetBoundingBox2D() const
{
    if (boundingBoxNeedsUpdate)
    {
        boundingBox = CalculateBoundingBox();
        boundingBoxNeedsUpdate = false;
    }

    return boundingBox;
}

void AgentAdapter::UpdateCollision(int collisionPartnerId)
{
    auto findIter = std::find_if(idsCollisionPartners.begin(), idsCollisionPartners.end(),
                                 [collisionPartnerId](const int &idInList)
    {
        return collisionPartnerId == idInList;
    });
    if (findIter == idsCollisionPartners.end())
    {
        // not in list yet
        idsCollisionPartners.push_back(collisionPartnerId);
    }
}

void AgentAdapter::UpdateCollision(int collisionPartnerId, int collisionDataId, void *collisionData)
{
    // dummy
}

IndicatorState AgentAdapter::GetIndicatorState()
{
    // TODO
    return IndicatorState::IndicatorState_Off;
}

void AgentAdapter::SetIndicatorState(IndicatorState indicatorState)
{
    // TODO
}

AgentInterface* AgentAdapter::GetAgentBehind(int laneId) const
{
    // TODO
    return nullptr;
}

CarInfo *AgentAdapter::GenerateCarInfo()
{
    // TODO
    return nullptr;
}

void AgentAdapter::SetAccelerationIntention(double accelerationIntention)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateAccelerationIntention(arg);
    },
    accelerationIntention);
}

double AgentAdapter::GetAccelerationIntention() const
{
    return accelerationIntention;
}

void AgentAdapter::SetDecelerationIntention(double decelerationIntention)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateDecelerationIntention(arg);
    },
    decelerationIntention);
}

double AgentAdapter::GetDecelerationIntention() const
{
    return decelerationIntention;
}

void AgentAdapter::SetAngleIntention(double angleIntention)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateAngleIntention(arg);
    },
    angleIntention);
}

double AgentAdapter::GetAngleIntention() const
{
    return angleIntention;
}

void AgentAdapter::SetCollisionState(bool collisionState)
{
    world->QueueAgentUpdate([this](bool arg)
    {
        UpdateCollisionState(arg);
    },
    collisionState);
}

bool AgentAdapter::GetCollisionState() const
{
    return collisionState;
}

std::vector<void *> AgentAdapter::GetCollisionData(int collisionPartnerId, int collisionDataId) const
{
    Q_UNUSED(collisionPartnerId);
    Q_UNUSED(collisionDataId);

    return std::vector<void*>();
}
const OWL::MovingObject* AgentAdapter::GetBaseTrafficObject() const
{
    return baseTrafficObject;
}

const polygon_t AgentAdapter::CalculateBoundingBox() const
{
    double length   = GetLength();
    double width    = GetWidth();
    double rotation = GetYawAngle();

    double x = GetPositionX();
    double y = GetPositionY();

    double center = GetDistanceReferencePointToLeadingEdge();

    double halfWidth = width / 2.0;

    point_t boxPoints[]
    {
        {center - length, -halfWidth},
        {center - length,  halfWidth},
        {center,           halfWidth},
        {center,          -halfWidth},
        {center - length, -halfWidth}
    };

    polygon_t box;
    polygon_t boxTemp;
    bg::append(box, boxPoints);

    bt::translate_transformer<double, 2, 2> translate(x, y);

    // rotation in mathematical negativ order (boost) -> invert to match
    bt::rotate_transformer<bg::radian, double, 2, 2> rotate(-rotation);

    bg::transform(box, boxTemp, rotate);
    bg::transform(boxTemp, box, translate);

    return box;
}
bool AgentAdapter::InitAgentParameter(int id,
                                      int agentTypeId,
                                      int spawnTime,
                                      const AgentSpawnItem *agentSpawnItem,
                                      const SpawnItemParameterInterface &spawnItemParameter)
{
    this->id = id;
    this->vehicleType = agentSpawnItem->GetVehicleType();
    this->spawnTime = spawnTime;

    if (AgentVehicleType::Car == vehicleType ||
        AgentVehicleType::Truck == vehicleType ||
        AgentVehicleType::Motorbike == vehicleType ||
        AgentVehicleType::Bicycle == vehicleType ||
        AgentVehicleType::Pedestrian == vehicleType)
    {
        baseTrafficObject = worldData->AddMovingObject();
    }
    else
    {
        LOG(CbkLogLevel::Error, "undefined traffic object type");
        return false;
    }

    if (!baseTrafficObject)
    {
        return false;
    }

    // set default values
    baseTrafficObject->SetHeight(1.0);
    baseTrafficObject->SetZ(0.0);
    baseTrafficObject->SetPitch(0.0f);
    baseTrafficObject->SetRoll(0.0f);

    UpdatePositionX(spawnItemParameter.GetPositionX());
    UpdatePositionY(spawnItemParameter.GetPositionY());
    UpdateAccelPedal(0.);
    UpdateBrakePedal(0.);
    UpdateVelocityX(spawnItemParameter.GetVelocityX());
    UpdateVelocityY(spawnItemParameter.GetVelocityY());
    UpdateAccelerationX(spawnItemParameter.GetAccelerationX());
    UpdateGear(0.);
    UpdateYaw(spawnItemParameter.GetYawAngle());

    UpdateWidth(agentSpawnItem->GetWidth());
    UpdateLength(agentSpawnItem->GetLength());
    UpdateWeight(agentSpawnItem->GetWeight());
    UpdateHeightCOG(agentSpawnItem->GetHeightCOG());
    UpdateWheelbase(agentSpawnItem->GetWheelbase());
    UpdateFrictionCoeff(agentSpawnItem->GetFrictionCoeff());
    UpdateTrackWidth(agentSpawnItem->GetTrackWidth());
    UpdateMomentInertiaRoll(agentSpawnItem->GetMomentInertiaRoll());
    UpdateMomentInertiaPitch(agentSpawnItem->GetMomentInertiaPitch());
    UpdateMomentInertiaYaw(agentSpawnItem->GetMomentInertiaYaw());
    UpdateMaxVelocity(70.);
    UpdateMaxCurvature(0.2);
    UpdateDistanceReferencePointToFrontAxle(agentSpawnItem->GetDistanceCOGtoFrontAxle());   // TODO corrections
    UpdateDistanceReferencePointToLeadingEdge(agentSpawnItem->GetDistanceCOGtoLeadingEdge());

    // calculate initial position
    locator.SetBaseTrafficObject(baseTrafficObject);
    Locate();

    return true;
}

bool AgentAdapter::Locate()
{
    boundingBoxNeedsUpdate = true;
    boundingBox2D = GetBoundingBox2D();
    locator.Locate(boundingBox2D);

    frontMainLaneId = locator.GetMainLaneId(World::Localization::Reference::Front);
    rearMainLaneId = locator.GetMainLaneId(World::Localization::Reference::Rear);
    frontLaneCoverages = locator.GetLaneCoverages(World::Localization::Reference::Front);
    rearLaneCoverages= locator.GetLaneCoverages(World::Localization::Reference::Rear);
    completlyInWorld = locator.AllPointsLocated();
    roadPos = locator.GetReferencePoint();
    remainder = locator.GetLaneRemainder();

    if(completlyInWorld == false) {
        LOG(CbkLogLevel::Warning, "Locate | Agent " + std::to_string(GetAgentId()) + " not completly covered by world");
    }

    if(frontLaneCoverages.size() == 1 && frontLaneCoverages[frontMainLaneId] < 1) {
        LOG(CbkLogLevel::Warning, "Locate | Front main lane coverage below 1mm");
    }

    if(rearLaneCoverages.size() == 1 && rearLaneCoverages[rearMainLaneId] < 1) {
        LOG(CbkLogLevel::Warning, "Locate | Rear main lane coverage below 1mm");
    }

    if(frontLaneCoverages.size() == 0 && rearLaneCoverages.size() == 0) {
        LOG(CbkLogLevel::Warning, "Locate | No sections covered by agent");
        return false;
    }

    return true;
}

bool AgentAdapter::Unlocate()
{
    locator.Unlocate();

    return true;
}

bool AgentAdapter::IsAgentInWorld()
{
    return completlyInWorld;
}

void AgentAdapter::SetPosition(Position pos)
{
    SetPositionX(pos.xPos);
    SetPositionY(pos.yPos);
    SetYawAngle(pos.yawAngle);
}

double AgentAdapter::GetDistanceToStartOfRoad() const
{
    return GetDistanceToStartOfRoad(MeasurementPoint::Front);
}

double AgentAdapter::GetDistanceToStartOfRoad(MeasurementPoint mp) const
{
    if (mp == MeasurementPoint::Front) {
        return roadPos.s + WorldObjectCommon::GetFrontDeltaS(GetLength(), GetWidth(), roadPos.hdg, distanceReferencePointToLeadingEdge);
    }
    if (mp == MeasurementPoint::Rear) {
        return roadPos.s - WorldObjectCommon::GetRearDeltaS(GetLength(), GetWidth(), roadPos.hdg, distanceReferencePointToLeadingEdge);
    }
    if (mp == MeasurementPoint::Reference) {
        auto value = roadPos.s;
        //LOG(CbkLogLevel::Warning, "GetDistanceToStartOfRoad(Ref)   | Agent " + std::to_string(GetAgentId()) + ", value: " + std::to_string(value));
        return value;
    }

    throw std::invalid_argument("measurement point not within valid bounds");
}

double AgentAdapter::GetLaneWidth()
{
    // TODO: use locator information
    //OWL::CLane& lane = worldData->GetLaneById(frontMainLaneId);

    if (baseTrafficObject->GetLaneAssignments().empty())
    {
        return INFINITY;
    }

    OWL::CLane& lane = *(baseTrafficObject->GetLaneAssignments().front());

    return lane.GetWidth(roadPos.s);
}

double AgentAdapter::GetLaneWidthLeft()
{
    OWL::CLane& lane = worldData->GetLaneById(frontMainLaneId);
    OWL::CLane& leftLane = lane.GetLeftLane();

    if (leftLane.Exists())
    {
        // TODO: implement width retrieval
        //return leftLane.GetWidthAtPosition(...);
        return 3.5;
    }

    return 0.0;
}

double AgentAdapter::GetLaneWidthRight()
{
    OWL::CLane& lane = worldData->GetLaneById(frontMainLaneId);
    OWL::CLane& rightLane = lane.GetRightLane();

    if (rightLane.Exists())
    {
        // TODO: implement width retrieval
        //return rightLane.GetWidthAtPosition(...);
        return 3.5;
    }

    return 0.0;
}

double AgentAdapter::GetCurvature()
{
    return GetCurvatureInDistance(0);
}

double AgentAdapter::GetCurvatureInDistance(double distance)
{
    OWL::CLanes& lanes = GetBaseTrafficObject()->GetLaneAssignments();

    // TODO: use lane position instead of road position
    return lanes.front()->GetCurvature(GetRoadPosition().s + distance);
}

bool AgentAdapter::IsSpecialAgent() const
{
    // TODO: dummy
    return false;
}

double AgentAdapter::GetDistanceToFrontAgent(int laneId)
{
    const AgentInterface* frontAgent = GetAgentInFront(laneId);

    if (frontAgent == nullptr)
    {
        return INFINITY;
    }

    return frontAgent->GetDistanceToStartOfRoad(MeasurementPoint::Rear) - GetDistanceToStartOfRoad(MeasurementPoint::Front);
}

double AgentAdapter::GetDistanceToFrontAgent()
{
    std::vector<AgentInterface*> agentsInFront = GetAllAgentsInFront();
    double minDist = INFINITY;

    for (auto agentInFront : agentsInFront)
    {
        double currDist = agentInFront->GetDistanceToStartOfRoad(MeasurementPoint::Rear) - GetDistanceToStartOfRoad(MeasurementPoint::Front);
        minDist = std::min(minDist, currDist);
    }

    return minDist;
}

double AgentAdapter::GetDistanceToRearAgent(int laneId)
{
    // TODO
    Q_UNUSED(laneId);
    return INFINITY;
}

double AgentAdapter::GetDistanceToAgent(AgentInterface* otherAgent)
{
    // TODO
    Q_UNUSED(otherAgent);
    return INFINITY;
}

template <typename T> struct SearchResult;

template <typename T>
struct SearchResult<T*>
{
    T* storedObject {nullptr};
    double storedDistance {std::numeric_limits<double>::max()};

    void StoreIfCloser(T* object, double minimumDistance)
    {
        auto rear = object->GetRoadPosition().s + object->GetDistanceReferencePointToLeadingEdge() - object->GetLength();
        if (rear > minimumDistance && rear < storedDistance)
        {
            storedObject = object;
            storedDistance = object->GetRoadPosition().s;
        }
    }

    bool Valid()
    {
        return storedObject != nullptr;
    }
};

bool AgentAdapter::OnRoad(const OWL::Interfaces::Road& road) const
{
    return &baseTrafficObject->GetRoad() == &road;
}

bool AgentAdapter::OnLane(const OWL::Interfaces::Lane& lane) const
{
    return &baseTrafficObject->GetLane() == &lane;
}

const AgentInterface* AgentAdapter::GetAgentInFront(int laneId) const
{
    Q_UNUSED(laneId);

    SearchResult<const AgentInterface*> searchResult;

    for (const auto& agentMapItem : world->GetAgents())
    {
        auto agent = static_cast<const AgentAdapter*>(agentMapItem.second);

        if (agent->OnRoad(baseTrafficObject->GetRoad()))
        {
            for (auto lane : baseTrafficObject->GetLaneAssignments())
            {
                if(agent->OnLane(*lane))
                {
                    // TODO: correct call
                    searchResult.StoreIfCloser(agent, roadPos.s + distanceReferencePointToLeadingEdge);
                }
            }
        }

    }

    //TODO: make return value const AgentInterface* if possible
    return const_cast<AgentInterface*>(searchResult.storedObject);
}

std::vector<AgentInterface*> AgentAdapter::GetAllAgentsInFront() const
{
    std::vector<AgentInterface*> frontAgents;
    for(auto coverageItem : frontLaneCoverages)
    {
        int laneId = coverageItem.first;
        auto agentInFront = GetAgentInFront(laneId);

        if(agentInFront != nullptr){
            frontAgents.push_back((AgentInterface*)agentInFront);
        }
    }
    return frontAgents;
}

bool AgentAdapter::ExistLaneLeft()
{
    OWL::CLanes& lanes = baseTrafficObject->GetLaneAssignments();

    // NOTE: checks first associated lane only
    if (lanes.size() > 0)
    {
        return lanes.front()->GetLeftLane().Exists();
    }

    return false;
}

bool AgentAdapter::ExistLaneRight()
{
    OWL::CLanes& lanes = baseTrafficObject->GetLaneAssignments();

    // NOTE: checks first associated lane only
    if (lanes.size() > 0)
    {
        return lanes.front()->GetRightLane().Exists();
    }

    return false;
}

void AgentAdapter::GetAgentsDirectlyInFront(double PeripheralPreviewDistance,
                                            AgentInterface *&agentFront,
                                            AgentInterface *&agentFrontLeft,
                                            AgentInterface *&agentFrontRight)
{
    Q_UNUSED(PeripheralPreviewDistance);
    Q_UNUSED(agentFront);
    Q_UNUSED(agentFrontLeft);
    Q_UNUSED(agentFrontRight);
}

double AgentAdapter::GetDistanceToEndOfLane(double sightDistance) const
{
    OWL::CLane& lane = worldData->GetLaneById(frontMainLaneId);

    // TODO: use lane position instead if road position
    double distance = lane.GetLength() - GetRoadPosition().s;

    if (distance > sightDistance)
    {
        return INFINITY;
    }
    else
    {
        return distance;
    }
}

void AgentAdapter::ObtainGroundTruthObjectLaneExistences(AreaOfInterest aoi,
                                                         AgentInterface *&agentAOI,
                                                         bool &hasRightLane,
                                                         bool &hasLeftLane,
                                                         double PreviewDistance,
                                                         double _carLengthEffective)
{
    Q_UNUSED(aoi);
    Q_UNUSED(agentAOI);
    Q_UNUSED(hasRightLane);
    Q_UNUSED(hasLeftLane);
    Q_UNUSED(PreviewDistance);
    Q_UNUSED(_carLengthEffective);
}

void AgentAdapter::SetCarInfo(CarInfo *carInfo)
{
    delete this->carInfo;
    this->carInfo = carInfo;
}

CarInfo *AgentAdapter::GetCarInfo() const
{
    return carInfo;
}

double AgentAdapter::GetDistanceToEndOfRamp(int laneId)
{
    Q_UNUSED(laneId);

    return INFINITY;
}

double AgentAdapter::GetPositionLateral() const
{
    return GetRoadPosition().t;
}

// don't take responsibility for extraInfo!
void AgentAdapter::SetCarInfoExtra(void *extraInfo)
{
    Q_UNUSED(extraInfo);
}

void* AgentAdapter::GetCarInfoExtra()
{
    return nullptr;
}

void AgentAdapter::AssignCarInfo(double accSensDist)
{
    CarInfo* carInfo = new CarInfo();
    SetCarInfo(carInfo);
}

void AgentAdapter::AssignCarInfoExtra()
{
}

double AgentAdapter::GetLaneDirection() const
{
    return GetYawAngle() + roadPos.hdg;
}

void AgentAdapter::Unregister() const
{
}

double AgentAdapter::GetLaneRemainder(Side side) const
{
    if (remainder.empty())
    {
        return 0.0;
    }

    if (side == Side::Left)
    {
        auto lRemainder = remainder.begin()->second;
        return std::get<1>(lRemainder.left) ? std::get<0>(lRemainder.left) : 0;
    }

    if (side == Side::Right)
    {
        auto rRemainder = remainder.rbegin()->second;
        return std::get<1>(rRemainder.right) ? std::get<0>(rRemainder.right) : 0;
    }

    throw std::invalid_argument("side not within valid bounds");
}
