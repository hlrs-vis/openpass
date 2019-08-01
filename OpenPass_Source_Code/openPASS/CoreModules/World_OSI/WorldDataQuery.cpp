/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "WorldDataQuery.h"

WorldDataQuery::WorldDataQuery(OWL::Interfaces::WorldData& worldData) : worldData{worldData}
{}

double WorldDataQuery::GetDistanceToEndOfLane(std::string roadId, OWL::OdId laneId, double initialSearchPosition, double maxSearchLength, std::list<OWL::LaneType> requestedLaneTypes) const
{
    initialSearchPosition = std::max(0.0, initialSearchPosition); //supress negative search distances
    const auto* lane = &GetLaneByOdId(roadId, laneId, initialSearchPosition);
    if (lane->Exists() == false)
    {
        return 0.0;
    }

    if (std::find(requestedLaneTypes.begin(), requestedLaneTypes.end(), lane->GetLaneType()) == requestedLaneTypes.end())
    {
        return 0.0;
    }

    double distance = 0.0;
    if (lane->IsInStreamDirection())
    {
        distance = lane->GetDistance(OWL::MeasurementPoint::RoadEnd) - initialSearchPosition;
        lane = lane->GetNext();
    }
    else
    {
        distance = initialSearchPosition - lane->GetDistance(OWL::MeasurementPoint::RoadStart);
        lane = lane->GetPrevious();
    }
    if (distance > maxSearchLength)
    {
        return static_cast<double>(INFINITY);
    }

    if(!lane)
    {
        return distance;
    }

    for (const auto& laneStream : lane->forwardLaneStream)
    {
        if (std::find(requestedLaneTypes.begin(), requestedLaneTypes.end(), laneStream.GetLaneType()) == requestedLaneTypes.end())
        {
            return distance;
        }

        distance += laneStream.GetLength();

        if (distance > maxSearchLength)
        {
            return static_cast<double>(INFINITY);
        }
    }

    return distance;
}

OWL::Id WorldDataQuery::GetStreamId(OWL::CLane& lane) const
{
    for (const auto& laneStream : lane.reverseLaneStream)
    {
        if (!laneStream.GetPrevious())
        {
            return laneStream.GetId();
        }
    }

    return -999;
}

std::pair<double, double> WorldDataQuery::GetLimitingDistances(OWL::CLane& lane) const
{
    double lowerDistance = 0.0;
    double upperDistance = 0.0;;

    if (lane.Exists())
    {
        for (const auto& laneStream : lane.forwardLaneStream)
        {
            if (!laneStream.GetNext())
            {
                upperDistance = laneStream.GetDistance(OWL::MeasurementPoint::RoadEnd);
            }
        }

        for (const auto& laneStream : lane.reverseLaneStream)
        {
            if (!laneStream.GetPrevious())
            {
                lowerDistance = laneStream.GetDistance(OWL::MeasurementPoint::RoadStart);
            }
        }
    }

    return std::make_pair(lowerDistance, upperDistance);
}

double WorldDataQuery::GetDistanceToStartOfLane(std::string roadId, OWL::OdId laneId, double initialSearchDistance,
        double maxSearchLength) const
{
    if (initialSearchDistance <= 0.0)
    {
        return 0.0;
    }

    const auto& lane = GetLaneByOdId(roadId, laneId, initialSearchDistance);
    if (lane.Exists() == false)
    {
        return 0.0;
    }

    double distance = 0.0;

    for (const auto& laneStream : lane.reverseLaneStream)
    {
        distance = initialSearchDistance - laneStream.GetDistance(OWL::MeasurementPoint::RoadStart);
        if (distance > maxSearchLength)
        {
            return static_cast<double>(INFINITY);
        }
    }

    return distance;
}

OWL::CSection* WorldDataQuery::GetSectionByDistance(std::string odRoadId, double distance) const
{
    for (const auto road : worldData.GetRoads())
    {
        if (worldData.GetRoadIdMapping().at(road.first) == odRoadId)
        {
            for (auto tmpSection : road.second->GetSections())
            {
                if (tmpSection->Covers(distance))
                {
                    return tmpSection;
                }
            }
        }
        continue;
    }

    return nullptr;
}


OWL::CLanes WorldDataQuery::GetLanesOfLaneTypeAtDistance(std::string roadId, double distance, std::list<OWL::LaneType> requestedLaneTypes) const
{
    std::list<OWL::CLane*> lanes;
    OWL::CSection* sectionAtDistance = GetSectionByDistance(roadId, distance);

    if (sectionAtDistance)
    {
        for (auto lane : sectionAtDistance->GetLanes())
        {
            if (std::find(requestedLaneTypes.begin(), requestedLaneTypes.end(), lane->GetLaneType()) != requestedLaneTypes.end())
            {
                lanes.push_back(lane);
            }
        }
    }

    return lanes;
}


bool WorldDataQuery::ExistsDrivingLaneOnSide(std::string roadId, OWL::OdId laneId, double distance, Side side)
{
    OWL::CLane& lane = GetLaneByOdId(roadId, laneId, distance);

    if (side == Side::Left)
    {
        return lane.GetLeftLane().Exists() && (lane.GetLeftLane().GetLaneType() == OWL::LaneType::Driving);
    }
    else
    {
        return lane.GetRightLane().Exists() && (lane.GetRightLane().GetLaneType() == OWL::LaneType::Driving);
    }
}

OWL::CLane& WorldDataQuery::GetLaneByOdId(std::string roadId, OWL::OdId odLaneId, double distance) const
{
    auto section = GetSectionByDistance(roadId, distance);
    if (!section)
    {
        return worldData.GetInvalidLane();
    }

    for (const OWL::Lane* lane : section->GetLanes())
    {
        // if a section covers a point the lanes also do
        if (worldData.GetLaneIdMapping().at(lane->GetId()) == odLaneId)
        {
            return *lane;
        }
    }

    return worldData.GetInvalidLane();
}


bool WorldDataQuery::IsSValidOnLane(std::string roadId, OWL::OdId laneId, double distance)
{
    if (distance < 0)
    {
        return false;
    }

    return GetLaneByOdId(roadId, laneId, distance).Exists();
}

int WorldDataQuery::GetNumberOfLanes(std::string roadId, double distance)
{
    auto section = GetSectionByDistance(roadId, distance);
    if(!section)
    {
        return 0;
    }
    return section->GetLanes().size();
}


double WorldDataQuery::GetNextValidSOnLaneInDownstream(std::string roadId, OWL::OdId laneId, double initialDistance,
        double stepSizeLookingForValidS)
{
    double maxSearchLength = OWL::EVENTHORIZON;
    double nextValidS = initialDistance + stepSizeLookingForValidS;
    double maxDistance = initialDistance + maxSearchLength;

    while (!IsSValidOnLane(roadId, laneId, nextValidS) && nextValidS < maxDistance)
    {
        nextValidS += stepSizeLookingForValidS;
    }

    if (nextValidS >= maxDistance)
    {
        return static_cast<double>(INFINITY);
    }

    return nextValidS;
}

double WorldDataQuery::GetLastValidSInUpstream(std::string roadId, OWL::OdId laneId, double initialDistance, double stepSizeLookingForValidS)
{
    double maxSearchLength = OWL::EVENTHORIZON;
    double lastValidSOnLane = initialDistance - stepSizeLookingForValidS;
    double minDistance = std::max(0.0, initialDistance - maxSearchLength);

    while (!IsSValidOnLane(roadId, laneId, lastValidSOnLane) && lastValidSOnLane > minDistance)
    {
        lastValidSOnLane -= stepSizeLookingForValidS;
    }

    if (lastValidSOnLane < minDistance)
    {
        return static_cast<double>(-INFINITY);
    }

    return lastValidSOnLane;
}


std::vector<const OWL::Interfaces::WorldObject*> WorldDataQuery::GetMovingObjectsInRange(std::string roadId, OWL::OdId laneId,
        double startDistance, double endDistance) const
{
    return GetObjectsOfTypeInRange<OWL::Interfaces::MovingObject>(roadId, laneId, startDistance, endDistance);
}


std::vector<const OWL::Interfaces::WorldObject*> WorldDataQuery::GetWorldObjectsInRange(std::string roadId, OWL::OdId laneId,
        double startDistance, double endDistance) const
{
    return GetObjectsOfTypeInRange<OWL::Interfaces::WorldObject>(roadId, laneId, startDistance, endDistance);
}

const OWL::Interfaces::WorldObject* WorldDataQuery::GetFirstObjectDownstream(OWL::Id streamId) const
{
    auto lane = worldData.GetLanes().at(streamId);

    for (const auto& lane : lane->forwardLaneStream)
    {
        OWL::Interfaces::WorldObjects worldObjects = lane.GetWorldObjects();
        worldObjects.sort([](const OWL::Interfaces::WorldObject * const wo1,
                             const OWL::Interfaces::WorldObject * const wo2)
        {
            return wo1->GetDistance(OWL::MeasurementPoint::RoadStart) <
                   wo2->GetDistance(OWL::MeasurementPoint::RoadStart);
        });

        if (!worldObjects.empty())
        {
            return worldObjects.front();
        }
    }

    return nullptr;
}

OWL::OdId WorldDataQuery::GetLaneId(OWL::Id streamId, double endDistance) const
{
    auto lane = worldData.GetLanes().at(streamId);

    for (const auto& lane : lane->forwardLaneStream)
    {
        if (lane.Covers(endDistance))
        {
            return worldData.GetLaneIdMapping().at(lane.GetId());
        }
    }

    return -999;
}

std::vector<OWL::Interfaces::TrafficSign *> WorldDataQuery::GetTrafficSignsInRange(std::string roadId, OWL::OdId laneId, double startDistance, double searchRange) const
{
    std::vector<OWL::Interfaces::TrafficSign*> foundTrafficSigns{};    

    const std::unordered_map<OWL::Id, OWL::Interfaces::TrafficSign*> trafficSigns = worldData.GetTrafficSigns();
    for (auto trafficSign : trafficSigns)
    {
        double signDistance = trafficSign.second->GetS();
        OWL::Id id = GetLaneByOdId(roadId, laneId, signDistance).GetId();
        if(searchRange >= 0)
        {
            if (signDistance >= startDistance && signDistance <= startDistance + searchRange && trafficSign.second->IsValidForLane(id))
            {
                foundTrafficSigns.push_back(trafficSign.second);
            }
        }
        else
        {
            if (signDistance <= startDistance && signDistance >= startDistance + searchRange && trafficSign.second->IsValidForLane(id))
            {
                foundTrafficSigns.push_back(trafficSign.second);
            }
        }

    }

    return foundTrafficSigns;
}

LaneQueryResult WorldDataQuery::QueryLane(std::string roadId, int laneId, double distance) const
{
    const auto& lane = GetLaneByOdId(roadId, laneId, distance);
    return BuildLaneQueryResult(lane);
}

std::list<LaneQueryResult> WorldDataQuery::QueryLanes(std::string roadId, double startDistance, double endDistance) const
{
    std::list<LaneQueryResult> laneQueryResults;
    for (const auto& road : worldData.GetRoads())
    {
        if (worldData.GetRoadIdMapping().at(road.first) == roadId)
        {
            for (auto section : road.second->GetSections())
            {
                if (section->CoversInterval(startDistance, endDistance))
                {
                    for (const auto& lane : section->GetLanes())
                    {
                        uint64_t streamId = GetStreamId(*lane);
                        auto streamMatcher = [streamId](const LaneQueryResult & queryResult)
                        {
                            return queryResult.streamId == streamId;
                        };

                        if (std::find_if(laneQueryResults.begin(),
                                         laneQueryResults.end(),
                                         streamMatcher) == laneQueryResults.end())
                        {
                            laneQueryResults.push_back(BuildLaneQueryResult(*lane));
                        }
                    }
                }
            }
            continue;
        }
    }

    return laneQueryResults;
}

LaneQueryResult WorldDataQuery::BuildLaneQueryResult(OWL::CLane& lane) const
{
    if (lane.Exists())
    {
        auto streamId = GetStreamId(lane);
        auto limits = GetLimitingDistances(lane);
        auto laneCategory = (lane.GetRightLaneCount() == 0) ?
                            LaneCategory::RightMostLane :
                            LaneCategory::RegularLane ;
        auto isDrivingLane = lane.GetLaneType() == OWL::LaneType::Driving;


        return LaneQueryResult(streamId, limits.first, limits.second, laneCategory, isDrivingLane);
    }

    return LaneQueryResult::InvalidResult();
}

std::pair<bool, double> WorldDataQuery::GetLateralDistance(GlobalRoadPosition src, GlobalRoadPosition dst) const
{
    OWL::CLane& srcLane = GetLaneByOdId(src.roadId, src.laneId, src.roadPosition.s);
    OWL::CLane& dstLane = GetLaneByOdId(dst.roadId, dst.laneId, dst.roadPosition.s);

    if (srcLane.GetStreamId() == dstLane.GetStreamId())
    {
        return {true, dst.roadPosition.t - src.roadPosition.t};
    }
    else
    {
        auto sourceStreamId = srcLane.GetStreamId();
        auto referencePosition = dst.roadPosition.s;

        // Check if source stream is a left neighbor of the destination stream and collect width of intermediates
        auto resultWidthLeft = GetWidthOfIntermediateLanesLeft(dstLane, referencePosition, sourceStreamId);
        if (std::get<0>(resultWidthLeft))
        {
            auto intermediateLanesWidth = std::get<1>(resultWidthLeft);
            auto srcLaneAtReference = std::get<2>(resultWidthLeft);

            auto srcWidth = srcLaneAtReference->GetWidth(referencePosition);
            auto dstWidth = dstLane.GetWidth(referencePosition);

            // note: this is a prognosis w.r.t. the projected t displacement of the source
            //       this means, that if the lane itself gets smaller, the first term might be negative
            double lateralDistance = (srcWidth / 2 + src.roadPosition.t) +
                    (dstWidth / 2 - dst.roadPosition.t) +
                    intermediateLanesWidth;

            return {true, lateralDistance};
        }

        // Check if source stream is a right neighbor of the destination stream and collect width of intermediates
        auto resultWidthRight = GetWidthOfIntermediateLanesRight(dstLane, referencePosition, sourceStreamId);
        if (std::get<0>(resultWidthRight))
        {
            auto intermediateLanesWidth = std::get<1>(resultWidthRight);
            auto srcLaneAtReference = std::get<2>(resultWidthRight);

            auto srcWidth = srcLaneAtReference->GetWidth(referencePosition);
            auto dstWidth = dstLane.GetWidth(referencePosition);

            // note: this is a prognosis w.r.t. the projected t displacement of the source
            //       this means, that if the lane itself gets smaller, the first term might be negative
            double lateralDistance = (srcWidth / 2 - src.roadPosition.t) +
                    (dstWidth / 2 + dst.roadPosition.t) +
                    intermediateLanesWidth;

            return {true, lateralDistance};
        }

        // Source and destination stream are unrelated (no neighboring connectin): Cannot measure lateral distance
        return {false, 0.0};
    }
}

std::tuple<bool, double, const OWL::Lane * const> WorldDataQuery::GetWidthOfIntermediateLanesLeft(OWL::CLane &lane, double distance, OWL::Id targetStreamId) const
{
    double width = 0.0;

    auto* neighboringLane = const_cast<OWL::Lane*>(&lane.GetLeftLane());
    while (neighboringLane->GetId() != OWL::InvalidId)
    {
        if (neighboringLane->GetStreamId() == targetStreamId)
        {
            return {true, width, neighboringLane};
        }
        else
        {
            width += neighboringLane->GetWidth(distance);
        }
        neighboringLane = const_cast<OWL::Lane*>(&neighboringLane->GetLeftLane());
    }

    return {false, 0.0, nullptr};
}

std::tuple<bool, double, const OWL::Lane * const> WorldDataQuery::GetWidthOfIntermediateLanesRight(OWL::CLane &lane, double distance, OWL::Id targetStreamId) const
{
    double width = 0.0;

    auto* neighboringLane = const_cast<OWL::Lane*>(&lane.GetRightLane());
    while (neighboringLane->GetId() != OWL::InvalidId)
    {
        if (neighboringLane->GetStreamId() == targetStreamId)
        {
            return {true, width, neighboringLane};
        }
        else
        {
            width += neighboringLane->GetWidth(distance);
        }
        neighboringLane = const_cast<OWL::Lane*>(&neighboringLane->GetRightLane());
    }

    return {false, 0.0, nullptr};
}
