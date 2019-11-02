/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  worldInterface.h
//! @brief This file provides the interface of the scenery and dynamic objects
//!        to custom libraries.
//-----------------------------------------------------------------------------

#ifndef WORLDINTERFACE_H
#define WORLDINTERFACE_H

#include <map>
#include <functional>
#include <list>
#include "Common/globalDefinitions.h"
#include "Interfaces/agentInterface.h"
#include "Interfaces/trafficObjectInterface.h"
#include "Interfaces/sceneryInterface.h"
#include "Interfaces/callbackInterface.h"

class LaneQueryResult
{
public:
    static LaneQueryResult InvalidResult()
    {
        return LaneQueryResult(false);
    }

    LaneQueryResult(uint64_t streamId, double startDistance, double endDistance, LaneCategory laneCategory,
                    bool isDrivingLane) :
        valid{true},
        streamId{streamId},
        startDistance{startDistance},
        endDistance{endDistance},
        laneCategory{laneCategory},
        isDrivingLane{isDrivingLane}
    {}

    bool valid { false };
    uint64_t streamId { 0 };
    double startDistance { 0.0 };
    double endDistance { 0.0 };
    LaneCategory laneCategory { LaneCategory::Undefined };
    bool isDrivingLane { false };

private:
    LaneQueryResult(bool valid) : valid{valid}
    {
    }
};

//-----------------------------------------------------------------------------
//! Provides access to world representation
//-----------------------------------------------------------------------------
class WorldInterface
{
public:
    WorldInterface() = default;
    WorldInterface(const WorldInterface &) = delete;
    WorldInterface(WorldInterface &&) = delete;
    WorldInterface &operator=(const WorldInterface &) = delete;
    WorldInterface &operator=(WorldInterface &&) = delete;
    virtual ~WorldInterface() = default;


    virtual bool Instantiate() {return false;}

    virtual bool isInstantiated() {return false;}

    //-----------------------------------------------------------------------------
    //! Retrieves the OSI ground truth
    //!
    //! @return                global view
    //-----------------------------------------------------------------------------
    virtual void* GetOsiGroundTruth() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves global view on complete world (from which a sensor can retrieve
    //! arbitrary information)
    //!
    //! @return                global view
    //-----------------------------------------------------------------------------
    virtual void *GetGlobalDrivingView() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves static objects (roads and junctions)
    //!
    //! @return                reference to scenery
    //-----------------------------------------------------------------------------
    virtual void *GetGlobalObjects() = 0;

    //-----------------------------------------------------------------------------
    //! Sets time of day (hour) for the simulation run
    //!
    //! @return                time of day [0-23]
    //-----------------------------------------------------------------------------
    virtual void SetTimeOfDay(int timeOfDay) = 0;

    //-----------------------------------------------------------------------------
    //! Sets weekday for the simulation run
    //!
    //! @return                weekday
    //-----------------------------------------------------------------------------
    virtual void SetWeekday(Weekday weekday) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the OSI world data
    //!
    //! @return                Pointer OSI world data structure
    //-----------------------------------------------------------------------------
    virtual void* GetWorldData() = 0;
    
    //-----------------------------------------------------------------------------
    //! Retrieves time of day (hour)
    //!
    //! @return                time of day [0, 23]
    //-----------------------------------------------------------------------------
    virtual std::string GetTimeOfDay() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves weekday
    //!
    //! @return                weekday
    //-----------------------------------------------------------------------------
    virtual Weekday GetWeekday() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the visibility distance (meter)
    //!
    //! @return                visibility distance
    //-----------------------------------------------------------------------------
    virtual double GetVisibilityDistance() const = 0;

    //-----------------------------------------------------------------------------
    //! Sets the world parameters like weekday, library
    //! @param[in]     worldParamter  parameter to setup world
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SetParameter(WorldParameter *worldParameter) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the world parameters like weekday, library
    //! @param[in]     worldParamter  parameter to setup world
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void ExtractParameter(ParameterInterface* parameters) = 0;

    //-----------------------------------------------------------------------------
    //! Reset the world for the next run
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void Reset() = 0;

    //-----------------------------------------------------------------------------
    //! Clear the world, reset internal states
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void Clear() = 0;

    //-----------------------------------------------------------------------------
    //! Create internal global driving view
    //!
    //! @return                weekday
    //-----------------------------------------------------------------------------
    virtual bool CreateGlobalDrivingView() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves specific agent
    //! @param[in]  id         id of agent
    //!
    //! @return                Agent reference
    //-----------------------------------------------------------------------------
    virtual AgentInterface *GetAgent(int id) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves all agents
    //!
    //! @return                Mapping of ids to agents
    //-----------------------------------------------------------------------------
    virtual const std::map<int, AgentInterface *> &GetAgents() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves all worldObjects that currently exist
    //!
    //! @return                Vector of all worldObjects
    //-----------------------------------------------------------------------------
    virtual const std::vector<const WorldObjectInterface*>& GetWorldObjects() const = 0;

    //-----------------------------------------------------------------------------
    //! Add agent to world
    //! @param[in]  id         id of agent
    //! @param[in]  agent      agent reference
    //!
    //! @return                true if successful
    //-----------------------------------------------------------------------------
    virtual bool AddAgent(int id, AgentInterface *agent) = 0;

    //-----------------------------------------------------------------------------
    //! queue functions and values to update agent when SyncGlobalData is called
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void QueueAgentUpdate(std::function<void(double)> func,
                                  double val) = 0;

    //-----------------------------------------------------------------------------
    //! queue functions and values to update agent when SyncGlobalData is called
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void QueueAgentUpdate(std::function<void()> func) = 0;

    //-----------------------------------------------------------------------------
    //! Add agent to list to be removed when SyncGlobalData is called
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void QueueAgentRemove(const AgentInterface *agent) = 0;

    //-----------------------------------------------------------------------------
    //! Update all agents / Remove agents and sync data of all agents within world
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual void SyncGlobalData() = 0;

    //-----------------------------------------------------------------------------
    //! Create a scenery in world.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool CreateScenery(SceneryInterface *scenery) = 0;

    //-----------------------------------------------------------------------------
    //! Create an agentAdapter for an agent to communicate between the agent of the
    //! framework and the world.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual AgentInterface *CreateAgentAdapterForAgent() = 0;

    //-----------------------------------------------------------------------------
    //! Returns one agent which is set to be special.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetSpecialAgent() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the last car in lane. Return nullptr if no car is found.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetLastCarInlane(int laneNumber) = 0;

    //-----------------------------------------------------------------------------
    //! Returns an agentInterface which is a bicycle. Returns nullptr if no bicyle
    //! found.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual const AgentInterface *GetBicycle() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve the world postion by the distance and the lane.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual Position GetPositionByDistanceAndLane(double distanceOnLane, double offset, std::string roadId,
            int laneId) const = 0;

    //-----------------------------------------------------------------------------
    //! Tries to create an internal scenery from a given file.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool CreateWorldScenery(const  std::string &sceneryFilename) = 0;

    //-----------------------------------------------------------------------------
    //! Tries to create an internal scenario from a given file.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool CreateWorldScenario(const  std::string &scenarioFilename) = 0;


    // Agent functions
    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (downstream) and returns the first agent found.
    //! Returns nullptr if no agent is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return First agent downstream
    //-----------------------------------------------------------------------------
    virtual AgentInterface* GetNextAgentInLane(std::string roadId, int laneId, double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at given distance + maximum search distance (upstream) and returns the first agent found.
    //! Returns nullptr if no agent is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return Last agent downstream
    //-----------------------------------------------------------------------------
    virtual AgentInterface* GetLastAgentInLane(std::string roadId, int laneId, double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (upstream) and returns the first agent found.
    //! Returns nullptr if no agent is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return First agent upstream
    //-----------------------------------------------------------------------------
    virtual AgentInterface* GetClosestAgentInUpstream(std::string roadId, int laneId,
            double initialSearchDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at given distance - maximum search distance (downstream) and returns the first agent found.
    //! Returns nullptr if no agent is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return Last agent upstream
    //-----------------------------------------------------------------------------
    virtual AgentInterface* GetFarthestAgentInUpstream(std::string roadId, int laneId,
            double initialSearchDistance) const = 0;

    // Obstacle functions
    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (downstream) and returns the first traffic object found.
    //! Returns nullptr if no traffic object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return First traffic object downstream
    //-----------------------------------------------------------------------------
    virtual TrafficObjectInterface* GetNextTrafficObjectInLane(std::string roadId, int laneId,
            double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at given distance + maximum search distance (upstream) and returns the first traffic object found.
    //! Returns nullptr if no traffic object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return Last traffic object downstream
    //-----------------------------------------------------------------------------
    virtual TrafficObjectInterface* GetLastTrafficObjectInLane(std::string roadId, int laneId,
            double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (upstream) and returns the first traffic object found.
    //! Returns nullptr if no traffic object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return First traffic object upstream
    //-----------------------------------------------------------------------------
    virtual TrafficObjectInterface* GetClosestTrafficObjectInUpstream(std::string roadId, int laneId,
            double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at given distance - maximum search distance (downstream) and returns the first traffic object found.
    //! Returns nullptr if no traffic object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return Last traffic object upstream
    //-----------------------------------------------------------------------------
    virtual TrafficObjectInterface* GetFarthestTrafficObjectInUpstream(std::string roadId, int laneId,
            double currentDistance) const = 0;

    // Generic functions
    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (downstream) and returns the first object found.
    //! Returns nullptr if no object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return First object downstream
    //-----------------------------------------------------------------------------
    virtual WorldObjectInterface* GetNextObjectInLane(std::string roadId, int laneId, double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (downstream) and returns the first object found.
    //! Returns nullptr if no object is found.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @param[in] searchDistance maximum search distance
    //! @return First object downstream
    //-----------------------------------------------------------------------------
    virtual WorldObjectInterface* GetNextObjectInLane(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (upstream) and returns the first object found.
    //! Returns nullptr if no object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return First object upstream
    //-----------------------------------------------------------------------------
    virtual WorldObjectInterface* GetLastObjectInLane(std::string roadId, int laneId, double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (upstream) and returns the first object found.
    //! Returns nullptr if no object is found.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @param[in] searchDistance maximum search distance
    //! @return First object upstream
    //-----------------------------------------------------------------------------
    virtual WorldObjectInterface* GetLastObjectInLane(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (upstream) and returns the first object found.
    //! Returns nullptr if no object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return First object upstream
    //-----------------------------------------------------------------------------
    virtual WorldObjectInterface* GetClosestObjectInUpstream(std::string roadId, int laneId,
            double currentDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at the given distance (upstream) and returns the first object found.
    //! Returns nullptr if no object is found.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @param[in] searchDistance maximum search distance
    //! @return First object upstream
    //-----------------------------------------------------------------------------
    virtual WorldObjectInterface* GetClosestObjectInUpstream(std::string roadId, int laneId, double currentDistance,
            double searchDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Searches the lane starting at given distance - maximum search distance (downstream) and returns the first object found.
    //! Returns nullptr if no object is found.
    //! Internal OWL::EVENTHORIZON is used as maximum search distance.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId   OpenDriveId of lane to search in
    //! @param[in] currentDistance start s coordinate
    //! @return Last object upstream
    //-----------------------------------------------------------------------------
    virtual WorldObjectInterface* GetFarthestObjectInUpstream(std::string roadId, int laneId,
            double currentDistance) const = 0;

    virtual WorldObjectInterface* GetFirstObjectDownstream(uint64_t streamId) const = 0;

    //-----------------------------------------------------------------------------
    //! Returns all agents in specified range (also agents partially in search interval).
    //! Returns empty list otherwise.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] startDistance  lower bound of search interval (s coordinate)
    //! @param[in] endDistance  upper bound of search interval (s coordinate)
    //!
    //! @return All agents in specified range
    //-----------------------------------------------------------------------------
    virtual std::vector<const AgentInterface*> GetAgentsInRange(std::string roadId, int laneId, double startDistance,
            double endDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Returns all objects in specified range (also objects partially in search interval).
    //! Returns empty list otherwise.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] startDistance  lower bound of search interval (s coordinate)
    //! @param[in] endDistance  upper bound of search interval (s coordinate)
    //!
    //! @return All objects in specified range
    //-----------------------------------------------------------------------------
    virtual std::vector<const WorldObjectInterface*> GetObjectsInRange(std::string roadId, int laneId, double startDistance,
            double endDistance) const = 0;
    //-----------------------------------------------------------------------------
    //! Returns ids of driving lanes at given distance, empty vector otherwise.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] distance  s coordinate
    //! @return Ids of lanes with type driving at given distance
    //-----------------------------------------------------------------------------
    virtual std::vector<int> GetDrivingLanesAtDistance(std::string roadId, double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Returns ids of driving lanes at given distance, empty vector otherwise.
    //!
    //! @param[in] roadId  OpenDriveId of the road to look in
    //! @param[in] distance  s coordinate
    //! @return Ids of lanes with type stop at given distance
    //-----------------------------------------------------------------------------
    virtual std::vector<int> GetStopLanesAtDistance(std::string roadId, double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Returns ids of exit lanes at given distance, empty vector otherwise.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] distance  s coordinate
    //! @return Ids of lanes with type stop at given distance
    //-----------------------------------------------------------------------------
    virtual std::vector<int> GetExitLanesAtDistance(std::string roadId, double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Returns ids of onRamps and offRamps at given distance, empty vector otherwise.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] distance  s coordinate
    //! @return Ids of lanes with type stop at given distance
    //-----------------------------------------------------------------------------
    virtual std::vector<int> GetRampsAtDistance(std::string roadId, double distance) = 0;
    //-----------------------------------------------------------------------------
    //! Retrieve a valid s coordinate on given lane (downstream)
    //! Internal a step-size is used which is set to 100
    //! Internal a max. search distance is used (OWL::Eventhorizon)
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  start s coordinate
    //! @param[out] next next valid s coordinate, INFINIT if valid s is not found
    //! @return true if valid s is found in search range, false otherwise
    //-----------------------------------------------------------------------------
    virtual bool GetNextValidSOnLane(std::string roadId, int laneId, double distance, double& next) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve a valid s coordinate on given lane (upstream)
    //! Internal a step-size is used which is set to 100
    //! Internal a max. search distance is used (OWL::Eventhorizon)
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  start s coordinate
    //! @param[out] last last valid s coordinate, -INFINIT if valid s is not found
    //! @return true if valid s is found in search range, false otherwise
    //-----------------------------------------------------------------------------
    virtual bool GetLastValidSOnLane(std::string roadId, int laneId, double distance, double& last) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieve whether s coordinate is valid on given lane.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @return true if s is valid at given distance, false otherwise
    //-----------------------------------------------------------------------------
    virtual bool IsSValidOnLane(std::string roadId, int laneId, double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Check if a left lane at given distance exists
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @return true if left lane exists at given distance, false otherwise
    //-----------------------------------------------------------------------------
    virtual bool ExistsLaneLeft(std::string roadId, int laneId, double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Check if a right lane at given distance exists
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @return true if right lane exists at given distance, false otherwise
    //-----------------------------------------------------------------------------
    virtual bool ExistsLaneRight(std::string roadId, int laneId, double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Returns number of lanes at given distance
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] distance  s coordinate
    //! @return number of lanes at distance.
    //-----------------------------------------------------------------------------
    virtual int GetNumberOfLanes(std::string roadId, double distance) = 0;

    //-----------------------------------------------------------------------------
    //! Returns interpolated value for curvature at distance.
    //! Neighbouring joints are used as interpolation support point.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @return curvature at distance
    //-----------------------------------------------------------------------------
    virtual double GetLaneCurvature(std::string roadId, int laneId, double distance) const = 0 ;

    //-----------------------------------------------------------------------------
    //! Returns interpolated value for width at distance.
    //! Neighbouring joints are used to interpolation support point.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @return width at distance
    //-----------------------------------------------------------------------------
    virtual double GetLaneWidth(std::string roadId, int laneId, double distance) const = 0 ;

    //-----------------------------------------------------------------------------
    //! Returns interpolated value for direction (heading) at distance.
    //! Neighbouring joints are used to interpolation support point.
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @return direction at distance
    //-----------------------------------------------------------------------------
    virtual double GetLaneDirection(std::string roadId, int laneId, double distance) const = 0 ;

    //-----------------------------------------------------------------------------
    //! Returns remaining distance to end of lane stream or until next lane which has non of the following types:
    //! Driving, Exit, OnRamp, OffRamp or Stop
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneNumber OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @param[in] maxSearchLength maximum search length
    //! @return remaining distance
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToEndOfLane(std::string roadId, int laneNumber, double initialSearchDistance,
                                          double maxSearchLength) = 0;

    //-----------------------------------------------------------------------------
    //! Returns remaining distance to end of lane stream or until next non driving lane (0 if this lane is no driving lane)
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneNumber OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @param[in] maxSearchLength maximum search length
    //! @return remaining distance
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToEndOfDrivingLane(std::string roadId, int laneNumber, double initialSearchDistance,
            double maxSearchLength) = 0;

    //-----------------------------------------------------------------------------
    //! Returns remaining distance to end of lane stream or until next non driving or stop lane (0 if this lane is no driving or stop lane)
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneNumber OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @param[in] maxSearchLength maximum search length
    //! @return remaining distance
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToEndOfDrivingOrStopLane(std::string roadId, int laneNumber, double initialSearchDistance,
            double maxSearchLength) = 0;

    //-----------------------------------------------------------------------------
    //! Returns remaining distance to end of lane stream or until next non ramp lane (0 if this lane is no ramp lane)
    //!
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @param[in] maxSearchLength maximum search length
    //! @return remaining distance
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToEndOfRamp(std::string roadId, int laneId, double initialSearchDistance,
                                          double maxSearchLength) = 0;

    //-----------------------------------------------------------------------------
    //! Returns remaining distance to end of lane stream or until next non exit lane (0 if this lane is no exit lane)
    //!
    //! @param[in] roadId  OpenDriveId of the road to search in
    //! @param[in] laneId OpenDriveId of lane to search in
    //! @param[in] distance  s coordinate
    //! @param[in] maxSearchLength maximum search length
    //! @return remaining distance
    //-----------------------------------------------------------------------------
    virtual double GetDistanceToEndOfExit(std::string roadId, int laneId, double initialSearchDistance,
                                          double maxSearchLength) = 0;
    //-----------------------------------------------------------------------------
    //! Retrieve whether a new agent intersects with an existing agent
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual bool IntersectsWithAgent(double x, double y, double rotation, double length, double width, double center) = 0;

    virtual polygon_t GetBoundingBoxAroundAgent(AgentInterface* agent, double width, double length) = 0;

    virtual Position RoadCoord2WorldCoord(RoadPosition roadCoord, std::string roadID = "") const = 0;

    virtual std::pair<bool, double> GetLateralDistance(GlobalRoadPosition src, GlobalRoadPosition dst) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves all traffic signs in front
    //!
    //! @return                TrafficSigns
    //-----------------------------------------------------------------------------
    virtual std::vector<CommonTrafficSign::Entity> GetTrafficSignsInRange(std::string roadId, int laneId,
            double startDistance, double searchRange) const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the friction
    //!
    //! @return                friction
    //-----------------------------------------------------------------------------
    virtual double GetFriction() const = 0;

    virtual LaneQueryResult QueryLane(std::string roadId, int laneId, double distance) const = 0;

    virtual std::list<LaneQueryResult> QueryLanes(std::string roadId, double startDistance, double endDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Converts stream Id for lane at given distance into  OpenDriveID
    //! Returns -999 if stream Id is invalid for given distance
    //!
    //! @param[in] streamId   stream id of lane
    //! @param[in] endDistance s coordinate
    //! @return OpenDriveId of lane at distance
    //-----------------------------------------------------------------------------
    virtual int GetLaneId(uint64_t streamId, double endDistance) const = 0;

    //-----------------------------------------------------------------------------
    //! Returns one agent which is set to be special.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual AgentInterface* GetEgoAgent() = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves agents that were removed from the world during the simulation run
    //!
    //! @return                List of agent references
    //-----------------------------------------------------------------------------
    virtual const std::list<const AgentInterface*>& GetRemovedAgents() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves all traffic objects
    //!
    //! @return                Traffic objects
    //-----------------------------------------------------------------------------
    virtual const std::vector<const TrafficObjectInterface*>& GetTrafficObjects() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns one agent with the specified scenarioName
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual AgentInterface* GetAgentByName(std::string& scenarioName) = 0;

    //-----------------------------------------------------------------------------
    //! Returns a list with all agents n the specified group
    //!
    //! @return
    //-----------------------------------------------------------------------------
    virtual std::list<AgentInterface*> GetAgentsByGroupType(AgentCategory& agentCategory) = 0;
};

#endif // WORLDINTERFACE_H
