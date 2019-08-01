/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <list>
#include <vector>
#include <memory>

#include "PointLocator.h"

namespace World {
namespace Localization {

PointLocator::PointLocator(PointAggregator& pointAggregator,
                           const OWL::Interfaces::Section* section) :
    pointAggregator(pointAggregator),
    section(section)
{
}

void PointLocator::Locate(const std::list<OWL::SearchablePoint>& searchPoints, double maxDistance)
{
    InitLocate(searchPoints);

    const auto& forwardSectionStream = section->forwardSectionStream;
    const auto& reverseSectionStream = section->reverseSectionStream;

    for (const auto& currSection : forwardSectionStream)
    {
        if(!searchPoints.empty())
        {
            section = &currSection;
            WrapLanes(section->IsInStreamDirection());
            CalculateMaxStepsAfterFirstHit(maxDistance);
            LocatePoints();
        }
    }

    if(!searchPoints.empty())
    {
        for (const auto& currSection : reverseSectionStream)
        {
            if(!searchPoints.empty())
            {
                section = &currSection;
                WrapLanes(!section->IsInStreamDirection());
                CalculateMaxStepsAfterFirstHit(maxDistance);
                LocatePoints();
            }
        }
    }
}

void PointLocator::InitLocate(const std::list<OWL::SearchablePoint>& searchPoints)
{
    this->searchPoints = {searchPoints};
    firstHit = false;
    searchInitializer = {};
}

void PointLocator::SaveSearchInitializerOnFirstHit()
{
    if (firstHit == false)
    {
        SaveSearchInitializer();
        RewindWalkers(searchInitializer.upstreamLaneWalkers);
        RewindWalkers(searchInitializer.downstreamLaneWalkers);
    }

    firstHit = true;
}

void PointLocator::LocatePoints()
{
    while (!searchPoints.empty() && stepsAfterFirstHit <= maxStepsAfterFirstHit)
    {
        bool hasDownstreamLaneWalker = !downstreamLaneWalkers.at(downstreamLaneWalkers.size() - 1).IsDepleted();
        bool hasUpstreamLaneWalker = !upstreamLaneWalkers.at(upstreamLaneWalkers.size() - 1).IsDepleted();

        if (!hasUpstreamLaneWalker && !hasDownstreamLaneWalker)
        {
            break;
        }
        GeometryProcessor gpDownstream(downstreamLaneWalkers.at(indexDownstreamWalker));
        GeometryProcessor gpUpstream(upstreamLaneWalkers.at(indexUpstreamWalker));

        auto searchPointsIterator = searchPoints.begin();
        while (searchPointsIterator != searchPoints.end())
        {
            auto searchPoint = *searchPointsIterator;
            const Common::Vector2d point{searchPoint.coordinate.x, searchPoint.coordinate.y};

            if (hasDownstreamLaneWalker && gpDownstream.Match(point))
            {
                const auto roadCoordinate = gpDownstream.GetRoadCoordinate(point, searchPoint.coordinate.hdg);
                pointAggregator.Add(section->GetRoad().GetId(),
                                    downstreamLaneWalkers.at(indexDownstreamWalker).GetLane().GetStreamId(),
                                    downstreamLaneWalkers.at(indexDownstreamWalker).GetLane(),
                                    roadCoordinate,
                                    searchPoint.pointType);

                sectionOffset = roadCoordinate.s;
                SaveSearchInitializerOnFirstHit();

                searchPointsIterator = searchPoints.erase(searchPointsIterator);
            }
            else if (hasUpstreamLaneWalker && gpUpstream.Match(point))
            {
                const auto roadCoordinate = gpUpstream.GetRoadCoordinate(point, searchPoint.coordinate.hdg);
                pointAggregator.Add(section->GetRoad().GetId(),
                                    upstreamLaneWalkers.at(indexUpstreamWalker).GetLane().GetStreamId(),
                                    upstreamLaneWalkers.at(indexUpstreamWalker).GetLane(),
                                    roadCoordinate,
                                    searchPoint.pointType);

                sectionOffset = roadCoordinate.s;
                SaveSearchInitializerOnFirstHit();

                searchPointsIterator = searchPoints.erase(searchPointsIterator);
            }
            else
            {
                searchPointsIterator++;
            }
        }
        Step();
    }

    downstreamLaneWalkers.clear();
    upstreamLaneWalkers.clear();
    stepsAfterFirstHit = 0;
}

void PointLocator::Step()
{
    if (firstHit)
    {
        ++stepsAfterFirstHit;
    }

    downstreamLaneWalkers.at(indexDownstreamWalker).Step();
    upstreamLaneWalkers.at(indexUpstreamWalker).Step();

    indexDownstreamWalker = (indexDownstreamWalker + 1) % downstreamLaneWalkers.size();
    indexUpstreamWalker = (indexUpstreamWalker + 1) % upstreamLaneWalkers.size();
}

void PointLocator::WrapLanes(bool startAtRoadStart)
{
    if (!(upstreamLaneWalkers.empty() && downstreamLaneWalkers.empty()))
    {
        return;
    }

    for (const auto lane : section->GetLanes())
    {
        upstreamLaneWalkers.push_back(LaneWalker(*lane, false, startAtRoadStart));
        downstreamLaneWalkers.push_back(LaneWalker(*lane, true, startAtRoadStart));
    }

    indexDownstreamWalker = 0;
    indexUpstreamWalker = 0;
}

void PointLocator::CalculateMaxStepsAfterFirstHit(double maxDistance)
{
    const auto& firstLaneElement = *section->GetLanes().front()->GetLaneGeometryElements().begin();
    double jointsDistance =  std::abs(firstLaneElement->joints.next.projectionAxes.sOffset - firstLaneElement->joints.current.projectionAxes.sOffset);

    constexpr double buffer = 2.0;  //on the inside of a curved road the s coordinate advances faster then the euclidean distance. Therefore we add an aditional buffer.
    maxStepsAfterFirstHit = static_cast<int>(std::ceil(maxDistance / jointsDistance * buffer)) * section->GetLanes().size();
}

bool PointLocator::AllPointsLocated()
{
    return searchPoints.empty();
}

void PointLocator::SaveSearchInitializer()
{
    searchInitializer = SearchInitializer(
                true,
                section,
                sectionOffset,
                upstreamLaneWalkers,
                downstreamLaneWalkers
                );
}

SearchInitializer PointLocator::GetSearchInitializer() const
{
    return searchInitializer;
}

void PointLocator::SetSearchInitializer(SearchInitializer searchInitializer)
{
    section = searchInitializer.section;
    sectionOffset = searchInitializer.sectionOffset;
    upstreamLaneWalkers = searchInitializer.upstreamLaneWalkers;
    downstreamLaneWalkers = searchInitializer.downstreamLaneWalkers;
}

void PointLocator::RewindWalkers(LaneWalkers& laneWalkers)
{
    for (auto& laneWalker : laneWalkers)
    {
        laneWalker.Rewind(sectionOffset);
    }
}

} // namespace Localization
} // namespace World
