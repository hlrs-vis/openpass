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
//! @file  SceneryConverter.h
//! @brief This file contains the converter of the scenery configuration.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include <list>
#include "Interfaces/worldInterface.h"
#include "Interfaces/sceneryInterface.h"
#include "Localization/LocalizationCache.h"
#include "Common/vector3d.h"
#include "WorldData.h"
#include "WorldDataQuery.h"

//-----------------------------------------------------------------------------
//! Class for the convertion of a scenery, i.e. the roads in it; the road geometry
//! is converted in class GeometryConverter.
//-----------------------------------------------------------------------------
class SceneryConverter
{

public:
    SceneryConverter(SceneryInterface *scenery,
                     OWL::Interfaces::WorldData& worldData,
                     World::Localization::Cache& localizationCache,
                     const CallbackInterface *callbacks);
    SceneryConverter(const SceneryConverter&) = delete;
    SceneryConverter(SceneryConverter&&) = delete;
    SceneryConverter& operator=(const SceneryConverter&) = delete;
    SceneryConverter& operator=(SceneryConverter&&) = delete;
    virtual ~SceneryConverter() = default;

    //-----------------------------------------------------------------------------
    //! Triggers the convertion process from OpenDrive to OSI in the following steps:
    //! - defines a unique direction within the road cluster (MarkDirections())
    //! - assigns IDs to OpenDrive lane sections
    //! - generates the mappings for lanes and lane sections from OpenDrive to OSI
    //! - then connects these sections
    //! - finally, triggers convertion of the road geometries
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool Convert();

    std::tuple<bool, double, double, double> CalculateAbsoluteCoordinates(RoadInterface* road, OWL::CSection* section, const RoadObjectInterface *object) const;

protected:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    //-----------------------------------------------------------------------------
    //! Returns the lane with the provided ID in the provided lane section.
    //!
    //! @param[in]  otherSection        OpenDrive lane section containing the lane
    //! @param[in]  otherId             ID of the required lane
    //! @return                         OpenDrive lane with the provided ID, if it exists
    //!                                 in the provided lane section, nullptr otherwise
    //-----------------------------------------------------------------------------
    RoadLaneInterface *GetOtherLane(RoadLaneSectionInterface *otherSection,
                           int otherId);

    //-----------------------------------------------------------------------------
    //! Returns the road from the scenery to which the provided link links to.
    //!
    //! Preconditions:
    //! - Scenery has to be setup properly
    //!
    //! @param[in]  roadLink            OpenDrive road link to the desired road
    //! @return                         Road which the provided link connects to, if
    //!                                 it exists in the scenery, nullptr otherwise
    //-----------------------------------------------------------------------------
    RoadInterface *GetConnectedRoad(RoadLinkInterface *roadLink);

    //-----------------------------------------------------------------------------
    //! Marks the direction of the provided road, on all of its lane sections
    //! and lanes within these lane sections to the provided value.
    //!
    //! @param[in]  road                OpenDrive road on which to set if it is
    //!                                 in direction
    //! @param[in]  inDirection         Specifies if road is in direction
    //-----------------------------------------------------------------------------
    void MarkDirectionRoad(RoadInterface *road,
                           bool inDirection);


    //-----------------------------------------------------------------------------
    //! Mark the directions of all roads in the scenery according to global direction
    //! definition.
    //!
    //! Notes:
    //! - The first road of each cluster implictely defines the driving direction
    //!   (OpenDrive lanes/roads which are connected with opposite directions will
    //!   be converted to OSI lanes/sections which point into the same direction)
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool MarkDirections();

    //-----------------------------------------------------------------------------
    //! Assigns unique IDs (beginning with 0) to all OpenDrive lane sections for
    //! all roads in the scenery.
    //!
    //! @return                         True
    //-----------------------------------------------------------------------------
    bool IndexElements();

    //-----------------------------------------------------------------------------
    //! Connects the OSI representation of the two provided lanes.
    //!
    //! @param[in]  currentLane         First lane to connect
    //! @param[in]  currentContactPoint Contact point of the first lane
    //! @param[in]  otherLane           Second lane to connect
    //! @param[in]  otherContactPoint   Contact point of the second lane
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectLaneToLane(RoadLaneInterface *currentLane,
                           ContactPointType currentContactPoint,
                           RoadLaneInterface *otherLane);

    //-----------------------------------------------------------------------------
    //! Connects a lane to a lane section in OSI by connecting the predecessor and
    //! successor of the provided lane, which are in the provided lane section, to
    //! the provided line.
    //!
    //! @param[in]  currentLane         Lane to connect
    //! @param[in]  currentContactPoint Contact point of the lane
    //! @param[in]  otherLaneSection    Lane section to connect
    //! @param[in]  otherContactPoint   Contact point of the lane section
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectLaneToSection(RoadLaneInterface *currentLane,
                              ContactPointType currentContactPoint,
                              RoadLaneSectionInterface *otherLaneSection);

    //-----------------------------------------------------------------------------
    //! Connects a lane section to a lane section in OSI by first connecting all
    //! lanes in the first section to the second section, then connecting all lanes
    //! in the second section to the first.
    //!
    //! Notes:
    //! - OpenDrive center lanes are skipped (width=0 by convention)
    //!
    //! @param[in]  firstLaneSection    First lane section to connect
    //! @param[in]  currentContactPoint Contact point of the first lane section
    //! @param[in]  secondLaneSection   Lane section to connect
    //! @param[in]  secondContactPoint  Contact point of the second lane section
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectLanes(RoadLaneSectionInterface *firstLaneSection,
                      ContactPointType firstContactPoint,
                      RoadLaneSectionInterface *secondLaneSection,
                      ContactPointType secondContactPoint);

    //-----------------------------------------------------------------------------
    //! Connects a road in the scenery to its predecessor and successor roads in
    //! OSI by connecting their respective lane sections. Only connects roads with roadlinktype Road
    //!
    //! @param[in]  road                Road which should be connected to its predecessors
    //!                                 and successors
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectRoadExternalWithElementTypeRoad(RoadInterface *road);

    //-----------------------------------------------------------------------------
    //! Connects a road with another road by setting successor of road, section and lanes
    //!
    //! @param[in]  currentRoad         road which should be connected to its successor
    //! @param[in]  otherRoad           successor of this currentRoad
    //! @param[in]  otherSection        section on otherRoad to connect to
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectExternalRoadSuccessor(RoadInterface *currentRoad, RoadInterface *otherRoad, RoadLaneSectionInterface *otherSection);

    //-----------------------------------------------------------------------------
    //! Connects a road with another road by setting predecessor of road, section and lanes
    //!
    //! @param[in]  currentRoad         road which should be connected to its predecessor
    //! @param[in]  otherRoad           predecessor of this currentRoad
    //! @param[in]  otherSection        section on otherRoad to connect to
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectExternalRoadPredecessor(RoadInterface *currentRoad, RoadInterface *otherRoad, RoadLaneSectionInterface *otherSection);

    //-----------------------------------------------------------------------------
    //! Connects a road in the scenery internally in OSI by connecting all of its
    //! stored lane sections with their predecessors and successors.
    //!
    //! @param[in]  road                Road in which the lanes and lane sections
    //!                                 should be connected
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectRoadInternal(RoadInterface *road);


    //-----------------------------------------------------------------------------
    //!Connects the incoming and connecting roads of the junction
    //!
    //! @param[in]  junction            Junction which should be connected
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectJunction(JunctionInterface *junction);

    //-----------------------------------------------------------------------------
    //! Connects a single path of a junction.
    //! It only sets the connections into the path, because the connections of the path
    //! itself are already set by ConnectRoadExternalWithElementTypeRoad
    //!
    //! @param[in]  incomingRoad            road going into the path
    //! @param[in]  connectingRoad          connecting road == path
    //! @param[in]  outgoingRoad            road going out of the path
    //! @param[in]  incomingContactPoint    contactPoint on the path connected to the incomingRoad
    //! @param[in]  outgoingContactPoint    contactPoint on the outgoing road connected to the path
    //! @param[in]  laneIdMapping           mapping of the lane ids between the incoming road and the path
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    void ConnectPathInJunction(RoadInterface *incomingRoad, RoadInterface *connectingRoad, RoadInterface*outgoingRoad,
                               ContactPointType incomingContactPoint, ContactPointType outgoingContactPoint, std::map<int, int> laneIdMapping);

    //-----------------------------------------------------------------------------
    //! Connects OSI sections for all roads in the scenery internally and externally.
    //!
    //!
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectRoads();

    //OSI Methods and Variables
    void CreateRoads();

    void CreateTrafficSigns();

    void CreateTrafficLight(RoadSignalInterface* signal, RoadInterface* road);

    std::list<RoadLaneInterface*> GetRoadLanesAtDistance(RoadInterface *road, double s);

    Common::Vector3d GetPositionForSCoordinate(RoadInterface* road, double s, double t);

    void CreateObjects();

    bool IsMovingObject(RoadObjectInterface* object);

    bool IsVehicle(RoadObjectInterface* object);

    osi3::StationaryObject_Classification_Type GetStationaryObjectType(RoadObjectInterface* object);

    osi3::MovingObject_Type GetMovingObjectType(RoadObjectInterface* object);

    osi3::MovingObject_VehicleClassification_Type GetVehicleType(RoadObjectInterface* object);

    void SetOsiPosition(osi3::BaseStationary* baseStationary,osi3::BaseMoving* baseMoving, RoadInterface* road,
                        double s, double t,
                        double length, double height, double width,
                        double yaw, double pitch, double roll);

    SceneryInterface *scenery;
    OWL::Interfaces::WorldData& worldData;
    WorldDataQuery worldDataQuery{worldData};
    World::Localization::Cache& localizationCache;
    const CallbackInterface *callbacks;
};

inline bool IsWithinLeftClosedInterval(double value, double start, double end)
{
    return value >= start && value < end;
}

template<typename T, typename A>
inline bool HasSucceedingElement(std::vector<T, A> const& vector, size_t currentIndex)
{
    return vector.size() > currentIndex + 1;
}


