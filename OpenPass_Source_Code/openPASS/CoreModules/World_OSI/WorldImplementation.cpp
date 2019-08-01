/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "WorldData.h"
#include "WorldImplementation.h"

#include "osi/osi_sensorview.pb.h"
#include "osi/osi_sensorviewconfiguration.pb.h"

WorldImplementation::WorldImplementation(const CallbackInterface* callbacks):
    agentNetwork(this, callbacks),
    callbacks(callbacks)
{}

WorldImplementation::~WorldImplementation()
{
}

bool WorldImplementation::AddAgent(int id, AgentInterface* agent)
{
    if (agentNetwork.AddAgent(id, agent))
    {
        worldObjects.push_back(agent);
        return true;
    }

    return false;
}

AgentInterface *WorldImplementation::GetAgent(int id) const
{
    return agentNetwork.GetAgent(id);
}

const std::vector<const WorldObjectInterface*>& WorldImplementation::GetWorldObjects() const
{
    return worldObjects;
}

const std::map<int, AgentInterface *> &WorldImplementation::GetAgents() const
{
    return agentNetwork.GetAgents();
}

const std::list<const AgentInterface*>& WorldImplementation::GetRemovedAgents() const
{
    return agentNetwork.GetRemovedAgents();
}

const std::vector<const TrafficObjectInterface*>& WorldImplementation::GetTrafficObjects() const
{
    return trafficObjects;
}

void WorldImplementation::ExtractParameter(ParameterInterface* parameters)
{
    auto intParameter = parameters->GetParametersInt();
    auto doubleParameter = parameters->GetParametersDouble();
    auto stringParameter = parameters->GetParametersString();

    worldParameter.timeOfDay = stringParameter.at("TimeOfDay");
    worldParameter.visibilityDistance = intParameter.at("VisibilityDistance");
    worldParameter.friction = doubleParameter.at("Friction");
    worldParameter.weather = stringParameter.at("Weather");
}

void WorldImplementation::Reset()
{
    auto lanes = worldData.GetLanes();
    for (auto lane : lanes)
    {
        lane.second->ClearMovingObjects();
    }
    worldData.Reset();
    worldParameter.Reset();
    agentNetwork.Clear();
    worldObjects.clear();
    worldObjects.insert(worldObjects.end(), trafficObjects.begin(), trafficObjects.end());
    localizationCache.Clear();
}

void WorldImplementation::Clear()
{
    for (auto worldObject : worldObjects)
    {
        delete worldObject;
    }
}

void* WorldImplementation::GetWorldData()
{
    return &worldData;
}

void* WorldImplementation::GetOsiGroundTruth()
{
    //    return &groundTruth;
    return nullptr;
}

void WorldImplementation::QueueAgentUpdate(std::function<void()> func)
{
    agentNetwork.QueueAgentUpdate(func);
}

void WorldImplementation::QueueAgentRemove(const AgentInterface* agent)
{
    agentNetwork.QueueAgentRemove(agent);

    auto it = std::find(worldObjects.begin(), worldObjects.end(), agent);
    if (it != worldObjects.end())
    {
        worldObjects.erase(it);
    }
}

void WorldImplementation::SyncGlobalData()
{
    auto lanes = worldData.GetLanes();
    for (auto lane : lanes)
    {
        lane.second->ClearMovingObjects();
    }
    agentNetwork.SyncGlobalData();
}

bool WorldImplementation::CreateScenery(SceneryInterface* scenery)
{
    this->scenery = scenery;

    SceneryConverter converter(scenery,
                               worldData,
                               localizationCache,
                               callbacks);
    if (converter.Convert())
    {
        InitTrafficObjects();
        return true;
    }

    return false;
}

AgentInterface* WorldImplementation::CreateAgentAdapterForAgent()
{
    AgentInterface* agentAdapter = new AgentAdapter(this, callbacks, localizationCache);

    return agentAdapter;
}

std::string WorldImplementation::GetTimeOfDay() const
{
    return worldParameter.timeOfDay;
}

double WorldImplementation::GetVisibilityDistance() const
{
    return worldParameter.visibilityDistance;
}

double WorldImplementation::GetFriction() const
{
    return worldParameter.friction;
}

std::vector<CommonTrafficSign::Entity> WorldImplementation::GetTrafficSignsInRange(std::string roadId, int laneId,
        double startDistance, double searchRange) const
{
    std::vector<CommonTrafficSign::Entity> foundSigns {};
    std::vector<OWL::Interfaces::TrafficSign*> signs = worldDataQuery.GetTrafficSignsInRange(roadId, laneId, startDistance,
            searchRange);

    for (auto& sign : signs)
    {
        CommonTrafficSign::Entity foundSign {};
        foundSign.type = sign->GetType();
        foundSign.value = sign->GetValue();
        foundSign.distanceToStartOfRoad = sign->GetS();
        foundSign.relativeDistance = sign->GetS() - startDistance;

        foundSigns.push_back(foundSign);
    }

    return foundSigns;
}

AgentInterface* WorldImplementation::GetEgoAgent()
{
    const std::map<int, AgentInterface*> agents = agentNetwork.GetAgents();

    for (auto iterator = agents.begin(); iterator != agents.end(); iterator++)
    {
        AgentInterface* agent = iterator->second;
        if (agent->IsEgoAgent())
        {
            return agent;
        }
    }

    return nullptr;
}

AgentInterface* WorldImplementation::GetAgentByName(std::string& scenarioName)
{
    const std::map<int, AgentInterface*> agents = agentNetwork.GetAgents();

    for (auto iterator = agents.begin(); iterator != agents.end(); iterator++)
    {
        AgentInterface* agent = iterator->second;
        if (agent->GetScenarioName() == (scenarioName))
        {
            return agent;
        }
    }

    return nullptr;
}

std::list<AgentInterface*> WorldImplementation::GetAgentsByGroupType(AgentCategory& agentCategory)
{
    const std::map<int, AgentInterface*> agents = agentNetwork.GetAgents();

    std::list<AgentInterface*> agentsWithGroupType;

    for (auto iterator = agents.begin(); iterator != agents.end(); iterator++)
    {
        AgentInterface* agent = iterator->second;
        if (agentCategory == AgentCategory::Any  || agent->GetAgentCategory() == agentCategory)
        {
            agentsWithGroupType.push_back(agent);
        }
    }

    return agentsWithGroupType;
}

//// Agent functions
AgentInterface* WorldImplementation::GetNextAgentInLane(std::string roadId, int laneId, double currentDistance) const
{
    auto movingObject = worldDataQuery.GetNextObjectInLane<OWL::Interfaces::MovingObject>(roadId,
                        static_cast<OWL::OdId>(laneId),
                        currentDistance,
                        OWL::EVENTHORIZON);

    if (!movingObject)
    {
        return nullptr;
    }

    return movingObject->GetLink<AgentInterface>();
}

AgentInterface* WorldImplementation::GetLastAgentInLane(std::string roadId, int laneId, double currentDistance) const
{
    auto movingObject = worldDataQuery.GetLastObjectInLane<OWL::Interfaces::MovingObject>(
                            roadId,
                            static_cast<OWL::OdId>(laneId),
                            currentDistance,
                            OWL::EVENTHORIZON);

    if (!movingObject)
    {
        return nullptr;
    }

    return movingObject->GetLink<AgentInterface>();
}

AgentInterface* WorldImplementation::GetClosestAgentInUpstream(std::string roadId, int laneId,
        double initialSearchDistance) const
{

    auto movingObject = worldDataQuery.GetClosestObjectInUpstream<OWL::Interfaces::MovingObject>(roadId,
                        static_cast<OWL::OdId>(laneId),
                        initialSearchDistance, OWL::EVENTHORIZON);

    if (!movingObject)
    {
        return nullptr;
    }
    return dynamic_cast<AgentInterface*>(movingObject);
}

AgentInterface* WorldImplementation::GetFarthestAgentInUpstream(std::string roadId, int laneId,
        double initialSearchDistance) const
{
    auto movingObject = worldDataQuery.GetFarthestObjectInUpstream<OWL::Interfaces::MovingObject>(roadId,
                        static_cast<OWL::OdId>(laneId), initialSearchDistance,
                        OWL::EVENTHORIZON);

    if (!movingObject)
    {
        return nullptr;
    }

    return movingObject->GetLink<AgentInterface>();
}

WorldObjectInterface* WorldImplementation::GetFirstObjectDownstream(uint64_t streamId) const
{
    auto worldObject = worldDataQuery.GetFirstObjectDownstream(streamId);
    return worldObject ? worldObject->GetLink<WorldObjectInterface>() : nullptr;
}

// Obstacle functions
TrafficObjectInterface* WorldImplementation::GetNextTrafficObjectInLane(std::string roadId, int laneId,
        double currentDistance) const
{
    auto stationaryObject = worldDataQuery.GetNextObjectInLane<OWL::Interfaces::StationaryObject>(
                                roadId,
                                static_cast<OWL::OdId>(laneId), currentDistance,
                                OWL::EVENTHORIZON);

    if (!stationaryObject)
    {
        return nullptr;
    }

    return stationaryObject->GetLink<TrafficObjectInterface>();
}

TrafficObjectInterface* WorldImplementation::GetLastTrafficObjectInLane(std::string roadId, int laneId,
        double currentDistance) const
{
    auto stationaryObject = worldDataQuery.GetLastObjectInLane<OWL::Interfaces::StationaryObject>(
                                roadId,
                                static_cast<OWL::OdId>(laneId),
                                currentDistance,
                                OWL::EVENTHORIZON);

    if (!stationaryObject)
    {
        return nullptr;
    }

    return stationaryObject->GetLink<TrafficObjectInterface>();
}

TrafficObjectInterface* WorldImplementation::GetClosestTrafficObjectInUpstream(std::string roadId, int laneId,
        double currentDistance) const
{
    auto stationaryObject = worldDataQuery.GetClosestObjectInUpstream<OWL::Interfaces::StationaryObject>
                            (roadId, static_cast<OWL::OdId>(laneId), currentDistance,
                             OWL::EVENTHORIZON);

    if (!stationaryObject)
    {
        return nullptr;
    }

    return stationaryObject->GetLink<TrafficObjectInterface>();
}

TrafficObjectInterface* WorldImplementation::GetFarthestTrafficObjectInUpstream(std::string roadId, int laneId,
        double currentDistance) const
{
    auto stationaryObject = worldDataQuery.GetFarthestObjectInUpstream<OWL::Interfaces::StationaryObject>
                            (roadId, static_cast<OWL::OdId>(laneId), currentDistance,
                             OWL::EVENTHORIZON);

    if (!stationaryObject)
    {
        return nullptr;
    }

    return stationaryObject->GetLink<TrafficObjectInterface>();
}

// Generic functions
WorldObjectInterface* WorldImplementation::GetNextObjectInLane(std::string roadId, int laneId,
        double currentDistance) const
{
    return GetNextObjectInLane(roadId, laneId, currentDistance, OWL::EVENTHORIZON);
}

WorldObjectInterface* WorldImplementation::GetNextObjectInLane(std::string roadId, int laneId, double currentDistance,
        double searchDistance) const
{
    auto worldObject = worldDataQuery.GetNextObjectInLane<OWL::Interfaces::WorldObject>(
                           roadId,
                           static_cast<OWL::OdId>(laneId),
                           currentDistance,
                           searchDistance);

    if (!worldObject)
    {
        return nullptr;
    }

    return worldObject->GetLink<WorldObjectInterface>();
}

WorldObjectInterface* WorldImplementation::GetLastObjectInLane(std::string roadId, int laneId,
        double currentDistance) const
{
    return GetLastObjectInLane(roadId, laneId, currentDistance, OWL::EVENTHORIZON);
}

WorldObjectInterface* WorldImplementation::GetLastObjectInLane(std::string roadId, int laneId, double currentDistance,
        double searchDistance) const
{
    auto worldObject = worldDataQuery.GetLastObjectInLane<OWL::Interfaces::WorldObject>(
                           roadId,
                           static_cast<OWL::OdId>(laneId),
                           currentDistance,
                           searchDistance);

    if (!worldObject)
    {
        return nullptr;
    }

    return worldObject->GetLink<WorldObjectInterface>();
}

WorldObjectInterface* WorldImplementation::GetClosestObjectInUpstream(std::string roadId, int laneId,
        double currentDistance) const
{
    return GetClosestObjectInUpstream(roadId, laneId, currentDistance, OWL::EVENTHORIZON);
}

WorldObjectInterface* WorldImplementation::GetClosestObjectInUpstream(std::string roadId, int laneId,
        double currentDistance, double searchDistance) const
{
    auto worldObject = worldDataQuery.GetClosestObjectInUpstream<OWL::Interfaces::WorldObject>(
                           roadId,
                           static_cast<OWL::OdId>(laneId),
                           currentDistance,
                           searchDistance);

    if (!worldObject)
    {
        return nullptr;
    }

    return worldObject->GetLink<WorldObjectInterface>();
}

WorldObjectInterface* WorldImplementation::GetFarthestObjectInUpstream(std::string roadId, int laneId,
        double currentDistance) const
{
    auto worldObject = worldDataQuery.GetFarthestObjectInUpstream<OWL::Interfaces::WorldObject>(roadId,
                       static_cast<OWL::OdId>
                       (laneId), currentDistance,
                       OWL::EVENTHORIZON);

    if (!worldObject)
    {
        return nullptr;
    }

    return worldObject->GetLink<WorldObjectInterface>();
}

std::pair<bool, double> WorldImplementation::GetLateralDistance(GlobalRoadPosition src, GlobalRoadPosition dst) const
{
    return worldDataQuery.GetLateralDistance(src, dst);
}

std::vector<const AgentInterface*> WorldImplementation::GetAgentsInRange(std::string roadId, int laneId,
        double startDistance,
        double endDistance) const
{
    auto movingObjects = worldDataQuery.GetMovingObjectsInRange(roadId, static_cast<OWL::OdId>(laneId), startDistance,
                         endDistance);
    std::vector<const AgentInterface*> agents;
    for (auto movingObject : movingObjects)
    {
        agents.push_back(movingObject->GetLink<AgentInterface>());
    }

    return agents;
}

std::vector<const WorldObjectInterface*> WorldImplementation::GetObjectsInRange(std::string roadId, int laneId,
        double startDistance,
        double endDistance) const
{
    auto worldObjects = worldDataQuery.GetWorldObjectsInRange(roadId, static_cast<OWL::OdId>(laneId), startDistance,
                        endDistance);
    std::vector<const WorldObjectInterface*> objects;
    for (auto worldObject : worldObjects)
    {
        objects.push_back(worldObject->GetLink<WorldObjectInterface>());
    }

    return objects;
}

Position WorldImplementation::GetPositionByDistanceAndLane(double distanceOnLane, double offset, std::string roadId,
        int laneId) const
{
    OWL::CLane& lane = worldDataQuery.GetLaneByOdId(roadId, laneId, distanceOnLane);
    const auto& referencePoint = lane.GetInterpolatedPointsAtDistance(distanceOnLane).reference;
    auto yaw = lane.GetDirection(distanceOnLane);

    return Position
    {
        referencePoint.x - std::sin(yaw) * offset,
        referencePoint.y + std::cos(yaw) * offset,
        yaw,
        lane.GetCurvature(distanceOnLane)
    };
}

bool WorldImplementation::GetNextValidSOnLane(std::string roadId, int laneId, double distance, double& nextValidS)
{
    nextValidS = worldDataQuery.GetNextValidSOnLaneInDownstream(roadId, laneId, distance, stepSizeLookingForValidS);

    if (nextValidS == INFINITY)
    {
        return false;
    }

    return true;
}

bool WorldImplementation::GetLastValidSOnLane(std::string roadId, int laneId, double distance, double& nextValidS)
{
    nextValidS = worldDataQuery.GetLastValidSInUpstream(roadId, laneId, distance, stepSizeLookingForValidS);

    if (nextValidS == -INFINITY)
    {
        return false;
    }

    return true;
}

bool WorldImplementation::IsSValidOnLane(std::string roadId, int laneId,
        double distance) //when necessary optional parameter with reference to get point
{
    return worldDataQuery.IsSValidOnLane(roadId, laneId, distance);
}

bool WorldImplementation::ExistsLaneLeft(std::string roadId, int laneId, double distance)
{
    return worldDataQuery.ExistsDrivingLaneOnSide(roadId, laneId, distance, Side::Left);
}

bool WorldImplementation::ExistsLaneRight(std::string roadId, int laneId, double distance)
{
    return worldDataQuery.ExistsDrivingLaneOnSide(roadId, laneId, distance, Side::Right);
}

int WorldImplementation::GetNumberOfLanes(std::string roadId, double distance)
{
    return worldDataQuery.GetNumberOfLanes(roadId, distance);
}

double WorldImplementation::GetLaneCurvature(std::string roadId, int laneId, double distance) const
{
    OWL::CLane& lane =  worldDataQuery.GetLaneByOdId(roadId, laneId, distance);
    return lane.GetCurvature(distance);
}

double WorldImplementation::GetLaneWidth(std::string roadId, int laneId, double distance) const
{
    const auto& lane = worldDataQuery.GetLaneByOdId(roadId, laneId, distance);
    return lane.GetWidth(distance);
}

double WorldImplementation::GetLaneDirection(std::string roadId, int laneId, double distance) const
{
    const auto& lane = worldDataQuery.GetLaneByOdId(roadId, laneId, distance);
    return lane.GetDirection(distance);
}

double WorldImplementation::GetDistanceToEndOfLane(std::string roadId, int laneId, double initialSearchDistance,
        double maximumSearchLength)
{
    return worldDataQuery.GetDistanceToEndOfLane(roadId, static_cast<OWL::OdId>(laneId), initialSearchDistance,
            maximumSearchLength, {OWL::LaneType::Driving, OWL::LaneType::Exit, OWL::LaneType::OnRamp, OWL::LaneType::OffRamp, OWL::LaneType::Stop});
}

double WorldImplementation::GetDistanceToEndOfDrivingLane(std::string roadId, int laneId, double initialSearchDistance,
        double maximumSearchLength)
{
    return worldDataQuery.GetDistanceToEndOfLane(roadId, static_cast<OWL::OdId>(laneId), initialSearchDistance,
            maximumSearchLength, {OWL::LaneType::Driving});
}

double WorldImplementation::GetDistanceToEndOfDrivingOrStopLane(std::string roadId, int laneId,
        double initialSearchDistance, double maximumSearchLength)
{
    return worldDataQuery.GetDistanceToEndOfLane(roadId, static_cast<OWL::OdId>(laneId), initialSearchDistance,
            maximumSearchLength, {OWL::LaneType::Driving, OWL::LaneType::Stop});
}

double WorldImplementation::GetDistanceToEndOfRamp(std::string roadId, int laneId, double initialSearchDistance,
        double maximumSearchLength)
{
    return worldDataQuery.GetDistanceToEndOfLane(roadId, static_cast<OWL::OdId>(laneId), initialSearchDistance,
            maximumSearchLength, {OWL::LaneType::OnRamp, OWL::LaneType::OffRamp});
}

double WorldImplementation::GetDistanceToEndOfExit(std::string roadId, int laneId, double initialSearchDistance,
        double maximumSearchLength)
{
    return worldDataQuery.GetDistanceToEndOfLane(roadId, static_cast<OWL::OdId>(laneId), initialSearchDistance,
            maximumSearchLength, {OWL::LaneType::Exit});
}

std::vector<int> WorldImplementation::GetDrivingLanesAtDistance(std::string roadId, double distance)
{
    std::vector<int> result;
    const auto drivingLanes = worldDataQuery.GetLanesOfLaneTypeAtDistance(roadId, distance, {OWL::LaneType::Driving});

    for (const auto lane : drivingLanes)
    {
        OWL::OdId laneId = worldData.GetLaneIdMapping().at(lane->GetId());
        result.push_back(static_cast<int>(laneId));
    }

    return result;
}

std::vector<int> WorldImplementation::GetStopLanesAtDistance(std::string roadId, double distance)
{
    std::vector<int> result;
    const auto stopLanes = worldDataQuery.GetLanesOfLaneTypeAtDistance(roadId, distance, {OWL::LaneType::Stop});

    for (const auto lane : stopLanes)
    {
        OWL::OdId laneId = worldData.GetLaneIdMapping().at(lane->GetId());
        result.push_back(static_cast<int>(laneId));
    }

    return result;
}

std::vector<int> WorldImplementation::GetExitLanesAtDistance(std::string roadId, double distance)
{
    std::vector<int> result;
    const auto stopLanes = worldDataQuery.GetLanesOfLaneTypeAtDistance(roadId, distance, {OWL::LaneType::Exit});

    for (const auto lane : stopLanes)
    {
        OWL::OdId laneId = worldData.GetLaneIdMapping().at(lane->GetId());
        result.push_back(static_cast<int>(laneId));
    }

    return result;
}

std::vector<int> WorldImplementation::GetRampsAtDistance(std::string roadId, double distance)
{
    std::vector<int> result;
    const auto stopLanes = worldDataQuery.GetLanesOfLaneTypeAtDistance(roadId, distance, {OWL::LaneType::OnRamp, OWL::LaneType::OffRamp});

    for (const auto lane : stopLanes)
    {
        OWL::OdId laneId = worldData.GetLaneIdMapping().at(lane->GetId());
        result.push_back(static_cast<int>(laneId));
    }

    return result;
}

bool WorldImplementation::IntersectsWithAgent(double x, double y, double rotation, double length, double width,
        double center)
{

    polygon_t polyNewAgent = World::Localization::GetBoundingBox(x, y, length, width, rotation, center);

    for (std::pair<int, AgentInterface*> agent : GetAgents())
    {
        polygon_t polyAgent = agent.second->GetBoundingBox2D();


        bool intersects = bg::intersects(polyNewAgent, polyAgent);
        if (intersects)
        {
            return true;
        }
    }

    return false;
}

polygon_t WorldImplementation::GetBoundingBoxAroundAgent(AgentInterface* agent, double width, double length)
{
    double x = agent->GetPositionX();
    double y = agent->GetPositionY();
    double rotation =  agent->GetYaw();
    double center = length * 0.5;
    polygon_t poly = World::Localization::GetBoundingBox(x, y, length, width, rotation, center);
    return poly;
}

Position WorldImplementation::RoadCoord2WorldCoord(RoadPosition roadCoord, std::string roadID) const
{
    Position worldCoord;
    worldCoord.xPos = 0;
    worldCoord.yPos = 0;
    worldCoord.yawAngle = 0;

    RoadInterface* road = nullptr;

    if (roadID.empty())
    {
        if (!scenery->GetRoads().empty())
        {
            road = scenery->GetRoads().begin()->second;
        }
    }
    else
    {
        for (auto& item : scenery->GetRoads())
        {
            RoadInterface* itemRoad = item.second;
            if (roadID == itemRoad->GetId())
            {
                road = itemRoad;
                break;
            }
        }
    }

    if (!road)
    {
        const std::string msg = "Could not find road with ID " + roadID;
        throw std::runtime_error(msg);
    }

    std::list<RoadGeometryInterface*> roadGeometries = road->GetGeometries();

    for (RoadGeometryInterface* roadGeometry : roadGeometries)
    {
        double roadGeometryLength = roadGeometry->GetLength();
        double roadGeometryStart = roadGeometry->GetS();
        double roadGeometryEnd = roadGeometryStart + roadGeometryLength;

        if (roadCoord.s > roadGeometryStart && roadCoord.s < roadGeometryEnd)
        {
            double localS = roadCoord.s - roadGeometryStart;
            Common::Vector2d coord = roadGeometry->GetCoord(0, localS, 0, roadCoord.t, 0, 0);
            double dir = roadGeometry->GetDir(0, localS, 0, roadCoord.t, 0);
            worldCoord.xPos = coord.x;
            worldCoord.yPos = coord.y;
            worldCoord.yawAngle = dir + roadCoord.hdg;
            break;
        }
    }

    return worldCoord;
}

void WorldImplementation::InitTrafficObjects()
{
    assert(trafficObjects.size() == 0);

    for (auto& baseTrafficObject : worldData.GetStationaryObjects())
    {
        TrafficObjectInterface* trafficObject = baseTrafficObject.second->GetLink<TrafficObjectInterface>();
        trafficObjects.push_back(trafficObject);
        worldObjects.push_back(trafficObject);
    }
}

LaneQueryResult WorldImplementation::QueryLane(std::string roadId, int laneId, double distance) const
{
    return worldDataQuery.QueryLane(roadId, laneId, distance);
}

std::list<LaneQueryResult> WorldImplementation::QueryLanes(std::string roadId, double startDistance,
        double endDistance) const
{
    return worldDataQuery.QueryLanes(roadId, startDistance, endDistance);
}

int WorldImplementation::GetLaneId(uint64_t streamId, double endDistance) const
{
    return static_cast<int>(worldDataQuery.GetLaneId(streamId, endDistance));
}
