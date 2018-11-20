/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <QFile>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

#include "SceneryConverter.h"
#include "GeometryConverter.h"
#include "WorldData.h"

SceneryConverter::SceneryConverter(SceneryInterface* scenery,
                                   OWL::WorldData& worldData,
                                   const CallbackInterface* callbacks) :
    scenery(scenery),
    worldData(worldData),
    callbacks(callbacks)
{}

RoadLaneInterface *SceneryConverter::GetOtherLane(RoadLaneSectionInterface *otherSection,
                                         int otherId)
{
    // search for connected lane in OpenDrive data structures since LaneId of
    // LINEAR_LANE Objects might be reordered according to direction definition
    RoadLaneInterface *connectedRoadLane;

    for(auto findIt = otherSection->GetLanes().begin();
        otherSection->GetLanes().end() != findIt;
        ++findIt)
    {
        connectedRoadLane = findIt->second;
        if(connectedRoadLane->GetId() == otherId)
        {
            return connectedRoadLane;
        }
    }

    return nullptr;
}

RoadInterface *SceneryConverter::GetConnectedRoad(RoadLinkInterface *roadLink)
{
    if(ContactPointType::Start != roadLink->GetContactPoint() &&
            ContactPointType::End != roadLink->GetContactPoint())
    {
        LOG(CbkLogLevel::Error, "no contact point defined for road link");
        return nullptr;
    }

    RoadInterface *connectedRoad = nullptr;
    for(auto &item : scenery->GetRoads())
    {
        RoadInterface *itemRoad = item.second;
        if(roadLink->GetElementId() == itemRoad->GetId())
        {
            connectedRoad = itemRoad;
            break;
        }
    }

    if(!connectedRoad)
    {
        LOG(CbkLogLevel::Error, "missing road for ElementId " + roadLink->GetElementId());
        return nullptr;
    }

    return connectedRoad;
}

void SceneryConverter::MarkDirectionRoad(RoadInterface *road,
                                         bool inDirection)
{
    LOG(CbkLogLevel::Debug, "direction of road " + road->GetId() + ": "
        + (inDirection ? "true" : "false"));

    road->SetInDirection(inDirection);

    for(RoadLaneSectionInterface *roadLaneSection : road->GetLaneSections())
    {
        roadLaneSection->SetInDirection(inDirection);

        for(auto &item : roadLaneSection->GetLanes())
        {
            RoadLaneInterface *roadLane = item.second;
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
                   [](const std::pair<std::string, RoadInterface*> &item){ return item.second; });

    while(!pendingRoads.empty()) // proceed until all roads have been marked
    {
        RoadInterface *road = pendingRoads.front();

        std::list<RoadInterface*> tmpCluster; // contains currently processed cluster
        tmpCluster.splice(tmpCluster.end(), pendingRoads, pendingRoads.begin());

        MarkDirectionRoad(road,
                          true); // first road in cluster defines direction within cluster

        LOG(CbkLogLevel::Debug, "process road cluster");

        while(!tmpCluster.empty()) // process current cluster
        {
            road = tmpCluster.front();
            tmpCluster.pop_front();

            if(road->GetLaneSections().empty())
            {
                LOG(CbkLogLevel::Error, "no sections given for road " + road->GetId());
                return false;
            }

            // collect all road links for this road and save them in this mapping
            std::map<std::tuple<RoadInterface*, RoadInterface*>, std::tuple<RoadLinkType, ContactPointType>> collectedRoadLinks;
            for(RoadLinkInterface *roadLink : road->GetRoadLinks())
            {
                if(RoadLinkElementType::Road == roadLink->GetElementType())
                {
                    RoadInterface *connectedRoad = GetConnectedRoad(roadLink);
                    if(!connectedRoad)
                    {
                        return false;
                    }

                    if(road == connectedRoad)
                    {
                        LOG(CbkLogLevel::Error, "self references are not supported, road " + road->GetId());
                        return false;
                    }

                    // if the connection between these two roads is found for the first time,
                    // add it to the mapping
                    if(0 == collectedRoadLinks.count(std::make_tuple(road, connectedRoad)) &&
                            0 == collectedRoadLinks.count(std::make_tuple(connectedRoad, road)))
                    {
                        collectedRoadLinks.insert({std::make_tuple(road,
                                                   connectedRoad),
                                                   std::make_tuple(roadLink->GetType(),
                                                   roadLink->GetContactPoint())}); // connectedRoad is not part of a junction
                    }
                }
                else
                {
                    assert(0); // catched by parser
                    return false;
                }
            }

            // process collected road links
            for(auto &item : collectedRoadLinks)
            {
                RoadInterface *connectedRoad = std::get<1>(item.first);
                RoadLinkType connectedRoadLinkType = std::get<0>(item.second);
                ContactPointType connectedContactPointType = std::get<1>(item.second);

                auto findIt = std::find_if(pendingRoads.begin(),
                                           pendingRoads.end(),
                                           [&connectedRoad](RoadInterface *road){ return road == connectedRoad; });
                if(pendingRoads.end() == findIt)
                {
                    continue; // road already processed (no not overwrite content)
                }

                bool connectedRoadInDirection = false;

                if(RoadLinkType::Predecessor != connectedRoadLinkType &&
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
                if(RoadLinkType::Predecessor == connectedRoadLinkType)
                {
                    if(ContactPointType::Start == connectedContactPointType)
                    {
                        connectedRoadInDirection = !roadInDirection;
                    }
                    else if(ContactPointType::End == connectedContactPointType)
                    {
                        connectedRoadInDirection = roadInDirection;
                    }
                }
                else if(RoadLinkType::Successor == connectedRoadLinkType)
                {
                    if(ContactPointType::Start == connectedContactPointType)
                    {
                        connectedRoadInDirection = roadInDirection;
                    }
                    else if(ContactPointType::End == connectedContactPointType)
                    {
                        connectedRoadInDirection = !roadInDirection;
                    }
                }

                MarkDirectionRoad(connectedRoad,
                                  connectedRoadInDirection);

                // update cluster
                auto findIter = std::find(pendingRoads.begin(), pendingRoads.end(), connectedRoad);
                if(pendingRoads.end() != findIter)
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
    for(auto &item : scenery->GetRoads())
    {
        RoadInterface *road = item.second;

        for(RoadLaneSectionInterface *roadLaneSection : road->GetLaneSections())
        {
            roadLaneSection->SetId(linearSectionId);
            ++linearSectionId;
        }
    }

    return true;
}

bool SceneryConverter::ConnectLaneToLane(RoadLaneInterface *currentLane,
                                         ContactPointType currentContactPoint,
                                         RoadLaneInterface *otherLane,
                                         ContactPointType otherContactPoint)
{
    //worldData.ConnectLanes(currentLane, currentContactPoint, otherLane, otherContactPoint);
    // calculate direction parameters
    bool currentDestBegin =
            (ContactPointType::Start == currentContactPoint)
            == currentLane->GetInDirection();
    bool currentPrevConn = currentDestBegin;
    bool otherDestBegin =
            (ContactPointType::Start == otherContactPoint)
            == otherLane->GetInDirection();
    bool otherPrevConn = otherDestBegin;

    if(currentPrevConn)
    {
        worldData.AddLaneSuccessor(*currentLane, *otherLane, !otherDestBegin);
        worldData.AddLanePredecessor(*otherLane, *currentLane, !currentDestBegin);
    }
    else
    {
        worldData.AddLaneSuccessor(*otherLane, *currentLane, !currentDestBegin);
        worldData.AddLanePredecessor(*currentLane, *otherLane, !otherDestBegin);
    }

    return true;
}

bool SceneryConverter::ConnectLaneToSection(RoadLaneInterface *currentLane,
                                            ContactPointType currentContactPoint,
                                            RoadLaneSectionInterface *otherLaneSection,
                                            ContactPointType otherContactPoint)
{
    assert(currentLane->GetId()); // no center lanes

    // process predecessor
    RoadLaneInterface *otherLane = GetOtherLane(otherLaneSection, currentLane->GetPredecessor().front());
    if(otherLane && !ConnectLaneToLane(currentLane,
                                       currentContactPoint,
                                       otherLane,
                                       otherContactPoint))
    {
        LOG(CbkLogLevel::Error, "could not connect lanes");
        return false;
    }

    // process successor
    otherLane = GetOtherLane(otherLaneSection, currentLane->GetSuccessor().front());
    if(otherLane && !ConnectLaneToLane(currentLane,
                                       currentContactPoint,
                                       otherLane,
                                       otherContactPoint))
    {
        LOG(CbkLogLevel::Error, "could not connect lanes");
        return false;
    }

    return true;
}

bool SceneryConverter::ConnectLanes(RoadLaneSectionInterface *firstLaneSection,
                                    ContactPointType firstContactPoint,
                                    RoadLaneSectionInterface *secondLaneSection,
                                    ContactPointType secondContactPoint)
{
    for(auto &item : firstLaneSection->GetLanes())
    {
        if(!ConnectLaneToSection(item.second,
                                 firstContactPoint,
                                 secondLaneSection,
                                 secondContactPoint))
        {
            return false;
        }
    }

    for(auto &item : secondLaneSection->GetLanes())
    {
        if(!ConnectLaneToSection(item.second,
                                 secondContactPoint,
                                 firstLaneSection,
                                 firstContactPoint))
        {
            return false;
        }
    }

    return true;
}

bool SceneryConverter::ConnectRoadExternalWithElementTypeRoad(RoadInterface* road)
{
    for(RoadLinkInterface* roadLink : road->GetRoadLinks())
    {
        if(roadLink->GetElementType() != RoadLinkElementType::Road) continue;

        if(roadLink->GetType() == RoadLinkType::Neighbor)
        {
            LOG(CbkLogLevel::Warning, "RoadLinkType 'Neighbor' not supported!");
            continue;
        }

        RoadInterface* otherRoad = scenery->GetRoad(roadLink->GetElementId());

        bool connectsToOtherStart = roadLink->GetContactPoint() == ContactPointType::Start;
        bool otherConnectsAtEnd = roadLink->GetType() == RoadLinkType::Successor;

        if(otherConnectsAtEnd)
        {
            worldData.SetRoadSuccessor(*road, *otherRoad);
        }
        else
        {
            worldData.SetRoadPredecessor(*road, *otherRoad);
        }

        RoadLaneSectionInterface* section = road->GetLaneSections().front();
        RoadLaneSectionInterface* otherSection = otherRoad->GetLaneSections().back();

        if(!connectsToOtherStart)
        {
            std::swap(section, otherSection);
        }

        worldData.ConnectSections(*section, *otherSection);

        //TGet succesor for each lane and add it to lanemapping
        std::map<int, int> laneIdMapping;
        for (auto &laneEntry: section->GetLanes()) {
            RoadLaneInterface* lane = laneEntry.second;
            std::list<int> succesor = lane->GetSuccessor();
            if(succesor.size()==1)
            {
                laneIdMapping[lane->GetId()] = succesor.front();
            }
        }

        worldData.ConnectLanes(*section, *otherSection, laneIdMapping, connectsToOtherStart);
    }

    return true;
}

bool SceneryConverter::ConnectRoadInternal(RoadInterface *road)
{
    // connect sections within a road
    std::list<RoadLaneSectionInterface*>::const_iterator it = road->GetLaneSections().begin();
    RoadLaneSectionInterface* previousLaneSection = *it;
    ++it;
    while(it != road->GetLaneSections().end()) // skipped for junctions since openDrive connecting roads contain only one lane section
    {
        RoadLaneSectionInterface *laneSection = *it;

        worldData.ConnectSections(*previousLaneSection, *laneSection);

        if(!ConnectLanes(previousLaneSection,
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

bool SceneryConverter::ConnectRoads()
{
    for(auto &item : scenery->GetRoads())
    {
        RoadInterface* road = item.second;

        ConnectRoadExternalWithElementTypeRoad(road);

        if(!ConnectRoadInternal(road))
        {
            LOG(CbkLogLevel::Error, "could not connect road " + item.second->GetId());
            return false;
        }
    }

    return true;
}

bool SceneryConverter::Convert()
{
    pendingCluster.clear();

    // define a unique directions of roads/lanes within each road cluster
    if(!MarkDirections())
    {
        return false;
    }

    CreateRoads();
    CreateObjects();

    // connect roads in data layer
    if(!ConnectRoads())
    {
        return false;
    }

    // TODO: fix lane pairings here

    // create geometries
    GeometryConverter converter(scenery,
                                worldData,
                                callbacks);
    if(!converter.Convert())
    {
        return false;
    }

    return true;
}


void SceneryConverter::CreateObjects()
{
    for(auto& item : scenery->GetRoads())
    {
        RoadInterface *road = item.second;

        std::list<RoadLaneSectionInterface*> roadLaneSections = road->GetLaneSections();

        for(RoadObjectInterface* object : road->GetRoadObjects())
        {
            double intervalStart = roadLaneSections.front()->GetStart();
            double s = object->GetS();

            for (auto roadLaneSectionIt = roadLaneSections.begin(); roadLaneSectionIt != roadLaneSections.end(); roadLaneSectionIt++)
            {
                double intervalEnd{INFINITY};

                if (std::next(roadLaneSectionIt) != roadLaneSections.end())
                {
                    intervalEnd = (*std::next(roadLaneSectionIt))->GetStart();
                }

                if( IsWithinLeftClosedInterval(s, intervalStart, intervalEnd) )
                {
                    worldData.AddStationaryObject(*object);
                    break;
                }

                intervalStart = intervalEnd;
            }
        }
    }
}

void SceneryConverter::CreateRoads()
{
    for (auto roadEntry : scenery->GetRoads())
    {
        /* const */ RoadInterface& odRoad = *(roadEntry.second);

        worldData.AddRoad(odRoad);

        for (auto section : odRoad.GetLaneSections())
        {
            // TODO: interface!
            /* const */ RoadLaneSectionInterface& odSection = *section;

            worldData.AddSection(odRoad, odSection);

            for (auto laneEntry : odSection.GetLanes())
            {
                const RoadLaneInterface& odLane = *(laneEntry.second);

                worldData.AddLane(odSection, odLane);
            }
        }
    }
}
