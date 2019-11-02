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

#include "gmock/gmock.h"

#include "OWL/DataTypes.h"
#include "WorldData.h"

#include <string>

class FakeLane : public OWL::Interfaces::Lane
{
public:
    MOCK_CONST_METHOD0(GetId,
                       OWL::Id());
    MOCK_CONST_METHOD0(GetLaneType,
                       OWL::LaneType());
    MOCK_CONST_METHOD0(Exists,
                       bool());
    MOCK_CONST_METHOD0(IsInStreamDirection,
                       bool());
    MOCK_CONST_METHOD0(GetSection,
                       const OWL::Interfaces::Section & ());
    MOCK_CONST_METHOD0(GetRoad,
                       const OWL::Interfaces::Road & ());
    MOCK_CONST_METHOD0(GetLaneGeometryElements,
                       const OWL::Interfaces::LaneGeometryElements & ());
    MOCK_CONST_METHOD0(GetLength,
                       double());
    MOCK_CONST_METHOD0(GetRightLaneCount,
                       int());
    MOCK_CONST_METHOD1(GetCurvature,
                       double(double distance));
    MOCK_CONST_METHOD1(GetWidth,
                       double(double distance));
    MOCK_CONST_METHOD1(GetDirection,
                       double(double distance));
    MOCK_CONST_METHOD1(GetInterpolatedPointsAtDistance,
                       const OWL::Primitive::LaneGeometryJoint::Points(double));
    MOCK_CONST_METHOD0(GetNext,
                       const OWL::Interfaces::Lane * ());
    MOCK_CONST_METHOD0(GetPrevious,
                       const OWL::Interfaces::Lane * ());
    MOCK_CONST_METHOD0(GetLeftLane,
                       const OWL::Interfaces::Lane & ());
    MOCK_CONST_METHOD0(GetRightLane,
                       const OWL::Interfaces::Lane & ());
    MOCK_METHOD0(UpdateStreamId,
                 void());
    MOCK_CONST_METHOD0(GetStreamId,
                       OWL::Id());
    MOCK_CONST_METHOD1(GetDistance,
                       double(OWL::MeasurementPoint mp));
    MOCK_CONST_METHOD1(Covers,
                       bool(double));
    MOCK_METHOD6(AddLaneGeometryJoint,
                 void(const Common::Vector2d& pointLeft,
                      const Common::Vector2d& pointCenter,
                      const Common::Vector2d& pointRight,
                      double sOffset, double curvature, double heading));
    MOCK_METHOD1(SetLaneType,
                 void(OWL::LaneType specifiedType));
    MOCK_METHOD1(SetLeftLane,
                 void(const OWL::Interfaces::Lane& lane));
    MOCK_METHOD1(SetRightLane,
                 void(const OWL::Interfaces::Lane& lane));
    MOCK_CONST_METHOD0(GetWorldObjects,
                       const OWL::Interfaces::WorldObjects & ());
    MOCK_METHOD1(AddMovingObject,
                 void(OWL::Interfaces::MovingObject& movingObject));
    MOCK_METHOD1(AddStationaryObject,
                 void(OWL::Interfaces::StationaryObject& stationaryObject));
    MOCK_METHOD1(AddWorldObject, void (OWL::Interfaces::WorldObject& worldObject));
    MOCK_METHOD0(ClearMovingObjects, void());

    MOCK_METHOD1(AddNext,
                 void(const OWL::Interfaces::Lane& lane));
    MOCK_METHOD1(AddPrevious,
                 void(const OWL::Interfaces::Lane& lane));
    MOCK_CONST_METHOD1(CopyToGroundTruth, void(osi3::GroundTruth&));
};
