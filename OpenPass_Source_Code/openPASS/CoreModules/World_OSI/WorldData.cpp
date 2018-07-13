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
//! @file  WorldData.cpp
//! @brief This file provides access to underlying data structures for
//!        scenery and dynamic objects
//-----------------------------------------------------------------------------

#include <exception>
#include <string>

#include "OWL/DataTypes.h"
#include "OWL/OpenDriveTypeMapper.h"

#include "roadInterface.h"

#include "WorldData.h"
#include "WorldDataException.h"

#include "osi/osi_worldinterface.pb.h"

namespace OWL {

void WorldData::AddLane(RoadLaneSectionInterface& odSection, const RoadLaneInterface& odLane)
{
    osi3::world::RoadSection* osiSection;

    try
    {
        osiSection = osiSections.at(&odSection);
    }
    catch (std::out_of_range&)
    {
        throw new OWL::NonExistentOsiRef("Section", std::to_string((odSection.GetId())));
    }

    int odLaneId = odLane.GetId();
    Id osiLaneId = CreateUid();

    Section& section = *(sections.at(osiSection->id().value()));
    osi3::world::RoadLane* osiLane = osiSection->add_lane();
    Lane& lane = *(new Implementation::Lane(osiLane, &section));

    osiLane->mutable_id()->set_value(osiLaneId);
    osiLane->set_reverse_direction(!odLane.GetInDirection());

    for (const auto& odLaneMapItem : odSection.GetLanes())
    {
        try
        {
            if (odLaneMapItem.first == odLaneId + 1)
            {
                // find a left neighbor
                osi3::world::RoadLane* leftOsiLane = osiLanes.at(odLaneMapItem.second);
                Lane& leftLane = *(lanes.at(leftOsiLane->id().value()));
                lane.SetLeftLane(leftLane);
                leftLane.SetRightLane(lane);
            }
            else if (odLaneMapItem.first == odLaneId - 1)
            {
                // is right neighbor
                osi3::world::RoadLane* rightOsiLane = osiLanes.at(odLaneMapItem.second);
                Lane& rightLane = *(lanes.at(rightOsiLane->id().value()));
                lane.SetRightLane(rightLane);
                rightLane.SetLeftLane(lane);
            }
        }
        catch (std::out_of_range&)
        {
            // Thats' fine. Adjacent Ids will be set when adding the missing lane.
        }
    }


    osiLanes[&odLane] = osiLane;
    lanes[osiLaneId] = &lane;

    section.AddLane(lane);
}

void WorldData::AddSection(const RoadInterface& odRoad, const RoadLaneSectionInterface& odSection)
{
    osi3::world::Road* osiRoad;

    try
    {
        osiRoad = osiRoads.at(&odRoad);
    }
    catch(std::out_of_range& e)
    {
        throw new OWL::NonExistentOsiRef("Road", odRoad.GetId());
    }

    osi3::world::RoadSection* osiSection = osiRoad->add_section();
    Id secId = CreateUid();
    osiSection->mutable_id()->set_value(secId);

    Section* section = new Implementation::Section(osiSection);

    osiSections[&odSection] = osiSection;
    sections[secId] = section;

    Road* road = roads.at(osiRoad->id().value());
    road->AddSection(*section);
}

void WorldData::AddRoad(/* const */ RoadInterface& odRoad)
{
    osi3::world::Road* osiRoad = osiGroundTruth.add_road();
    Road* road = new Implementation::Road(osiRoad);

    Id roadId = CreateUid();
    osiRoad->mutable_id()->set_value(roadId);

    // TODO: GetRoadType parameter ?!
    const RoadTypeInformation odRoadType = odRoad.GetRoadType(0.0);
    osiRoad->set_type(OpenDriveTypeMapper::RoadType(odRoadType));

    osiRoads[&odRoad] = osiRoad;
    roads[roadId] = road;
}

void WorldData::ConnectLanes(/* const */ RoadLaneSectionInterface& firstOdSection,
                             /* const */ RoadLaneSectionInterface& secondOdSection,
                             const std::map<int, int>& lanePairs,
                             bool isPrev)
{
    for (auto pair : lanePairs)
    {
        /* const */ RoadLaneInterface* firstOdLane = firstOdSection.GetLanes().at(pair.first);
        /* const */ RoadLaneInterface* secondOdLane = secondOdSection.GetLanes().at(pair.second);

        osi3::world::RoadLane* firstOsiLane = osiLanes.at(firstOdLane);
        osi3::world::RoadLane* secondOsiLane = osiLanes.at(secondOdLane);

        // TODO: generate and assign lane pairings instead of successor/predecessor
    }
}

void WorldData::AddLanePairing(/* const */ RoadLaneInterface& odLane,
                               /* const */ RoadLaneInterface& predecessorOdLane,
                               /* const */ RoadLaneInterface& successorOdLane)
{
    osi3::world::RoadLane* osiLane = osiLanes.at(&odLane);
    const osi3::world::RoadLane* predecessorOsiLane = osiLanes.at(&predecessorOdLane);
    const osi3::world::RoadLane* successorOsiLane = osiLanes.at(&successorOdLane);

    osi3::world::RoadLanePairing* pairing = osiLane->add_lane_pairing();

    pairing->mutable_antecessor_lane_id()->set_value(predecessorOsiLane->id().value());
    pairing->mutable_successor_lane_id()->set_value(successorOsiLane->id().value());
}

void WorldData::AddLaneSuccessor(/* const */ RoadLaneInterface& odLane,
                                 /* const */ RoadLaneInterface& successorOdLane,
                                 /* const */ bool connectAtSuccessorEnd)
{
    osi3::world::RoadLane* osiLane = osiLanes.at(&odLane);
    const osi3::world::RoadLane* successorOsiLane = osiLanes.at(&successorOdLane);

    Lane* lane = lanes.at(osiLane->id().value());
    Lane* nextLane = lanes.at(successorOsiLane->id().value());
    lane->AddNextLane(*nextLane);
}

void WorldData::AddLanePredecessor(/* const */ RoadLaneInterface& odLane,
                                   /* const */ RoadLaneInterface& predecessorOdLane,
                                   /* const */ bool connectAtPredecessorEnd)
{
    osi3::world::RoadLane* osiLane = osiLanes.at(&odLane);
    const osi3::world::RoadLane* predecessorOsiLane = osiLanes.at(&predecessorOdLane);

    Lane* lane = lanes.at(osiLane->id().value());
    Lane* prevLane = lanes.at(predecessorOsiLane->id().value());
    lane->AddPreviousLane(*prevLane);
}

MovingObject* WorldData::AddMovingObject()
{
    Id id = CreateUid();
    osi3::MovingObject* osiMovingObject = osiGroundTruth.add_moving_object();
    MovingObject* movingObject = new MovingObject(osiMovingObject);

    osiMovingObject->mutable_id()->set_value(id);

    movingObjects[id] = movingObject;

    return movingObject;
}

void WorldData::RemoveMovingObjectById(Id id)
{
    bool found = false;
    auto osiMovingObjects = osiGroundTruth.moving_object();

    for (int i = 0; i < osiMovingObjects.size(); ++i)
    {
       if (osiMovingObjects.Get(i).id().value() == id)
       {
           osiMovingObjects.SwapElements(i, osiMovingObjects.size() - 1);
           found = true;
       }
    }

    if (found)
    {
        osiMovingObjects.RemoveLast();
        delete movingObjects.at(id);
        movingObjects.erase(id);
    }
}

void WorldData::AddStationaryObject(const RoadObjectInterface& odObject)
{
    osi3::StationaryObject* osiStationaryObject = osiGroundTruth.add_stationary_object();
    StationaryObject* stationaryObject = new StationaryObject(osiStationaryObject);
    Id id = CreateUid();

    osiStationaryObject->mutable_id()->set_value(id);

    Primitive::RoadCoordinate position;
    position.s = odObject.GetS();
    position.t = odObject.GetT();

    stationaryObjects[id] = stationaryObject;
}

void WorldData::ConnectSections(const RoadLaneSectionInterface& firstOdSection,
                                const RoadLaneSectionInterface& secondOdSection)
{
    osi3::world::RoadSection* firstOsiSection = osiSections.at(&firstOdSection);
    osi3::world::RoadSection* secondOsiSection = osiSections.at(&secondOdSection);

    firstOsiSection->mutable_successor_section()->set_value(secondOsiSection->id().value());
    secondOsiSection->mutable_antecessor_section()->set_value(firstOsiSection->id().value());
}

void WorldData::SetRoadPredecessorIntersection(const RoadInterface& road, const RoadInterface& predecessorIntersection)
{
    osi3::world::Road* osiRoad = osiRoads.at(&road);
    osi3::world::Road* predecessorOsiIntersection = osiRoads.at(&predecessorIntersection);

    osiRoad->mutable_antecessor_road()->mutable_intersection_id()->set_value(predecessorOsiIntersection->id().value());
}

void WorldData::SetRoadSuccessorIntersection(const RoadInterface& road, const RoadInterface& successorIntersection)
{
    osi3::world::Road* osiRoad = osiRoads.at(&road);
    osi3::world::Road* successorOsiIntersection = osiRoads.at(&successorIntersection);

    osiRoad->mutable_successor_road()->mutable_intersection_id()->set_value(successorOsiIntersection->id().value());
}

void WorldData::SetRoadPredecessor(const RoadInterface& road, const RoadInterface& predecessorRoad)
{
    osi3::world::Road* osiRoad = osiRoads.at(&road);
    osi3::world::Road* predecessorOsiRoad = osiRoads.at(&predecessorRoad);

    osiRoad->mutable_antecessor_road()->mutable_road_id()->set_value(predecessorOsiRoad->id().value());
}

void WorldData::SetRoadSuccessor(const RoadInterface& road, const RoadInterface& successorRoad)
{
    osi3::world::Road* osiRoad = osiRoads.at(&road);
    osi3::world::Road* successorOsiRoad = osiRoads.at(&successorRoad);

    osiRoad->mutable_successor_road()->mutable_road_id()->set_value(successorOsiRoad->id().value());
}

osi3::world::RoadLane* WorldData::GetOsiLaneIdByOdId(int odLaneId)
{
    for (auto lane : osiLanes)
    {
        if (lane.first->GetId() == odLaneId)
        {
            return lane.second;
        }
    }

    return nullptr;
}

void WorldData::AddLaneGeometryPoint(const RoadLaneInterface& odLane,
                                     const Common::Vector2d& pointLeft,
                                     const Common::Vector2d& pointCenter,
                                     const Common::Vector2d& pointRight,
                                     const double sOffset,
                                     const double curvature,
                                     const double heading)
{
    osi3::world::RoadLane* osiLane = osiLanes.at(&odLane);

    osi3::world::LaneGeometryJoint* osiGeometry = osiLane->add_geometry();
    osi3::Vector2d* osiVector;

    Lane* lane = lanes.at(osiLane->id().value());

    lane->AddLaneGeometryJoint(pointLeft, pointCenter, pointRight, sOffset, curvature, heading);

    osiVector = osiGeometry->mutable_left();
    osiVector->set_x(pointLeft.x);
    osiVector->set_y(pointLeft.y);

    osiVector = osiGeometry->mutable_reference();
    osiVector->set_x(pointCenter.x);
    osiVector->set_y(pointCenter.y);

    osiVector = osiGeometry->mutable_right();
    osiVector->set_x(pointRight.x);
    osiVector->set_y(pointRight.y);

    osiGeometry->set_curvature(curvature);
    osiGeometry->set_heading(heading);

    int pointCount = osiLane->geometry_size();

    if (pointCount == 1)
    {
        osiLane->set_length(0.0);
    }
    else
    {
        const osi3::Vector2d& currentOsiPoint = osiGeometry->reference();
        const osi3::Vector2d& previousOsiPoint = osiLane->geometry(pointCount - 2).reference();
        double deltaX = currentOsiPoint.x() - previousOsiPoint.x();
        double deltaY = currentOsiPoint.y() - previousOsiPoint.y();
        double laneLength = osiLane->length();
        osiLane->set_length(laneLength + std::hypot(deltaX, deltaY));
    }
}

WorldData::~WorldData()
{
    for (auto road : roads)
    {
        delete road.second;
    }

    roads.clear();

    for (auto section : sections)
    {
        delete section.second;
    }

    sections.clear();

    for (auto lane : lanes)
    {
        delete lane.second;
    }

    lanes.clear();
}

const std::unordered_map<Id, Lane*>& WorldData::GetLanes() const
{
    return lanes;
}

CLane& WorldData::GetLaneById(Id id) const
{
    try
    {
        return *(lanes.at(id));
    }
    catch (const std::out_of_range& /* e */)
    {
        return invalidLane;
    }
}

const std::unordered_map<Id, Section*>& WorldData::GetSections() const
{
    return sections;
}

CSection& WorldData::GetSectionById(Id id) const
{
    return *(sections.at(id));
}


const std::unordered_map<Id, Road*>& WorldData::GetRoads() const
{
    return roads;
}

CRoad& WorldData::GetRoadById(Id id) const
{
    return *(roads.at(id));
}

const std::unordered_map<Id, StationaryObject*>& WorldData::GetStationaryObjects() const
{
    return stationaryObjects;
}

CStationaryObject& WorldData::GetStationaryObjectById(Id id) const
{
    return *(stationaryObjects.at(id));
}

const std::unordered_map<Id, MovingObject*>& WorldData::GetMovingObjects() const
{
    return movingObjects;
}

CMovingObject& WorldData::GetMovingObjectById(Id id) const
{
    return *(movingObjects.at(id));
}

}
