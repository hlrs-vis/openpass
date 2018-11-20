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
//! @file  DataTypes.h
//! @brief This file provides the basic datatypes of the osi world layer (OWL)
//-----------------------------------------------------------------------------

#pragma once

#include <list>
#include <memory>
#include <vector>

#include "roadInterface.h"
#include "roadLaneInterface.h"
#include "roadLaneSectionInterface.h"

#include "OWL/LaneGeometryElement.h"
#include "OWL/LaneGeometryJoint.h"
#include "OWL/Primitives.h"

#include "osi/osi_worldinterface.pb.h"

namespace OWL {

using Id = uint64_t;
constexpr Id InvalidId = std::numeric_limits<::google::protobuf::uint64>::max();
using Angle = float;

enum class PointType
{
    Reference,
    Edge,
    Boundary
};

struct SearchablePoint
{
    Primitive::AbsCoordinate coordinate;
    PointType pointType;
};

struct LocatedPoint
{
    Primitive::RoadCoordinate coordinate;
    PointType pointType;
};

namespace Interfaces
{
class Lane;
class Section;
class Road;
class MovingObject;

using LaneGeometryElements = std::vector<Primitive::LaneGeometryElement*>;
using LaneGeometryJoints   = std::vector<Primitive::LaneGeometryJoint>;
using Lanes                = std::list<const Lane*>;
using Sections             = std::list<const Section*>;
using Roads                = std::list<const Road*>;
using MovingObjects        = std::list<MovingObject*>;

class Lane
{
public:
    virtual ~Lane() = default;

    virtual Id GetId() const = 0;
    virtual bool Exists() const = 0;

    virtual const Section& GetSection() const = 0;

    virtual const LaneGeometryElements& GetLaneGeometryElements() const = 0;
    virtual double GetLength() const = 0;
    virtual int GetRightLaneCount() const = 0;

    virtual double GetCurvature(double distance) const = 0;
    virtual double GetWidth(double distance) const = 0;

    virtual const Lanes& GetNextLanes() const = 0;
    virtual const Lanes& GetPreviousLanes() const = 0;
    virtual const Lane& GetLeftLane() const = 0;
    virtual const Lane& GetRightLane() const = 0;

    virtual void AddNextLane(const Interfaces::Lane& lane) = 0;
    virtual void AddPreviousLane(const Interfaces::Lane& lane) = 0;
    virtual void AddLaneGeometryJoint(const Common::Vector2d& pointLeft,
                                      const Common::Vector2d& pointCenter,
                                      const Common::Vector2d& pointRight,
                                      double sOffset,
                                      double curvature,
                                      double heading) = 0;

    virtual void SetLeftLane(const Interfaces::Lane& lane) = 0;
    virtual void SetRightLane(const Interfaces::Lane& lane) = 0;

    virtual Interfaces::MovingObjects* GetMovingObjects() const = 0;
    virtual void AddMovingObject(OWL::Interfaces::MovingObject& movingObject) = 0;
};

class Section
{
public:
    virtual ~Section() = default;

    virtual Id GetId() const = 0;
    virtual bool Exists() const = 0;

    virtual void AddLane(const Interfaces::Lane& lane) = 0;
    virtual void SetRoad(Interfaces::Road* road) = 0;

    virtual const Lanes& GetLanes() const = 0;
    virtual const Interfaces::Road& GetRoad() const = 0;

    virtual double GetLength() const = 0;
};

class Road
{
public:
    virtual ~Road() = default;

    virtual Id GetId() const = 0;
    virtual bool Exists() const = 0;

    virtual void AddSection(Interfaces::Section& section) = 0;

    virtual const Sections& GetSections() const = 0;
    virtual double GetLength() const = 0;
};

class MovingObject
{
public:
    virtual ~MovingObject() = default;
    virtual Id GetId() const = 0;

    virtual const Interfaces::Lane& GetLane() const = 0;
    virtual const Interfaces::Section& GetSection() const = 0;
    virtual const Interfaces::Road& GetRoad() const = 0;

    virtual Primitive::Dimension GetDimension() const = 0;
    virtual Primitive::AbsPosition GetAbsPosition() const = 0;
    virtual Primitive::LanePosition GetLanePosition() const = 0;
    virtual Primitive::RoadCoordinate GetRoadCoordinate() const = 0;

    virtual Primitive::AbsOrientation GetAbsOrientation() const = 0;
    virtual Primitive::LaneOrientation GetLaneOrientation() const = 0;

    virtual Primitive::AbsVelocity GetAbsVelocity() const = 0;
    virtual double GetAbsVelocityDouble() const = 0;
    virtual Primitive::RoadVelocity GetRoadVelocity() const = 0;
    virtual Primitive::LaneVelocity GetLaneVelocity() const = 0;

    virtual Primitive::AbsAcceleration GetAbsAcceleration() const = 0;
    virtual double GetAbsAccelerationDouble() const = 0;
    virtual Primitive::RoadAcceleration GetRoadAcceleration() const = 0;
    virtual Primitive::LaneAcceleration GetLaneAcceleration() const = 0;

    virtual Primitive::AbsOrientationRate GetAbsOrientationRate() const = 0;
    virtual Primitive::RoadOrientationRate GetRoadOrientationRate() const = 0;
    virtual Primitive::LaneOrientationRate GetLaneOrientationRate() const = 0;

    virtual void SetDimension(const Primitive::Dimension& newDimension) = 0;
    virtual void SetLength(const double newLength) = 0;
    virtual void SetWidth(const double newWidth) = 0;
    virtual void SetHeight(const double newHeight) = 0;

    virtual void SetAbsPosition(const Primitive::AbsPosition& newPosition) = 0;
    virtual void SetX(const double newX) = 0;
    virtual void SetY(const double newY) = 0;
    virtual void SetZ(const double newZ) = 0;

    virtual void SetLanePosition(const Primitive::LanePosition& newPosition) = 0;

    virtual void SetRoadCoordinate(const Primitive::RoadCoordinate& newCoordinate) = 0;

    virtual void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation) = 0;
    virtual void SetYaw(const double newYaw) = 0;
    virtual void SetPitch(const double newPitch) = 0;
    virtual void SetRoll(const double newRoll) = 0;

    virtual void SetLaneOrientation(const Primitive::LaneOrientation& newOrientation) = 0;

    virtual void SetAbsVelocity(const Primitive::AbsVelocity& newVelocity) = 0;
    virtual void SetAbsVelocity(const double newVelocity) = 0;
    virtual void SetRoadVelocity(const Primitive::RoadVelocity& newVelocity) = 0;
    virtual void SetLaneVelocity(const Primitive::LaneVelocity& newVelocity) = 0;

    virtual void SetAbsAcceleration(const Primitive::AbsAcceleration& newAcceleration) = 0;
    virtual void SetAbsAcceleration(const double newAcceleration) = 0;
    virtual void SetRoadAcceleration(const Primitive::RoadAcceleration& newAcceleration) = 0;
    virtual void SetLaneAcceleration(const Primitive::LaneAcceleration& newAcceleration) = 0;

    virtual void SetAbsOrientationRate(const Primitive::AbsOrientationRate& newOrientationRate) = 0;
    virtual void SetRoadOrientationRate(const Primitive::RoadOrientationRate& newOrientationRate) = 0;
    virtual void SetLaneOrientationRate(const Primitive::LaneOrientationRate& newOrientationRate) = 0;

    virtual void AddLaneAssignment(const Interfaces::Lane& lane) = 0;
    virtual const Interfaces::Lanes& GetLaneAssignments() const = 0;
    virtual void ClearLaneAssignments() = 0;
};

} // namespace Interfaces



namespace Implementation {

class Lane : public Interfaces::Lane
{
public:
    Lane(osi3::world::RoadLane* osiLane, const Interfaces::Section* section);
    ~Lane() override;

    Id GetId() const override;
    bool Exists() const override;

    const Interfaces::Section& GetSection() const override;

    double GetLength() const override;
    int GetRightLaneCount() const override;

    double GetCurvature(double distance) const override;
    double GetWidth(double distance) const override;

    const Interfaces::Lanes& GetNextLanes() const override;
    const Interfaces::Lanes& GetPreviousLanes() const override;
    const Interfaces::Lane& GetLeftLane() const override;
    const Interfaces::Lane& GetRightLane() const override;

    void AddLanePairing(const Interfaces::Lane& prevLane, const Interfaces::Lane& nextLane);
    void AddNextLane(const Interfaces::Lane& lane) override;
    void AddPreviousLane(const Interfaces::Lane& lane) override;
    const Interfaces::LaneGeometryElements& GetLaneGeometryElements() const override;
    void AddLaneGeometryJoint(const Common::Vector2d& pointLeft,
                              const Common::Vector2d& pointCenter,
                              const Common::Vector2d& pointRight,
                              double sOffset,
                              double curvature,
                              double heading) override;

    void SetLeftLane(const Interfaces::Lane& lane) override;
    void SetRightLane(const Interfaces::Lane& lane) override;

    Interfaces::MovingObjects* GetMovingObjects() const override;
    void AddMovingObject(OWL::Interfaces::MovingObject& movingObject) override;

protected:
    osi3::world::RoadLane* osiLane{nullptr};

private:
    const Interfaces::Section* section;
    Interfaces::LaneGeometryJoints laneGeometryJoints;
    Interfaces::LaneGeometryElements laneGeometryElements;
    Interfaces::Lanes nextLanes;
    Interfaces::Lanes previousLanes;
    const Interfaces::Lane* leftLane;
    const Interfaces::Lane* rightLane;
    bool leftLaneIsDummy{section == nullptr};
    bool rightLaneIsDummy{section == nullptr};
};

class InvalidLane : public Lane
{
public:
    InvalidLane() : Lane(new osi3::world::RoadLane(), nullptr)
    {
        // set 'invalid' id
        osiLane->mutable_id()->set_value(InvalidId);
    }

    virtual ~InvalidLane();
    InvalidLane(const InvalidLane&) = delete;
    InvalidLane& operator=(const InvalidLane&) = delete;
    InvalidLane(InvalidLane&&) = delete;
    InvalidLane& operator=(InvalidLane&&) = delete;
};

class Section : public Interfaces::Section
{
public:
    Section(osi3::world::RoadSection* osiSection);

    Id GetId() const override;
    bool Exists() const override;

    void AddLane(const Interfaces::Lane& lane) override;
    const Interfaces::Lanes& GetLanes() const override;

    void SetRoad(Interfaces::Road* road) override;
    const Interfaces::Road& GetRoad() const override;

    double GetLength() const override;

protected:
    osi3::world::RoadSection* osiSection;

private:
    Interfaces::Lanes lanes;
    const Interfaces::Road* road;
    Interfaces::Sections nextSections;
    Interfaces::Sections previousSections;

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
    InvalidSection() : Section(new osi3::world::RoadSection())
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

    Id GetId() const override;
    bool Exists() const override;

    const Interfaces::Sections& GetSections() const override;
    void AddSection(Interfaces::Section& section) override;

    double GetLength() const override;

protected:
    osi3::world::Road* osiRoad;

private:
    Interfaces::Sections sections;

public:
    Road(const Road&) = delete;
    Road& operator=(const Road&) = delete;
    Road(Road&&) = delete;
    Road& operator=(Road&&) = delete;

    virtual ~Road() override = default;
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

class StationaryObject
{
public:
    StationaryObject(osi3::StationaryObject* base);
    ~StationaryObject();

    Id GetId() const;

    const Interfaces::Lane& GetLane() const;
    const Interfaces::Section& GetSection() const;
    const Interfaces::Road& GetRoad() const;

    void SetAbsPosition(const Primitive::AbsPosition& newPosition);
/*
    const Primitive::AbsPosition& GetAbsPosition() const;
    const Primitive::RoadPosition& GetRoadPosition() const;
    const Primitive::LanePosition& GetLanePosition() const;

    const Primitive::AbsOrientation& GetAbsOrientation() const;
    const Primitive::RoadOrientation& GetRoadOrientation() const;
    const Primitive::LaneOrientation& GetLaneOrientation() const;

    Primitive::AbsPosition GetAbsPosition() const;
    Primitive::LanePosition GetLanePosition() const;

    Primitive::AbsOrientation GetAbsOrientation() const;
    Primitive::LaneOrientation GetLaneOrientation() const;

    void SetLanePosition(const Primitive::LanePosition& newPosition);

    void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation);
    void SetLaneOrientation(const Primitive::LaneOrientation& newOrientation);
*/
private:
    osi3::StationaryObject* osiObject;
};

class MovingObject : public Interfaces::MovingObject
{
public:
    MovingObject(osi3::MovingObject* osiMovingObject);
    ~MovingObject() = default;

    Id GetId() const;

    const Interfaces::Lane& GetLane() const;
    const Interfaces::Section& GetSection() const;
    const Interfaces::Road& GetRoad() const;

    Primitive::Dimension GetDimension() const;
    Primitive::AbsPosition GetAbsPosition() const;
    Primitive::LanePosition GetLanePosition() const;
    Primitive::RoadCoordinate GetRoadCoordinate() const;

    Primitive::AbsOrientation GetAbsOrientation() const;
    Primitive::LaneOrientation GetLaneOrientation() const;

    Primitive::AbsVelocity GetAbsVelocity() const;
    double GetAbsVelocityDouble() const;
    Primitive::RoadVelocity GetRoadVelocity() const;
    Primitive::LaneVelocity GetLaneVelocity() const;

    Primitive::AbsAcceleration GetAbsAcceleration() const;
    double GetAbsAccelerationDouble() const;
    Primitive::RoadAcceleration GetRoadAcceleration() const;
    Primitive::LaneAcceleration GetLaneAcceleration() const;

    Primitive::AbsOrientationRate GetAbsOrientationRate() const;
    Primitive::RoadOrientationRate GetRoadOrientationRate() const;
    Primitive::LaneOrientationRate GetLaneOrientationRate() const;

    void SetDimension(const Primitive::Dimension& newDimension);
    void SetLength(const double newLength);
    void SetWidth(const double newWidth);
    void SetHeight(const double newHeight);

    void SetAbsPosition(const Primitive::AbsPosition& newPosition);
    void SetX(const double newX);
    void SetY(const double newY);
    void SetZ(const double newZ);

    void SetLanePosition(const Primitive::LanePosition& newPosition);

    void SetRoadCoordinate(const Primitive::RoadCoordinate& newCoordinate);

    void SetAbsOrientation(const Primitive::AbsOrientation& newOrientation);
    void SetYaw(const double newYaw);
    void SetPitch(const double newPitch);
    void SetRoll(const double newRoll);

    void SetLaneOrientation(const Primitive::LaneOrientation& newOrientation);

    void SetAbsVelocity(const Primitive::AbsVelocity& newVelocity);
    void SetAbsVelocity(const double newVelocity);
    void SetRoadVelocity(const Primitive::RoadVelocity& newVelocity);
    void SetLaneVelocity(const Primitive::LaneVelocity& newVelocity);

    void SetAbsAcceleration(const Primitive::AbsAcceleration& newAcceleration);
    void SetAbsAcceleration(const double newAcceleration);
    void SetRoadAcceleration(const Primitive::RoadAcceleration& newAcceleration);
    void SetLaneAcceleration(const Primitive::LaneAcceleration& newAcceleration);

    void SetAbsOrientationRate(const Primitive::AbsOrientationRate& newOrientationRate);
    void SetRoadOrientationRate(const Primitive::RoadOrientationRate& newOrientationRate);
    void SetLaneOrientationRate(const Primitive::LaneOrientationRate& newOrientationRate);

    void AddLaneAssignment(const Interfaces::Lane& lane);
    const Interfaces::Lanes& GetLaneAssignments() const;
    void ClearLaneAssignments();

private:
    osi3::MovingObject* osiObject;
    OWL::Primitive::RoadCoordinate roadCoordinate{0.0, 0.0, 0.0};
    Interfaces::Lanes assignedLanes;

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

} // namespace Implementation

} // namespace OWL
