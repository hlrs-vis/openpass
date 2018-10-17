/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "OWL/DataTypes.h"
#include "PointAggregator.h"
#include "PointLocator.h"

namespace World {
namespace Localization {

void PointAggregator::Add(const OWL::Section& section,
         double sectionOffset,
         const OWL::Lane& lane,
         OWL::LocatedPoint locatedPoint)
{
    rawPoints.push_back(SearchResult{lane, locatedPoint});
    lanes.push_back(&lane);
}

SearchResults PointAggregator::Get(OWL::PointType pointType) const
{
    SearchResults searchResults;

    std::copy_if(rawPoints.begin(), rawPoints.end(), std::back_inserter(searchResults),
              [&](const SearchResult& searchResult)
              {
                  return searchResult.point.pointType == pointType;
              });
    return searchResults;
}

std::list<const OWL::Lane*> PointAggregator::GetLanes() const
{
    return lanes;
}

SearchResult PointAggregator::GetMinT() const
{
    // TODO: implement
    return { OWL::Implementation::InvalidLane(), OWL::LocatedPoint() };
}

SearchResult PointAggregator::GetMaxT() const
{
    // TODO: implement
    return { OWL::Implementation::InvalidLane(), OWL::LocatedPoint() };
}

}
}
