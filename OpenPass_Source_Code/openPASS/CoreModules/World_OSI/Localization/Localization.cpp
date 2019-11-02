/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include <exception>

#include "Localization.h"
#include "PointLocator.h"
#include "PolygonSampler.h"
#include "CoverageCalculator.h"

namespace World {
namespace Localization {

polygon_t GetBoundingBox(double x, double y, double length, double width, double rotation, double center)
{
    double halfWidth = width / 2;

    point_t boxPoints[]
    {
        point_t{center - length, -halfWidth},
        point_t{center - length,  halfWidth},
        point_t{center,           halfWidth},
        point_t{center,          -halfWidth},
        point_t{center - length, -halfWidth}
    };

    polygon_t box;
    polygon_t boxTemp;
    bg::append(box, boxPoints);

    bt::translate_transformer<double, 2, 2> translate(x, y);

    // rotation in mathematical negativ order (boost) -> invert to match
    bt::rotate_transformer<bg::radian, double, 2, 2> rotate(-rotation);

    bg::transform(box, boxTemp, rotate);
    bg::transform(boxTemp, box, translate);

    return box;
}

BaseTrafficObjectLocator::BaseTrafficObjectLocator(
    OWL::Interfaces::WorldObject& worldObject,
    OWL::Interfaces::WorldData& worldData,
    Cache& cache) :
    baseTrafficObject{worldObject},
    worldData{worldData},
    pointAggregator{},
    cache{cache},
    lazyCoverageSolver{worldData}
{
}

std::list<int> BaseTrafficObjectLocator::GetIdRange(OWL::Id leftCornerId, OWL::Id rightCornerId, OWL::Id mainLaneId)
{
    std::list<int> range{static_cast<int>(worldData.GetLaneIdMapping().at(mainLaneId))};
    if (leftCornerId != OWL::InvalidId)
    {
        range.push_back(static_cast<int>(worldData.GetLaneIdMapping().at(leftCornerId)));
    }
    if (rightCornerId != OWL::InvalidId)
    {
        range.push_back(static_cast<int>(worldData.GetLaneIdMapping().at(rightCornerId)));
    }

    range.sort();
    range.unique();

    if (range.size() == 1) { return range; }

    assert(*range.begin() < *range.rbegin());

    int missing = *range.begin() + 1;
    while (missing < *range.rbegin())
    {
        range.push_back(missing);
        missing++;
    }
    range.sort();
    range.unique();
    return range;
}

Result BaseTrafficObjectLocator::Locate(const polygon_t& boundingBox, double maxDistance)
{
    pointAggregator = make_unique<PointAggregator>();

    const OWL::Primitive::AbsPosition objectPosition = baseTrafficObject.GetReferencePointPosition();
    const OWL::Primitive::AbsOrientation objectOrientation = baseTrafficObject.GetAbsOrientation();

    OWL::SearchablePoint referencePoint { {
            objectPosition.x,
            objectPosition.y,
            objectOrientation.yaw
        },
        OWL::PointType::Reference };

    PolygonSampler polygonSampler(boundingBox, BOUNDING_BOX_SAMPLE_WIDTH);
    std::list<OWL::SearchablePoint> sampledBoundaryPoints = polygonSampler.sample();

    for (auto& sampledPoint : sampledBoundaryPoints)
    {
        sampledPoint.coordinate.hdg = objectOrientation.yaw;
    }

    auto mainLaneLocator = GenerateMainLaneLocatorPoint(sampledBoundaryPoints, objectOrientation.yaw);
    sampledBoundaryPoints.push_back(referencePoint);
    sampledBoundaryPoints.push_back(mainLaneLocator);

    if(searchInitializer.valid == false)
    {
        for (const auto& roadPair : worldData.GetRoads())
        {
            PointLocator localizer(*pointAggregator, roadPair.second->GetSections().front());
            localizer.Locate(sampledBoundaryPoints, maxDistance);
            searchInitializer = localizer.GetSearchInitializer();
            if(searchInitializer.valid == true)
            {
                break;
            }
        }
    }
    else
    {
        PointLocator localizer(*pointAggregator, searchInitializer.section);
        localizer.SetSearchInitializer(searchInitializer);
        localizer.Locate(sampledBoundaryPoints, maxDistance);
        searchInitializer = localizer.GetSearchInitializer();
    }

    if (!pointAggregator->IsLocalizable())
    {
        return Result::Invalid();
    }

    pointAggregator->Finalize();

    for (auto lane : pointAggregator->GetTouchedLanes())
    {
        auto movingObject = dynamic_cast<OWL::MovingObject*>(&baseTrafficObject);
        if (movingObject)
        {
            const_cast<OWL::Interfaces::Lane*>(lane)->AddMovingObject(*movingObject);
            baseTrafficObject.AddLaneAssignment(*lane);
            continue;
        }

        auto stationaryObject = dynamic_cast<OWL::StationaryObject*>(&baseTrafficObject);
        if (stationaryObject)
        {
            const_cast<OWL::Interfaces::Lane*>(lane)->AddStationaryObject(*stationaryObject);
            baseTrafficObject.AddLaneAssignment(*lane);
            continue;
        }
    }

    auto cornerIds = pointAggregator->GetFrontCornerIds();
    auto mainLaneId = pointAggregator->GetMainLaneId();
    auto mainRoadId = pointAggregator->GetMainRoadId();
    auto referenceLaneId = pointAggregator->GetReferenceLaneId();
    auto referenceRoadId = pointAggregator->GetReferenceRoadId();
    auto frontLaneIds = GetIdRange(std::get<0>(cornerIds), std::get<1>(cornerIds), mainLaneId);

    std::list<int> touchedLaneIds;
    auto touchedLanes = pointAggregator->GetTouchedLanes();
    for (auto lane : touchedLanes)
    {
        touchedLaneIds.push_back(static_cast<int>(worldData.GetLaneIdMapping().at(lane->GetId())));
    }
    touchedLaneIds.sort();
    touchedLaneIds.unique();

    lazyCoverageSolver.Clear();

    GlobalRoadPosition referenceRoadPosition{worldData.GetRoadIdMapping().at(referenceRoadId),
                static_cast<int>(worldData.GetLaneIdMapping().at(referenceLaneId)),
                pointAggregator->GetReference().s,
                pointAggregator->GetReference().t,
                pointAggregator->GetReference().hdg};

    return Result(referenceRoadPosition,
                  worldData.GetRoadIdMapping().at(mainRoadId),
                  static_cast<int>(worldData.GetLaneIdMapping().at(mainLaneId)),
                  !pointAggregator->AllPointsLocated(),
                  pointAggregator->IsCrossingLanes(),
                  frontLaneIds,
                  touchedLaneIds);
}

Remainders BaseTrafficObjectLocator::BaseTrafficObjectLocator::GetLaneRemainders() const
{
    return lazyCoverageSolver.GetLaneRemainders(pointAggregator);
}

std::vector<GlobalRoadPosition> BaseTrafficObjectLocator::GetBoundaryPoints() const
{
    return lazyCoverageSolver.GetBoundaryPoints(pointAggregator);
}


OWL::SearchablePoint BaseTrafficObjectLocator::GenerateMainLaneLocatorPoint(const std::list<OWL::SearchablePoint>&
        sampledBoundaryPoints, double direction)
{
    auto firstCornerIter = std::find_if(sampledBoundaryPoints.begin(),
                                        sampledBoundaryPoints.end(),
    [](const OWL::SearchablePoint & point) { return point.pointType == OWL::PointType::CornerLeftFront;});
    auto secondCornerIter = std::find_if(sampledBoundaryPoints.begin(),
                                         sampledBoundaryPoints.end(),
    [](const OWL::SearchablePoint & point) { return point.pointType == OWL::PointType::CornerRightFront;});

    if (firstCornerIter != sampledBoundaryPoints.end() && secondCornerIter != sampledBoundaryPoints.end())
    {
        OWL::SearchablePoint mainPoint {{
                (firstCornerIter->coordinate.x + secondCornerIter->coordinate.x) / 2,
                (firstCornerIter->coordinate.y + secondCornerIter->coordinate.y) / 2,
                direction
            },
            OWL::PointType::MainLaneLocator};

        return mainPoint;
    }

    throw std::logic_error("Could not find necessary points");
}

void BaseTrafficObjectLocator::Unlocate()
{
    baseTrafficObject.ClearLaneAssignments();
}

std::vector<GlobalRoadPosition> LazyCoverageSolver::GetBoundaryPoints(const std::unique_ptr<PointAggregator>&
        pointAggregator)
{
    ApplyOnDemand(pointAggregator);

    std::vector<GlobalRoadPosition> boundaryPoints;

    const auto& left = coverages.begin()->left;
    if (left)
    {
        auto roadId = worldData.GetRoadIdMapping().at(left->lane.GetSection().GetRoad().GetId());
        auto laneId =  static_cast<int>(worldData.GetLaneIdMapping().at(left->lane.GetId()));
        boundaryPoints.push_back({roadId,
                                  laneId,
                                  left->point.coordinate.s,
                                  left->point.coordinate.t,
                                  left->point.coordinate.hdg});
    }

    const auto& right = coverages.rbegin()->right;
    if (right)
    {
        auto roadId = worldData.GetRoadIdMapping().at(right->lane.GetSection().GetRoad().GetId());
        auto laneId = static_cast<int>(worldData.GetLaneIdMapping().at(right->lane.GetId()));
        boundaryPoints.push_back({roadId,
                                  laneId,
                                  right->point.coordinate.s,
                                  right->point.coordinate.t,
                                  right->point.coordinate.hdg});
    }

    return boundaryPoints;
}

Remainders LazyCoverageSolver::GetLaneRemainders(const std::unique_ptr<PointAggregator>& pointAggregator)
{
    ApplyOnDemand(pointAggregator);
    Remainders remainders;

    for (const auto& coverage : coverages)
    {
        remainders.push_back({coverage.remainderLeft, coverage.remainderRight});
    }

    return remainders;
}

void LazyCoverageSolver::ApplyOnDemand(const std::unique_ptr<PointAggregator>& pointAggregator)
{
    if (coverages.empty() && pointAggregator->IsLocalizable())
    {
        coverages = CoverageCalculator::Process(*pointAggregator);
    }
}

}
}
