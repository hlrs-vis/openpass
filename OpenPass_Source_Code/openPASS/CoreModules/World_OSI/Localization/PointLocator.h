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

#include <tuple>

#include "vector2d.h"

#include "WorldData.h"
#include "OWL/DataTypes.h"
#include "LaneWalker.h"
#include "GeometryProcessor.h"
#include "PointAggregator.h"
#include "SearchInitializer.h"

namespace World {
namespace Localization {
class PointAggregator;
}
}

namespace OWL
{

/// retrieve item in vector whose value is the smallest
template <typename T>
size_t getMinIndex(std::vector<T>& values)
{
    double minimum = std::numeric_limits<T>::max();
    size_t minimumIndex = 0;

    for (size_t i = 0; i < values.size(); ++i)
    {
        auto value = values[i];
        if (value < minimum) {
            minimum = value;
            minimumIndex = i;
        }
    }

    return minimumIndex;
}

/*
class SearchInitializer
{

};
*/
using LaneWalkers = std::vector<std::shared_ptr<LaneWalker>>;

class PointLocator
{
private:
    static constexpr double motionPreviewDistance = 44.4; // m/s

    World::Localization::PointAggregator& pointAggregator;
    const OWL::Interfaces::Sections& sections;
    OWL::Interfaces::Sections::const_iterator sectionIterator;
    std::list<SearchablePoint> searchPoints;

    World::Localization::SearchInitializer searchInitializer;
    bool firstSearchHitAlreadySet{false};

    // search state objects:
    CSection* section;
    double sectionOffset;
    LaneWalkers laneWalkers;

    bool referencePointLocated = false;

    std::shared_ptr<LaneWalker> currentLaneWalker;
    double currentWalkerSOffset{0};
    bool suppressWalkerChange{false};
    size_t locatedPoints{0};

public:
    PointLocator(World::Localization::PointAggregator& pointAggregator,
                 const OWL::Interfaces::Sections &sections,
                 std::list<SearchablePoint> searchPoints);

    void locate();
    void establishSearchState();
    void suppressWalkerSwitchIfBelowMotionPreviewDistance();
    void locatePoint();
    LaneWalkers wrapLanes();
    void setCurrentLaneWalker();
    void removeDepletedWalkers();
    bool pointLocated() const;
    void suppressWalkerSwitchIfOnTrack(int& remainingPointsLast);
    bool allPointsLocated();

    bool firstSearchHit()
    {
        if (firstSearchHitAlreadySet) {
            return false;
        }

        if (referencePointLocated)
        {
            firstSearchHitAlreadySet = true;
            return true;
        }

        return false;
    }

    void saveSearchInitializer()
    {
        searchInitializer =
        {
            true,
            sectionIterator,
            sections.end(),
            sectionOffset,
            laneWalkers
        };
    }

    World::Localization::SearchInitializer getSearchInitializer() const
    {
        return searchInitializer;
    }

    void setSearchInitializer(World::Localization::SearchInitializer searchInitializer)
    {
        if (isLoadable(searchInitializer))
        {
            sectionIterator = searchInitializer.sectionIter;
            sectionOffset = searchInitializer.sectionOffset;
            laneWalkers = std::move(searchInitializer.laneWalkers);

            invalidateSearchInitializer();
        }
    }

    bool isLoadable(World::Localization::SearchInitializer si) const
    {
        return si.valid && (si.sectionIterEnd == sections.end());
    }

    void invalidateSearchInitializer()
    {
        firstSearchHitAlreadySet = false;
        this->searchInitializer = {};
    }

};

} //namespace OWL
