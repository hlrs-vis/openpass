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

#include <exception>
#include <list>
#include <memory>
#include <tuple>
#include <qglobal.h>

#include "Interfaces/roadInterface/roadInterface.h"
#include "Interfaces/roadInterface/roadLaneInterface.h"
#include "Interfaces/roadInterface/roadLaneSectionInterface.h"

#include "OWL/DataTypes.h"
#include "OWL/LaneGeometryElement.h"
#include "OWL/LaneGeometryJoint.h"
#include "OWL/Primitives.h"

#include "osi/osi_groundtruth.pb.h"
#include "osi/osi_worldinterface.pb.h"
#include "WorldObjectAdapter.h"

namespace OWL {
Interfaces::Lane::Lane() : forwardLaneStream{this}, reverseLaneStream{this} {}
Interfaces::Section::Section() : forwardSectionStream{this}, reverseSectionStream{this} {}

namespace Implementation {

Lane::Lane(osi3::world::RoadLane* osiLane, const Interfaces::Section* section, bool isInStreamDirection) :
    osiLane(osiLane),
    section(section),
    isInStreamDirection(isInStreamDirection),
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

void Lane::UpdateStreamId()
{
    for (const auto& laneStream : reverseLaneStream)
    {
        if ((laneStream.IsInStreamDirection() && !laneStream.GetPrevious())
                || (!laneStream.IsInStreamDirection() && !laneStream.GetNext()))
        {
            streamId = laneStream.GetId();
            return;
        }
    }
}

void Lane::CopyToGroundTruth(osi3::GroundTruth& target) const
{
    auto newLane = target.add_lane();
    newLane->CopyFrom(osiLane->base_lane());
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

const Interfaces::Road& Lane::GetRoad() const
{
    return section->GetRoad();
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


std::tuple<const Primitive::LaneGeometryJoint*, const Primitive::LaneGeometryJoint*> Lane::GetNeighbouringJoints(
        double distance) const
{
    const Primitive::LaneGeometryJoint* nextJoint = nullptr;
    const Primitive::LaneGeometryJoint* prevJoint = nullptr;

    const auto& nextJointIt = std::find_if(laneGeometryJoints.cbegin(),
                                           laneGeometryJoints.cend(),
                                           [distance](const Primitive::LaneGeometryJoint & joint)
    {
        return joint.projectionAxes.sOffset > distance;
    });


    if (nextJointIt != laneGeometryJoints.cend())
    {
        nextJoint = &(*nextJointIt);
    }

    if (nextJointIt != laneGeometryJoints.cbegin())
    {
        const auto& prevJointIt = std::prev(nextJointIt);
        prevJoint = &(*prevJointIt);
    }

    return { prevJoint, nextJoint };
}

const Primitive::LaneGeometryJoint::Points  Lane::GetInterpolatedPointsAtDistance(double distance) const
{
    Primitive::LaneGeometryJoint::Points interpolatedPoints {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    const Primitive::LaneGeometryJoint* prevJoint;
    const Primitive::LaneGeometryJoint* nextJoint;
    std::tie(prevJoint, nextJoint) = GetNeighbouringJoints(distance);

    if (!prevJoint && !nextJoint)
    {
        return interpolatedPoints;
    }
    else
        if (prevJoint && !nextJoint)
        {
            return prevJoint->points;
        }
        else
            if (nextJoint && !prevJoint)
            {
                return nextJoint->points;
            }

    double interpolationFactor = (distance - prevJoint->projectionAxes.sOffset) / (nextJoint->projectionAxes.sOffset -
                                                                                   prevJoint->projectionAxes.sOffset);

    interpolatedPoints.left = prevJoint->points.left * (1.0 - interpolationFactor) + nextJoint->points.left *
            interpolationFactor;
    interpolatedPoints.right = prevJoint->points.right * (1.0 - interpolationFactor) + nextJoint->points.right *
            interpolationFactor;
    interpolatedPoints.reference = prevJoint->points.reference * (1.0 - interpolationFactor) + nextJoint->points.reference *
            interpolationFactor;

    return interpolatedPoints;
}

double Lane::GetCurvature(double distance) const
{
    const Primitive::LaneGeometryJoint* prevJoint;
    const Primitive::LaneGeometryJoint* nextJoint;
    std::tie(prevJoint, nextJoint) = GetNeighbouringJoints(distance);

    if (!prevJoint && !nextJoint)
    {
        return 0.0;
    }
    else
        if (prevJoint && !nextJoint)
        {
            return prevJoint->curvature;
        }
        else
            if (nextJoint && !prevJoint)
            {
                return 0.0;
            }

    double interpolationFactor = (distance - prevJoint->projectionAxes.sOffset) / (nextJoint->projectionAxes.sOffset -
                                                                                   prevJoint->projectionAxes.sOffset);
    double interpolatedCurvature = (1 - interpolationFactor) * prevJoint->curvature + interpolationFactor *
            nextJoint->curvature;

    return interpolatedCurvature;
}

double Lane::GetWidth(double distance) const
{
    const Primitive::LaneGeometryJoint* prevJoint;
    const Primitive::LaneGeometryJoint* nextJoint;
    std::tie(prevJoint, nextJoint) = GetNeighbouringJoints(distance);

    if (!prevJoint && !nextJoint)
    {
        return 0.0;
    }
    else
        if (prevJoint && !nextJoint)
        {
            return (prevJoint->points.left - prevJoint->points.right).Length();
        }
        else
            if (nextJoint && !prevJoint)
            {
                return 0.0;
            }

    double interpolationFactor = (distance - prevJoint->projectionAxes.sOffset) / (nextJoint->projectionAxes.sOffset -
                                                                                   prevJoint->projectionAxes.sOffset);
    double nextWidth = (nextJoint->points.left - nextJoint->points.right).Length();
    double prevWidth = (prevJoint->points.left - prevJoint->points.right).Length();
    double interpolatedWidth = (1.0 - interpolationFactor) * prevWidth + interpolationFactor * nextWidth;

    return interpolatedWidth;
}

double Lane::GetDirection(double distance) const
{
    const Primitive::LaneGeometryJoint* prevJoint;
    const Primitive::LaneGeometryJoint* nextJoint;
    std::tie(prevJoint, nextJoint) = GetNeighbouringJoints(distance);

    if (!prevJoint && !nextJoint)
    {
        return 0.0;
    }
    else
        if (prevJoint && !nextJoint)
        {
            return prevJoint->projectionAxes.sHdg;
        }
        else
            if (nextJoint && !prevJoint)
            {
                return 0.0;
            }

    double interpolationFactor = (distance - prevJoint->projectionAxes.sOffset) / (nextJoint->projectionAxes.sOffset -
                                                                                   prevJoint->projectionAxes.sOffset);
    double interpolatedDirection = (1.0 - interpolationFactor) * prevJoint->projectionAxes.sHdg + interpolationFactor *
            nextJoint->projectionAxes.sHdg;

    return interpolatedDirection;
}

const Interfaces::Lane* Lane::GetNext() const
{
    return next;
}

const Interfaces::Lane* Lane::GetPrevious() const
{
    return previous;
}

const Interfaces::Lane& Lane::GetLeftLane() const
{
    return *leftLane;
}

const Interfaces::Lane& Lane::GetRightLane() const
{
    return *rightLane;
}

bool Lane::IsInStreamDirection() const
{
    return isInStreamDirection;
}

Id Lane::GetStreamId() const
{
    return streamId;
}

double Lane::GetDistance(MeasurementPoint measurementPoint) const
{
    if (laneGeometryElements.empty())
    {
        throw std::logic_error("Unexpected Lane without LaneGeometryElements");
    }

    if (measurementPoint == MeasurementPoint::RoadStart)
    {
        return laneGeometryElements.front()->joints.current.projectionAxes.sOffset;
    }

    if (measurementPoint == MeasurementPoint::RoadEnd)
    {
        return laneGeometryElements.back()->joints.next.projectionAxes.sOffset;
    }

    throw std::invalid_argument("measurement point not within valid bounds");
}

LaneType Lane::GetLaneType() const
{
    return laneType;
}

bool Lane::Covers(double distance) const
{
    if (GetDistance(MeasurementPoint::RoadStart) <= distance)
    {
        return next ?
                    GetDistance(MeasurementPoint::RoadEnd) > distance :
                    GetDistance(MeasurementPoint::RoadEnd) >= distance;

    }
    return false;
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

void Lane::SetLaneType(LaneType specifiedType)
{
    laneType =  specifiedType;
}

const Interfaces::WorldObjects& Lane::GetWorldObjects() const
{
    return worldObjects;
}

//const Interfaces::MovingObjects& Lane::GetMovingObjects() const
//{
//    return movingObjects;
//}

//const Interfaces::StationaryObjects& Lane::GetStationaryObjects() const
//{
//    return stationaryObjects;
//}

void Lane::AddMovingObject(Interfaces::MovingObject& movingObject)
{
    worldObjects.push_back(&movingObject);
    movingObjects.push_back(&movingObject);
}

void Lane::AddStationaryObject(Interfaces::StationaryObject& stationaryObject)
{
    worldObjects.push_back(&stationaryObject);
    stationaryObjects.push_back(&stationaryObject);
}

void Lane::AddWorldObject(Interfaces::WorldObject& worldObject)
{
    if (worldObject.Is<MovingObject>())
    {
        AddMovingObject(*worldObject.As<MovingObject>());
    }
    else
        if (worldObject.Is<StationaryObject>())
        {
            AddStationaryObject(*worldObject.As<StationaryObject>());
        }
}

void Lane::ClearMovingObjects()
{
    worldObjects.clear();
    worldObjects.insert(worldObjects.end(), stationaryObjects.begin(), stationaryObjects.end());
    movingObjects.clear();
}

void Lane::AddLanePairing(const Interfaces::Lane& prevLane, const Interfaces::Lane& nextLane)
{
    osi3::world::RoadLanePairing* pairing = osiLane->add_lane_pairing();
    pairing->mutable_antecessor_lane_id()->set_value(prevLane.GetId());
    pairing->mutable_successor_lane_id()->set_value(nextLane.GetId());
}

void Lane::AddNext(const Interfaces::Lane& lane)
{
    if (next)
    {
        throw std::logic_error("Lanes are not allowed to have more than one successor");
    }
    next = &lane;
    UpdateStreamId();
}

void Lane::AddPrevious(const Interfaces::Lane& lane)
{
    if (previous)
    {
        throw std::logic_error("Lanes are not allowed to have more than one predecessor");
    }
    previous = &lane;
    UpdateStreamId();
}

const Interfaces::LaneGeometryElements& Lane::GetLaneGeometryElements() const
{
    return laneGeometryElements;
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

    //newJoint.projectionAxes.sOffset = previousJoint.projectionAxes.sOffset + std::hypot(pointCenter.x - previousJoint.points.reference.x,
    //                                                                                   pointCenter.y - previousJoint.points.reference.y);
    //newJoint.projectionAxes.sHdg = -2.0 * std::acos(proj.Dot(previousJoint.projectionAxes.s) / (proj.Length() * previousJoint.projectionAxes.s.Length()));

    Primitive::LaneGeometryElement* newElement = new Primitive::LaneGeometryElement(previousJoint, newJoint);
    laneGeometryElements.push_back(newElement);
    laneGeometryJoints.push_back(newJoint);
}

Section::Section(osi3::world::RoadSection* osiSection, bool isInStreamDirection) : osiSection(osiSection), isInStreamDirection(isInStreamDirection)
{}

Id Section::GetId() const
{
    return osiSection->id().value();
}

void Section::AddNext(const Interfaces::Section &section)
{
    if (next)
    {
        throw std::logic_error("Sections are not allowed to have more than one successor");
    }
    next = &section;
    UpdateStreamId();
}

void Section::AddPrevious(const Interfaces::Section &section)
{
    if (previous)
    {
        throw std::logic_error("Section are not allowed to have more than one predecessor");
    }
    previous = &section;
    UpdateStreamId();
}

const Interfaces::Section *Section::GetNext() const
{
    return next;
}

const Interfaces::Section *Section::GetPrevious() const
{
    return previous;
}

bool Section::IsInStreamDirection() const
{
    return isInStreamDirection;
}

void Section::UpdateStreamId()
{
    for (const auto& sectionStream : reverseSectionStream)
    {
        if ((sectionStream.IsInStreamDirection() && !sectionStream.GetPrevious())
                || (!sectionStream.IsInStreamDirection() && !sectionStream.GetNext()))
        {
            streamId = sectionStream.GetId();
            return;
        }
    }
}

Id Section::GetStreamId() const
{
    return streamId;
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

double Section::GetDistance(MeasurementPoint measurementPoint) const
{
    if (measurementPoint == MeasurementPoint::RoadStart)
    {
        return GetSOffset();
    }

    if (measurementPoint == MeasurementPoint::RoadEnd)
    {
        return GetSOffset() + GetLength();
    }

    throw std::invalid_argument("measurement point not within valid bounds");

}

bool Section::Covers(double distance) const
{
    if (GetDistance(MeasurementPoint::RoadStart) <= distance)
    {
        return nextSections.empty() ?
                    GetDistance(MeasurementPoint::RoadEnd) >= distance :
                    GetDistance(MeasurementPoint::RoadEnd) > distance;
    }
    return false;
}

bool Section::CoversInterval(double startDistance, double endDistance) const
{
    double sectionStart = GetDistance(MeasurementPoint::RoadStart);
    double sectionEnd = GetDistance(MeasurementPoint::RoadEnd);

    bool startDistanceSmallerSectionEnd = nextSections.empty() ?
                startDistance <= sectionEnd
              : startDistance < sectionEnd;
    bool endDistanceGreaterSectionStart = previousSections.empty() ? endDistance >= sectionStart
                                                                   : endDistance > sectionStart;

    return startDistanceSmallerSectionEnd && endDistanceGreaterSectionStart;
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

double Section::GetSOffset() const
{
    return osiSection->s_offset();
}


Road::Road(osi3::world::Road* osiRoad) : osiRoad(osiRoad)
{
}

Id Road::GetId() const
{
    return osiRoad->id().value();
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
    double length = 0;
    for (const auto& section : sections)
    {
        length += section->GetLength();
    }
    return length;
}

StationaryObject::StationaryObject(osi3::StationaryObject* osiStationaryObject, void* linkedObject) :
    osiObject{osiStationaryObject}
{
    this->linkedObject = linkedObject;
}

void StationaryObject::CopyToGroundTruth(osi3::GroundTruth& target) const
{
    auto newStationaryObject = target.add_stationary_object();
    newStationaryObject->CopyFrom(*osiObject);
}

Primitive::Dimension StationaryObject::GetDimension() const
{
    const osi3::Dimension3d& osiDimension = osiObject->base().dimension();
    Primitive::Dimension dimension;

    dimension.length = osiDimension.length();
    dimension.width  = osiDimension.width();
    dimension.height = osiDimension.height();

    return dimension;
}

Primitive::AbsOrientation StationaryObject::GetAbsOrientation() const
{
    osi3::Orientation3d osiOrientation = osiObject->base().orientation();
    Primitive::AbsOrientation orientation;

    orientation.yaw = osiOrientation.yaw();
    orientation.pitch = osiOrientation.pitch();
    orientation.roll = osiOrientation.roll();

    return orientation;
}

Primitive::AbsPosition StationaryObject::GetReferencePointPosition() const
{
    const osi3::Vector3d osiPosition = osiObject->base().position();
    Primitive::AbsPosition position;

    position.x = osiPosition.x();
    position.y = osiPosition.y();
    position.z = osiPosition.z();

    return position;
}

double StationaryObject::GetAbsVelocityDouble() const
{
    return 0.0;
}

double StationaryObject::GetAbsAccelerationDouble() const
{
    return 0.0;
}

RoadPosition StationaryObject::GetRoadCoordinate() const
{
    return roadCoordinate;
}

Id StationaryObject::GetId() const
{
    return osiObject->id().value();
}

void StationaryObject::SetReferencePointPosition(const Primitive::AbsPosition& newPosition)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();

    osiPosition->set_x(newPosition.x);
    osiPosition->set_y(newPosition.y);
    osiPosition->set_z(newPosition.z);
}

void StationaryObject::SetDimension(const Primitive::Dimension& newDimension)
{
    osi3::Dimension3d* osiDimension = osiObject->mutable_base()->mutable_dimension();

    osiDimension->set_length(newDimension.length);
    osiDimension->set_width(newDimension.width);
    osiDimension->set_height(newDimension.height);
}

void StationaryObject::SetAbsOrientation(const Primitive::AbsOrientation& newOrientation)
{
    osi3::Orientation3d* osiOrientation = osiObject->mutable_base()->mutable_orientation();

    osiOrientation->set_yaw(newOrientation.yaw);
    osiOrientation->set_pitch(newOrientation.pitch);
    osiOrientation->set_roll(newOrientation.roll);
}

void StationaryObject::AddLaneAssignment(const Interfaces::Lane& lane)
{
    assignedLanes.push_back(&lane);
}

const Interfaces::Lanes& StationaryObject::GetLaneAssignments() const
{
    return assignedLanes;
}

void StationaryObject::ClearLaneAssignments()
{
    assignedLanes.clear();
}

double StationaryObject::GetDistance(MeasurementPoint measurementPoint) const
{
    if (measurementPoint == MeasurementPoint::RoadEnd)
    {
        Primitive::Dimension dimension = GetDimension();
        return roadCoordinate.s + WorldObjectCommon::GetFrontDeltaS(dimension.length, dimension.width, roadCoordinate.hdg,
                                                                    dimension.length / 2.0);
    }
    if (measurementPoint == MeasurementPoint::RoadStart)
    {
        Primitive::Dimension dimension = GetDimension();
        return roadCoordinate.s - WorldObjectCommon::GetFrontDeltaS(dimension.length, dimension.width, roadCoordinate.hdg,
                                                                    dimension.length / 2.0);
    }
    throw std::invalid_argument("measurement point not within valid bounds");
}

void StationaryObject::SetRoadCoordinate(const RoadPosition& newCoordinate)
{
    roadCoordinate = newCoordinate;
}

MovingObject::MovingObject(osi3::MovingObject* osiMovingObject, void* linkedObject) :
    osiObject{osiMovingObject}
{
    this->linkedObject = linkedObject;
    SetIndicatorState(IndicatorState::IndicatorState_Off);
    SetBrakeLightState(false);
    SetHeadLight(false);
    SetHighBeamLight(false);
}

void MovingObject::CopyToGroundTruth(osi3::GroundTruth& target) const
{
    auto newMovingObject = target.add_moving_object();
    newMovingObject->CopyFrom(*osiObject);
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

double MovingObject::GetDistanceReferencePointToLeadingEdge() const
{
    return osiObject->base().dimension().length() * 0.5 - osiObject->vehicle_attributes().bbcenter_to_rear().x();
}

double MovingObject::GetDistance(MeasurementPoint measurementPoint) const
{
    if (measurementPoint == MeasurementPoint::RoadEnd)
    {
        if (!frontDistance.IsValid())
        {
            Primitive::Dimension dimension = GetDimension();
            frontDistance.Update(roadCoordinate.s + WorldObjectCommon::GetFrontDeltaS(dimension.length, dimension.width, roadCoordinate.hdg,
                                                                                      GetDistanceReferencePointToLeadingEdge()));
        }
        return frontDistance.Get();
    }
    if (measurementPoint == MeasurementPoint::RoadStart)
    {
        if (!rearDistance.IsValid())
        {
            Primitive::Dimension dimension = GetDimension();
            rearDistance.Update(roadCoordinate.s + WorldObjectCommon::GetRearDeltaS(dimension.length, dimension.width, roadCoordinate.hdg,
                                                                                    GetDistanceReferencePointToLeadingEdge()));
        }
        return rearDistance.Get();
    }
    throw std::invalid_argument("measurement point not within valid bounds");
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

void MovingObject::SetDistanceReferencPointToLeadingEdge(const double distance)
{
    osiObject->mutable_vehicle_attributes()->mutable_bbcenter_to_rear()->set_x(osiObject->base().dimension().length() * 0.5 - distance);
}

Primitive::AbsPosition MovingObject::GetReferencePointPosition() const
{
    const osi3::Vector3d& osiPosition = osiObject->base().position();
    const double yaw = osiObject->base().orientation().yaw();
    const osi3::Vector3d& bbCenterToRear = osiObject->vehicle_attributes().bbcenter_to_rear();

    Primitive::AbsPosition position;

    position.x = osiPosition.x() + std::cos(yaw) * bbCenterToRear.x();
    position.y = osiPosition.y() + std::sin(yaw) * bbCenterToRear.x();
    position.z = osiPosition.z();

    return position;
}

void MovingObject::SetReferencePointPosition(const Primitive::AbsPosition& newPosition)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();
    const double yaw = osiObject->base().orientation().yaw();
    const osi3::Vector3d& bbCenterToRear = osiObject->vehicle_attributes().bbcenter_to_rear();

    osiPosition->set_x(newPosition.x - std::cos(yaw) * bbCenterToRear.x());
    osiPosition->set_y(newPosition.y - std::sin(yaw) * bbCenterToRear.x());
    osiPosition->set_z(newPosition.z);
    frontDistance.Invalidate();
    rearDistance.Invalidate();
}

void MovingObject::SetX(const double newX)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();
    const double yaw = osiObject->base().orientation().yaw();
    const osi3::Vector3d& bbCenterToRear = osiObject->vehicle_attributes().bbcenter_to_rear();

    osiPosition->set_x(newX - std::cos(yaw) * bbCenterToRear.x());
}

void MovingObject::SetY(const double newY)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();
    const double yaw = osiObject->base().orientation().yaw();
    const osi3::Vector3d& bbCenterToRear = osiObject->vehicle_attributes().bbcenter_to_rear();

    osiPosition->set_y(newY - std::sin(yaw) * bbCenterToRear.x());
}

void MovingObject::SetZ(const double newZ)
{
    osi3::Vector3d* osiPosition = osiObject->mutable_base()->mutable_position();
    osiPosition->set_z(newZ);
}

RoadPosition MovingObject::GetRoadCoordinate() const
{
    return roadCoordinate;
}

void MovingObject::SetRoadCoordinate(const RoadPosition& newCoordinate)
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
    const auto referencePosition = GetReferencePointPosition(); //AbsPosition needs to be evaluated with "old" yaw
    osiOrientation->set_yaw(newOrientation.yaw);
    osiOrientation->set_pitch(newOrientation.pitch);
    osiOrientation->set_roll(newOrientation.roll);
    frontDistance.Invalidate();
    rearDistance.Invalidate();
    SetReferencePointPosition(referencePosition); //Changing yaw also changes position of the boundingBox center
}

void MovingObject::SetYaw(const double newYaw)
{
    const auto referencePosition = GetReferencePointPosition();
    osi3::Orientation3d* osiOrientation = osiObject->mutable_base()->mutable_orientation();
    osiOrientation->set_yaw(newYaw);
    frontDistance.Invalidate();
    rearDistance.Invalidate();
    SetReferencePointPosition(referencePosition); //Changing yaw also changes position of the boundingBox center
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

void MovingObject::SetIndicatorState(IndicatorState indicatorState)
{
    if (indicatorState == IndicatorState::IndicatorState_Off)
    {
        osiObject->mutable_vehicle_classification()->mutable_light_state()->set_indicator_state(
                    osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_OFF);
        return;
    }
    else
        if (indicatorState == IndicatorState::IndicatorState_Left)
        {
            osiObject->mutable_vehicle_classification()->mutable_light_state()->set_indicator_state(
                        osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_LEFT);
            return;
        }
        else
            if (indicatorState == IndicatorState::IndicatorState_Right)
            {
                osiObject->mutable_vehicle_classification()->mutable_light_state()->set_indicator_state(
                            osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_RIGHT);
                return;
            }
            else
                if (indicatorState == IndicatorState::IndicatorState_Warn)
                {
                    osiObject->mutable_vehicle_classification()->mutable_light_state()->set_indicator_state(
                                osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_WARNING);
                    return;
                }

    throw std::logic_error("Indicator state is not supported");
}

IndicatorState MovingObject::GetIndicatorState() const
{
    const auto& osiState = osiObject->vehicle_classification().light_state().indicator_state();

    if (osiState == osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_OFF)
    {
        return IndicatorState::IndicatorState_Off;
    }
    else
        if (osiState == osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_LEFT)
        {
            return IndicatorState::IndicatorState_Left;
        }
        else
            if (osiState == osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_RIGHT)
            {
                return IndicatorState::IndicatorState_Right;
            }
            else
                if (osiState == osi3::MovingObject_VehicleClassification_LightState_IndicatorState_INDICATOR_STATE_WARNING)
                {
                    return IndicatorState::IndicatorState_Warn;
                }

    throw std::logic_error("Indicator state is not supported");
}

void MovingObject::SetBrakeLightState(bool brakeLightState)
{
    if (brakeLightState)
    {
        osiObject->mutable_vehicle_classification()->mutable_light_state()->set_brake_light_state(
                    osi3::MovingObject_VehicleClassification_LightState_BrakeLightState_BRAKE_LIGHT_STATE_NORMAL);
    }
    else
    {
        osiObject->mutable_vehicle_classification()->mutable_light_state()->set_brake_light_state(
                    osi3::MovingObject_VehicleClassification_LightState_BrakeLightState_BRAKE_LIGHT_STATE_OFF);
    }
}

bool MovingObject::GetBrakeLightState() const
{
    const auto& osiState = osiObject->vehicle_classification().light_state().brake_light_state();

    if (osiState == osi3::MovingObject_VehicleClassification_LightState_BrakeLightState_BRAKE_LIGHT_STATE_NORMAL)
    {
        return true;
    }
    else
        if (osiState == osi3::MovingObject_VehicleClassification_LightState_BrakeLightState_BRAKE_LIGHT_STATE_OFF)
        {
            return false;
        }

    throw std::logic_error("BrakeLightState is not supported");
}

void MovingObject::SetHeadLight(bool headLight)
{
    if (headLight)
    {
        osiObject->mutable_vehicle_classification()->mutable_light_state()->set_head_light(
                    osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_ON);
    }
    else
    {
        osiObject->mutable_vehicle_classification()->mutable_light_state()->set_head_light(
                    osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_OFF);
    }
}

bool MovingObject::GetHeadLight() const
{
    const auto& osiState = osiObject->vehicle_classification().light_state().head_light();

    if (osiState == osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_ON)
    {
        return true;
    }
    else
        if (osiState == osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_OFF)
        {
            return false;
        }

    throw std::logic_error("HeadLightState is not supported");
}

void MovingObject::SetHighBeamLight(bool highbeamLight)
{
    if (highbeamLight)
    {
        osiObject->mutable_vehicle_classification()->mutable_light_state()->set_high_beam(
                    osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_ON);
    }
    else
    {
        osiObject->mutable_vehicle_classification()->mutable_light_state()->set_high_beam(
                    osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_OFF);
    }
}

bool MovingObject::GetHighBeamLight() const
{
    const auto& osiState = osiObject->vehicle_classification().light_state().high_beam();

    if (osiState == osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_ON)
    {
        return true;
    }
    else
        if (osiState == osi3::MovingObject_VehicleClassification_LightState_GenericLightState_GENERIC_LIGHT_STATE_OFF)
        {
            return false;
        }

    throw std::logic_error("HighBeamLightState is not supported");
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
    Primitive::AbsOrientation orientation = GetAbsOrientation();
    double sign = 1.0;

    double velocityAngle = std::atan2(velocity.vy, velocity.vx);

    if (std::abs(velocity.vx) == 0.0 && std::abs(velocity.vy) == 0.0)
    {
        velocityAngle = 0.0;
    }

    double angleBetween = velocityAngle - orientation.yaw;

    if ((std::abs(angleBetween) - M_PI_2) > 0)
    {
        sign = -1.0;
    }

    return std::hypot(velocity.vx, velocity.vy) * sign;
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
    Primitive::AbsOrientation orientation = GetAbsOrientation();
    double sign = 1.0;

    double accAngle = std::atan2(acceleration.ay, acceleration.ax);

    if (std::abs(acceleration.ax) == 0.0 && std::abs(acceleration.ay) == 0.0)
    {
        accAngle = 0.0;
    }

    double angleBetween = accAngle - orientation.yaw;

    if ((std::abs(angleBetween) - M_PI_2) > 0)
    {
        sign = -1.0;
    }

    return std::hypot(acceleration.ax, acceleration.ay) * sign;
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

void MovingObject::SetAbsOrientationRate(const Primitive::AbsOrientationRate& newOrientationRate)
{
    osi3::Orientation3d* osiOrientationRate = osiObject->mutable_base()->mutable_orientation_rate();

    osiOrientationRate->set_yaw(newOrientationRate.yawRate);
    osiOrientationRate->set_pitch(newOrientationRate.pitchRate);
    osiOrientationRate->set_roll(newOrientationRate.rollRate);
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

Angle Vehicle::GetSteeringWheelAngle()
{
    throw std::logic_error("not implemented");
}

void Vehicle::SetSteeringWheelAngle(const Angle newValue)
{
    Q_UNUSED(newValue);
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


TrafficSign::TrafficSign(osi3::TrafficSign* osiObject) : osiSign{osiObject}
{
}

CommonTrafficSign::Type TrafficSign::GetType() const
{
    return  OpenDriveTypeMapper::OsiToOdTrafficSignType(osiSign->main_sign().classification().type());
}


void TrafficSign::SetType(RoadSignalType type)
{
    auto osiType = OpenDriveTypeMapper::TrafficSignType(type);
    osiSign->mutable_main_sign()->mutable_classification()->set_type(osiType);
}

bool TrafficSign::IsValidForLane(OWL::Id laneId) const
{
    auto assignedLanes = osiSign->main_sign().classification().assigned_lane_id();
    for (auto lane : assignedLanes)
    {
        if (lane.value() == laneId)
        {
            return true;
        }
    }
    return false;
}

void TrafficSign::CopyToGroundTruth(osi3::GroundTruth& target) const
{
    auto newTrafficSign = target.add_traffic_sign();
    newTrafficSign->CopyFrom(*osiSign);
} // namespace Implementation

}

} // namespace OWL
