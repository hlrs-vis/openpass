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
//! @brief PointAggregator

#pragma once

#include <algorithm>
#include <vector>
#include <tuple>
#include <utility>
#include <map>

#include "OWL/DataTypes.h"
#include "LocalizationCommon.h"
#include "PointLocator.h"

#include "gmock/gmock.h"

enum class StreamType
{
    Empty,
    Clustered,
    Single,
    Neighbours,
    Range
};

using SearchResults = std::vector<SearchResult>;
using SearchStreams = std::map<uint64_t, std::list<SearchResult>>;

class PointAggregatorInterface
{
public:
    virtual ~PointAggregatorInterface() = default;
    virtual void Finalize() = 0;
    virtual StreamType GetStreamType() const = 0;
    virtual const std::pair<std::list<SearchResult>, std::list<SearchResult>> GetBoundaryStreams() const = 0;
    virtual bool IsAligned() const = 0;
    virtual bool IsLocalizable() const = 0;
    virtual bool MissingReference() const = 0;
    virtual bool MissingCornerLeftFront() const = 0;
    virtual bool MissingCornerLeftRear() const = 0;
    virtual bool MissingCornerRightFront() const = 0;
    virtual bool MissingCornerRightRear() const = 0;
    virtual bool IsCrossingLanes() const = 0;
    virtual bool AllPointsLocated() const = 0;
    virtual std::vector<const OWL::Lane*> GetTouchedLanes() const = 0;
    virtual const RoadPosition GetReference() const = 0;
    virtual std::pair<OWL::Id, OWL::Id> GetFrontCornerIds() const = 0;
};

class FakePointAggregator : public PointAggregatorInterface
{
public:
    MOCK_METHOD0(Finalize, void());
    MOCK_CONST_METHOD0(GetStreamType, StreamType());
    MOCK_CONST_METHOD0(GetBoundaryStreams, const std::pair<std::list<SearchResult>, std::list<SearchResult>>());
    MOCK_CONST_METHOD0(IsAligned, bool());
    MOCK_CONST_METHOD0(IsLocalizable, bool());
    MOCK_CONST_METHOD0(MissingReference,  bool());
    MOCK_CONST_METHOD0(MissingCornerLeftFront,  bool());
    MOCK_CONST_METHOD0(MissingCornerLeftRear,  bool());
    MOCK_CONST_METHOD0(MissingCornerRightFront,  bool());
    MOCK_CONST_METHOD0(MissingCornerRightRear,  bool());
    MOCK_CONST_METHOD0(IsCrossingLanes, bool());
    MOCK_CONST_METHOD0(AllPointsLocated, bool());
    MOCK_CONST_METHOD0(GetTouchedLanes, std::vector<const OWL::Lane*>());
    MOCK_CONST_METHOD0(GetReference, const RoadPosition());
    MOCK_CONST_METHOD0(GetFrontCornerIds, std::pair<OWL::Id, OWL::Id>());
};

namespace World {
namespace Localization {

///
/// Utility class of PointAggregator for finding touched lanes from list of points
///
class StreamCollector
{
private:
    const SearchStreams& streamAggregation;
    const std::list<SearchResult>& rawPoints;

    std::vector<OWL::Id> touchedStreams;
    std::vector<const OWL::Lane*> touchedLanes;
    std::deque<OWL::Id> s;

    const OWL::Lane& GetFirstUpstream();
    const OWL::Lane& GetLastDownstream();

    [[noreturn]]
    void report_error(std::string what)
    {
        throw std::logic_error(what);
    }

    template<typename T>
    T PreservingOrderUnique(T iterBegin, T iterEnd)
    {
        std::vector<T> tmp;
        for (auto iter = iterBegin; iter != iterEnd; ++iter)
        {
            tmp.emplace_back(iter);
        }
        std::sort(tmp.begin(), tmp.end(), [](T const & lhs, T const & rhs) {return *lhs < *rhs;});
        tmp.erase(std::unique(tmp.begin(), tmp.end(), [](T const & a, T const & b) { return *a == *b; }), tmp.end());
        std::sort(tmp.begin(), tmp.end());

        size_t i = 0;
        for (auto iter = iterBegin; iter != iterEnd && i != tmp.size(); ++iter)
        {
            if (tmp[i] == iter)
            {
                ++i;
                std::iter_swap(iter, iterBegin++);
            }
        }

        return iterBegin;
    }

    template<typename T>
    void MakeUnique(T& entity)
    {
        entity.erase(PreservingOrderUnique(entity.begin(), entity.end()), entity.end());
    }

    /// Remove all lanes which are not touched through connecting streams
    void RemoveLanesWithUntouchedStreams();

    /// \Check if given stream ID is connected as a left neighbor
    /// \param lane originating lane
    /// \param streamId search for stream id
    /// \return True if connected
    bool IsConnectedLeft(const OWL::Lane& lane, uint64_t streamId) const;

    /// Remove streams, which are not touched
    void TrimTouchedStreams();

    /// Collects all lanes and streams of a section and reports if a certain id was hit
    /// \param section originating lane
    /// \param lastId id from previous call to look for if it was found again
    /// \return true if last id was found again
    bool CollectLanesAndStreamIds(const OWL::Section& section, OWL::Id lastId);

    /// Collects all lanes and streams of the sections, covered by the injected points
    void CollectPotentiallyTouchedLanesAndStreams();

public:
    StreamCollector(
        SearchStreams& streamAggregation,
        std::list<SearchResult>& rawPoints) :
        streamAggregation{streamAggregation},
        rawPoints{rawPoints}
    {}

    /// \brief GetTouchedLanes
    /// \return List of touched lanes
    std::vector<const OWL::Lane*> GetTouchedLanes() const
    {
        return touchedLanes;
    }

    /// \brief GetTouchedElements
    /// \return List of touched IDs
    std::deque<OWL::Id> GetTouchedElements();
};

class PointAggregator : public PointAggregatorInterface
{
public:
    PointAggregator();

    /// \brief Add located point
    /// \param osiRoadId
    /// \param streamId
    /// \param lane
    /// \param coordinate
    /// \param pointType
    void Add(const uint64_t osiRoadId,
             const uint64_t streamId,
             const OWL::Lane& lane,
             const RoadPosition coordinate,
             const OWL::PointType& pointType);

    /// \brief Get evaluated stream type
    ///        (e.g. single lane, or neigbour)
    /// \return the type
    StreamType GetStreamType() const override;

    OWL::Id GetMainLaneId() const
    {
        assert(mainLaneLocatorSet);
        return mainLaneId;
    }

    OWL::Id GetReferenceLaneId() const
    {
        assert(referenceSet);
        return referenceLaneId;
    }

    OWL::Id GetMainRoadId() const
    {
        assert(mainLaneLocatorSet);
        return mainRoadId;
    }

    OWL::Id GetReferenceRoadId() const
    {
        assert(referenceSet);
        return referenceRoadId;
    }

    std::pair<OWL::Id, OWL::Id> GetFrontCornerIds() const override
    {
        return std::make_pair(cornerLeftFrontId, cornerRightFrontId);
    }

    /// \brief Calculates derived values after all points have been aggregated
    void Finalize() override;

    bool IsCrossingLanes() const override;
    bool AllPointsLocated() const override;
    bool IsLocalizable() const override;
    bool IsAligned() const override;
    bool MissingMainLaneLocator() const;
    bool MissingReference() const override;
    bool MissingCornerLeftFront() const override;
    bool MissingCornerLeftRear() const override;
    bool MissingCornerRightFront() const override;
    bool MissingCornerRightRear() const override;
    std::vector<const OWL::Lane*> GetTouchedLanes() const override;
    const std::pair<std::list<SearchResult>, std::list<SearchResult>> GetBoundaryStreams() const override;
    const RoadPosition GetReference() const override;

private:
    const OWL::Lane& GetFirstUpstream();
    const OWL::Lane& GetLastDownstream();

    bool aligned{true};
    void SetAligned(double yaw);

    bool referenceSet {false};
    RoadPosition reference {};

    bool mainLaneLocatorSet {false};
    OWL::Id mainLaneId {OWL::InvalidId};
    OWL::Id mainRoadId {OWL::InvalidId};
    OWL::Id referenceLaneId {OWL::InvalidId};
    OWL::Id referenceRoadId {OWL::InvalidId};

    bool cornerLeftFrontSet {false};
    OWL::Id cornerLeftFrontId {OWL::InvalidId};

    bool cornerRightFrontSet {false};
    OWL::Id cornerRightFrontId {OWL::InvalidId};

    bool cornerLeftRearSet {false};
    bool cornerRightRearSet {false};

    SearchStreams streamAggregation;
    std::list<SearchResult> rawPoints;
    std::deque<OWL::Id> streamIds;
    StreamCollector streamCollector;
};

} // namespace Localization
} // namespace World
