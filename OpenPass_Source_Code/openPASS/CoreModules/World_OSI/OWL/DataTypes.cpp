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

#include <exception>
#include <list>
#include <memory>

#include "roadInterface.h"
#include "roadLaneInterface.h"
#include "roadLaneSectionInterface.h"

#include "OWL/DataTypes.h"
#include "OWL/LaneGeometryElement.h"
#include "OWL/LaneGeometryJoint.h"
#include "OWL/Primitives.h"

#include "osi/osi_worldinterface.pb.h"

namespace OWL {

namespace Implementation {

Lane::Lane(osi3::world::RoadLane* osiLane, const Interfaces::Section* section) :
    osiLane(osiLane),
    section(section),
    leftLane(section ? new InvalidLane() : nullptr),
    rightLane(section ? new InvalidLane() : nullptr)
{
}

Lane::~Lane()
{
    for (auto laneGeometryElement : laneGeometryElements)
    {
        delete laneGeometryElement;
    }
}

Id Lane::GetId() const
{
    return osiLane->id().value();
}

bool Lane::Exists() const
{
    return osiLane->id().value() != InvalidId;
}

const Interfaces::Section& Lane::GetSection() const
{
    return *section;
}

int Lane::GetRightLaneCount() const
{
    int count = 0;
    const Interfaces::Lane* currentLane = &(GetRightLane());

    while (currentLane->Exists())
    {
        count++;
        currentLane = &(currentLane->GetRightLane());
    }

    return count;
}

double Lane::GetLength() const
{
    return osiLane->length();
}

double Lane::GetCurvature(double distance) const
{
    const auto& nextJoint = std::find_if(laneGeometryJoints.cbegin(),
                                         laneGeometryJoints.cend(),
                                         [distance](const Primitive::LaneGeometryJoint& joint) {
                                             return joint.projectionAxes.sOffset > distance;
                                         });

    if (nextJoint == laneGeometryJoints.cend() ||
        nextJoint == laneGeometryJoints.cbegin())
    {
        return 0.0;
    }

    return nextJoint->curvature;
}

double Lane::GetWidth(double distance) const
{
    const auto& nextJoint = std::find_if(laneGeometryJoints.cbegin(),
                                         laneGeometryJoints.cend(),
                                         [distance](const Primitive::LaneGeometryJoint& joint) {
                                             return joint.projectionAxes.sOffset > distance;
                                         });

    if (nextJoint == laneGeometryJoints.cend() ||
        nextJoint == laneGeometryJoints.cbegin())
    {
        return 0.0;
    }

    // TODO: interpolate between two sample points
    return ((*nextJoint).points.left - (*nextJoint).points.right).Length();
}

const Interfaces::Lanes& Lane::GetNextLanes() const
{
    return nextLanes;
}

const Interfaces::Lanes& Lane::GetPreviousLanes() const
{
    return previousLanes;
}

const Interfaces::Lane& Lane::GetLeftLane() const
{
    return *leftLane;
}

const Interfaces::Lane& Lane::GetRightLane() const
{
    return *rightLane;
}

void Lane::SetLeftLane(const Interfaces::Lane& lane)
{
    if (leftLaneIsDummy)
    {
        leftLaneIsDummy = false;
        delete leftLane;
    }

    leftLane = &lane;
    osiLane->mutable_left_adjacent_lane_id()->set_value(lane.GetId());
}

void Lane::SetRightLane(const Interfaces::Lane& lane)
{
    if (rightLaneIsDummy)
    {
        rightLaneIsDummy = false;
        delete rightLane;
    }

    rightLane = &lane;
    osiLane->mutable_right_adjacent_lane_id()->set_value(lane.GetId());
}

Interfaces::MovingObjects* Lane::GetMovingObjects() const
{
    throw std::logic_error("not implemented");
}

void Lane::AddMovingObject(Interfaces::MovingObject& movingObject)
{
    throw std::logic_error("not implemented");
}

void Lane::AddLanePairing(const Interfaces::Lane& prevLane, const Interfaces::Lane& nextLane)
{
    osi3::world::RoadLanePairing* pairing = osiLane->add_lane_pairing();
    pairing->mutable_antecessor_lane_id()->set_value(prevLane.GetId());
    pairing->mutable_successor_lane_id()->set_value(nextLane.GetId());
}

void Lane::AddNextLane(const Interfaces::Lane& lane)
{
    nextLanes.push_back(&lane);
}

void Lane::AddPreviousLane(const Interfaces::Lane& lane)
{
    previousLanes.push_back(&lane);
}

const Interfaces::LaneGeometryElements& Lane::GetLaneGeometryElements() const
{
    return laneGeometryElements; //todo: fill
}

void Lane::AddLaneGeometryJoint(const Common::Vector2d& pointLeft,
                                const Common::Vector2d& pointCenter,
                                const Common::Vector2d& pointRight,
                                double sOffset,
                                double curvature,
                                double heading)
{
    Primitive::LaneGeometryJoint newJoint;

    newJoint.points.left      = pointLeft;
    newJoint.points.reference = pointCenter;
    newJoint.points.right     = pointRight;
    newJoint.curvature        = curvature;

    // TODO!
    Common::Vector2d proj = pointLeft;
    proj.Sub(pointRight);
    newJoint.projectionAxes.t = proj;
    proj.Rotate(-M_PI_2);
    newJoint.projectionAxes.s = proj;
    newJoint.projectionAxes.curvatureCorrection = { 1, 3 };
    newJoint.projectionAxes.sHdg = heading;
    newJoint.projectionAxes.sOffset = sOffset;

    if (laneGeometryJoints.empty())
    {
        laneGeometryJoints.push_back(newJoint);
        return;
    }

    const Primitive::LaneGeometryJoint& previousJoint = laneGeometryJoints.back();

    Primitive::LaneGeometryElement* newElement = new Primitive::LaneGeometryElement(previousJoint, newJoint);
    laneGeometryElements.push_back(newElement);
    laneGeometryJoints.push_back(newJoint);
}

Section::Section(osi3::world::RoadSection* osiSection) : osiSection(osiSection)
{
}

Id Section::GetId() const
{
    return osiSection->id().value();
}

bool Section::Exists() const
{
    return osiSection->id().value() != InvalidId;
}

void Section::AddLane(const Interfaces::Lane& lane)
{
    lanes.push_back(&lane);
}

void Section::SetRoad(Interfaces::Road* road)
{
    this->road = road;
}

const Interfaces::Road& Section::GetRoad() const
{
    return *road;
}

const Interfaces::Lanes& Section::GetLanes() const
{
    return lanes;
}

double Section::GetLength() const
{
    double sum = 0.0;

    if (lanes.empty())
    {
        return 0.0;
    }

    //return osiSection->length();
    for (const auto& lane : lanes)
    {
        sum += lane->GetLength();
    }

    return sum / lanes.size();
}


Road::Road(osi3::world::Road* osiRoad) : osiRoad(osiRoad)
{
}

Id Road::GetId() const
{
    return osiRoad->id().value();
}

bool Road::Exists() const
{
    return osiRoad->id().value() != InvalidId;
}

const Interfaces::Sections& Road::GetSections() const
{
    return sections;
}

void Road::AddSection(Interfaces::Section& section)
{
    section.SetRoad(this);
    sections.push_back(&section);
}

double Road::GetLength() const
{
    double length = 0.0;

    for (const auto& section : sections)
    {
        length += section->GetLength();
    }

    return length;
}


Id StationaryObject::GetId() const
{
    return osiObject->id().value();
}

StationaryObject::StationaryObject(osi3::StationaryObject* base) : osiObject(base)
{
}

void StationaryObject::SetAbsPosition(const Primitive::AbsPosition& newPosition)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();

    osiPosition->set_x(newPosition.x);
    osiPosition->set_y(newPosition.y);
    osiPosition->set_z(newPosition.z);
}

MovingObject::MovingObject(osi3::MovingObject* osiMovingObject) : osiObject(osiMovingObject)
{
}

Id MovingObject::GetId() const
{
    return osiObject->id().value();
}

Primitive::Dimension MovingObject::GetDimension() const
{
    const osi3::Dimension3d& osiDimension = osiObject->base().dimension();
    Primitive::Dimension dimension;

    dimension.length = osiDimension.length();
    dimension.width  = osiDimension.width();
    dimension.height = osiDimension.height();

    return dimension;
}

void MovingObject::SetDimension(const Primitive::Dimension& newDimension)
{
    osi3::Dimension3d* osiDimension = osiObject->mutable_base()->mutable_dimension();

    osiDimension->set_length(newDimension.length);
    osiDimension->set_width(newDimension.width);
    osiDimension->set_height(newDimension.height);
}

void MovingObject::SetLength(const double newLength)
{
    osi3::Dimension3d* osiDimension = osiObject->mutable_base()->mutable_dimension();
    osiDimension->set_length(newLength);
}

void MovingObject::SetWidth(const double newWidth)
{
    osi3::Dimension3d* osiDimension = osiObject->mutable_base()->mutable_dimension();
    osiDimension->set_width(newWidth);
}

void MovingObject::SetHeight(const double newHeight)
{
    osi3::Dimension3d* osiDimension = osiObject->mutable_base()->mutable_dimension();
    osiDimension->set_height(newHeight);
}

Primitive::AbsPosition MovingObject::GetAbsPosition() const
{
    const osi3::Vector3d osiPosition = osiObject->base().position();
    Primitive::AbsPosition position;

    position.x = osiPosition.x();
    position.y = osiPosition.y();
    position.z = osiPosition.z();

    return position;
}

void MovingObject::SetAbsPosition(const Primitive::AbsPosition& newPosition)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();

    osiPosition->set_x(newPosition.x);
    osiPosition->set_y(newPosition.y);
    osiPosition->set_z(newPosition.z);
}

void MovingObject::SetX(const double newX)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();
    osiPosition->set_x(newX);
}

void MovingObject::SetY(const double newY)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();
    osiPosition->set_y(newY);
}

void MovingObject::SetZ(const double newZ)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();
    osiPosition->set_z(newZ);
}

void MovingObject::SetLanePosition(const Primitive::LanePosition& newPosition)
{
    throw std::logic_error("not implemented");
}

Primitive::LanePosition MovingObject::GetLanePosition() const
{
   throw std::logic_error("not implemented");
}

Primitive::RoadCoordinate MovingObject::GetRoadCoordinate() const
{
    return roadCoordinate;
}

void MovingObject::SetRoadCoordinate(const Primitive::RoadCoordinate& newCoordinate)
{
    roadCoordinate = newCoordinate;
}

Primitive::AbsOrientation MovingObject::GetAbsOrientation() const
{
    osi3::Orientation3d osiOrientation = osiObject->base().orientation();
    Primitive::AbsOrientation orientation;

    orientation.yaw = osiOrientation.yaw();
    orientation.pitch = osiOrientation.pitch();
    orientation.roll = osiOrientation.roll();

    return orientation;
}

void MovingObject::SetAbsOrientation(const Primitive::AbsOrientation& newOrientation)
{
    osi3::Orientation3d* osiOrientation = osiObject->mutable_base()->mutable_orientation();

    osiOrientation->set_yaw(newOrientation.yaw);
    osiOrientation->set_pitch(newOrientation.pitch);
    osiOrientation->set_roll(newOrientation.roll);
}

void MovingObject::SetYaw(const double newYaw)
{
    osi3::Orientation3d* osiOrientation = osiObject->mutable_base()->mutable_orientation();
    osiOrientation->set_yaw(newYaw);
}

void MovingObject::SetPitch(const double newPitch)
{
    osi3::Orientation3d* osiOrientation = osiObject->mutable_base()->mutable_orientation();
    osiOrientation->set_pitch(newPitch);
}

void MovingObject::SetRoll(const double newRoll)
{
    osi3::Orientation3d* osiOrientation = osiObject->mutable_base()->mutable_orientation();
    osiOrientation->set_roll(newRoll);
}

void MovingObject::SetLaneOrientation(const Primitive::LaneOrientation& newOrientation)
{
    throw std::logic_error("not implemented");
}


Primitive::LaneOrientation MovingObject::GetLaneOrientation() const
{
   throw std::logic_error("not implemented");
}

Primitive::AbsVelocity MovingObject::GetAbsVelocity() const
{
    const osi3::Vector3d osiVelocity = osiObject->base().velocity();
    Primitive::AbsVelocity velocity;

    velocity.vx = osiVelocity.x();
    velocity.vy = osiVelocity.y();
    velocity.vz = osiVelocity.z();

    return velocity;
}

double MovingObject::GetAbsVelocityDouble() const
{
    Primitive::AbsVelocity velocity = GetAbsVelocity();

    return std::hypot(velocity.vx, velocity.vy);
}

void MovingObject::SetAbsVelocity(const Primitive::AbsVelocity& newVelocity)
{
    osi3::Vector3d* osiVelocity = osiObject->mutable_base()->mutable_velocity();

    osiVelocity->set_x(newVelocity.vx);
    osiVelocity->set_y(newVelocity.vy);
    osiVelocity->set_z(newVelocity.vz);
}

void MovingObject::SetAbsVelocity(const double newVelocity)
{
    osi3::Vector3d* osiVelocity = osiObject->mutable_base()->mutable_velocity();

    double yaw = GetAbsOrientation().yaw;
    double cos_val = std::cos(yaw);
    double sin_val = std::sin(yaw);

    osiVelocity->set_x(newVelocity * cos_val);
    osiVelocity->set_y(newVelocity * sin_val);
    osiVelocity->set_z(0.0);
}

void MovingObject::SetRoadVelocity(const Primitive::RoadVelocity& newVelocity)
 {
    throw std::logic_error("not implemented");
}

Primitive::RoadVelocity MovingObject::GetRoadVelocity() const
{
    throw std::logic_error("not implemented");
}

void MovingObject::SetLaneVelocity(const Primitive::LaneVelocity &newVelocity)
{
    throw std::logic_error("not implemented");
}

Primitive::LaneVelocity MovingObject::GetLaneVelocity() const
{
    throw std::logic_error("not implemented");
}

Primitive::AbsAcceleration MovingObject::GetAbsAcceleration() const
{
    const osi3::Vector3d osiAcceleration = osiObject->base().acceleration();
    Primitive::AbsAcceleration acceleration;

    acceleration.ax = osiAcceleration.x();
    acceleration.ay = osiAcceleration.y();
    acceleration.az = osiAcceleration.z();

    return acceleration;
}

double MovingObject::GetAbsAccelerationDouble() const
{
    Primitive::AbsAcceleration acceleration = GetAbsAcceleration();

    return std::hypot(acceleration.ax, acceleration.ay);
}

void MovingObject::SetAbsAcceleration(const Primitive::AbsAcceleration& newAcceleration)
{
    osi3::Vector3d* osiAcceleration = osiObject->mutable_base()->mutable_acceleration();

    osiAcceleration->set_x(newAcceleration.ax);
    osiAcceleration->set_y(newAcceleration.ay);
    osiAcceleration->set_z(newAcceleration.az);
}

void MovingObject::SetAbsAcceleration(const double newAcceleration)
{
    osi3::Vector3d* osiAcceleration = osiObject->mutable_base()->mutable_acceleration();

    double yaw = GetAbsOrientation().yaw;
    double cos_val = std::cos(yaw);
    double sin_val = std::sin(yaw);

    osiAcceleration->set_x(newAcceleration * cos_val);
    osiAcceleration->set_y(newAcceleration * sin_val);
    osiAcceleration->set_z(0.0);
}

void MovingObject::SetRoadAcceleration(const Primitive::RoadAcceleration& newAcceleration)
 {
    throw std::logic_error("not implemented");
}

Primitive::RoadAcceleration MovingObject::GetRoadAcceleration() const
{
    throw std::logic_error("not implemented");
}

void MovingObject::SetLaneAcceleration(const Primitive::LaneAcceleration& newAcceleration)
 {
    throw std::logic_error("not implemented");
}

Primitive::LaneAcceleration MovingObject::GetLaneAcceleration() const
{
    throw std::logic_error("not implemented");
}

void MovingObject::SetAbsOrientationRate(const Primitive::AbsOrientationRate& newOrientationRate)
{
    osi3::Orientation3d* osiOrientationRate = osiObject->mutable_base()->mutable_orientation();

    osiOrientationRate->set_yaw(newOrientationRate.yawRate);
    osiOrientationRate->set_pitch(newOrientationRate.pitchRate);
    osiOrientationRate->set_roll(newOrientationRate.rollRate);
}

Primitive::AbsOrientationRate MovingObject::GetAbsOrientationRate() const
{
    const osi3::Orientation3d osiOrientationRate = osiObject->base().orientation_rate();

    return Primitive::AbsOrientationRate
    {
        osiOrientationRate.yaw(),
        osiOrientationRate.pitch(),
        osiOrientationRate.roll()
    };
}

void MovingObject::SetRoadOrientationRate(const Primitive::RoadOrientationRate& newOrientationRate)
{
    throw std::logic_error("not implemented");
}

Primitive::RoadOrientationRate MovingObject::GetRoadOrientationRate() const
{
    throw std::logic_error("not implemented");
}

void MovingObject::SetLaneOrientationRate(const Primitive::LaneOrientationRate& newOrientationRate)
 {
    throw std::logic_error("not implemented");
}

Primitive::LaneOrientationRate MovingObject::GetLaneOrientationRate() const
{
    throw std::logic_error("not implemented");
}

void MovingObject::AddLaneAssignment(const Interfaces::Lane& lane)
{
    osi3::Identifier* assignedId = osiObject->add_assigned_lane_id();
    assignedId->set_value(lane.GetId());
    assignedLanes.push_back(&lane);
}

const Interfaces::Lanes& MovingObject::GetLaneAssignments() const
{
    return assignedLanes;
}

void MovingObject::ClearLaneAssignments()
{
    osiObject->mutable_assigned_lane_id()->Clear();
    assignedLanes.clear();
}

const Interfaces::Road& MovingObject::GetRoad() const
{
    const Interfaces::Section& section = GetSection();

    if (section.Exists())
    {
        return section.GetRoad();
    }

    return invalidRoad;
}

const Interfaces::Section &MovingObject::GetSection() const
{
    const Interfaces::Lane& lane = GetLane();

    if (lane.Exists())
    {
        return lane.GetSection();
    }

    return invalidSection;
}

const Interfaces::Lane& MovingObject::GetLane() const
{
    if (assignedLanes.empty())
    {
        return invalidLane;
    }

    return *(assignedLanes.front());
}

Angle Vehicle::GetSteeringWheelAngle()
{
    throw std::logic_error("not implemented");
}

void Vehicle::SetSteeringWheelAngle(const Angle newValue)
{
    throw std::logic_error("not implemented");
}

InvalidLane::~InvalidLane()
{
    if (osiLane)
    {
        delete osiLane;
    }
}

InvalidSection::~InvalidSection()
{
    if (osiSection)
    {
        delete osiSection;
    }
}

InvalidRoad::~InvalidRoad()
{
    if (osiRoad)
    {
        delete osiRoad;
    }
}

} // namespace Implementation

} // namespace OWL
