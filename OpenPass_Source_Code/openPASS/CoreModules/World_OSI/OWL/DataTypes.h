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
//! @file  DataTypes.h
//! @brief This file provides the basic datatypes of the osi world layer (OWL)
//-----------------------------------------------------------------------------

#pragma once

#include <list>
#include <memory>
#include <vector>
#include <iterator>
#include <tuple>

#include "Interfaces/worldObjectInterface.h"
#include "Interfaces/roadInterface/roadInterface.h"
#include "Interfaces/roadInterface/roadLaneInterface.h"
#include "Interfaces/roadInterface/roadLaneSectionInterface.h"
#include "Interfaces/roadInterface/roadSignalInterface.h"
#include "OpenDriveTypeMapper.h"

#include "OWL/LaneGeometryElement.h"
#include "OWL/LaneGeometryJoint.h"
#include "OWL/Primitives.h"

#include "globalDefinitions.h"
#include "osi/osi_common.pb.h"
#include "osi/osi_groundtruth.pb.h"
#include "osi/osi_trafficsign.pb.h"
#include "osi/osi_worldinterface.pb.h"

namespace OWL {

using Id = uint64_t;
constexpr Id InvalidId = std::numeric_limits<::google::protobuf::uint64>::max();

using OdId = int64_t;

using Angle = float;

constexpr double EVENTHORIZON = 2000;

enum class MeasurementPoint
{
    RoadStart,
    RoadEnd
};


/// \brief Info on the type of a point, related to an agent
enum class PointType
{
    Reference,
    MainLaneLocator,
    CornerLeftRear,
    CornerLeftFront,
    CornerRightFront,
    CornerRightRear,
    EdgeLeft,
    EdgeRight
};


enum class LaneType
{
    Undefined = 0,
    None,
    Driving,
    Stop,
    Shoulder,
    Biking,
    Sidewalk,
    Border,
    Restricted,
    Parking,
    Bidirectional,
    Median,
    Special1,
    Special2,
    Special3,
    Roadworks,
    Tram,
    Rail,
    Entry,
    Exit,
    OffRamp,
    OnRamp
};

struct SearchablePoint
{
    Primitive::AbsCoordinate coordinate;
    PointType pointType;
};

struct LocatedPoint
{
    RoadPosition coordinate;
    PointType pointType;
};

template<typename T>
struct Lazy
{
private:
    T value{};
    bool valid{false};

public:
    void Update(T newValue)
    {
        value = newValue;
        valid = true;
    }

    void Invalidate()
    {
        valid = false;
    }

    T Get() const
    {
        if (valid)
        {
            return value;
        }
        else
        {
            throw std::logic_error("retrieved invalidated lazy");
        }
    }

    bool IsValid() const
    {
        return valid;
    }
};

template<typename T>
class ForwardStream : public std::iterator < std::forward_iterator_tag, T >
{
public:
    T* item;

    ForwardStream(T* item) : item{item} {}
    ForwardStream() : item{nullptr} {}

    ForwardStream& operator++()
    {
        item = item->IsInStreamDirection() ? item->GetNext() : item->GetPrevious();
        return *this;
    }

    ForwardStream operator++(int)
    {
        ForwardStream tmp(*this);
        item = item->IsInStreamDirection() ? item->GetNext() : item->GetPrevious();
        return tmp;
    }

    T& operator*() const { return *item; }
    T* operator->() const { return item; }
    bool operator!=(const ForwardStream& other) const { return !(*this == other); }
    bool operator==(const ForwardStream& other) const { return item == other.item; }
};

template<typename T>
class ReverseStream : public std::iterator<std::forward_iterator_tag, T>
{
public:
    T* item;

    ReverseStream(T* item) : item{item} {}
    ReverseStream() : item{nullptr} {}

    ReverseStream& operator++()
    {
        item = item->IsInStreamDirection() ? item->GetPrevious() : item->GetNext();
        return *this;
    }

    ReverseStream operator++(int)
    {
        ReverseStream tmp(*this);
        item = item->IsInStreamDirection() ? item->GetPrevious() : item->GetNext();
        return tmp;
    }

    T& operator*() const { return *item; }
    T* operator->() const { return item; }
    bool operator!=(const ReverseStream& other) const { return !(*this == other); }
    bool operator==(const ReverseStream& other) const { return item == other.item; }
};


namespace Interfaces {
class Lane;
class Section;
class Road;
class Junction;
class WorldObject;
class StationaryObject;
class MovingObject;

using LaneGeometryElements = std::vector<Primitive::LaneGeometryElement*>;
using LaneGeometryJoints   = std::vector<Primitive::LaneGeometryJoint>;
using Lanes                = std::list<const Lane*>;
using Sections             = std::list<const Section*>;
using Roads                = std::list<const Road*>;
using WorldObjects         = std::list<WorldObject*>;
using MovingObjects        = std::list<MovingObject*>;
using StationaryObjects    = std::list<StationaryObject*>;


//! Represents consecutive lanes as specified by the OpenDrive successor definitons
class ForwardLaneStream
{
private:
    Lane* current{nullptr};
public:
    ForwardLaneStream() : current{nullptr} {}

    //! Constructs a ForwardLaneStream starting at the given lane
    //! @param current  starting lane of the stream
    ForwardLaneStream(Lane* current) : current{current} {}

    //! Returns an Iterator for lanes in downstream direction defined by the OpenDrive successors
    ForwardStream<const Lane> begin() const { return {current}; }
    ForwardStream<const Lane> end() const { return {nullptr}; }
};

//! Represents consecutive lanes in reverse order as specified by the OpenDrive predecessor definitons
class ReverseLaneStream
{
private:
    Lane* current{nullptr};
public:
    ReverseLaneStream() : current{nullptr} {}

    //! Constructs a ReverseLaneStream starting at the given lane
    //! @param current  starting lane of the stream
    ReverseLaneStream(Lane* current) : current{current} {}

    //! Returns an Iterator for lanes in upstream direction defined by the OpenDrive predecessors
    ReverseStream<const Lane> begin() const { return {current}; }
    ReverseStream<const Lane> end() const { return {nullptr}; }
};


//! Represents consecutive sections. This is primary derived from the OpenDrive laneSections order.
class ForwardSectionStream
{
private:
    Section* current{nullptr};
public:
    ForwardSectionStream() : current{nullptr} {}

    //! Constructs a ForwardSectionStream starting at the given section
    //! @param current  starting section of the stream
    ForwardSectionStream(Section* current) : current{current} {}

    //! Returns an Iterator for sections in downstream direction.
    ForwardStream<const Section> begin() const { return {current}; }
    ForwardStream<const Section> end() const { return {nullptr}; }
};

//! Represents consecutive sections in reverse order. This is primary derived from the OpenDrive laneSections order.
class ReverseSectionStream
{
private:
    Section* current{nullptr};
public:
    ReverseSectionStream() : current{nullptr} {}

    //! Constructs a ReverseSectionStream starting at the given section
    //! @param current  starting section of the stream
    ReverseSectionStream(Section* current) : current{current} {}

    //! Returns an Iterator for sections in upstream direction.
    ReverseStream<const Section> begin() const { return {current}; }
    ReverseStream<const Section> end() const { return {nullptr}; }
};


//! This class represents a single lane inside a section
class Lane
{
public:
    Lane();
    virtual ~Lane() = default;

    /*!
     * \brief Copies the underlying OSI object to the given GroundTruth
     *
     * \param[in]   target   The target OSI GroundTruth
     */
    virtual void CopyToGroundTruth(osi3::GroundTruth& target) const = 0;

    //! Returns the OSI Id
    virtual Id GetId() const = 0;

    //! Returns false of this lane is invalid, otherwise returns true
    virtual bool Exists() const = 0;

    //!Returns the section that this lane is part of
    virtual const Section& GetSection() const = 0;

    //!Returns the road that this lane is part of
    virtual const Road& GetRoad() const = 0;

    //!Returns all lane geometry elements of the lane
    virtual const LaneGeometryElements& GetLaneGeometryElements() const = 0;

    //!Returns the length of the lane
    virtual double GetLength() const = 0;

    //!Returns the number of lanes to the right of this lane
    virtual int GetRightLaneCount() const = 0;

    //!Returns the type of the lane
    virtual LaneType GetLaneType() const = 0;

    //!Returns the curvature of the lane at the specified distance
    //!
    //! @param distance s coordinate
    virtual double GetCurvature(double distance) const = 0;

    //!Returns the width of the lane at the specified distance
    //!
    //! @param distance s coordinate
    virtual double GetWidth(double distance) const = 0;

    //!Returns the direction of the lane at the specified distance
    //!
    //! @param distance s coordinate
    virtual double GetDirection(double distance) const = 0;

    //!Returns the left, right and reference point at the specified s coordinate interpolated between the geometry joints
    //!
    //! @param distance s coordinate
    virtual const Primitive::LaneGeometryJoint::Points GetInterpolatedPointsAtDistance(double distance) const = 0;

    //!Returns the sucessor of this lane or nullptr if the lane has no successor
    virtual const Lane* GetNext() const = 0;

    //!Returns the predecessor of this lane or nullptr if the lane has no predecessor
    virtual const Lane* GetPrevious() const = 0;

    //!Returns the (possibly invalid) lane to the right of this lane
    virtual const Lane& GetLeftLane() const = 0;

    //!Returns the (possibly invalid) lane to the right of this lane
    virtual const Lane& GetRightLane() const = 0;

    //!Returns whether the direction of the lane aligns with the lane stream direction
    //!If false, then the next lane in the forwardLaneStream
    //!is the predecessor instead of the successor
    virtual bool IsInStreamDirection() const = 0;

    //!Calculates the stream id of the lane, i.e. the Id of the first lane in the stream
    virtual void UpdateStreamId() = 0;

    //!Returns the stream id of the lane, i.e. the Id of the first lane in the stream
    virtual Id GetStreamId() const = 0;

    //!Returns the s coordinate of the lane start if measure point is road start
    //! or the s coordinate of the lane end if measure point is road end
    virtual double GetDistance(MeasurementPoint measurementPoint) const = 0;

    //!Returns true if the specified distance is valid on this lane, otherwise returns false
    virtual bool Covers(double distance) const = 0;

    //!Sets the sucessor of the lane. Throws an error if the lane already has a sucessor
    virtual void AddNext(const Interfaces::Lane& lane) = 0;

    //!Sets the predecessor of the lane. Throws an error if the lane already has a predecessor
    virtual void AddPrevious(const Interfaces::Lane& lane) = 0;

    //!Adds a new lane geometry joint to the end of the current list of joints
    //!
    //! @param pointLeft    left point of the new joint
    //! @param pointCenter  reference point of the new joint
    //! @param pointRight   right point of the new joint
    //! @param sOffset      s offset of the new joint
    //! @param curvature    curvature of the lane at sOffset
    //! @param heading      heading of the lane at sOffset
    virtual void AddLaneGeometryJoint(const Common::Vector2d& pointLeft,
                                      const Common::Vector2d& pointCenter,
                                      const Common::Vector2d& pointRight,
                                      double sOffset,
                                      double curvature,
                                      double heading) = 0;

    //!Sets type of the lane
    virtual void SetLaneType(LaneType specifiedType) = 0;

    //!Sets lane to the left of this lane
    virtual void SetLeftLane(const Interfaces::Lane& lane) = 0;

    //!Sets lane to the right of this lane
    virtual void SetRightLane(const Interfaces::Lane& lane) = 0;


    //!Returns a vector of all WorldObjects that are currently in this lane
    virtual const Interfaces::WorldObjects& GetWorldObjects() const = 0;

    //!Adds a MovingObject to the list of objects currently in this lane
    virtual void AddMovingObject(OWL::Interfaces::MovingObject& movingObject) = 0;

    //!Adds a StationaryObject to the list of objects currently in this lane
    virtual void AddStationaryObject(OWL::Interfaces::StationaryObject& stationaryObject) = 0;

    //!Adds a WorldObject to the list of objects currently in this lane
    virtual void AddWorldObject(Interfaces::WorldObject& worldObject) = 0;

    //!Removes all MovingObjects from the list of objects currently in this lane while keeping StationaryObjects
    virtual void ClearMovingObjects() = 0;

    //!ForwardLaneStream starting with this lane
    const ForwardLaneStream forwardLaneStream;

    //!ReverseLaneStream starting with this lane
    const ReverseLaneStream reverseLaneStream;
};


//!This class represents one section of a road.
class Section
{
public:
    Section();
    virtual ~Section() = default;

    //!Returns the OSI Id of the section
    virtual Id GetId() const = 0;

    //!Sets the sucessor of the section. Throws an error if the section already has a sucessor
    virtual void AddNext(const Interfaces::Section& section) = 0;

    //!Sets the predecessor of the section. Throws an error if the section already has a predecessor
    virtual void AddPrevious(const Interfaces::Section& section) = 0;

    //!Returns the sucessor of this section or nullptr if section lane has no successor
    virtual const Section* GetNext() const = 0;

    //!Returns the predecessor of this section or nullptr if the section has no predecessor
    virtual const Section* GetPrevious() const = 0;

    //!Returns whether the direction of the section aligns with the section stream direction
    //!If false, then the next lane in the forwardSectionStream
    //!is the predecessor instead of the successor
    virtual bool IsInStreamDirection() const = 0;

    //!Calculates the stream id of the section, i.e. the Id of the first section in the stream
    virtual void UpdateStreamId() = 0;

    //!Returns the stream id of the section, i.e. the Id of the first section in the stream
    virtual Id GetStreamId() const = 0;

    //!Adds a lane to this section
    virtual void AddLane(const Interfaces::Lane& lane) = 0;

    //!Sets the road that this section is part of
    virtual void SetRoad(Interfaces::Road* road) = 0;

    //!Returns a vector of all lanes of the section
    virtual const Lanes& GetLanes() const = 0;

    //!Returns the road that this section is part of
    virtual const Interfaces::Road& GetRoad() const = 0;

    //!Returns the s coordinate of the section start if measure point is road start
    //! or the s coordinate of the section end if measure point is road end
    virtual double GetDistance(MeasurementPoint measurementPoint) const = 0;

    //!Returns true if the specified distance is valid on this sections, otherwise returns false
    virtual bool Covers(double distance) const = 0;

    //!Returns true at least one s coordinate between startDistance and endDistance is valid on this section
    virtual bool CoversInterval(double startDistance, double endDistance) const = 0;

    //!Returns the s coordinate of the section start
    virtual double GetSOffset() const = 0;

    //!Returns the length of the section
    virtual double GetLength() const = 0;

    //!ForwardLaneStream starting with this section
    const ForwardSectionStream forwardSectionStream;

    //!ReverseLaneStream starting with this section
    const ReverseSectionStream reverseSectionStream;
};

//!This class represents a road in the world
class Road
{
public:
    virtual ~Road() = default;

    //!Returns the OSI Id of the road
    virtual Id GetId() const = 0;

    //!Adds a section to this road
    virtual void AddSection(Interfaces::Section& section) = 0;

    //!Returns a vector of all sections that this road consists of
    virtual const Sections& GetSections() const = 0;

    //!Returns the length of the road
    virtual double GetLength() const = 0;
};

//!This class represents a junction in the world
class Junction
{
public:
    virtual ~Junction() = default;

    //!Returns the Id of the Junction
    virtual std::string GetId() const = 0;

    //!Adds a connecting road to this road
    virtual void AddConnectingRoad(const Interfaces::Road* connectingRoad) = 0;

    //!Returns a vector of all connecting roads that this road consists of
    virtual const Roads& GetConnectingRoads() const = 0;
};

//!Common base for all objects in the world that can be on the road and have a position and a dimension
//! WorldObjects are either MovingObjects or StationaryObjects
class WorldObject
{
public:
    virtual ~WorldObject() = default;

    /*!
     * \brief Copies the underlying OSI object to the given GroundTruth
     *
     * \param[in]   target   The target OSI GroundTruth
     */
    virtual void CopyToGroundTruth(osi3::GroundTruth& target) const = 0;

    //!Returns the OSI Id of the object
    virtual Id GetId() const = 0;

    //!Returns the dimension of the object
    virtual Primitive::Dimension GetDimension() const = 0;

    //!Returns the position of the reference point of the object in absolute coordinates (i. e. world coordinates)
    virtual Primitive::AbsPosition GetReferencePointPosition() const = 0;

    //!Returns the orientation of the object in absolute coordinates (i. e. world coordinates)
    virtual Primitive::AbsOrientation GetAbsOrientation() const = 0;

    //!Returns the absolute value of the velocity if the object is moving in road direction
    //! or the inverse if the object is moving against the road direction
    virtual double GetAbsVelocityDouble() const = 0;

    //!Returns the absolute value of the acceleration if the object is moving in road direction
    //! or the inverse if the object is moving against the road direction
    virtual double GetAbsAccelerationDouble() const = 0;

    //!Returns the road coordinates of the object
    virtual RoadPosition GetRoadCoordinate() const = 0;

    //!Assigns a lane to this object
    virtual void AddLaneAssignment(const Interfaces::Lane& lane) = 0;

    //!Returns a vector of all lanes that this object is assigned to
    virtual const Interfaces::Lanes& GetLaneAssignments() const = 0;

    //!Clears the list of lanes that this object is assigned to
    virtual void ClearLaneAssignments() = 0;

    //!Sets the position of the reference point of the object in absolute coordinates (i. e. world coordinates)
    virtual void SetReferencePointPosition(const Primitive::AbsPosition& newPosition) = 0;

    //!Sets the dimension of the object
    virtual void SetDimension(const Primitive::Dimension& newDimension) = 0;

    //!Sets the orientation of the object in absolute coordinates (i. e. world coordinates)
    virtual void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation) = 0;

    //!Sets the road coordinates of the object
    virtual void SetRoadCoordinate(const RoadPosition& newCoordinate) = 0;

    //!Returns the s coordinate of the point with the highest s coordinate if measurement is
    //! road end or the s coordinate of the point with the lowest s coordinate if measurement is
    //! road start
    virtual double GetDistance(MeasurementPoint measurementPoint) const = 0;

    //!Returns the WorldObjectInterface that this WorldObject is linked to and casts it to T*
    //! T can be
    //! -WorldObjectsAdapter or
    //! -AgentAdapter, if this is a MovingObject or
    //! -TrafficAdapter, if this is a StationaryObject
    template <typename T>
    T* GetLink()
    {
        auto link = dynamic_cast<T*>(static_cast<WorldObjectInterface*>(linkedObject));
        assert(link != nullptr);
        return link;
    }

    //!Returns the WorldObjectInterface that this WorldObject is linked to and casts it to T*
    //! T can be
    //! -WorldObjectsAdapter or
    //! -AgentAdapter, if this is a MovingObject or
    //! -TrafficAdapter, if this is a StationaryObject
    template <typename T>
    T* GetLink() const
    {
        auto link = dynamic_cast<T*>(static_cast<WorldObjectInterface*>(linkedObject));
        assert(link != nullptr);
        return link;
    }

    //!Returns true is this object is of type T otherwise false
    template <typename T>
    bool Is() const
    {
        return !(dynamic_cast<const T*>(this) == nullptr);
    }

    //!Returns this object casted to T*
    template <typename T>
    T* As() const
    {
        return static_cast<T*>(this);
    }

    //!Returns this object casted to T*
    template <typename T>
    T* As()
    {
        return static_cast<T*>(this);
    }

protected:
    void* linkedObject{nullptr};
};

class StationaryObject : public Interfaces::WorldObject
{
public:
    virtual ~StationaryObject() = default;

    virtual RoadPosition GetRoadCoordinate() const = 0;

    virtual void SetReferencePointPosition(const Primitive::AbsPosition& newPosition) = 0;
    virtual void SetDimension(const Primitive::Dimension& newDimension) = 0;
    virtual void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation) = 0;
    virtual void SetRoadCoordinate(const RoadPosition& newCoordinate) = 0;
};


class MovingObject : public Interfaces::WorldObject
{
public:
    virtual ~MovingObject() = default;

    virtual RoadPosition GetRoadCoordinate() const = 0;
    virtual Primitive::LaneOrientation GetLaneOrientation() const = 0;

    virtual Primitive::AbsVelocity GetAbsVelocity() const = 0;

    virtual Primitive::AbsAcceleration GetAbsAcceleration() const = 0;

    virtual Primitive::AbsOrientationRate GetAbsOrientationRate() const = 0;

    virtual void SetDimension(const Primitive::Dimension& newDimension) = 0;
    virtual void SetLength(const double newLength) = 0;
    virtual void SetWidth(const double newWidth) = 0;
    virtual void SetHeight(const double newHeight) = 0;
    virtual void SetDistanceReferencPointToLeadingEdge(const double distance) = 0;
    virtual double GetDistanceReferencePointToLeadingEdge() const = 0;

    virtual void SetReferencePointPosition(const Primitive::AbsPosition& newPosition) = 0;
    virtual void SetX(const double newX) = 0;
    virtual void SetY(const double newY) = 0;
    virtual void SetZ(const double newZ) = 0;

    virtual void SetRoadCoordinate(const RoadPosition& newCoordinate) = 0;

    virtual void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation) = 0;
    virtual void SetYaw(const double newYaw) = 0;
    virtual void SetPitch(const double newPitch) = 0;
    virtual void SetRoll(const double newRoll) = 0;

    virtual void SetAbsVelocity(const Primitive::AbsVelocity& newVelocity) = 0;
    virtual void SetAbsVelocity(const double newVelocity) = 0;

    virtual void SetAbsAcceleration(const Primitive::AbsAcceleration& newAcceleration) = 0;
    virtual void SetAbsAcceleration(const double newAcceleration) = 0;

    virtual void SetAbsOrientationRate(const Primitive::AbsOrientationRate& newOrientationRate) = 0;

    virtual void AddLaneAssignment(const Interfaces::Lane& lane) = 0;
    virtual const Interfaces::Lanes& GetLaneAssignments() const = 0;
    virtual void ClearLaneAssignments() = 0;

    virtual void SetIndicatorState(IndicatorState indicatorState) = 0;
    virtual IndicatorState GetIndicatorState() const = 0;
    virtual void SetBrakeLightState(bool brakeLightState) = 0;
    virtual bool GetBrakeLightState() const = 0;
    virtual void SetHeadLight(bool headLight) = 0;
    virtual bool GetHeadLight() const = 0;
    virtual void SetHighBeamLight(bool highbeamLight) = 0;
    virtual bool GetHighBeamLight() const = 0;
};


class TrafficSign
{
public:
    virtual ~TrafficSign() = default;

    virtual std::string GetId() const = 0;
    virtual double GetS() const = 0;
    virtual double GetValue() const = 0;
    virtual CommonTrafficSign::Type GetType() const = 0;
    virtual bool IsValidForLane(OWL::Id laneId) const = 0;

    virtual void SetS(double sPos) = 0;
    virtual void SetValue(double speed) = 0;
    virtual void SetValidForLane(OWL::Id laneId) = 0;
    virtual void SetType(RoadSignalType type) = 0;

    /*!
     * \brief Copies the underlying OSI object to the given GroundTruth
     *
     * \param[in]   target   The target OSI GroundTruth
     */
    virtual void CopyToGroundTruth(osi3::GroundTruth& target) const = 0;
};



} // namespace Interfaces

namespace Implementation {


class Lane : public Interfaces::Lane
{
public:
    //! @param[in] osiLane  representation of the road in OpenDrive
    //! @param[in] section  section that this lane is part of
    //! @param[in] isInStreamDirection  flag whether this lane is in the same direction as the LaneStream it belongs to
    Lane(osi3::world::RoadLane* osiLane, const Interfaces::Section* section, bool isInStreamDirection);
    ~Lane() override;

    void CopyToGroundTruth(osi3::GroundTruth& target) const override;

    Id GetId() const override;
    bool Exists() const override;
    const Interfaces::Section& GetSection() const override;
    const Interfaces::Road& GetRoad() const override;

    double GetLength() const override;
    int GetRightLaneCount() const override;

    double GetCurvature(double distance) const override;
    double GetWidth(double distance) const override;
    double GetDirection(double distance) const override;

    const Interfaces::Lane* GetNext() const override;
    const Interfaces::Lane* GetPrevious() const override;
    const Interfaces::Lane& GetLeftLane() const override;
    const Interfaces::Lane& GetRightLane() const override;
    bool IsInStreamDirection() const override;

    void UpdateStreamId() override;
    Id GetStreamId() const override;

    double GetDistance(MeasurementPoint measurementPoint) const override;
    LaneType GetLaneType() const override;
    bool Covers(double distance) const override;

    void AddLanePairing(const Interfaces::Lane& prevLane, const Interfaces::Lane& nextLane);
    void AddNext(const Interfaces::Lane& lane) override;
    void AddPrevious(const Interfaces::Lane& lane) override;
    const Interfaces::LaneGeometryElements& GetLaneGeometryElements() const override;
    void AddLaneGeometryJoint(const Common::Vector2d& pointLeft,
                              const Common::Vector2d& pointCenter,
                              const Common::Vector2d& pointRight,
                              double sOffset,
                              double curvature,
                              double heading) override;

    void SetLeftLane(const Interfaces::Lane& lane) override;
    void SetRightLane(const Interfaces::Lane& lane) override;
    void SetLaneType(LaneType specifiedType) override;

    const Interfaces::WorldObjects& GetWorldObjects() const override;
    //    const Interfaces::MovingObjects& GetMovingObjects() const override;
    //    const Interfaces::StationaryObjects& GetStationaryObjects() const override;

    void AddMovingObject(OWL::Interfaces::MovingObject& movingObject) override;
    void AddStationaryObject(OWL::Interfaces::StationaryObject& stationaryObject) override;
    void AddWorldObject(Interfaces::WorldObject& worldObject) override;
    void ClearMovingObjects() override;

    std::tuple<const Primitive::LaneGeometryJoint*, const Primitive::LaneGeometryJoint*> GetNeighbouringJoints(
            double distance) const;

    const Primitive::LaneGeometryJoint::Points GetInterpolatedPointsAtDistance(double distance) const override;

protected:
    osi3::world::RoadLane* osiLane{nullptr};

private:
    LaneType laneType;
    Interfaces::WorldObjects worldObjects;
    Interfaces::MovingObjects movingObjects;
    Interfaces::StationaryObjects stationaryObjects;
    const Interfaces::Section* section;
    Interfaces::LaneGeometryJoints laneGeometryJoints;
    Interfaces::LaneGeometryElements laneGeometryElements;
    bool isInStreamDirection;
    const Interfaces::Lane* next{nullptr};
    const Interfaces::Lane* previous{nullptr};
    const Interfaces::Lane* leftLane;
    const Interfaces::Lane* rightLane;
    Id streamId{InvalidId};
    bool leftLaneIsDummy{section == nullptr};
    bool rightLaneIsDummy{section == nullptr};
};

class InvalidLane : public Lane
{
public:
    InvalidLane() : Lane(new osi3::world::RoadLane(), nullptr, true)
    {
        // set 'invalid' id
        osiLane->mutable_id()->set_value(InvalidId);
        UpdateStreamId();
    }

    virtual ~InvalidLane();
    InvalidLane(const InvalidLane&) = delete;
    InvalidLane& operator=(const InvalidLane&) = delete;
    InvalidLane(InvalidLane&&) = delete;
    InvalidLane& operator=(InvalidLane&&) = delete;
    const Interfaces::Lane& GetLeftLane() const override
    {
        return *this;
    }
    const Interfaces::Lane& GetRightLane() const override
    {
        return *this;
    }
};

class Section : public Interfaces::Section
{
public:
    Section(osi3::world::RoadSection* osiSection, bool isInStreamDirection);
    Id GetId() const override;

    void AddNext(const Interfaces::Section& section) override;
    void AddPrevious(const Interfaces::Section& section) override;
    const Interfaces::Section* GetNext() const override;
    const Interfaces::Section* GetPrevious() const override;

    bool IsInStreamDirection() const override;
    void UpdateStreamId() override;
    Id GetStreamId() const override;

    void AddLane(const Interfaces::Lane& lane) override;
    const Interfaces::Lanes& GetLanes() const override;

    void SetRoad(Interfaces::Road* road) override;
    const Interfaces::Road& GetRoad() const override;

    virtual double GetDistance(MeasurementPoint measurementPoint) const override;
    virtual bool Covers(double distance) const override;
    virtual bool CoversInterval(double startDistance, double endDistance) const override;

    double GetSOffset() const override;
    double GetLength() const override;

protected:
    osi3::world::RoadSection* osiSection;

private:
    Interfaces::Lanes lanes;
    const Interfaces::Road* road;
    const Interfaces::Section* next{nullptr};
    const Interfaces::Section* previous{nullptr};
    Interfaces::Sections nextSections;
    Interfaces::Sections previousSections;
    bool isInStreamDirection;
    Id streamId{InvalidId};

public:
    Section(const Section&) = delete;
    Section& operator=(const Section&) = delete;
    Section(Section&&) = delete;
    Section& operator=(Section&&) = delete;

    virtual ~Section() override = default;
};

class InvalidSection : public Section
{
public:
    InvalidSection() : Section(new osi3::world::RoadSection(), true)
    {
        // set 'invalid' id
        osiSection->mutable_id()->set_value(InvalidId);
    }

    virtual ~InvalidSection();
    InvalidSection(const InvalidSection&) = delete;
    InvalidSection& operator=(const InvalidSection&) = delete;
    InvalidSection(InvalidSection&&) = delete;
    InvalidSection& operator=(InvalidSection&&) = delete;
};

class Road : public Interfaces::Road
{
public:
    Road(osi3::world::Road* osiRoad);

    Road(const Road&) = delete;
    Road& operator=(const Road&) = delete;
    Road(Road&&) = delete;
    Road& operator=(Road&&) = delete;

    virtual ~Road() override = default;

    Id GetId() const override;
    const Interfaces::Sections& GetSections() const override;
    void AddSection(Interfaces::Section& section) override;
    double GetLength() const override;

protected:
    osi3::world::Road* osiRoad;

private:
    Interfaces::Sections sections;
};

class InvalidRoad : public Road
{
public:
    InvalidRoad() : Road(new osi3::world::Road())
    {
        // set 'invalid' id
        osiRoad->mutable_id()->set_value(InvalidId);
    }

    virtual ~InvalidRoad();
    InvalidRoad(const InvalidRoad&) = delete;
    InvalidRoad& operator=(const InvalidRoad&) = delete;
    InvalidRoad(InvalidRoad&&) = delete;
    InvalidRoad& operator=(InvalidRoad&&) = delete;
};

class Junction : public Interfaces::Junction
{
public:
    Junction(std::string id) :
        id(id)
    {}

    virtual ~Junction() override = default;

    virtual std::string GetId() const override
    {
        return id;
    }

    virtual void AddConnectingRoad(const Interfaces::Road* connectingRoad) override
    {
        connectingRoads.push_back(connectingRoad);
    }

    virtual const std::list<const Interfaces::Road*>& GetConnectingRoads() const override
    {
        return connectingRoads;
    }

    Junction(const Junction&) = delete;
    Junction& operator=(const Junction&) = delete;
    Junction(Junction&&) = delete;
    Junction& operator=(Junction&&) = delete;

private:
    std::string id;
    std::list<const Interfaces::Road*> connectingRoads;
};

class StationaryObject : public Interfaces::StationaryObject
{
public:
    StationaryObject(osi3::StationaryObject* osiStationaryObject, void* linkedObject);
    virtual ~StationaryObject() override = default;

    virtual Primitive::Dimension GetDimension() const override;

    virtual Primitive::AbsOrientation GetAbsOrientation() const override;
    virtual Primitive::AbsPosition GetReferencePointPosition() const override;
    virtual double GetAbsVelocityDouble() const override;
    virtual double GetAbsAccelerationDouble() const override;
    virtual RoadPosition GetRoadCoordinate() const override;

    void SetReferencePointPosition(const Primitive::AbsPosition& newPosition) override;
    void SetDimension(const Primitive::Dimension& newDimension) override;
    void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation) override;

    Id GetId() const override;
    void AddLaneAssignment(const Interfaces::Lane& lane) override;
    const Interfaces::Lanes& GetLaneAssignments() const override;
    void ClearLaneAssignments() override;
    double GetDistance(MeasurementPoint measurementPoint) const override;
    void SetRoadCoordinate(const RoadPosition& newCoordinate) override;

    void CopyToGroundTruth(osi3::GroundTruth& target) const override;
private:
    osi3::StationaryObject* osiObject;
    Interfaces::Lanes assignedLanes;
    RoadPosition roadCoordinate{0.0, 0.0, 0.0};
};

class MovingObject : public Interfaces::MovingObject
{
public:
    MovingObject(osi3::MovingObject* osiMovingObject, void* linkedObject);
    virtual ~MovingObject() override = default;

    virtual Id GetId() const override;

    virtual Primitive::Dimension GetDimension() const override;
    virtual Primitive::AbsPosition GetReferencePointPosition() const override;
    virtual RoadPosition GetRoadCoordinate() const override;
    virtual double GetDistanceReferencePointToLeadingEdge() const override;

    virtual Primitive::AbsOrientation GetAbsOrientation() const override;
    virtual Primitive::LaneOrientation GetLaneOrientation() const override;

    virtual Primitive::AbsVelocity GetAbsVelocity() const override;
    virtual double GetAbsVelocityDouble() const override;

    virtual Primitive::AbsAcceleration GetAbsAcceleration() const override;
    virtual double GetAbsAccelerationDouble() const override;

    virtual Primitive::AbsOrientationRate GetAbsOrientationRate() const override;

    virtual double GetDistance(MeasurementPoint measurementPoint) const override;

    virtual void SetDimension(const Primitive::Dimension& newDimension) override;
    virtual void SetLength(const double newLength) override;
    virtual void SetWidth(const double newWidth) override;
    virtual void SetHeight(const double newHeight) override;
    virtual void SetDistanceReferencPointToLeadingEdge(const double distance) override;

    virtual void SetReferencePointPosition(const Primitive::AbsPosition& newPosition) override;
    virtual void SetX(const double newX) override;
    virtual void SetY(const double newY) override;
    virtual void SetZ(const double newZ) override;

    virtual void SetRoadCoordinate(const RoadPosition& newCoordinate) override;

    virtual void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation) override;
    virtual void SetYaw(const double newYaw) override;
    virtual void SetPitch(const double newPitch) override;
    virtual void SetRoll(const double newRoll) override;

    virtual void SetAbsVelocity(const Primitive::AbsVelocity& newVelocity) override;
    virtual void SetAbsVelocity(const double newVelocity) override;

    virtual void SetAbsAcceleration(const Primitive::AbsAcceleration& newAcceleration) override;
    virtual void SetAbsAcceleration(const double newAcceleration) override;

    virtual void SetAbsOrientationRate(const Primitive::AbsOrientationRate& newOrientationRate) override;

    void AddLaneAssignment(const Interfaces::Lane& lane) override;
    const Interfaces::Lanes& GetLaneAssignments() const override;
    void ClearLaneAssignments() override;

    virtual void SetIndicatorState(IndicatorState indicatorState) override;
    virtual IndicatorState GetIndicatorState() const override;
    virtual void SetBrakeLightState(bool brakeLightState) override;
    virtual bool GetBrakeLightState() const override;
    virtual void SetHeadLight(bool headLight) override;
    virtual bool GetHeadLight() const override;
    virtual void SetHighBeamLight(bool highbeamLight) override;
    virtual bool GetHighBeamLight() const override;

    void CopyToGroundTruth(osi3::GroundTruth& target) const override;
private:
    osi3::MovingObject* osiObject;
    RoadPosition roadCoordinate{0.0, 0.0, 0.0}; //currently as "lane" coord -> t is not constant over road
    Interfaces::Lanes assignedLanes;

    mutable Lazy<double> frontDistance;
    mutable Lazy<double> rearDistance;

    const Implementation::InvalidLane invalidLane;
    const Implementation::InvalidSection invalidSection;
    const Implementation::InvalidRoad invalidRoad;
};

class Vehicle : public MovingObject
{
public:
    Angle GetSteeringWheelAngle();
    void SetSteeringWheelAngle(const Angle newValue);
};

class TrafficSign : public Interfaces::TrafficSign
{
public:

    TrafficSign(osi3::TrafficSign* osiObject);

    virtual ~TrafficSign() override = default;

    virtual std::string GetId() const override
    {
        return id;
    }

    virtual double GetS() const override
    {
        return s;
    }

    virtual double GetValue() const override
    {
        return osiSign->main_sign().classification().value().value();
    }

    virtual CommonTrafficSign::Type GetType() const override;


    virtual void SetS(double sPos) override
    {
        s = sPos;
    }
    virtual void SetValue(double value) override
    {
        osiSign->mutable_main_sign()->mutable_classification()->mutable_value()->set_value(value);
    }

    virtual void SetValidForLane(OWL::Id laneId) override
    {
        osiSign->mutable_main_sign()->mutable_classification()->add_assigned_lane_id()->set_value(laneId);
    }

    virtual void SetType(RoadSignalType type) override;

    virtual bool IsValidForLane(OWL::Id laneId) const override;

    virtual void CopyToGroundTruth(osi3::GroundTruth& target) const override;

private:
    std::string id {""};
    double s {0.0};

    osi3::TrafficSign* osiSign {nullptr};
};

} // namespace Implementation


/*
using LaneGeometryElement  = Primitive::LaneGeometryElement;
using Lane                 = Implementation::Lane;
using Section              = Implementation::Section;
using Road                 = Implementation::Road;
using StationaryObject     = Implementation::StationaryObject;
using MovingObject         = Implementation::MovingObject;
using Vehicle              = Implementation::Vehicle;

using CLaneGeometryElement  = const Primitive::LaneGeometryElement;
using CLane                 = const Implementation::Lane;
using CSection              = const Implementation::Section;
using CRoad                 = const Implementation::Road;
using CStationaryObject     = const Implementation::StationaryObject;
using CMovingObject         = const Implementation::MovingObject;
using CVehicle              = const Implementation::Vehicle;
*/

//using Sections             = std::list<Implementation::Section<>>;

//using StationaryObject     = Interfaces::StationaryObject;
//using MovingObject         = Interfaces::MovingObject;
//using Vehicle              = Interfaces::Vehicle;

//using LaneGeometryElements = std::list<Primitive::LaneGeometryElement>;
//using Lanes                = std::list<Interfaces::Lane>;
//using Roads                = std::list<Interfaces::Road>;
//using StationaryObjects    = std::list<Interfaces::StationaryObject>;
//using MovingObjects        = std::list<Interfaces::MovingObject>;
//using Vehicles             = std::list<Interfaces::Vehicle>;

} // namespace OWL
