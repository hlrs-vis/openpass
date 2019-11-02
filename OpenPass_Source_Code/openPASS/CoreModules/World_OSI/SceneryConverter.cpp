/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <algorithm>
#include <utility>
#include <limits>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <cmath>
#include <tuple>
#include <sstream>
#include <QFile>
#include "SceneryConverter.h"
#include "GeometryConverter.h"
#include "TrafficObjectAdapter.h"
#include "cmath"

#include "WorldData.h"
#include "WorldDataQuery.h"

SceneryConverter::SceneryConverter(SceneryInterface* scenery,
                                   OWL::Interfaces::WorldData& worldData,
                                   World::Localization::Cache& localizationCache,
                                   const CallbackInterface* callbacks) :
    scenery(scenery),
    worldData(worldData),
    localizationCache{localizationCache},
    callbacks(callbacks)
{}

RoadLaneInterface* SceneryConverter::GetOtherLane(RoadLaneSectionInterface* otherSection,
        int otherId)
{
    // search for connected lane in OpenDrive data structures since LaneId of
    // LINEAR_LANE Objects might be reordered according to direction definition
    RoadLaneInterface* connectedRoadLane;

    for (auto findIt = otherSection->GetLanes().begin();
            otherSection->GetLanes().end() != findIt;
            ++findIt)
    {
        connectedRoadLane = findIt->second;
        if (connectedRoadLane->GetId() == otherId)
        {
            return connectedRoadLane;
        }
    }

    return nullptr;
}

RoadInterface* SceneryConverter::GetConnectedRoad(RoadLinkInterface* roadLink)
{
    if (ContactPointType::Start != roadLink->GetContactPoint() &&
            ContactPointType::End != roadLink->GetContactPoint())
    {
        LOG(CbkLogLevel::Error, "no contact point defined for road link");
        return nullptr;
    }

    RoadInterface* connectedRoad = nullptr;
    for (auto& item : scenery->GetRoads())
    {
        RoadInterface* itemRoad = item.second;
        if (roadLink->GetElementId() == itemRoad->GetId())
        {
            connectedRoad = itemRoad;
            break;
        }
    }

    if (!connectedRoad)
    {
        LOG(CbkLogLevel::Error, "missing road for ElementId " + roadLink->GetElementId());
        return nullptr;
    }

    return connectedRoad;
}

void SceneryConverter::MarkDirectionRoad(RoadInterface* road,
        bool inDirection)
{
    LOG(CbkLogLevel::Debug, "direction of road " + road->GetId() + ": "
        + (inDirection ? "true" : "false"));

    road->SetInDirection(inDirection);

    for (RoadLaneSectionInterface* roadLaneSection : road->GetLaneSections())
    {
        roadLaneSection->SetInDirection(inDirection);

        for (auto& item : roadLaneSection->GetLanes())
        {
            RoadLaneInterface* roadLane = item.second;
            roadLane->SetInDirection(inDirection);
        }
    }
}

bool SceneryConverter::MarkDirections()
{
    std::list<RoadInterface*> pendingRoads;
    std::transform(scenery->GetRoads().begin(),
                   scenery->GetRoads().end(),
                   std::back_inserter(pendingRoads),
    [](const std::pair<std::string, RoadInterface*>& item) { return item.second; });

    while (!pendingRoads.empty()) // proceed until all roads have been marked
    {
        RoadInterface* road = pendingRoads.front();

        std::list<RoadInterface*> tmpCluster; // contains currently processed cluster
        tmpCluster.splice(tmpCluster.end(), pendingRoads, pendingRoads.begin());

        MarkDirectionRoad(road,
                          true); // first road in cluster defines direction within cluster

        LOG(CbkLogLevel::Debug, "process road cluster");

        while (!tmpCluster.empty()) // process current cluster
        {
            road = tmpCluster.front();
            tmpCluster.pop_front();

            if (road->GetLaneSections().empty())
            {
                LOG(CbkLogLevel::Error, "no sections given for road " + road->GetId());
                return false;
            }

            // collect all road links for this road and save them in this mapping
            std::map<std::tuple<RoadInterface*, RoadInterface*>, std::tuple<RoadLinkType, ContactPointType>> collectedRoadLinks;
            for (RoadLinkInterface* roadLink : road->GetRoadLinks())
            {
                if (RoadLinkElementType::Road == roadLink->GetElementType())
                {
                    RoadInterface* connectedRoad = GetConnectedRoad(roadLink);
                    if (!connectedRoad)
                    {
                        return false;
                    }

                    if (road == connectedRoad)
                    {
                        LOG(CbkLogLevel::Error, "self references are not supported, road " + road->GetId());
                        return false;
                    }

                    // if the connection between these two roads is found for the first time,
                    // add it to the mapping
                    if (0 == collectedRoadLinks.count(std::make_tuple(road, connectedRoad)) &&
                            0 == collectedRoadLinks.count(std::make_tuple(connectedRoad, road)))
                    {
                        collectedRoadLinks.insert({std::make_tuple(road,
                                                   connectedRoad),
                                                   std::make_tuple(roadLink->GetType(),
                                                           roadLink->GetContactPoint())}); // connectedRoad is not part of a junction
                    }
                }
                else
                    if (RoadLinkElementType::Junction == roadLink->GetElementType())
                    {
                        //handle junctions
                    }
                    else
                    {
                        assert(0); // catched by parser
                        return false;
                    }
            }

            // process collected road links
            for (auto& item : collectedRoadLinks)
            {
                RoadInterface* connectedRoad = std::get<1>(item.first);
                RoadLinkType connectedRoadLinkType = std::get<0>(item.second);
                ContactPointType connectedContactPointType = std::get<1>(item.second);

                auto findIt = std::find_if(pendingRoads.begin(),
                                           pendingRoads.end(),
                [&connectedRoad](RoadInterface * road) { return road == connectedRoad; });
                if (pendingRoads.end() == findIt)
                {
                    continue; // road already processed (no not overwrite content)
                }

                bool connectedRoadInDirection = false;

                if (RoadLinkType::Predecessor != connectedRoadLinkType &&
                        RoadLinkType::Successor != connectedRoadLinkType) // catch neighbor road link type
                {
                    LOG(CbkLogLevel::Error, "only predecessor and successor road links are supported");
                    return false;
                }

                assert(ContactPointType::Start == connectedContactPointType ||
                       ContactPointType::End == connectedContactPointType); // catched by importer

                // if the connected road is in direction, depends whether the
                // current road is in direction and how they are connected
                bool roadInDirection = road->GetInDirection();
                if (RoadLinkType::Predecessor == connectedRoadLinkType)
                {
                    if (ContactPointType::Start == connectedContactPointType)
                    {
                        connectedRoadInDirection = !roadInDirection;
                    }
                    else
                        if (ContactPointType::End == connectedContactPointType)
                        {
                            connectedRoadInDirection = roadInDirection;
                        }
                }
                else
                    if (RoadLinkType::Successor == connectedRoadLinkType)
                    {
                        if (ContactPointType::Start == connectedContactPointType)
                        {
                            connectedRoadInDirection = roadInDirection;
                        }
                        else
                            if (ContactPointType::End == connectedContactPointType)
                            {
                                connectedRoadInDirection = !roadInDirection;
                            }
                    }

                MarkDirectionRoad(connectedRoad,
                                  connectedRoadInDirection);

                // update cluster
                auto findIter = std::find(pendingRoads.begin(), pendingRoads.end(), connectedRoad);
                if (pendingRoads.end() != findIter)
                {
                    pendingRoads.remove(connectedRoad);
                    tmpCluster.push_back(connectedRoad);
                }
            } // for each road link
        } // process next road within current cluster
    } // for each cluster

    return true;
}

bool SceneryConverter::IndexElements()
{
    int linearSectionId = 0;
    for (auto& item : scenery->GetRoads())
    {
        RoadInterface* road = item.second;

        for (RoadLaneSectionInterface* roadLaneSection : road->GetLaneSections())
        {
            roadLaneSection->SetId(linearSectionId);
            ++linearSectionId;
        }
    }

    return true;
}

bool SceneryConverter::ConnectLaneToLane(RoadLaneInterface* currentLane,
        ContactPointType currentContactPoint,
        RoadLaneInterface* otherLane)
{
    // calculate direction parameters
    bool currentDestBegin = ContactPointType::Start == currentContactPoint;

    if (!currentDestBegin)
    {
        worldData.AddLaneSuccessor(*currentLane, *otherLane);
    }
    else
    {
        worldData.AddLanePredecessor(*currentLane, *otherLane);
    }

    return true;
}

bool SceneryConverter::ConnectLaneToSection(RoadLaneInterface* currentLane,
        ContactPointType currentContactPoint,
        RoadLaneSectionInterface* otherLaneSection)
{
    assert(currentLane->GetId()); // no center lanes

    if (currentContactPoint == ContactPointType::Start)
    {
        // process predecessor
        RoadLaneInterface* otherLane = GetOtherLane(otherLaneSection, currentLane->GetPredecessor().front());
        if (otherLane && !ConnectLaneToLane(currentLane,
                                            currentContactPoint,
                                            otherLane))
        {
            LOG(CbkLogLevel::Error, "could not connect lanes");
            return false;
        }
    }
    else
    {
        // process successor
        RoadLaneInterface* otherLane = GetOtherLane(otherLaneSection, currentLane->GetSuccessor().front());
        if (otherLane && !ConnectLaneToLane(currentLane,
                                            currentContactPoint,
                                            otherLane))
        {
            LOG(CbkLogLevel::Error, "could not connect lanes");
            return false;
        }
    }

    return true;
}

bool SceneryConverter::ConnectLanes(RoadLaneSectionInterface* firstLaneSection,
                                    ContactPointType firstContactPoint,
                                    RoadLaneSectionInterface* secondLaneSection,
                                    ContactPointType secondContactPoint)
{
    for (auto& item : firstLaneSection->GetLanes())
    {
        if (!ConnectLaneToSection(item.second,
                                  firstContactPoint,
                                  secondLaneSection))
        {
            return false;
        }
    }

    for (auto& item : secondLaneSection->GetLanes())
    {
        if (!ConnectLaneToSection(item.second,
                                  secondContactPoint,
                                  firstLaneSection))
        {
            return false;
        }
    }

    return true;
}

bool SceneryConverter::ConnectRoadExternalWithElementTypeRoad(RoadInterface* road)
{
    for (RoadLinkInterface* roadLink : road->GetRoadLinks())
    {
        if (roadLink->GetElementType() != RoadLinkElementType::Road) { continue; }

        if (roadLink->GetType() == RoadLinkType::Neighbor)
        {
            LOG(CbkLogLevel::Warning, "RoadLinkType 'Neighbor' not supported!");
            continue;
        }

        RoadInterface* otherRoad = scenery->GetRoad(roadLink->GetElementId());
        RoadLaneSectionInterface* otherSection;

        if (roadLink->GetContactPoint() == ContactPointType::Start)
        {
            otherSection = otherRoad->GetLaneSections().front();
        }
        else
        {
            otherSection = otherRoad->GetLaneSections().back();
        }

        if (roadLink->GetType() == RoadLinkType::Successor)
        {
            if (!ConnectExternalRoadSuccessor(road, otherRoad, otherSection))
            {
                return false;
            }
        }
        else
        {
            if (!ConnectExternalRoadPredecessor(road, otherRoad, otherSection))
            {
                return false;
            }
        }
    }

    return true;
}

bool SceneryConverter::ConnectExternalRoadSuccessor(RoadInterface* currentRoad, RoadInterface* otherRoad,
        RoadLaneSectionInterface* otherSection)
{
    worldData.SetRoadSuccessor(*currentRoad, *otherRoad);
    RoadLaneSectionInterface* currentSection = currentRoad->GetLaneSections().back();
    worldData.SetSectionSuccessor(*currentSection, *otherSection);
    for (auto& laneEntry : currentSection->GetLanes())
    {
        RoadLaneInterface* lane = laneEntry.second;
        std::list<int> successorLaneId = lane->GetSuccessor();
        if (successorLaneId.size() == 1)
        {
            try
            {
                RoadLaneInterface* successorLane = otherSection->GetLanes().at(successorLaneId.front());
                worldData.AddLaneSuccessor(*lane, *successorLane);
            }
            catch (const std::out_of_range&)
            {
                LOG(CbkLogLevel::Error, "LaneSuccessorId not found");
                return false;
            }
        }
    }
    return true;
}

bool SceneryConverter::ConnectExternalRoadPredecessor(RoadInterface* currentRoad, RoadInterface* otherRoad,
        RoadLaneSectionInterface* otherSection)
{
    worldData.SetRoadPredecessor(*currentRoad, *otherRoad);
    RoadLaneSectionInterface* currentSection = currentRoad->GetLaneSections().front();
    worldData.SetSectionPredecessor(*currentSection, *otherSection);
    for (auto& laneEntry : currentSection->GetLanes())
    {
        RoadLaneInterface* lane = laneEntry.second;
        std::list<int> predecessorLaneId = lane->GetPredecessor();
        if (predecessorLaneId.size() == 1)
        {
            try
            {
                RoadLaneInterface* predecessorLane = otherSection->GetLanes().at(predecessorLaneId.front());
                worldData.AddLanePredecessor(*lane, *predecessorLane);
            }
            catch (const std::out_of_range&)
            {
                LOG(CbkLogLevel::Error, "LanePredecessorId not found");
                return false;
            }
        }
    }
    return true;
}

bool SceneryConverter::ConnectRoadInternal(RoadInterface* road)
{
    // connect sections within a road
    auto it = road->GetLaneSections().begin();
    RoadLaneSectionInterface* previousLaneSection = *it;
    ++it;
    while (it !=
            road->GetLaneSections().end()) // skipped for junctions since openDrive connecting roads contain only one lane section
    {
        RoadLaneSectionInterface* laneSection = *it;

        worldData.SetSectionSuccessor(*previousLaneSection, *laneSection);
        worldData.SetSectionPredecessor(*laneSection, *previousLaneSection);

        if (!ConnectLanes(previousLaneSection,
                          ContactPointType::End,
                          laneSection,
                          ContactPointType::Start))
        {
            LOG(CbkLogLevel::Error, "could not connect sections");
            return false;
        }

        previousLaneSection = laneSection;
        ++it;
    }

    return true;
}

bool SceneryConverter::ConnectJunction(JunctionInterface* junction)
{
    worldData.AddJunction(junction);
    for (auto& entry : junction->GetConnections())
    {
        ConnectionInterface* connection = entry.second;

        std::string incomingRoadId = connection->GetIncommingRoadId();
        std::string connectingId = connection->GetConnectingRoadId();
        RoadInterface* incomingRoad = scenery->GetRoad(incomingRoadId);
        RoadInterface* connectingRoad = scenery->GetRoad(connectingId);
        std::string outgoingRoadId;
        ContactPointType incomingContactPoint;
        ContactPointType outgoingContactPoint;
        for (auto roadLink : connectingRoad->GetRoadLinks())
        {
            if (roadLink->GetType() == RoadLinkType::Predecessor)
            {
                incomingContactPoint = roadLink->GetContactPoint();
            }
            if (roadLink->GetType() == RoadLinkType::Successor)
            {
                outgoingRoadId = roadLink->GetElementId();
                outgoingContactPoint = roadLink->GetContactPoint();
            }
        }
        RoadInterface* outgoingRoad = scenery->GetRoad(outgoingRoadId);

        if (incomingRoad == nullptr || connectingRoad == nullptr || outgoingRoad == nullptr)
        {
            return false;
        }

        if ((connectingRoad->GetLaneSections().size() > 0) && incomingRoad->GetLaneSections().size() > 0)
        {
            std::map<int, int> laneIdMapping = connection->GetLinks();

            ConnectPathInJunction(incomingRoad, connectingRoad, outgoingRoad, incomingContactPoint, outgoingContactPoint,
                                  laneIdMapping);
        }
        else
        {
            if (incomingRoad->GetLaneSections().size() == 0)
            {
                LOG(CbkLogLevel::Error, "Connecting Road has no sections " + connectingRoad->GetId());
            }

            if (connectingRoad->GetLaneSections().size() == 0)
            {
                LOG(CbkLogLevel::Error, "Connecting Road has no sections " + connectingRoad->GetId());
            }


        }

    }

    return true;
}

void SceneryConverter::ConnectPathInJunction(RoadInterface* incomingRoad, RoadInterface* connectingRoad,
        RoadInterface* outgoingRoad, ContactPointType incomingContactPoint, ContactPointType outgoingContactPoint,
        std::map<int, int> laneIdMapping)
{
    RoadLaneSectionInterface* incomingRoadSection;
    if (incomingContactPoint == ContactPointType::Start)
    {
        incomingRoadSection = incomingRoad->GetLaneSections().front();
    }
    else
    {
        incomingRoadSection = incomingRoad->GetLaneSections().back();
    }
    RoadLaneSectionInterface* connectingRoadFirstSection = connectingRoad->GetLaneSections().front();
    RoadLaneSectionInterface* connectingRoadLastSection = connectingRoad->GetLaneSections().back();
    RoadLaneSectionInterface* outgoingRoadSection;
    if (outgoingContactPoint == ContactPointType::Start)
    {
        outgoingRoadSection = outgoingRoad->GetLaneSections().front();
    }
    else
    {
        outgoingRoadSection = outgoingRoad->GetLaneSections().back();
    }
    for (auto lanePair : laneIdMapping)
    {
        RoadLaneInterface* incomingLane = incomingRoadSection->GetLanes().at(lanePair.first);
        RoadLaneInterface* connectingLane = connectingRoadFirstSection->GetLanes().at(lanePair.second);
        ConnectLaneToLane(incomingLane, incomingContactPoint, connectingLane);
    }

    for (auto outgoingLane : outgoingRoadSection->GetLanes())
    {
        //! If isInStreamdirection is false, then the next lane in the forwardLaneStream
        //! is the predecessor instead of the successor
        int connectingLaneId;
        if (outgoingContactPoint == ContactPointType::Start)
        {
            connectingLaneId = outgoingLane.second->GetPredecessor().front();
        }
        else
        {
            connectingLaneId = outgoingLane.second->GetSuccessor().front();
        }
        RoadLaneInterface* connectingLane = connectingRoadLastSection->GetLanes().at(connectingLaneId);
        ConnectLaneToLane(outgoingLane.second, outgoingContactPoint, connectingLane);
    }
}

bool SceneryConverter::ConnectRoads()
{
    for (auto& item : scenery->GetRoads())
    {
        RoadInterface* road = item.second;

        ConnectRoadExternalWithElementTypeRoad(road);

        if (!ConnectRoadInternal(road))
        {
            LOG(CbkLogLevel::Error, "could not connect road " + item.second->GetId());
            return false;
        }
    }

    for (auto& item : scenery->GetJunctions())
    {
        if (!ConnectJunction(item.second))
        {
            return false;
        }
    }
    return true;
}

bool SceneryConverter::Convert()
{
    // define a unique directions of roads/lanes within each road cluster
    if (!MarkDirections())
    {
        return false;
    }

    CreateRoads();

    // connect roads in data layer
    if (!ConnectRoads())
    {
        return false;
    }

    // create geometries
    GeometryConverter converter(scenery,
                                //                                sectionMapping,
                                //                                xfLaneMapping,
                                //                                laneMapping,
                                worldData,
                                callbacks);
    if (!converter.Convert())
    {
        return false;
    }

    CreateObjects();
    CreateTrafficSigns();

    return true;
}

void SceneryConverter::CreateObjects()
{
    for (auto& item : scenery->GetRoads())
    {
        RoadInterface* road = item.second;

        std::vector<RoadLaneSectionInterface*> roadLaneSections = road->GetLaneSections();

        for (RoadObjectInterface* object : road->GetRoadObjects())
        {
            auto section = worldDataQuery.GetSectionByDistance(road->GetId(), object->GetS());

            if (section == nullptr) //potenzieller odrive fehler
            {
                LOG(CbkLogLevel::Warning, "Object ignored: s-coordinate not within road");
                continue;
            }

            bool isOnRoad;
            double x, y, yaw;
            std::tie(isOnRoad, x, y, yaw) = CalculateAbsoluteCoordinates(road, section, object);

            if (isOnRoad)
            {
                OWL::Primitive::AbsPosition pos{x, y, 0};
                OWL::Primitive::Dimension dim{object->GetLength(), object->GetWidth(), object->GetHeight()};
                OWL::Primitive::AbsOrientation orientation{object->GetHdg(), object->GetPitch(), object->GetRoll()};
                new TrafficObjectAdapter(worldData, localizationCache, pos, dim, orientation);
            }
        }
    }
}

void SceneryConverter::CreateTrafficSigns()
{
    for (auto& item : scenery->GetRoads())
    {
        RoadInterface* road = item.second;

        for (RoadSignalInterface* signal : road->GetRoadSignals())
        {
            auto section = worldDataQuery.GetSectionByDistance(road->GetId(), signal->GetS());

            if (section == nullptr) //potenzieller odrive fehler
            {
                LOG(CbkLogLevel::Warning, "Object ignored: s-coordinate not within road");
                continue;
            }

            OWL::Interfaces::TrafficSign& trafficSign = worldData.AddTrafficSign();
            trafficSign.SetS(signal->GetS());
            trafficSign.SetValue(signal->GetValue());
            trafficSign.SetType(signal->GetType());
            for (auto lane : section->GetLanes())
            {
                OWL::OdId odId = worldData.GetLaneIdMapping().at(lane->GetId());
                if (signal->IsValidForLane(odId))
                {
                    trafficSign.SetValidForLane(lane->GetId());
                }
            }
        }
    }
}


std::tuple<bool, double, double, double> SceneryConverter::CalculateAbsoluteCoordinates(RoadInterface* road,
        OWL::CSection* section, const RoadObjectInterface* object) const
{
    double absolutS = object->GetS();
    double t = object->GetT();
    double hdg = object->GetHdg();

    //Note: only negative t supported so far
    if (t > 0)
    {
        return std::make_tuple(false, 0, 0, 0);
    }

    double absT = -t;
    double leftBoundary = 0;


    for (int i = -1; i >= -static_cast<int>(section->GetLanes().size()); i--)
    {
        OWL::CLane& lane = worldDataQuery.GetLaneByOdId(road->GetId(), i, absolutS);
        double rightBoundary = leftBoundary + lane.GetWidth(absolutS);

        if (absT >= leftBoundary && absT <= rightBoundary)
        {
            auto interpolatedPoint = lane.GetInterpolatedPointsAtDistance(absolutS);
            double dir = lane.GetDirection(absolutS);

            double x = interpolatedPoint.left.x + (absT - leftBoundary) * sin(dir);
            double y = interpolatedPoint.left.y + (absT - leftBoundary) * -cos(dir);
            double yaw = dir + hdg;

            return std::make_tuple(true, x, y, yaw);
        }

        leftBoundary = rightBoundary;
    }

    //Note: Only t <= roadWidth supported
    return std::make_tuple(false, 0, 0, 0);
}



void SceneryConverter::CreateRoads()
{
    for (auto roadEntry : scenery->GetRoads())
    {
        RoadInterface& odRoad = *(roadEntry.second);

        worldData.AddRoad(odRoad);

        for (auto section : odRoad.GetLaneSections())
        {
            RoadLaneSectionInterface& odSection = *section;

            worldData.AddSection(odRoad, odSection);

            for (auto laneEntry : odSection.GetLanes())
            {
                RoadLaneInterface& odLane = *(laneEntry.second);

                worldData.AddLane(odSection, odLane);
            }
        }
    }
}
