/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  World.h
//! @brief This file includes the static and dynamic objects used during a
//!        simulation run.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/worldInterface.h"
#include "worldBinding.h"

namespace SimulationSlave {

class World : public WorldInterface
{
public:
    World(WorldBinding* worldBinding) :
        worldBinding(worldBinding)
    {}

    World(const World&) = delete;
    World(World&&) = delete;
    World& operator=(const World&) = delete;
    World& operator=(World&&) = delete;

    ~World() = default;

    bool AddAgent(int id, AgentInterface* agent) override
    {
        return implementation->AddAgent(id, agent);
    }

    AgentInterface* GetAgent(int id) const override
    {
        return implementation->GetAgent(id);
    }

    AgentInterface* GetAgentByName(std::string& scenarioName) override
    {
        return implementation->GetAgentByName(scenarioName);
    }

    std::list<AgentInterface*> GetAgentsByGroupType(AgentCategory& agentCategory) override
    {
        return implementation->GetAgentsByGroupType(agentCategory);
    }

    const std::vector<const WorldObjectInterface*>& GetWorldObjects() const override
    {
        return implementation->GetWorldObjects();
    }

    const std::map<int, AgentInterface*>& GetAgents() const override
    {
        return implementation->GetAgents();
    }

    const std::vector<const TrafficObjectInterface*>& GetTrafficObjects() const override
    {
        return implementation->GetTrafficObjects();
    }

    const std::list<const AgentInterface*>& GetRemovedAgents() const override
    {
        return implementation->GetRemovedAgents();
    }

    // framework internal methods to access members without restrictions
    void ExtractParameter(ParameterInterface* parameters) override
    {
        return implementation->ExtractParameter(parameters);
    }

    void Reset() override
    {
        return implementation->Reset();
    }

    void Clear() override
    {
        return implementation->Clear();
    }

    std::string GetTimeOfDay() const override
    {
        return implementation->GetTimeOfDay();
    }

    double GetVisibilityDistance() const override
    {
        return implementation->GetVisibilityDistance();
    }

    double GetFriction() const override
    {
        return implementation->GetFriction();
    }

    void* GetOsiGroundTruth() override
    {
        return implementation->GetOsiGroundTruth();
    }

    void* GetWorldData() override
    {
        return implementation->GetWorldData();
    }

    void QueueAgentUpdate(std::function<void()> func) override
    {
        return implementation->QueueAgentUpdate(func);
    }

    void QueueAgentRemove(const AgentInterface* agent) override
    {
        return implementation->QueueAgentRemove(agent);
    }

    void SyncGlobalData() override
    {
        return implementation->SyncGlobalData();
    }

    bool CreateScenery(SceneryInterface* scenery) override
    {
        return implementation->CreateScenery(scenery);
    }

    AgentInterface* CreateAgentAdapterForAgent() override
    {
        return implementation->CreateAgentAdapterForAgent();
    }

    AgentInterface* GetEgoAgent() override
    {
        return implementation->GetEgoAgent();
    }


    // Agent functions (moving)
    AgentInterface* GetNextAgentInLane(std::string roadId, int laneId, double currentDistance) const override
    {
        return implementation->GetNextAgentInLane(roadId, laneId, currentDistance);
    }

    AgentInterface* GetLastAgentInLane(std::string roadId, int laneId, double currentDistance) const override
    {
        return implementation->GetLastAgentInLane(roadId, laneId, currentDistance);
    }

    AgentInterface* GetClosestAgentInUpstream(std::string roadId, int laneId, double initialSearchDistance) const override
    {
        return implementation->GetClosestAgentInUpstream(roadId, laneId, initialSearchDistance);
    }

    AgentInterface* GetFarthestAgentInUpstream(std::string roadId, int laneId, double initialSearchDistance) const override
    {
        return implementation->GetFarthestAgentInUpstream(roadId, laneId, initialSearchDistance);
    }

    // Obstacle functions (stationary)
    TrafficObjectInterface* GetNextTrafficObjectInLane(std::string roadId, int laneId,
            double currentDistance) const override
    {
        return implementation->GetNextTrafficObjectInLane(roadId, laneId, currentDistance);
    }

    TrafficObjectInterface* GetLastTrafficObjectInLane(std::string roadId, int laneId,
            double currentDistance) const override
    {
        return implementation->GetLastTrafficObjectInLane(roadId, laneId, currentDistance);
    }

    TrafficObjectInterface* GetClosestTrafficObjectInUpstream(std::string roadId, int laneId,
            double initialSearchDistance) const override
    {
        return implementation->GetClosestTrafficObjectInUpstream(roadId, laneId, initialSearchDistance);
    }

    TrafficObjectInterface* GetFarthestTrafficObjectInUpstream(std::string roadId, int laneId,
            double initialSearchDistance) const override
    {
        return implementation->GetFarthestTrafficObjectInUpstream(roadId, laneId, initialSearchDistance);
    }

    //Object funtions (generic)
    WorldObjectInterface* GetNextObjectInLane(std::string roadId, int laneId, double currentDistance) const override
    {
        return implementation->GetNextObjectInLane(roadId, laneId, currentDistance);
    }

    WorldObjectInterface* GetNextObjectInLane(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const override
    {
        return implementation->GetNextObjectInLane(roadId, laneId, currentDistance, searchDistance);
    }

    WorldObjectInterface* GetLastObjectInLane(std::string roadId, int laneId, double currentDistance) const override
    {
        return implementation->GetLastObjectInLane(roadId, laneId, currentDistance);
    }

    WorldObjectInterface* GetLastObjectInLane(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const override
    {
        return implementation->GetLastObjectInLane(roadId, laneId, currentDistance, searchDistance);
    }

    WorldObjectInterface* GetClosestObjectInUpstream(std::string roadId, int laneId,
            double initialSearchDistance) const override
    {
        return implementation->GetClosestObjectInUpstream(roadId, laneId, initialSearchDistance);
    }

    WorldObjectInterface* GetClosestObjectInUpstream(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const override
    {
        return implementation->GetClosestObjectInUpstream(roadId, laneId, currentDistance, searchDistance);
    }

    WorldObjectInterface* GetFarthestObjectInUpstream(std::string roadId, int laneId,
            double initialSearchDistance) const override
    {
        return implementation->GetFarthestObjectInUpstream(roadId, laneId, initialSearchDistance);
    }

    WorldObjectInterface* GetFirstObjectDownstream(uint64_t streamId) const override
    {
        return implementation->GetFirstObjectDownstream(streamId);
    }

    std::vector<const AgentInterface*> GetAgentsInRange(std::string roadId, int laneId, double startDistance,
            double endDistance) const override
    {
        return implementation->GetAgentsInRange(roadId, laneId, startDistance, endDistance);
    }

    std::vector<const WorldObjectInterface*> GetObjectsInRange(std::string roadId, int laneId, double startDistance,
            double endDistance) const override
    {
        return implementation->GetObjectsInRange(roadId, laneId, startDistance, endDistance);
    }

    Position GetPositionByDistanceAndLane(double distance, double offset, std::string roadId, int laneId) const override
    {
        return implementation->GetPositionByDistanceAndLane(distance, offset, roadId, laneId);
    }

    bool GetNextValidSOnLane(std::string roadId, int laneId, double distance, double& next) override
    {
        return implementation->GetNextValidSOnLane(roadId, laneId, distance, next);
    }

    bool GetLastValidSOnLane(std::string roadId, int laneId, double distance, double& last) override
    {
        return implementation->GetLastValidSOnLane(roadId, laneId, distance, last);
    }

    bool IsSValidOnLane(std::string roadId, int laneId, double distance) override
    {
        return implementation->IsSValidOnLane(roadId, laneId, distance);
    }

    bool ExistsLaneLeft(std::string roadId, int laneId, double distance) override
    {
        return implementation->ExistsLaneLeft(roadId, laneId, distance);
    }

    bool ExistsLaneRight(std::string roadId, int laneId, double distance) override
    {
        return implementation->ExistsLaneRight(roadId, laneId, distance);
    }

    int GetNumberOfLanes(std::string roadId, double distance) override
    {
        return implementation->GetNumberOfLanes(roadId, distance);
    }

    double GetLaneCurvature(std::string roadId, int laneId, double distance) const override
    {
        return implementation->GetLaneCurvature(roadId, laneId, distance);
    }

    double GetLaneWidth(std::string roadId, int laneId, double distance) const override
    {
        return implementation->GetLaneWidth(roadId, laneId, distance);
    }

    double GetLaneDirection(std::string roadId, int laneId, double distance) const override
    {
        return implementation->GetLaneDirection(roadId, laneId, distance);
    }

    double GetDistanceToEndOfLane(std::string roadId, int laneId, double initialSearchDistance,
                                  double maxSearchLength) override
    {
        return implementation->GetDistanceToEndOfLane(roadId, laneId, initialSearchDistance, maxSearchLength);
    }

    double GetDistanceToEndOfDrivingLane(std::string roadId, int laneId, double initialSearchDistance,
                                         double maxSearchLength) override
    {
        return implementation->GetDistanceToEndOfDrivingLane(roadId, laneId, initialSearchDistance, maxSearchLength);
    }

    double GetDistanceToEndOfDrivingOrStopLane(std::string roadId, int laneId, double initialSearchDistance,
            double maxSearchLength) override
    {
        return implementation->GetDistanceToEndOfDrivingOrStopLane(roadId, laneId, initialSearchDistance, maxSearchLength);
    }

    double GetDistanceToEndOfRamp(std::string roadId, int laneId, double initialSearchDistance,
                                  double maxSearchLength) override
    {
        return implementation->GetDistanceToEndOfRamp(roadId, laneId, initialSearchDistance, maxSearchLength);
    }

    double GetDistanceToEndOfExit(std::string roadId, int laneId, double initialSearchDistance,
                                  double maxSearchLength) override
    {
        return implementation->GetDistanceToEndOfExit(roadId, laneId, initialSearchDistance, maxSearchLength);
    }

    int GetLaneId(uint64_t streamId, double endDistance) const override
    {
        return implementation->GetLaneId(streamId, endDistance);
    }

    LaneQueryResult QueryLane(std::string roadId, int laneId, double distance) const override
    {
        return implementation->QueryLane(roadId, laneId, distance);
    }

    std::list<LaneQueryResult> QueryLanes(std::string roadId, double startDistance, double endDistance) const override
    {
        return implementation->QueryLanes(roadId, startDistance, endDistance);
    }

    bool IntersectsWithAgent(double x, double y, double rotation, double length, double width, double center) override
    {
        return implementation->IntersectsWithAgent(x, y, rotation, length, width, center);
    }

    Position RoadCoord2WorldCoord(RoadPosition roadCoord, std::string roadID = "") const override
    {
        return implementation->RoadCoord2WorldCoord(roadCoord, roadID);
    }

    //-----------------------------------------------------------------------------
    //! Instantiate the world by creating a WorldInterface out of a world library
    //! with given world parameters
    //! @param[in]  worldParameter  parameters of world inclusive library path
    //!
    //! @return                true if successful
    //-----------------------------------------------------------------------------
    bool Instantiate()
    {
        if (!worldBinding)
        {
            return false;
        }
        else
            if (!implementation)
            {
                implementation = worldBinding->Instantiate();
                if (!implementation)
                {
                    return false;
                }
            }
        return true;
    }

    //-----------------------------------------------------------------------------
    //! Returns true if world was already instantiated
    //!
    //! @return                true if world was already instantiated
    //-----------------------------------------------------------------------------
    bool isInstantiated()
    {
        return implementation;
    }


    std::vector<int> GetDrivingLanesAtDistance(std::string roadId, double distance) override
    {
        return implementation->GetDrivingLanesAtDistance(roadId, distance);
    }

    std::vector<int> GetStopLanesAtDistance(std::string roadId, double distance) override
    {
        return implementation->GetStopLanesAtDistance(roadId, distance);
    }

    std::vector<int> GetExitLanesAtDistance(std::string roadId, double distance) override
    {
        return implementation->GetExitLanesAtDistance(roadId, distance);
    }

    std::vector<int> GetRampsAtDistance(std::string roadId, double distance) override
    {
        return implementation->GetRampsAtDistance(roadId, distance);
    }

    virtual polygon_t GetBoundingBoxAroundAgent(AgentInterface* agent, double width, double length)
    {
        return implementation->GetBoundingBoxAroundAgent(agent, width, length);
    }


    virtual std::vector<CommonTrafficSign::Entity> GetTrafficSignsInRange(std::string roadId, int laneId,
            double startDistance, double searchRange) const override
    {
        return implementation->GetTrafficSignsInRange(roadId, laneId, startDistance, searchRange);
    }

    virtual std::pair<bool, double> GetLateralDistance(GlobalRoadPosition src, GlobalRoadPosition dst) const override
    {
        return implementation->GetLateralDistance(src, dst);
    }
    virtual void *GetGlobalDrivingView() override
    {
        return implementation->GetGlobalDrivingView();
    }
    virtual void *GetGlobalObjects() override
    {
        return implementation->GetGlobalObjects();
    }
    virtual void SetTimeOfDay(int timeOfDay) override
    {
        return implementation->SetTimeOfDay(timeOfDay);
    }
    virtual void SetWeekday(Weekday weekday) override
    {
        return implementation->SetWeekday(weekday);
    }
    virtual Weekday GetWeekday() const override
    {
        return implementation->GetWeekday();
    }
    virtual void SetParameter(WorldParameter *worldParameter) override
    {
        return implementation->SetParameter(worldParameter);
    }
    virtual bool CreateGlobalDrivingView() override
    {
        return implementation->CreateGlobalDrivingView();
    }
    virtual const AgentInterface *GetSpecialAgent() override
    {
        return implementation->GetSpecialAgent();
    }
    virtual const AgentInterface *GetLastCarInlane(int laneNumber) override
    {
        return implementation->GetLastCarInlane(laneNumber);
    }
    virtual const AgentInterface *GetBicycle() const override
    {
        return implementation->GetBicycle();
    }
    virtual void QueueAgentUpdate(std::function<void(double)> func,
                                  double val) override
    {
        return implementation->QueueAgentUpdate(func, val);
    }
    virtual bool CreateWorldScenery(const std::string &sceneryFilename) override
    {
        return implementation->CreateWorldScenery(sceneryFilename);
    }
    virtual bool CreateWorldScenario(const std::string &scenarioFilename) override
    {
        return implementation->CreateWorldScenario(scenarioFilename);
    }

private:
    WorldBinding* worldBinding = nullptr;
    WorldInterface* implementation = nullptr;
};

} // namespace SimulationSlave


