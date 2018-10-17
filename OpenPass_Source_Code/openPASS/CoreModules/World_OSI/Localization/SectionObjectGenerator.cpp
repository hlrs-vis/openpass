/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "SectionObjectGenerator.h"

namespace World {
namespace Localization {

void SectionObjectGenerator::Generate(OWL::MovingObject& baseTrafficObject)
{
    rearLaneCoverage.clear();
    frontLaneCoverage.clear();

    baseTrafficObject.ClearLaneAssignments();

    const auto& pointAggregation = pointAggregator.Get(OWL::PointType::Reference);

    for(const auto& searchResult : pointAggregation)
    {
        baseTrafficObject.AddLaneAssignment(searchResult.lane);
        baseTrafficObject.SetRoadCoordinate(searchResult.point.coordinate);
    }
}

int SectionObjectGenerator::GetFrontMainLaneId()
{
    SearchResults locatedPoints = pointAggregator.Get(OWL::PointType::Reference);

    if (locatedPoints.size() == 0)
    {
        return 0;
    }

    OWL::CLane& lane = locatedPoints.front().lane;

    return static_cast<int>(lane.GetId());
}

int SectionObjectGenerator::GetRearMainLaneId()
{
    SearchResults locatedPoints = pointAggregator.Get(OWL::PointType::Reference);

    if (locatedPoints.size() == 0)
    {
        return -1;
    }

    OWL::CLane& lane = locatedPoints.front().lane;

    return static_cast<int>(lane.GetId());
}

std::map<int, double> SectionObjectGenerator::GetFrontLaneCoverage() const
{
    return frontLaneCoverage;
}

std::map<int, double> SectionObjectGenerator::GetRearLaneCoverage() const
{
    return rearLaneCoverage;
}

std::map<int, Remainder> SectionObjectGenerator::GetFrontRemainder() const
{
    return frontLaneRemainder;
}

void SectionObjectGenerator::FilterSmallCoverages()
{
    FilterSmallCoverages(frontLaneCoverage);
    FilterSmallCoverages(rearLaneCoverage);
}


void SectionObjectGenerator::FilterSmallCoverages(std::map<int, double> &laneCoverage)
{
    if(laneCoverage.size() > 1)
    {
        auto maxElement = std::max_element(laneCoverage.begin(), laneCoverage.end(),
                                           [](
                                           const std::pair<const int, double>& laneCoverage1,
                                           const std::pair<const int, double>& laneCoverage2)
        {
            return laneCoverage1.second < laneCoverage2.second;
        });

        std::map<int, double>::iterator coverageIterator = laneCoverage.begin();

        for(; coverageIterator != laneCoverage.end();)
        {
            if (coverageIterator->second < 1e-3) {
                laneCoverage.erase(coverageIterator++);
            }
            else {
                ++coverageIterator;
            }
        }

        if(laneCoverage.size() == 0) {
            laneCoverage[maxElement->first] = maxElement->second;
        }
    }
}

}
}
