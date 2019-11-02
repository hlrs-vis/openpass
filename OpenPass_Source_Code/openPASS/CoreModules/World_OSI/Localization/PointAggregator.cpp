/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "PointAggregator.h"

namespace World {
namespace Localization {

PointAggregator::PointAggregator() :
    streamCollector{streamAggregation, rawPoints}
{
}

void PointAggregator::Add(const uint64_t osiRoadId,
                          const uint64_t streamId,
                          const OWL::Lane& lane,
                          const RoadPosition coordinate,
                          const OWL::PointType& pointType)
{
    RoadPosition tmp = coordinate;
    tmp.s = std::max(0.0, tmp.s);

    switch (pointType)
    {
        case OWL::PointType::Reference:
            referenceRoadId = osiRoadId;
            referenceLaneId = lane.GetId();
            reference = tmp;
            SetAligned(reference.hdg);
            referenceSet = true;
            break;
        case OWL::PointType::MainLaneLocator:
            mainRoadId = osiRoadId;
            mainLaneId = lane.GetId();
            mainLaneLocatorSet = true;
            break;
        case OWL::PointType::CornerLeftRear:
            cornerLeftRearSet = true;
            break;
        case OWL::PointType::CornerLeftFront:
            cornerLeftFrontId = lane.GetId();
            cornerLeftFrontSet = true;
            break;
        case OWL::PointType::CornerRightFront:
            cornerRightFrontId = lane.GetId();
            cornerRightFrontSet = true;
            break;
        case OWL::PointType::CornerRightRear:
            cornerRightRearSet = true;
            break;
        case OWL::PointType::EdgeLeft:
            break;
        case OWL::PointType::EdgeRight:
            break;
    }

    streamAggregation[streamId].push_back(SearchResult{lane, OWL::LocatedPoint{tmp, pointType}});
    rawPoints.push_back(SearchResult{lane, OWL::LocatedPoint{tmp, pointType}});
}

const RoadPosition PointAggregator::GetReference() const
{
    return reference;
}

bool PointAggregator::IsCrossingLanes() const
{
    return GetStreamType() == StreamType::Neighbours;
}

bool PointAggregator::AllPointsLocated() const
{
    return referenceSet &&
           mainLaneLocatorSet &&
           cornerLeftRearSet &&
           cornerLeftFrontSet &&
           cornerRightFrontSet &&
           cornerRightRearSet;
}

StreamType PointAggregator::GetStreamType() const
{
    // most likly case, therefore first check
    if (streamAggregation.size() == 1)
    {
        return StreamType::Single;
    }

    if (streamAggregation.empty())
    {
        return StreamType::Empty;
    }

    return StreamType::Neighbours;
}

void PointAggregator::SetAligned(double yaw)
{
    aligned = std::abs(yaw) <= M_PI_2;
}

void PointAggregator::Finalize()
{
    streamIds.clear();

    if (streamAggregation.empty())
    {
        return;
    }

    streamIds = streamCollector.GetTouchedElements();
}

const std::pair<std::list<SearchResult>, std::list<SearchResult>> PointAggregator::GetBoundaryStreams() const
{
    // most likly case, therefore first check
    if (streamAggregation.size() == 1)
    {
        return std::make_pair(streamAggregation.cbegin()->second, std::list<SearchResult>());
    }

    if (streamAggregation.empty())
    {
        return std::make_pair(std::list<SearchResult>(), std::list<SearchResult>());
    }

    return std::make_pair(streamAggregation.cbegin()->second, streamAggregation.crbegin()->second);
}

bool PointAggregator::IsAligned() const
{
    return aligned;
}

bool PointAggregator::IsLocalizable() const
{
    return mainLaneLocatorSet &&
           referenceSet &&
           (cornerLeftFrontSet || cornerRightFrontSet) &&
           (cornerLeftRearSet || cornerRightRearSet);
}

bool PointAggregator::MissingMainLaneLocator() const
{
    return !mainLaneLocatorSet;
}

bool PointAggregator::MissingReference() const
{
    return !referenceSet;
}

bool PointAggregator::MissingCornerLeftFront() const
{
    return !cornerLeftFrontSet;
}

bool PointAggregator::MissingCornerLeftRear() const
{
    return !cornerLeftRearSet;
}

bool PointAggregator::MissingCornerRightFront() const
{
    return !cornerRightFrontSet;
}

bool PointAggregator::MissingCornerRightRear() const
{
    return !cornerRightRearSet;
}

std::vector<const OWL::Lane*> PointAggregator::GetTouchedLanes() const
{
    return streamCollector.GetTouchedLanes();
}

const OWL::Lane &StreamCollector::GetFirstUpstream()
{
    assert(!rawPoints.empty());
    const SearchResult* firstUpstream = &rawPoints.front();

    double firstS = std::numeric_limits<double>::max();

    for (const auto& searchResult : rawPoints)
    {
        if (searchResult.point.coordinate.s < firstS)
        {
            firstS = searchResult.point.coordinate.s;
            firstUpstream = &searchResult;
        }

    }
    return firstUpstream->lane;
}

const OWL::Lane &StreamCollector::GetLastDownstream()
{
    assert(!rawPoints.empty());
    const SearchResult* lastDownstream = &rawPoints.front();

    double lastS = std::numeric_limits<double>::lowest();

    for (const auto& searchResult : rawPoints)
    {
        if (searchResult.point.coordinate.s > lastS)
        {
            lastS = searchResult.point.coordinate.s;
            lastDownstream = &searchResult;
        }

    }
    return lastDownstream->lane;
}

void StreamCollector::RemoveLanesWithUntouchedStreams()
{
    touchedLanes.erase(
                std::remove_if(touchedLanes.begin(),
                               touchedLanes.end(),
                               [&](const OWL::Lane * lane)
    {
        return (std::find(s.begin(),
                          s.end(),
                          lane->GetStreamId()) == s.end());
    }),
                touchedLanes.end());
}

bool StreamCollector::IsConnectedLeft(const OWL::Lane &lane, uint64_t streamId) const
{
    OWL::Lane* neigbouringLane = const_cast<OWL::Lane*>(&lane.GetLeftLane());
    while (neigbouringLane->GetId() != OWL::InvalidId)
    {
        if (neigbouringLane->GetStreamId() == streamId)
        {
            return true;
        }
        neigbouringLane = const_cast<OWL::Lane*>(&neigbouringLane->GetLeftLane());
    }
    return false;
}

void StreamCollector::TrimTouchedStreams()
{
    MakeUnique(touchedStreams);
    s.clear();

    for (auto streamId : touchedStreams)
    {
        if (s.empty()) // first element
        {
            s.push_back(streamId);
            continue;
        }

        for (auto lane : touchedLanes)
        {
            // don't compare within same stream
            if (lane->GetStreamId() == streamId)
            {
                continue;
            }

            if (IsConnectedLeft(*lane, streamId))
            {
                s.push_front(streamId);
                break;
            }
            else //is connected right => if not cluster -> yet not implemented!
            {
                s.push_back(streamId);
                break;
            }
        }
    }

    int offsetFirst = -1;
    int offsetLast = -1;
    int i = 0;

    for (auto streamId : s)
    {
        if (streamAggregation.find(streamId) != streamAggregation.end())
        {
            if (offsetFirst < 0)
            {
                offsetFirst = i;
            }
            offsetLast = i + 1;
        }
        ++i;
    }

    s = std::deque<OWL::Id>(s.begin() + offsetFirst, s.begin() + offsetLast);
}

bool StreamCollector::CollectLanesAndStreamIds(const OWL::Section &section, OWL::Id lastId)
{
    bool lastReached = false;

    for (auto lane : section.GetLanes())
    {
        if (lane->GetId() == lastId)
        {
            lastReached = true;
        }

        touchedStreams.push_back(lane->GetStreamId());
        touchedLanes.push_back(lane);
    }

    return lastReached;
}

void StreamCollector::CollectPotentiallyTouchedLanesAndStreams()
{
    touchedStreams.clear();
    touchedLanes.clear();

    const auto& firstLane = GetFirstUpstream();
    const auto& lastLane = GetLastDownstream();

    auto currentSection = &firstLane.GetSection();
    auto lastId = lastLane.GetId();

    for (const auto& section : currentSection->forwardSectionStream)
    {
        if (CollectLanesAndStreamIds(section, lastId))
        {
            break;
        }
    }
}

std::deque<OWL::Id> StreamCollector::GetTouchedElements()
{
    CollectPotentiallyTouchedLanesAndStreams();
    TrimTouchedStreams();
    RemoveLanesWithUntouchedStreams();
    return s;
}

}
}
