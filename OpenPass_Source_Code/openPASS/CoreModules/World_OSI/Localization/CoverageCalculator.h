/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <vector>
#include <limits>
#include <utility>

#include "LocalizationCommon.h"
#include "PointAggregator.h"

namespace World {
namespace Localization {

class CoverageEngine;
class SingleCoverageEngine;
class NeighbouringCoverageEngine;

class CoverageCalculator
{
public:
    static Coverages Process(const PointAggregatorInterface& pointAggregator);
};

class CoverageEngine
{
protected:
    const PointAggregatorInterface& pointAggregator;

private:
    bool IsAligned() const
    {
        return pointAggregator.IsAligned();
    }

    bool MissingCornerLeft() const
    {
        return pointAggregator.MissingCornerLeftFront() ||
               pointAggregator.MissingCornerLeftRear();
    }

    bool MissingCornerRight() const
    {
        return pointAggregator.MissingCornerRightFront() ||
               pointAggregator.MissingCornerRightRear();
    }

    bool MissingWholeFront() const
    {
        return pointAggregator.MissingCornerLeftFront() && pointAggregator.MissingCornerRightFront();
    }

    bool MissingWholeRear() const
    {
        return pointAggregator.MissingCornerLeftRear() && pointAggregator.MissingCornerRightRear();
    }

    bool MissingDiagonal() const
    {
        return (pointAggregator.MissingCornerLeftFront() && pointAggregator.MissingCornerRightRear()) ||
               (pointAggregator.MissingCornerRightFront() && pointAggregator.MissingCornerLeftRear());
    }

    std::pair<double, double> GetRemainder(const OWL::Lane& lane, RoadPosition coordinate) const
    {
        auto width = lane.GetWidth(std::max(coordinate.s, 0.0));
        return std::make_pair(width / 2.0 - coordinate.t, width / 2.0 + coordinate.t);
    }

    Coverages LeftAndRightCoverage() const
    {
        return Coverage(true, true);
    }

    Coverages LeftCoverageOnly() const
    {
        return Coverage(false, true);
    }

    Coverages RightCoverageOnly() const
    {
        return Coverage(true, false);
    }

    Coverages FullCoverage() const
    {
        return Coverage(false, false);
    }

protected:
    CoverageInformation Coverage(bool left, bool right, const std::list<SearchResult>& searchResults) const
    {
        if (!left && !right) { return CoverageInformation{}; }

        auto remainderLeft = std::numeric_limits<double>::max();
        auto remainderRight = std::numeric_limits<double>::max();

        const SearchResult* searchResultLeft{nullptr};
        const SearchResult* searchResultRight{nullptr};

        for (const auto& searchResult : searchResults)
        {
            auto remainder = GetRemainder(searchResult.lane, searchResult.point.coordinate);

            if (std::get<0>(remainder) < remainderLeft)
            {
                remainderLeft = std::get<0>(remainder);
                searchResultLeft = &searchResult;
            }
            if (std::get<1>(remainder) < remainderRight)
            {
                remainderRight = std::get<1>(remainder);
                searchResultRight = &searchResult;
            }
        }

        CoverageInformation cov;
        if (left && right)
        {
            cov.left = std::make_shared<const SearchResult>(*searchResultLeft);
            cov.right = std::make_shared<const SearchResult>(*searchResultRight);
            cov.remainderLeft = remainderLeft;
            cov.remainderRight = remainderRight;
            return std::move(cov);
        }
        else
        {
            if (left) { cov.left = std::make_shared<const SearchResult>(*searchResultLeft); }
            if (right) { cov.right = std::make_shared<const SearchResult>(*searchResultRight); }
            cov.remainderLeft = left ? remainderLeft : 0.0;
            cov.remainderRight = right ? remainderRight : 0.0;
            return std::move(cov);
        }
    }

public:
    CoverageEngine(const PointAggregatorInterface& pointAggregator) : pointAggregator(pointAggregator) {}
    virtual ~CoverageEngine() = default;

    Coverages Calculate() const
    {
        assert(pointAggregator.IsLocalizable());

        if (MissingDiagonal()) { return FullCoverage(); }
        if (MissingCornerLeft()) { return IsAligned() ? LeftCoverageOnly() : RightCoverageOnly(); }
        if (MissingCornerRight()) { return IsAligned() ?  RightCoverageOnly() : LeftCoverageOnly(); }
        return LeftAndRightCoverage();
    }

    virtual Coverages Coverage(bool left, bool right) const = 0;
};

class SingleCoverageEngine : public CoverageEngine
{
public:
    using CoverageEngine::Coverage;

    SingleCoverageEngine(const PointAggregatorInterface& pointAggregator) : CoverageEngine{pointAggregator} {}

    Coverages Coverage(bool left, bool right) const override
    {
        const auto& searchStreams = pointAggregator.GetBoundaryStreams();

        return
        {
            std::move(Coverage(left, right, std::get<0>(searchStreams)))
        };
    }
};

class NeighbouringCoverageEngine : public CoverageEngine
{
public:
    using CoverageEngine::Coverage;

    NeighbouringCoverageEngine(const PointAggregatorInterface& pointAggregator) : CoverageEngine{pointAggregator} {}
    virtual ~NeighbouringCoverageEngine() = default;

    Coverages Coverage(bool left, bool right) const override
    {
        const auto& searchStreams = pointAggregator.GetBoundaryStreams();

        auto leftStream =  std::get<0>(searchStreams);
        auto rightStream = std::get<1>(searchStreams);

        return
        {
            Coverage(left, false, leftStream),
            Coverage(false, right, rightStream)
        };
    }
};

} // namespace Localization
} // namespace World

