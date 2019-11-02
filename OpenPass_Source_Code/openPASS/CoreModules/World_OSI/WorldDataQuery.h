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

#include <utility>
#include <tuple>
#include "OWL/DataTypes.h"
#include "WorldData.h"
#include <numeric>

//! Helper class for complex queries on the world data
class WorldDataQuery
{
public:
    WorldDataQuery(OWL::Interfaces::WorldData& worldData);

    //! Checks if object is of type T and within the specified range when supplied an offset
    //! Returns true if so; false, otherwise
    //!
    //! @param worldObject object to check
    //! @param minS minimum valid s position within the road
    //! @param maxS maximum valid s position within the road
    template<typename T>
    bool ObjectIsOfTypeAndWithinRange(const OWL::Interfaces::WorldObject * const worldObject, const double minS, const double maxS) const
    {
        return (worldObject->Is<T>() &&
                worldObject->GetDistance(OWL::MeasurementPoint::RoadEnd) > minS &&
                worldObject->GetDistance(OWL::MeasurementPoint::RoadStart) < maxS);
    }

    //!Returns first object of type T in the ForwardLaneStream starting at the lane with given OpenDrive Id and s coordinate.
    //! Objects partially inside the search range are also considered.
    //! Return nullptr if there is no object in maxSearchLength
    //!
    //! @param roadId the string identifier of the road to begin searching in
    //! @param laneId OpenDrive id of lane to search in
    //! @param initialSearchDistance start s coordinates for search
    //! @param maxSearchLength maximum look ahead distance
    template <typename T>
    OWL::Interfaces::WorldObject* GetNextObjectInLane(std::string roadId, OWL::OdId laneId, double initialSearchDistance,
            double maxSearchLength) const
    {
        const auto& lane = GetLaneByOdId(roadId, laneId, initialSearchDistance);
        if (!lane.Exists())
        {
            return nullptr;
        }

        maxSearchLength = std::max(0.0, maxSearchLength); //supress negative search distances

        auto previousRoadLength = lane.GetRoad().GetLength();
        auto previousRoadId = lane.GetRoad().GetId();

        // these values track the previous roads and the sum of their lengths in order to track how far we have searched over adjacent rows
        std::vector<double> previousRoadLengths{};
        double previousRoadLengthSum{0.0};
        for (const auto& lane : lane.forwardLaneStream)
        {
            if (lane.GetRoad().GetId() != previousRoadId)
            {
                previousRoadLengths.emplace_back(previousRoadLength);
            }
            previousRoadLengthSum = std::accumulate(previousRoadLengths.begin(), previousRoadLengths.end(), 0.0);

            if (lane.GetDistance(OWL::MeasurementPoint::RoadStart) + previousRoadLengthSum > initialSearchDistance + maxSearchLength)
            {
                break;
            }

            OWL::Interfaces::WorldObjects worldObjects = lane.GetWorldObjects();

            worldObjects.sort([](const OWL::Interfaces::WorldObject * const wo1,
                                 const OWL::Interfaces::WorldObject * const wo2)
            {
                return wo1->GetDistance(OWL::MeasurementPoint::RoadStart) <
                       wo2->GetDistance(OWL::MeasurementPoint::RoadStart);
            });

            double rangeStart = initialSearchDistance - previousRoadLengthSum;
            double rangeEnd = initialSearchDistance + maxSearchLength - previousRoadLengthSum;
            if (!lane.IsInStreamDirection())
            {
                rangeStart = lane.GetSection().GetLength() - initialSearchDistance + maxSearchLength - previousRoadLengthSum;
                rangeEnd = lane.GetSection().GetLength() - initialSearchDistance - previousRoadLengthSum;
            }
            for (OWL::Interfaces::WorldObject* worldObject : worldObjects)
            {
                if(ObjectIsOfTypeAndWithinRange<T>(worldObject, rangeStart, rangeEnd))
                {
                    return worldObject;
                }
            }

            previousRoadLength = lane.GetRoad().GetLength();
            previousRoadId = lane.GetRoad().GetId();
        }

        return nullptr;
    }

    //!Returns first object of type T in the ReverseLaneStream starting at the lane with given OpenDrive Id and initialSearchDistance + maxSearchLength.
    //! Objects partially inside the search range are also considered.
    //! Return nullptr if there is no object in maxSearchLength
    //!
    //! @param roadId the string identifier of the road to begin searching in
    //! @param laneId OpenDrive id of lane to search in
    //! @param initialSearchDistance start s coordinates for search
    //! @param maxSearchLength maximum look ahead distance
    template <typename T>
    const OWL::Interfaces::WorldObject* GetLastObjectInLane(std::string roadId, OWL::OdId laneId, double initialSearchDistance,
            double maxSearchLength) const
    {
        // gets the objects in the range sorted from closest to furthest
        const auto objectsInRange = GetObjectsOfTypeInRange<T>(roadId, laneId, initialSearchDistance, initialSearchDistance + maxSearchLength);
        if (objectsInRange.size() > 0)
        {
            return *(objectsInRange.end()-1);
        }

        return nullptr;
    }

    //!Returns first object of type T in the ReverseLaneStream starting at the lane with given OpenDrive Id and s coordinate.
    //! Objects partially inside the search range are also considered.
    //! Return nullptr if there is no object in maxSearchLength
    //!
    //! @param roadId the string identifier of the road to begin searching in
    //! @param laneId OpenDrive id of lane to search in
    //! @param initialSearchDistance start s coordinates for search
    //! @param maxSearchLength maximum look ahead distance
    template <typename T>
    OWL::Interfaces::WorldObject* GetClosestObjectInUpstream(std::string roadId, OWL::OdId laneId,
            double initialSearchDistance,
            double maxSearchLength) const
    {
        //supress negative search distances
        maxSearchLength = std::max(0.0, maxSearchLength);

        const auto& lane = GetLaneByOdId(roadId, laneId, initialSearchDistance);
        if (!lane.Exists())
        {
            return nullptr;
        }

        double referenceSearchPosition = initialSearchDistance;
        double remainingSearchLength = maxSearchLength;
        auto previousRoadId = lane.GetRoad().GetId();
        for (const auto& lane : lane.reverseLaneStream)
        {
            if (remainingSearchLength <= 0.0)
            {
                break;
            }

            if (lane.GetRoad().GetId() != previousRoadId)
            {
                previousRoadId = lane.GetRoad().GetId();
                remainingSearchLength -= referenceSearchPosition;
                // set the new reference to the end of the 'road'
                referenceSearchPosition = lane.GetDistance(OWL::MeasurementPoint::RoadStart) + lane.GetLength();
            }

            OWL::Interfaces::WorldObjects worldObjects = lane.GetWorldObjects();
            worldObjects.sort([](const OWL::Interfaces::WorldObject * const wo1,
                                 const OWL::Interfaces::WorldObject * const wo2)
            {
                return wo1->GetDistance(OWL::MeasurementPoint::RoadEnd) <
                       wo2->GetDistance(OWL::MeasurementPoint::RoadEnd);
            });

            for (auto worldObject : worldObjects)
            {
                double minimumSearchPosition = std::min(initialSearchDistance - maxSearchLength, 0.0);
                if (ObjectIsOfTypeAndWithinRange<T>(worldObject, minimumSearchPosition, referenceSearchPosition))
                {
                    return worldObject;
                }
            }
        }

        return nullptr;
    }

    //!Returns first object of type T in the ForwardLaneStream starting at the lane with given OpenDrive Id and initialSearchDistance - maxSearchLength.
    //! Objects partially inside the search range are also considered.
    //! Return nullptr if there is no object in maxSearchLength
    //!
    //! @param roadId the string identifier of the road to begin searching in
    //! @param laneId OpenDrive id of lane to search in
    //! @param initialSearchDistance start s coordinates for search
    //! @param maxSearchLength maximum look ahead distance
    template <typename T>
    OWL::Interfaces::WorldObject* GetFarthestObjectInUpstream(std::string roadId, OWL::OdId laneId,
            double initialSearchDistance,
            double maxSearchLength) const
    {
        //supress negative search distances
        maxSearchLength = std::max(0.0, maxSearchLength);

        const auto& lane = GetLaneByOdId(roadId, laneId, initialSearchDistance);
        if (!lane.Exists())
        {
            return nullptr;
        }

        double referenceSearchPosition = initialSearchDistance;
        double remainingSearchLength = maxSearchLength;
        auto previousRoadId = lane.GetRoad().GetId();
        OWL::Interfaces::WorldObject* farthestObject = nullptr;
        for (const auto& lane : lane.reverseLaneStream)
        {
            if (remainingSearchLength <= 0.0)
            {
                break;
            }

            if (lane.GetRoad().GetId() != previousRoadId)
            {
                previousRoadId = lane.GetRoad().GetId();
                remainingSearchLength -= referenceSearchPosition;
                // set the new reference to the end of the 'road'
                referenceSearchPosition = lane.GetDistance(OWL::MeasurementPoint::RoadStart) + lane.GetLength();
            }

            OWL::Interfaces::WorldObjects worldObjects = lane.GetWorldObjects();
            worldObjects.sort([](const OWL::Interfaces::WorldObject * const wo1,
                                 const OWL::Interfaces::WorldObject * const wo2)
            {
                return wo1->GetDistance(OWL::MeasurementPoint::RoadEnd) <
                       wo2->GetDistance(OWL::MeasurementPoint::RoadEnd);
            });

            for (auto worldObject : worldObjects)
            {
                double minimumSearchPosition = std::min(initialSearchDistance - maxSearchLength, 0.0);
                if (ObjectIsOfTypeAndWithinRange<T>(worldObject, minimumSearchPosition, referenceSearchPosition))
                {
                    farthestObject = worldObject;
                }
            }
        }

        return farthestObject;
    }

    //!Returns the objects of type T in the ForwardLaneStream, sorted from nearest to furthest, starting at the lane with given roadId, laneId and initialSearchDistance.
    //! Objects partially inside the search range are also considered.
    //! Return an empty collection if there is no object in maxSearchLength
    //!
    //! @param roadId the string identifier of the road to begin searching in
    //! @param laneId OpenDrive id of lane to search in
    //! @param initialSearchDistance start s coordinates for search
    //! @param maxSearchLength maximum look ahead distance
    template<typename T>
    std::vector<const OWL::Interfaces::WorldObject*> GetObjectsOfTypeInRange(const std::string& roadId,
                                                                             const OWL::OdId laneId,
                                                                             const double startDistance,
                                                                             const double endDistance) const
    {
        const double maxSearchLength = std::max(0.0, endDistance - startDistance); //supress negative search distances
        const auto& lane = GetLaneByOdId(roadId, laneId, startDistance);
        std::vector<const OWL::Interfaces::WorldObject*> objectsInRange{};
        if (!lane.Exists())
        {
            return objectsInRange;
        }

        double previousRoadLengthEstimate = lane.GetRoad().GetLength();
        auto previousRoadId = lane.GetRoad().GetId();

        std::vector<double> previousRoadLengths{};
        double previousRoadLengthSum{0.0};
        for (const auto& lane : lane.forwardLaneStream)
        {
            if (lane.GetRoad().GetId() != previousRoadId)
            {
                previousRoadLengths.emplace_back(previousRoadLengthEstimate);
            }
            previousRoadLengthSum = std::accumulate(previousRoadLengths.begin(), previousRoadLengths.end(), 0.0);

            if ((lane.IsInStreamDirection() ? lane.GetDistance(OWL::MeasurementPoint::RoadStart) : (lane.GetSection().GetLength() - lane.GetDistance(OWL::MeasurementPoint::RoadEnd))) + previousRoadLengthSum > startDistance + maxSearchLength)
            {
                break;
            }

            OWL::Interfaces::WorldObjects worldObjects = lane.GetWorldObjects();

            worldObjects.sort([](const OWL::Interfaces::WorldObject * const wo1,
                                 const OWL::Interfaces::WorldObject * const wo2)
            {
                return wo1->GetDistance(OWL::MeasurementPoint::RoadStart) <
                       wo2->GetDistance(OWL::MeasurementPoint::RoadStart);
            });

            double rangeStart = startDistance - previousRoadLengthSum;
            double rangeEnd = startDistance + maxSearchLength - previousRoadLengthSum;
            if (!lane.IsInStreamDirection())
            {
                rangeStart = lane.GetRoad().GetLength() - (startDistance + maxSearchLength - previousRoadLengthSum);
                rangeEnd = lane.GetRoad().GetLength() - (startDistance - previousRoadLengthSum);
            }
            for (OWL::Interfaces::WorldObject* worldObject : worldObjects)
            {
                if(ObjectIsOfTypeAndWithinRange<T>(worldObject, rangeStart, rangeEnd))
                {
                    if (std::find(objectsInRange.cbegin(), objectsInRange.cend(), worldObject) == objectsInRange.cend())
                    {
                        objectsInRange.emplace_back(worldObject);
                    }
                }
            }
            previousRoadLengthEstimate = lane.GetRoad().GetLength();
            previousRoadId = lane.GetRoad().GetId();
        }

        return objectsInRange;
    }

    //!Returns the first object in the ReverseLaneStream with specified streamId
    const OWL::Interfaces::WorldObject* GetFirstObjectDownstream(OWL::Id streamId) const;

    //! Returns all MovingObjects in the ForwardLaneStream starting at the lane with given OpenDrive Id and startDistance
    //! ending at endDistance.
    //! Objects partially inside the search interval are also considered.
    std::vector<const OWL::Interfaces::WorldObject*> GetMovingObjectsInRange(std::string roadId, OWL::OdId laneId,
            double startDistance,
            double endDistance) const;

    //! Returns all WorldObjects in the ForwardLaneStream starting at the lane with given OpenDrive Id and startDistance
    //! ending at endDistance.
    //! Objects partially inside the search interval are also considered.
    std::vector<const OWL::Interfaces::WorldObject*> GetWorldObjectsInRange(std::string roadId, OWL::OdId id,
            double startDistance,
            double endDistance) const;

    //! Iterates over ForwardLaneStream until either the type of the next lane does not match one of the specified LaneTypes
    //! or maxSearchLength is reached. Returns the relative distance to the end of last matching lane.
    //! Returns INFINITY if end of lane is outside maxSearchLength.
    //! Returns 0 if lane does not exist or LaneType does not match at initialSearchDistance
    //!
    //! @param id OpenDrive Id of lane
    //! @param initialSearchPosition start s-coordinate
    //! @param maxSearchLength maxmium look ahead distance
    //! @param requestedLaneTypes filter of LaneTypes
    double GetDistanceToEndOfLane(std::string roadId, OWL::OdId id, double initialSearchPosition, double maxSearchLength,
                                  std::list<OWL::LaneType> requestedLaneTypes) const;

    //! Returns relative distance to start of last lane in ReverseLaneStream.
    //! Returns INFINITY if end of lane is outside maxSearchLength.
    //! Returns 0 if lane does not exist at initialSearchDistance.
    //!
    //! @param id OpenDrive Id of lane
    //! @param initialSearchPosition start s-coordinate
    //! @param maxSearchLength maxmium look ahead distance
    double GetDistanceToStartOfLane(std::string roadId, OWL::OdId id, double initialSearchDistance,
                                    double maxSearchLength) const;

    //! Checks if given s-coordinate is valid for specified laneId.
    //!
    //! @param laneId OpenDrive Id of lane
    //! @param distance s-coordinate
    bool IsSValidOnLane(std::string roadId, OWL::OdId laneId, double distance);

    //! Returns number of lanes at given s-coordinate
    //!
    //! @param distance s-coordinate
    int GetNumberOfLanes(std::string roadId, double distance);

    //! Returns lane at specified distance.
    //! Returns InvalidLane if there is no lane at given distance and OpenDriveId
    //!
    //! @param odLaneId OpendDrive Id of Lane
    //! @param distance s-coordinate
    OWL::CLane& GetLaneByOdId(std::string odRoadId, OWL::OdId odLaneId, double distance) const;

    //! Returns section at specified distance.
    //! Returns nullptr if there is no section at given distance
    //!
    //! @param odRaodId ID of road in OpenDrive
    //! @param distance s-coordinate
    OWL::CSection* GetSectionByDistance(std::string odRoadId, double distance) const;

    //! Returns all lanes of given LaneType at specified distance.
    //!
    //! @param distance s-coordinate
    //! @param requestedLaneTypes filter of laneTypes
    OWL::CLanes GetLanesOfLaneTypeAtDistance(std::string roadId, double distance,
            std::list<OWL::LaneType> requestedLaneTypes) const;

    //! Returns next s where there is a valid lane with given laneId.
    //! Returns INFINITY if no valid is s is found.
    //! Internally IsSValidOnLane is called at initialDistance + n*stepSizeLookingForValidS to probe for a valid s.
    //! OWL::EVENTHORIZON is used as maximum look ahead distance
    //!
    //! @param laneId OpenDrive id of lane
    //! @param initialDistance s-coordinate
    //! @param stepSizeLookingForValidS step-size for probing for valid s
    double GetNextValidSOnLaneInDownstream(std::string roadId, OWL::OdId laneId, double initialDistance,
                                           double stepSizeLookingForValidS) ;

    //! Returns largest s not larger than initialDistance where there is a valid lane with given laneId.
    //! Returns -INFINITY if no valid is s is found.
    //! Internally IsSValidOnLane is called at initialDistance - n*stepSizeLookingForValidS to probe for a valid s.
    //! OWL::EVENTHORIZON is used as maximum look ahead distance
    //!
    //! @param laneId OpenDrive id of lane
    //! @param initialDistance s-coordinate
    //! @param stepSizeLookingForValidS step-size for probing for valid s
    double GetLastValidSInUpstream(std::string roadId, OWL::OdId laneId, double initialDistance,
                                   double stepSizeLookingForValidS) ;

    //! Returns true if a lane at given distance with LaneType::Driving exists on specified side.
    //!
    //! @param laneId OpenDrive id of lane
    //! @param distance s-coordinate
    //! @param side side to check (left/right)
    bool ExistsDrivingLaneOnSide(std::string roadId, OWL::OdId laneId, double distance, Side side);

    //! Returns OSI Id of last lane in ReverseLaneStream.
    //! Returns -999 for invalid lane
    OWL::Id GetStreamId(OWL::CLane& lane) const;

    //! Returns start and end s-coordinate of lane stream
    std::pair<double, double> GetLimitingDistances(OWL::CLane& lane) const;

    //! Returns OpenDrive Id of lane at given distance for specified streamId
    OWL::OdId GetLaneId(OWL::Id streamId, double endDistance) const;

    //! Returns all TrafficSigns valid for the lanes in ForwardLaneStream
    //!
    //! @param laneId OpenDrive Id of lane
    //! @param distance s-coordinate
    std::vector<OWL::Interfaces::TrafficSign*> GetTrafficSignsInRange(std::string roadId, OWL::OdId laneId,
            double startDistance, double searchRange) const;


    LaneQueryResult QueryLane(std::string roadId, int laneId, double distance) const;

    std::list<LaneQueryResult> QueryLanes(std::string roadId, double startDistance, double endDistance) const;

    LaneQueryResult BuildLaneQueryResult(OWL::CLane& lane) const;

    std::pair<bool, double> GetLateralDistance(GlobalRoadPosition src, GlobalRoadPosition dst) const;


private:
    OWL::Interfaces::WorldData& worldData;

    std::tuple<bool, double, OWL::Lane const* const> GetWidthOfIntermediateLanesLeft(
        OWL::CLane& lane,
        double distance,
        OWL::Id targetStreamId) const;

    std::tuple<bool, double, OWL::Lane const* const> GetWidthOfIntermediateLanesRight(
        OWL::CLane& lane,
        double distance,
        OWL::Id targetStreamId) const;
};
