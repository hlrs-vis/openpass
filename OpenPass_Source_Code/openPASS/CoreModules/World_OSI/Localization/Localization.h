/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
//-----------------------------------------------------------------------------
//! @file
//! @brief

#pragma once

#include <memory>

#include "globalDefinitions.h"
#include "LocalizationCommon.h"
#include "SearchInitializer.h"
#include "PointAggregator.h"

namespace World {
namespace Localization {

constexpr double BOUNDING_BOX_SAMPLE_WIDTH = 6.0;
constexpr double MOTION_PREVIEW_DISTANCE = 5.55; // 200km/h in 100ms

class Result
{
public:
    bool isLocalizable {false};
    GlobalRoadPosition globalRoadPosition{}; //! Information regarding reference point
    std::string mainRoadId{};
    int mainLaneId{};
    bool isLeavingWorld {false};
    bool isCrossingLanes {false};

    std::list<int> frontLaneIds {};
    std::list<int> touchedLaneIds {};

    static Result Invalid()
    {
        return Result{};
    }

    Result() = default;

    Result(GlobalRoadPosition globalRoadPosition,
           std::string mainRoadId,
           int mainLaneId,
           bool isLeavingWorld,
           bool isCrossingLanes,
           std::list<int> frontLaneIds,
           std::list<int> touchedLaneIds) :
        isLocalizable{true},
        globalRoadPosition{globalRoadPosition},
        mainRoadId{mainRoadId},
        mainLaneId{mainLaneId},
        isLeavingWorld{isLeavingWorld},
        isCrossingLanes{isCrossingLanes},
        frontLaneIds{frontLaneIds},
        touchedLaneIds{touchedLaneIds}
    {}
};

class LazyCoverageSolver
{
    const OWL::Interfaces::WorldData& worldData;
    Coverages coverages;

public:
    LazyCoverageSolver(const OWL::Interfaces::WorldData& worldData) :
        worldData{worldData}
    {}

    void Clear() { coverages.clear(); }

    std::vector<GlobalRoadPosition> GetBoundaryPoints(const std::unique_ptr<PointAggregator>& pointAggregator);
    Remainders GetLaneRemainders(const std::unique_ptr<PointAggregator>& pointAggregator);

private:
    void ApplyOnDemand(const std::unique_ptr<PointAggregator>& pointAggregator);
};

class BaseTrafficObjectLocator
{
private:
    OWL::Interfaces::WorldObject& baseTrafficObject;
    OWL::Interfaces::WorldData& worldData;
    std::unique_ptr<PointAggregator> pointAggregator;
    SearchInitializer searchInitializer{};
    Cache& cache;

    mutable LazyCoverageSolver lazyCoverageSolver;

    OWL::SearchablePoint GenerateMainLaneLocatorPoint(const std::list<OWL::SearchablePoint>& sampledBoundaryPoints,
                                                      double direction);

    std::list<int> GetIdRange(OWL::Id leftCornerId, OWL::Id rightCornerId, OWL::Id mainLaneId);

public:
    BaseTrafficObjectLocator(
            OWL::Interfaces::WorldObject& worldObject,
            OWL::Interfaces::WorldData& worldData,
            Cache& cache);

    Result Locate(const polygon_t& boundingBox, double maxDistance);
    Remainders GetLaneRemainders() const;
    std::vector<GlobalRoadPosition> GetBoundaryPoints() const;

    void Unlocate();
};

}
}
