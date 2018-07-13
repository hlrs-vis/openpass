/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <list>
#include <vector>
#include <memory>

#include "GeometryProcessor.h"
#include "LaneWalker.h"
#include "PointAggregator.h"
#include "PointLocator.h"
#include "WorldData.h"

namespace OWL {

using LaneWalkers = std::vector<std::shared_ptr<LaneWalker>>;

PointLocator::PointLocator(World::Localization::PointAggregator& pointAggregator,
                           const OWL::Interfaces::Sections &sections,
                           std::list<SearchablePoint> searchPoints) :
    pointAggregator(pointAggregator),
    sections(sections),
    sectionIterator(sections.end()),
    searchPoints(searchPoints)
{
}

void PointLocator::locate()
{
    establishSearchState();

    for (auto iter = sectionIterator; iter != sections.end(); ++iter)
    {
        sectionIterator = iter;
        section = *sectionIterator;

        if (laneWalkers.empty()) {
            laneWalkers = wrapLanes();
        }

        locatePoint();
        sectionOffset += section->GetLength();
    }
}


void PointLocator::establishSearchState()
{
    if (sectionIterator == sections.end()) {
        sectionIterator = sections.begin();
        sectionOffset = 0;
        laneWalkers.clear();
    }
}


void PointLocator::suppressWalkerSwitchIfBelowMotionPreviewDistance()
{
    double searchedOffset  = currentLaneWalker->GetSOffset() - currentWalkerSOffset;
    if (searchedOffset < motionPreviewDistance) {
        suppressWalkerChange = true;
    }
}

void PointLocator::locatePoint()
{
    removeDepletedWalkers();

    while (laneWalkers.size() > 0)
    {
        setCurrentLaneWalker();
        GeometryProcessor gp(currentLaneWalker->GetLaneGeometryElement());

        auto searchPointsIterator = searchPoints.begin();
        while(searchPointsIterator != searchPoints.end())
        {
            auto searchPoint = *searchPointsIterator;
            const Common::Vector2d point{searchPoint.coordinate.x, searchPoint.coordinate.y};

            if (gp.Match(point))
            {
                const auto roadCoordinate = gp.GetRoadCoordinate(point, searchPoint.coordinate.hdg);
                LocatedPoint searchResult{roadCoordinate, searchPoint.pointType};
                pointAggregator.Add(*section,
                                    sectionOffset,
                                    currentLaneWalker->GetLane(),
                                    searchResult);

                locatedPoints++;
                //referencePointLocated = true;

                //saveSearchInitializer();
                searchPointsIterator = searchPoints.erase(searchPointsIterator);
            }
            else
            {
                searchPointsIterator++;
            }
        }
        //suppressWalkerSwitchIfOnTrack(remainingPoints);
        suppressWalkerSwitchIfBelowMotionPreviewDistance();

        currentLaneWalker->Step();
        removeDepletedWalkers();
    }
}

LaneWalkers PointLocator::wrapLanes()
{
    LaneWalkers laneWalkers;

    for (const auto lane : section->GetLanes()) {
        laneWalkers.push_back(std::make_shared<LaneWalker>(*lane));
    }

    return laneWalkers;
}

void PointLocator::setCurrentLaneWalker()
{
    if (suppressWalkerChange && currentLaneWalker->IsDepleted() == false) {
        return;
    }

    std::vector<double> searchHorizon;

    for (const auto& laneWalker : laneWalkers) {
        searchHorizon.push_back(laneWalker->GetSOffset());
    }

    auto index = getMinIndex(searchHorizon);
    if (laneWalkers.at(index) != currentLaneWalker)
    {
        currentLaneWalker = laneWalkers.at(index);
        currentWalkerSOffset = currentLaneWalker->GetSOffset();
    }
}

void PointLocator::removeDepletedWalkers()
{
    laneWalkers.erase(std::remove_if(laneWalkers.begin(),
                                     laneWalkers.end(),
                                     [](const std::shared_ptr<LaneWalker>& lw)
                                     {
                                         return lw->IsDepleted();
                                     }),
                                     laneWalkers.end());
}

bool PointLocator::allPointsLocated()
{
    return searchPoints.empty();
}

void PointLocator::suppressWalkerSwitchIfOnTrack(int& remainingPointsLast)
{
    int remainingPoints = referencePointLocated ? 0 : 1;
    suppressWalkerChange = (remainingPointsLast > remainingPoints);
    remainingPointsLast = remainingPoints;
}

} // namespace OWL
