/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  WorldData.h
//! @brief This file provides access to underlying data structures for
//!        scenery and dynamic objects
//-----------------------------------------------------------------------------

#pragma once

#include <unordered_map>

#include "OWL/DataTypes.h"
#include "roadInterface.h"

#include "osi/osi_worldinterface.pb.h"

namespace OWL {

using Lane              = Interfaces::Lane;
using Section           = Interfaces::Section;
using Road              = Interfaces::Road;
using StationaryObject  = Implementation::StationaryObject;
using MovingObject      = Implementation::MovingObject;
using Vehicle           = Implementation::Vehicle;

using CLane              = const Interfaces::Lane;
using CSection           = const Interfaces::Section;
using CRoad              = const Interfaces::Road;
using CStationaryObject  = const Implementation::StationaryObject;
using CMovingObject      = const Implementation::MovingObject;
using CVehicle           = const Implementation::Vehicle;

using Lanes             = std::list<Lane*>;
using Sections          = std::list<Section*>;
using Roads             = std::list<Road*>;
using StationaryObjects = std::list<StationaryObject*>;
using MovingObjects     = std::list<MovingObject*>;
using Vehicles          = std::list<Vehicle*>;

using CLanes             = const std::list<CLane*>;
using CSections          = const std::list<CSection*>;
using CRoads             = const std::list<CRoad*>;
using CStationaryObjects = const std::list<CStationaryObject*>;
using CMovingObjects     = const std::list<CMovingObject*>;
using CVehicles          = const std::list<CVehicle*>;

class WorldDataInterface
{
public:
    virtual ~WorldDataInterface() = default;
};

class WorldData : WorldDataInterface
{
public:
    ~WorldData();

    void AddLane(RoadLaneSectionInterface& odSection, const RoadLaneInterface& odLane);
    void AddSection(const RoadInterface& odRoad, const RoadLaneSectionInterface& odSection);
    void AddRoad(/* const*/ RoadInterface& odRoad);
    void AddLanePairing(/* const */ RoadLaneInterface& odLane,
                        /* const */ RoadLaneInterface& predecessorOdLane,
                        /* const */ RoadLaneInterface& successorOdLane);
    void AddLaneSuccessor(/* const */ RoadLaneInterface& odLane,
                                 /* const */ RoadLaneInterface& successorOdLane,
                                 /* const */ bool connectAtSuccessorEnd);
    void AddLanePredecessor(/* const */ RoadLaneInterface& odLane,
                                 /* const */ RoadLaneInterface& predecessorOdLane,
                                 /* const */ bool connectAtPredecessorEnd);

    MovingObject* AddMovingObject();
    void RemoveMovingObjectById(Id id);

    void ConnectLanes(/*const*/ RoadLaneSectionInterface& firstOdSection,
                      /*const*/ RoadLaneSectionInterface& secondOdSection,
                      const std::map<int,int>& lanePairs,
                      bool isPrev);
    void ConnectSections(const RoadLaneSectionInterface& firstOdSection, const RoadLaneSectionInterface& secondOdSection);
    void SetRoadSuccessor(const RoadInterface& road,  const RoadInterface& successorRoad);
    void SetRoadPredecessor(const RoadInterface& road,  const RoadInterface& predecessorRoad);
    void SetRoadSuccessorIntersection(const RoadInterface& road,  const RoadInterface& successorIntersection);
    void SetRoadPredecessorIntersection(const RoadInterface& road,  const RoadInterface& predecessorIntersection);

    void AddStationaryObject(const RoadObjectInterface& odObject);

    //void AddMovingObject(Shared::MovingObject& object);

    void AddLaneGeometryPoint(const RoadLaneInterface& odLane,
                              const Common::Vector2d& pointLeft,
                              const Common::Vector2d& pointCenter,
                              const Common::Vector2d& pointRight,
                              const double sOffset,
                              const double curvature,
                              const double heading);

    CLane& GetLaneById(Id id) const;
    CSection& GetSectionById(Id id) const;
    CRoad& GetRoadById(Id id) const;
    CStationaryObject& GetStationaryObjectById(Id id) const;
    CMovingObject& GetMovingObjectById(Id id) const;

    const std::unordered_map<Id, Lane*>& GetLanes() const;
    const std::unordered_map<Id, Section*>& GetSections() const;
    const std::unordered_map<Id, Road*>& GetRoads() const;
    const std::unordered_map<Id, StationaryObject*>& GetStationaryObjects() const;
    const std::unordered_map<Id, MovingObject*>& GetMovingObjects() const;

private:
    osi3::world::RoadLane* GetOsiLaneIdByOdId(int odLaneId);

    uint64_t next_free_uid{0};

    std::unordered_map<Id, Lane*>             lanes;
    std::unordered_map<Id, Section*>          sections;
    std::unordered_map<Id, Road*>             roads;
    std::unordered_map<Id, StationaryObject*> stationaryObjects;
    std::unordered_map<Id, MovingObject*>     movingObjects;

    std::unordered_map<const RoadInterface*, osi3::world::Road*>                   osiRoads;
    std::unordered_map<const RoadLaneSectionInterface*, osi3::world::RoadSection*> osiSections;
    std::unordered_map<const RoadLaneInterface*, osi3::world::RoadLane*>           osiLanes;

    osi3::world::WorldInterface osiGroundTruth;

    const Implementation::InvalidLane invalidLane;

    inline uint64_t CreateUid()
    {
        return next_free_uid++;
    }
};

}  // namespace OWL
