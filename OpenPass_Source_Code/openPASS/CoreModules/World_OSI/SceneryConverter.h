/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  SceneryConverter.h
//! @brief This file contains the converter of the scenery configuration.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include <list>
#include "sceneryInterface.h"
#include "vector3d.h"
#include "worldInterface.h"
#include "WorldData.h"

//todo: delete!
template <class T>
class SimpleHasher
{
private:
    std::map<const std::string, T> cache;

public:
    T get(const std::string key)
    {
        auto search = cache.find(key);
        if(search != cache.end())
        {
            return search->second;
        }

        T value = static_cast<T>(cache.size() - 1);
        cache[key] = (T) value;
        return value;
    }

    void clear()
    {
        cache.clear();
    }
};


//-----------------------------------------------------------------------------
//! Class for the convertion of a scenery, i.e. the roads in it; the road geometry
//! is converted in class GeometryConverter.
//-----------------------------------------------------------------------------
class SceneryConverter
{

public:
    SceneryConverter(SceneryInterface *scenery,
                     OWL::WorldData& worldData,
                     const CallbackInterface *callbacks);
    SceneryConverter(const SceneryConverter&) = delete;
    SceneryConverter(SceneryConverter&&) = delete;
    SceneryConverter& operator=(const SceneryConverter&) = delete;
    SceneryConverter& operator=(SceneryConverter&&) = delete;
    virtual ~SceneryConverter() = default;

    //-----------------------------------------------------------------------------
    //! Triggers the convertion process from OpenDrive to OSI world in the
    //! following steps:
    //! - defines a unique direction within the road cluster (MarkDirections())
    //! - assigns IDs to OpenDrive lane sections
    //! - generates the mappings for lanes and lane sections from OpenDrive to OWL
    //! - then connects these sections
    //! - finally, triggers convertion of the road geometries
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool Convert();

    bool CreateXfObjects();

    bool CreateTrafficDevices();

    // TODO
    /*
    bool GenerateAndLinkSectionObject(TRAFFIC_DEVICE * trafficDevice,
                                      Interface::SECTION * section,
                                      SECTION_OBJ **sectionObject);

    bool GenerateAndLinkLaneObject(SECTION_OBJ *sectionObj,
                                   Interface::LANE *xfLane,
                                   LANE_OBJ **laneObject);
    */

    /*!
    * \brief Returns the distance to the section start.
    *
    * This function recursivly adds up the lengths of the previous sections then returns the sum.
    *
    * @param[in]     currentDistance           Current distance to section start that was already calculated.
    * @param[in]     section                   Section for which the distance is being calculated.
    * @return        Distance to section start in m.
    */
    // TODO
    //static double GetDistanceToSectionStart (double currentDistance, const Interface::SECTION *section);

    //static std::tuple<bool, double, double, double> CalculateAbsoluteCoordinates(Interface::SECTION* section, const RoadObjectInterface *object);
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
    //!   be converted to OWL lanes/sections which point into the same direction)
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
    //! Connects the OWL representation of the two provided lanes.
    //!
    //! @param[in]  currentLane         First lane to connect
    //! @param[in]  currentContactPoint Contact point of the first lane
    //! @param[in]  otherLane           Second lane to connect
    //! @param[in]  otherContactPoint   Contact point of the second lane
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectLaneToLane(RoadLaneInterface *currentLane,
                           ContactPointType currentContactPoint,
                           RoadLaneInterface *otherLane,
                           ContactPointType otherContactPoint);

    //-----------------------------------------------------------------------------
    //! Connects a lane to a lane section in OWL by connecting the predecessor and
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
                              RoadLaneSectionInterface *otherLaneSection,
                              ContactPointType otherContactPoint);

    //-----------------------------------------------------------------------------
    //! Connects a lane section to a lane section in OWL by first connecting all
    //! lanes in the first section to the second section, then connecting all lanes
    //! in the second section to the first.
    //!
    //! Preconditions:
    //! - A mapping from lane section objects to LINEAR_SECTION objects already
    //!     exists
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
    //! Connects the lanes from two connected roads.
    //!
    //! Preconditions:
    //! - A mapping from lane section objects to LINEAR_SECTION objects already
    //!     exists
    //!
    //!
    //! @param[in]  firstLaneSection    First lane section to connect
    //! @param[in]  currentContactPoint Contact point of the first lane section
    //! @param[in]  secondLaneSection   Lane section to connect
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectExternalLanes(RoadLaneSectionInterface *firstLaneSection,
                      ContactPointType firstContactPoint,
                      RoadLaneSectionInterface *secondLaneSection,
                      std::map<int,int> * laneIdMapping
                              );



    //-----------------------------------------------------------------------------
    //! Connects a road in the scenery to its predecessor and successor roads in
    //! OWL by connecting their respective lane sections. Only connects roads with roadlinktype Road
    //!
    //! @param[in]  road                Road which should be connected to its predecessors
    //!                                 and successors
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectRoadExternalWithElementTypeRoad(RoadInterface *road);


    //-----------------------------------------------------------------------------
    //! Connects a road in the scenery internally in OWL by connecting all of its
    //! stored lane sections with their predecessors and successors.
    //!
    //! @param[in]  road                Road in which the lanes and lane sections
    //!                                 should be connected
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectRoadInternal(RoadInterface *road);

    //-----------------------------------------------------------------------------
    //! Connects OWL sections for all roads in the scenery internally and externally.
    //!
    //! Precondition:
    //! - Lanes in scenery are sorted within each laneSection with
    //!   descending ids from left to right (in driving direction)
    //!   (see comment in XfLinearSection.h, Rev. 5820)
    //!
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool ConnectRoads();

    //-----------------------------------------------------------------------------
    //! Adds a mapping from the lane sections in the provided OpenDrive road to OWL
    //! linear sections to the stored mappings, then calls AddToSection for each section.
    //!
    //! @param[in]  road                OpenDrive road to be converted to linear
    //!                                 sections
    //! @return                         False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool CreateLinearSections(RoadInterface *road);


    //-----------------------------------------------------------------------------
    //! Adds mappings for all OpenDrive lane sections and lanes to their corresponding
    //! OWL counterparts for all roads in the scenery. Calls CreateLinearSections
    //! for each road.
    //!
    //! Precondition:
    //! - Lanes in scenery are sorted within each laneSection with
    //!   descending ids from left to right (in driving direction)
    //!   (see comment in XfLinearSection.h, Rev. 5820)
    //!
    //! Notes:
    //! - OpenDrive center lanes are skipped (width=0 by convention)
    //!
    //! @return                     False, if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool CreateSections();

    SceneryInterface *scenery;
    OWL::WorldData& worldData;

    std::list<RoadInterface*> pendingCluster; // road cluster which is currently processed

    constexpr static const double SAMPLING_RATE = 1.0; // 1m sampling rate of reference line

    const CallbackInterface *callbacks;

    int trafficObjectId = 0;
    int trafficDeviceId = 0;

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

    int trafficSignCounter{0};

    int laneCounter {0};

    int objectCounter {0};
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
