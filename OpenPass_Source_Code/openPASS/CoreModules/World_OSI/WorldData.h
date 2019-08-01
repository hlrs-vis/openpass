/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  WorldData.h
//! @brief This file provides access to underlying data structures for
//!        scenery and dynamic objects
//-----------------------------------------------------------------------------

#pragma once

#include <unordered_map>

#include "OWL/DataTypes.h"
#include "Interfaces/roadInterface/roadInterface.h"
#include "Interfaces/roadInterface/junctionInterface.h"
#include "Interfaces/worldInterface.h"

#include "osi/osi_groundtruth.pb.h"
#include "osi/osi_sensorview.pb.h"
#include "osi/osi_sensorviewconfiguration.pb.h"
#include "osi/osi_worldinterface.pb.h"

namespace OWL {

using Lane              = Interfaces::Lane;
using Section           = Interfaces::Section;
using Road              = Interfaces::Road;
using Junction          = Interfaces::Junction;
using StationaryObject  = Implementation::StationaryObject;
using MovingObject      = Implementation::MovingObject;
using Vehicle           = Implementation::Vehicle;
using TrafficSign       = Implementation::TrafficSign;

using CLane              = const Interfaces::Lane;
using CSection           = const Interfaces::Section;
using CRoad              = const Interfaces::Road;
using CStationaryObject  = const Implementation::StationaryObject;
using CMovingObject      = const Implementation::MovingObject;
using CVehicle           = const Implementation::Vehicle;

using Lanes             = std::list<Lane*>;
using Sections          = std::list<Section*>;
using Roads             = std::list<Road*>;
using StationaryObjects = std::list<StationaryObject*>;
using MovingObjects     = std::list<MovingObject*>;
using Vehicles          = std::list<Vehicle*>;

using CLanes             = const std::list<CLane*>;
using CSections          = const std::list<CSection*>;
using CRoads             = const std::list<CRoad*>;
using CStationaryObjects = const std::list<CStationaryObject*>;
using CMovingObjects     = const std::list<CMovingObject*>;
using CVehicles          = const std::list<CVehicle*>;

namespace Interfaces {

//!This class contains the entire road network and all objects in the world
class WorldData
{
public:
    virtual ~WorldData() = default;

    //!Deletes all objects, roads, sections and lanes
    virtual void Clear() = 0;

    /*!
     * \brief Creates a OSI SensorView
     *
     * \param[in]   conf      SensorViewConfiguration to create SensorView from
     * \param[in]   host_id   The Id of the associated Agent
     *
     * \return      A OSI SensorView with filtered GroundTruth
     */
    virtual osi3::SensorView GetSensorView(osi3::SensorViewConfiguration& conf, int agentId) = 0;

    //!Returns a map of all Roads with their OSI Id
    virtual const std::unordered_map<Id, Road*>& GetRoads() const = 0;

    //!Creates a new MovingObject linked to an AgentAdapter and returns it
    //!
    //!@param linkedObject  Object of type AgentAdapter which will be linked to new MovingObject
    virtual Interfaces::MovingObject& AddMovingObject(void* linkedObject) = 0;

    //!Creates a new StationaryObject linked to a TrafficObjectAdapter and returns it
    //!
    //!@param linkedObject  Object of type TrafficObjectAdapter which will be linked to new StationaryObject
    virtual Interfaces::StationaryObject& AddStationaryObject(void* linkedObject) = 0;

    //!Creates a new TrafficSign and returns it
    virtual Interfaces::TrafficSign& AddTrafficSign() = 0;

    //!Deletes the moving object with the specified Id
    virtual void RemoveMovingObjectById(Id id) = 0; // change Id to MovingObject

    //!Returns the mapping of OSI Ids to OpenDrive Ids for lanes
    virtual const std::unordered_map<Id, OdId>& GetLaneIdMapping() const = 0;

    //!Returns the mapping of OSI Ids to OpenDrive Ids for roads
    virtual const std::unordered_map<Id, std::string>& GetRoadIdMapping() const = 0;

    //!Returns an invalid lane
    virtual const Implementation::InvalidLane& GetInvalidLane() const = 0;

    //!Returns a map of all lanes with their OSI Id
    virtual const std::unordered_map<Id, Lane*>& GetLanes() const = 0;

    //!Returns a map of all sections with their OSI Id
    virtual const std::map<Id, Section*>& GetSections() const = 0;

    //!Returns a map of all traffic signs with their OSI Id
    virtual const std::unordered_map<Id, TrafficSign*>& GetTrafficSigns() const = 0;

    //!Creates a new lane with parameters specified by the OpenDrive lane
    //!
    //!@param odSection OpenDrive section to add lane to
    //!@param odLane    OpenDrive lane to add
    virtual void AddLane(RoadLaneSectionInterface& odSection, const RoadLaneInterface& odLane) = 0;

    //!Creates a new section with parameters specified by the OpenDrive section
    //!
    //!@param odRoad    OpenDrive road to add section to
    //!@param odSection OpenDrive section to add
    virtual void AddSection(const RoadInterface& odRoad, const RoadLaneSectionInterface& odSection) = 0;

    //!Creates a new road with parameters specified by the OpenDrive road
    //!
    //!@param odRoad    OpenDrive road to add
    virtual void AddRoad(const RoadInterface& odRoad) = 0;

    //!Creates a new junction with parameters specified by the OpenDrive junction
    //!
    //!@param odJunction    OpenDrive junction to add
    virtual void AddJunction(const JunctionInterface* odJunction) = 0;

    //!Adds a connection road (path) to a junction
    virtual void AddJunctionConnection(const JunctionInterface* odJunction, const RoadInterface& odRoad) = 0;

    //!Adds a new lane geometry joint to the end of the current list of joints of the specified lane
    //! and a new geometry element
    //!
    //! @param pointLeft    left point of the new joint
    //! @param pointCenter  reference point of the new joint
    //! @param pointRight   right point of the new joint
    //! @param sOffset      s offset of the new joint
    //! @param curvature    curvature of the lane at sOffset
    //! @param heading      heading of the lane at sOffset
    virtual void AddLaneGeometryPoint(const RoadLaneInterface& odLane,
                                      const Common::Vector2d& pointLeft,
                                      const Common::Vector2d& pointCenter,
                                      const Common::Vector2d& pointRight,
                                      const double sOffset,
                                      const double curvature,
                                      const double heading) = 0;

    //!Sets successorOdLane as successor of odLane
    virtual void AddLaneSuccessor(/* const */ RoadLaneInterface& odLane,
            /* const */ RoadLaneInterface& successorOdLane) = 0;

    //!Sets predecessorOdLane as predecessor of odLane
    virtual void AddLanePredecessor(/* const */ RoadLaneInterface& odLane,
            /* const */ RoadLaneInterface& predecessorOdLane) = 0;

    //!Sets successorRoad as successor of road in OSI
    virtual void SetRoadSuccessor(const RoadInterface& road,  const RoadInterface& successorRoad) = 0;

    //!Sets predecessorRoad as predecessor of road in OSI
    virtual void SetRoadPredecessor(const RoadInterface& road,  const RoadInterface& predecessorRoad) = 0;

    //!Sets successorSection as successor of section in OSI
    virtual void SetSectionSuccessor(const RoadLaneSectionInterface& section,  const RoadLaneSectionInterface& successorSection) = 0;

    //!Sets predecessorSection as predecessor of section in OSI
    virtual void SetSectionPredecessor(const RoadLaneSectionInterface& section,  const RoadLaneSectionInterface& predecessorSection) = 0;

    //!Currently not implemented
    virtual void ConnectLanes(/*const*/ RoadLaneSectionInterface& firstOdSection,
                                        /*const*/ RoadLaneSectionInterface& secondOdSection,
                                        const std::map<int, int>& lanePairs,
                                        bool isPrev) = 0;

    //!Resets the world for new run; deletes all moving objects
    virtual void Reset() = 0;
};

}

class WorldData : public Interfaces::WorldData
{
public:
    ~WorldData() override;

    void Clear() override;

    osi3::SensorView GetSensorView(osi3::SensorViewConfiguration& conf, int agentId) override;

    /*!
     * \brief Retrieves a filtered OSI GroundTruth
     *
     * \param[in]   conf        The OSI SensorViewConfiguration to be used for filtering
     * \param[in]   reference   Host of the sensor
     *
     * \return      A OSI GroundTruth filtered by the given SensorViewConfiguration
     */
    osi3::GroundTruth GetFilteredGroundTruth(const osi3::SensorViewConfiguration& conf, const Interfaces::MovingObject& reference);

    /*!
     * \brief Retrieves the TrafficSigns located in the given sector (geometric shape)
     *
     * \param[in]   origin      Origin of the sector shape
     * \param[in]   radius      Radius of the sector shape
     * \param[in]   absYawMin   Right boundary angle of the sector shape
     * \param[in]   absYawMax   Left boundary angle of the sector shape
     *
     * \return      Vector of TrafficSing pointers located in the given sector
     */
    std::vector<const Interfaces::TrafficSign*> GetTrafficSignsInSector(const Primitive::AbsPosition& origin,
                                                                        double radius,
                                                                        double absYawMin,
                                                                        double absYawMax);

    /*!
     * \brief Retrieves the StationaryObjects located in the given sector (geometric shape)
     *
     * \param[in]   origin      Origin of the sector shape
     * \param[in]   radius      Radius of the sector shape
     * \param[in]   absYawMin   Right boundary angle of the sector shape
     * \param[in]   absYawMax   Left boundary angle of the sector shape
     *
     * \return      Vector of StationaryObject pointers located in the given sector
     */
    std::vector<const Interfaces::StationaryObject*> GetStationaryObjectsInSector(const Primitive::AbsPosition& origin,
                                                                                  double radius,
                                                                                  double absYawMin,
                                                                                  double absYawMax);

    /*!
     * \brief Retrieves the MovingObjects located in the given sector (geometric shape)
     *
     * \param[in]   origin      Origin of the sector shape
     * \param[in]   radius      Radius of the sector shape
     * \param[in]   absYawMin   Right boundary angle of the sector shape
     * \param[in]   absYawMax   Left boundary angle of the sector shape
     *
     * \return      Vector of MovingObject pointers located in the given sector
     */
    std::vector<const Interfaces::MovingObject*> GetMovingObjectsInSector(const Primitive::AbsPosition& origin,
                                                                          double radius,
                                                                          double absYawMin,
                                                                          double absYawMax);

    /*!
     * \brief Retrieves the OWL Id of an agent
     *
     * \param   agentId[in]     Agent id (as used in AgentInterface)
     *
     * \return  OWL Id of the underlying OSI object
     */
    OWL::Id GetOwlId(int agentId);

    void AddLane(RoadLaneSectionInterface &odSection, const RoadLaneInterface& odLane) override;
    void AddSection(const RoadInterface& odRoad, const RoadLaneSectionInterface& odSection) override;
    void AddRoad(const RoadInterface& odRoad) override;
    void AddJunction(const JunctionInterface* odJunction) override;
    void AddJunctionConnection(const JunctionInterface* odJunction, const RoadInterface& odRoad) override;

    void AddLanePairing(/* const */ RoadLaneInterface& odLane,
                                    /* const */ RoadLaneInterface& predecessorOdLane,
                                    /* const */ RoadLaneInterface& successorOdLane);
    void AddLaneSuccessor(/* const */ RoadLaneInterface& odLane,
                                      /* const */ RoadLaneInterface& successorOdLane) override;
    void AddLanePredecessor(/* const */ RoadLaneInterface& odLane,
                                        /* const */ RoadLaneInterface& predecessorOdLane) override;


    void ConnectLanes(/*const*/ RoadLaneSectionInterface& firstOdSection,
                                /*const*/ RoadLaneSectionInterface& secondOdSection,
                                const std::map<int, int>& lanePairs,
                                bool isPrev) override;
    void SetRoadSuccessor(const RoadInterface& road,  const RoadInterface& successorRoad) override;
    void SetRoadPredecessor(const RoadInterface& road,  const RoadInterface& predecessorRoad) override;
    void SetRoadSuccessorIntersection(const RoadInterface& road,  const RoadInterface& successorIntersection);
    void SetRoadPredecessorIntersection(const RoadInterface& road,  const RoadInterface& predecessorIntersection);
    void SetSectionSuccessor(const RoadLaneSectionInterface& section,  const RoadLaneSectionInterface& successorSection) override;
    void SetSectionPredecessor(const RoadLaneSectionInterface& section,  const RoadLaneSectionInterface& predecessorSection) override;

    Interfaces::MovingObject& AddMovingObject(void* linkedObject) override;
    Interfaces::StationaryObject& AddStationaryObject(void* linkedObject) override;
    Interfaces::TrafficSign& AddTrafficSign() override;

    void RemoveMovingObjectById(Id id) override;

    void AddLaneGeometryPoint(const RoadLaneInterface& odLane,
                              const Common::Vector2d& pointLeft,
                              const Common::Vector2d& pointCenter,
                              const Common::Vector2d& pointRight,
                              const double sOffset,
                              const double curvature,
                              const double heading) override;

    const std::unordered_map<Id, MovingObject*>& GetMovingObjects() const;
    const std::unordered_map<Id, StationaryObject*>& GetStationaryObjects() const;

    CSection& GetSectionById(Id id) const;
    CRoad& GetRoadById(Id id) const;
    CStationaryObject& GetStationaryObjectById(Id id) const;
    CMovingObject& GetMovingObjectById(Id id) const;

    const std::unordered_map<Id, Lane*>& GetLanes() const override;
    const std::map<Id, Section*>& GetSections() const override;
    const std::unordered_map<Id, Road*>& GetRoads() const override;
    const std::unordered_map<Id, Interfaces::TrafficSign*>& GetTrafficSigns() const override;
    const Implementation::InvalidLane& GetInvalidLane() const override {return invalidLane;}

    const std::unordered_map<Id, OdId>& GetLaneIdMapping() const override
    {
        return laneIdMapping;
    }
    const std::unordered_map<Id, std::string>& GetRoadIdMapping() const override
    {
        return roadIdMapping;
    }

    /*!
     * \brief Normalizes angles to +/- PI
     *
     * \param[in]   angle   Angle to normalize [rad]
     *
     * \return Normalized angle in range [-PI, PI] [rad]
     */
    double NormalizeAngle(double angle);

    /*!
     * \brief Applies a sector-shaped filter on a list of objects
     *
     * \param[in]   objects               List of objects to apply filter to
     * \param[in]   origin                Origin of sector-shape
     * \param[in]   radius                Radius of the sector-shape. Negative value results in empty result
     * \param[in]   leftBoundaryAngle     Angle of left boundary of sector-shape (view from origin) [-PI, PI] [rad]
     * \param[in]   rightBoundaryAngle    Angle of right boundary of sector-shape (view from origin) [-PI, PI] [rad]
     *
     * \return      Vector of objects inside the sector-shape
     */
    template<typename T>
    std::vector<const T*> ApplySectorFilter(const std::vector<T*>& objects,
                                            const Primitive::AbsPosition& origin,
                                            double radius,
                                            double leftBoundaryAngle,
                                            double rightBoundaryAngle)
    {
        std::vector<const T*> filteredObjects;
        constexpr double EPS = 1e-9;
        bool anglesDiffer = std::abs(leftBoundaryAngle - rightBoundaryAngle) > EPS;

        if (!anglesDiffer || radius <= 0.0)
        {
            return filteredObjects;
        }

        leftBoundaryAngle  = NormalizeAngle(leftBoundaryAngle);
        rightBoundaryAngle = NormalizeAngle(rightBoundaryAngle);

        bool wrappedAngle = leftBoundaryAngle < rightBoundaryAngle;
        anglesDiffer = std::abs(leftBoundaryAngle - rightBoundaryAngle) > EPS;

        for (const auto& object : objects)
        {
            const auto& absPosition = object->GetReferencePointPosition();
            const auto relativePosition = absPosition - origin;
            const auto distance = relativePosition.distance();

            if (distance > radius)
            {
                continue;
            }

            if (anglesDiffer && distance > 0.0)
            {
                double direction = std::atan2(relativePosition.y, relativePosition.x);

                if (wrappedAngle)
                {
                    if (direction < rightBoundaryAngle && direction > leftBoundaryAngle)
                    {
                        continue;
                    }
                }
                else if (direction < rightBoundaryAngle || direction > leftBoundaryAngle)
                {
                    continue;
                }
            }

            filteredObjects.push_back(object);
        }

        return filteredObjects;
    }

    void Reset() override;

private:

    uint64_t next_free_uid{0};

    std::unordered_map<Id, OdId>              laneIdMapping;
    std::unordered_map<Id, std::string>       roadIdMapping;

    std::unordered_map<Id, Lane*>             lanes;
    std::map<Id, Section*>          sections;
    std::unordered_map<Id, Road*>             roads;
    std::unordered_map<std::string, Junction*>  junctions;
    std::unordered_map<Id, StationaryObject*> stationaryObjects;
    std::unordered_map<Id, MovingObject*>     movingObjects;
    std::unordered_map<Id, Interfaces::TrafficSign*>      trafficSigns;

    std::unordered_map<const RoadInterface*, osi3::world::Road*>                   osiRoads;
    std::unordered_map<const RoadLaneSectionInterface*, osi3::world::RoadSection*> osiSections;
    std::unordered_map<const RoadLaneInterface*, osi3::world::RoadLane*>           osiLanes;

    osi3::world::WorldInterface osiGroundTruth;

    const Implementation::InvalidLane invalidLane;

    inline uint64_t CreateUid()
    {
        return next_free_uid++;
    }
};

}
