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

#pragma once

#include <algorithm>
#include <qglobal.h>
#include "Interfaces/worldInterface.h"
#include "AgentNetwork.h"
#include "SceneryConverter.h"
#include "LocalizationCache.h"
#include "Interfaces/parameterInterface.h"

#include "WorldData.h"
#include "WorldDataQuery.h"

namespace osi3
{
class SensorView;
class SensorViewConfiguration;
}

struct WorldParameterOSI
{
    void Reset()
    {
        timeOfDay = "";
        visibilityDistance = 0;
        friction = 0.0;
        weather = "";
    }

    std::string timeOfDay {""};
    int visibilityDistance {0};
    double friction {0.0};
    std::string weather {""};
};

#include "osi/osi_groundtruth.pb.h"

/** \addtogroup World
* @{
* \brief implementation of a world interface
*
* The World module implements a WorldInterface which is used
* by the framework and the agents.
* It is used to set up a basic simulated world with static objects like
* roads, intersection and curves and dynamic objects like cars, bicycles and
* pedastrians.
*
* The World handles all simulated objects.
*
* \section world_inputs Inputs
* name | meaning
* -----|---------
* callbacks | CallbackInterface to send log information to the framework
*
* \section world_outputs Outputs
 name | meaning
* -----|---------
* *WoldInterface | Provides a reference to an implementation of a WorldInterface to the framework.
*
* \section world_configParameters Parameters to be specified in runConfiguration.xml
* tag | meaning
* -----|---------
* World | Name of World library. "World" by default.
*
* @see WorldInterface
*
* @} */

/*!
 * \brief Implementation of a WorldInterface
 *
 * This class implements all function of the WorldInterface. It is responsible for all
 * dynamic and static objects in a given scenery.
 *
 * \ingroup World
 */
class WorldImplementation : public WorldInterface
{
public:
    const std::string MODULENAME = "WORLD";

    WorldImplementation(const CallbackInterface* callbacks);
    WorldImplementation(const WorldImplementation&) = delete;
    WorldImplementation(WorldImplementation&&) = delete;
    WorldImplementation& operator=(const WorldImplementation&) = delete;
    WorldImplementation& operator=(WorldImplementation&&) = delete;

    virtual ~WorldImplementation() override;

    bool AddAgent(int id, AgentInterface* agent) override;
    AgentInterface* GetAgent(int id) const override;
    const std::vector<const WorldObjectInterface*>& GetWorldObjects() const;
    const std::map<int, AgentInterface *> &GetAgents() const override;
    const std::list<const AgentInterface*>& GetRemovedAgents() const;

    const std::vector<const TrafficObjectInterface*>& GetTrafficObjects() const;

    // framework internal methods to access members without restrictions
    void ExtractParameter(ParameterInterface* parameters);

    void Reset();
    void Clear() override;

    // model callbacks
    std::string GetTimeOfDay() const override;

    void* GetWorldData() override;
    void* GetOsiGroundTruth() override;

    void QueueAgentUpdate(std::function<void()> func) override;
    void QueueAgentRemove(const AgentInterface* agent) override;
    void SyncGlobalData() override;

    bool CreateScenery(SceneryInterface* scenery) override;

    AgentInterface* CreateAgentAdapterForAgent() override;

    AgentInterface* GetEgoAgent();

    AgentInterface* GetAgentByName(std::string& scenarioName);

    std::list<AgentInterface*> GetAgentsByGroupType(AgentCategory& agentCategory);

    // Agent functions
    AgentInterface* GetNextAgentInLane(std::string roadId, int laneId, double currentDistance) const override;
    AgentInterface* GetLastAgentInLane(std::string roadId, int laneId, double currentDistance) const override;
    AgentInterface* GetClosestAgentInUpstream(std::string roadId, int laneId, double initialSearchDistance) const override;
    AgentInterface* GetFarthestAgentInUpstream(std::string roadId, int laneId, double initialSearchDistance) const override;

    // Obstacle functions
    TrafficObjectInterface* GetNextTrafficObjectInLane(std::string roadId, int laneId,
            double currentDistance) const override;
    TrafficObjectInterface* GetLastTrafficObjectInLane(std::string roadId, int laneId,
            double currentDistance) const override;
    TrafficObjectInterface* GetClosestTrafficObjectInUpstream(std::string roadId, int laneId,
            double currentDistance) const override;
    TrafficObjectInterface* GetFarthestTrafficObjectInUpstream(std::string roadId, int laneId,
            double currentDistance) const override;

    // Generic functions
    WorldObjectInterface* GetNextObjectInLane(std::string roadId, int laneId, double currentDistance) const override;
    WorldObjectInterface* GetNextObjectInLane(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const override;
    WorldObjectInterface* GetLastObjectInLane(std::string roadId, int laneId, double currentDistance) const override;
    WorldObjectInterface* GetLastObjectInLane(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const override;
    WorldObjectInterface* GetClosestObjectInUpstream(std::string roadId, int laneId, double currentDistance) const override;
    WorldObjectInterface* GetClosestObjectInUpstream(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const override;
    WorldObjectInterface* GetFarthestObjectInUpstream(std::string roadId, int laneId,
            double currentDistance) const override;

    WorldObjectInterface* GetFirstObjectDownstream(uint64_t streamId) const override;
    int GetLaneId(uint64_t streamId, double endDistance) const;

    LaneQueryResult QueryLane(std::string roadId, int laneId, double distance) const;
    std::list<LaneQueryResult> QueryLanes(std::string roadId, double startDistance, double endDistance) const;

    std::vector<const AgentInterface*> GetAgentsInRange(std::string roadId, int laneId, double startDistance,
            double endDistance) const override;
    std::vector<const WorldObjectInterface*> GetObjectsInRange(std::string roadId, int laneId, double startDistance,
            double endDistance) const override;

    Position GetPositionByDistanceAndLane(double distanceOnLane, double offset, std::string roadId,
                                          int laneId) const override;

    std::vector<int> GetDrivingLanesAtDistance(std::string roadId, double distance) override;
    std::vector<int> GetStopLanesAtDistance(std::string roadId, double distance) override;
    std::vector<int> GetExitLanesAtDistance(std::string roadId, double distance) override;
    std::vector<int> GetRampsAtDistance(std::string roadId, double distance) override;

    bool GetLastValidSOnLane(std::string roadId, int laneId, double distance, double& nextValidS) override;
    bool GetNextValidSOnLane(std::string roadId, int laneId, double distance, double& nextValidS) override;
    bool IsSValidOnLane(std::string roadId, int laneId, double distance) override;
    bool ExistsLaneLeft(std::string roadId, int laneId, double distance) override;
    bool ExistsLaneRight(std::string roadId, int laneId, double distance) override;
    int GetNumberOfLanes(std::string roadId, double distance) override;


    double GetLaneCurvature(std::string roadId, int laneId, double distance) const override;
    double GetLaneWidth(std::string roadId, int laneId, double distance) const override;
    double GetLaneDirection(std::string roadId, int laneId, double distance) const override;

    double GetDistanceToEndOfLane(std::string roadId, int laneId, double initialSearchDistance,
                                  double maximumSearchLength) override;
    double GetDistanceToEndOfDrivingLane(std::string roadId, int laneId, double initialSearchDistance,
                                         double maximumSearchLength) override;
    double GetDistanceToEndOfDrivingOrStopLane(std::string roadId, int laneId, double initialSearchDistance,
            double maximumSearchLength) override;
    double GetDistanceToEndOfRamp(std::string roadId, int laneId, double initialSearchDistance,
                                  double maximumSearchLength) override;
    double GetDistanceToEndOfExit(std::string roadId, int laneId, double initialSearchDistance,
                                  double maximumSearchLength) override;

    bool IntersectsWithAgent(double x, double y, double rotation, double length, double width, double center) override;

    polygon_t GetBoundingBoxAroundAgent(AgentInterface* agent, double width, double length) override;

    Position RoadCoord2WorldCoord(RoadPosition roadCoord, std::string roadID) const override;

    double GetVisibilityDistance() const override;

    std::pair<bool, double> GetLateralDistance(GlobalRoadPosition src, GlobalRoadPosition dst) const override;

    std::vector<CommonTrafficSign::Entity> GetTrafficSignsInRange(std::string roadId, int laneId, double startDistance,
            double searchRange) const override;

    double GetFriction() const override;

    virtual void *GetGlobalDrivingView() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void *GetGlobalObjects() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetTimeOfDay(int timeOfDay) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetWeekday(Weekday weekday) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual Weekday GetWeekday() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void SetParameter(WorldParameter *worldParameter) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool CreateGlobalDrivingView() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const AgentInterface *GetSpecialAgent() override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const AgentInterface *GetLastCarInlane(int laneNumber) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual const AgentInterface *GetBicycle() const override
    {
        throw std::runtime_error("not implemented");
    }
    virtual void QueueAgentUpdate(std::function<void(double)> func,
                                  double val) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool CreateWorldScenery(const  std::string &sceneryFilename) override
    {
        throw std::runtime_error("not implemented");
    }
    virtual bool CreateWorldScenario(const  std::string &scenarioFilename) override
    {
        throw std::runtime_error("not implemented");
    }
protected:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char* file,
             int line,
             const std::string& message)
    {
        if (callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    void InitTrafficObjects();

    LaneQueryResult BuildLaneQueryResult(OWL::CLane& lane) const;

    OWL::WorldData worldData;
    WorldDataQuery worldDataQuery{worldData};
    std::vector<const TrafficObjectInterface*> trafficObjects;

    // world parameters
    WorldParameterOSI worldParameter;

    AgentNetwork agentNetwork;

    const CallbackInterface* callbacks;

    mutable std::vector<const WorldObjectInterface*> worldObjects;
    std::map<AgentInterface*, AgentAdapter*> agentList;

    const int stepSizeLookingForValidS = 100;
    SceneryInterface* scenery;

    std::unordered_map<const OWL::Interfaces::MovingObject*, AgentInterface*> movingObjectMapping{{nullptr, nullptr}};
    std::unordered_map<const OWL::Interfaces::MovingObject*, TrafficObjectInterface*> stationaryObjectMapping{{nullptr, nullptr}};

    World::Localization::Cache localizationCache;
};

LaneQueryResult LaneQueryResultFromLane(OWL::CLane& lane, int laneId);
