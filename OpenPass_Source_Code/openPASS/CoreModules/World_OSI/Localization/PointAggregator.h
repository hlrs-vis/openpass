/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

#include <algorithm>
#include <list>
#include <tuple>
#include <utility>

#include "OWL/DataTypes.h"
#include "LocalizationCommon.h"
#include "PointLocator.h"

struct SearchResult
{
    //const OWL::Section* section;
    //double sectionOffset;
    const OWL::Lane& lane;
    OWL::LocatedPoint point;
};

class PointAggregatorInterface
{
public:
    virtual ~PointAggregatorInterface() = default;
    virtual std::list<const OWL::Lane*> GetLanes() const = 0;
    virtual SearchResult GetMinT() const = 0;
    virtual SearchResult GetMaxT() const = 0;
    virtual std::list<SearchResult> Get(OWL::PointType pointType) const = 0;
};

namespace World {
namespace Localization {

using SearchResults = std::list<SearchResult>;

class PointAggregator : public PointAggregatorInterface
{
public:
    void Add(const OWL::Section& section,
             double sectionOffset,
             const OWL::Lane& lane,
             OWL::LocatedPoint locatedPoint);

    SearchResults Get(OWL::PointType pointType) const override;

    std::list<const OWL::Lane*> GetLanes() const override;

    SearchResult GetMinT() const override;
    SearchResult GetMaxT() const override;

private:
    SearchResults rawPoints;
    std::list<const OWL::Lane*> lanes;
};

} // namespace Localization
} // namespace World
