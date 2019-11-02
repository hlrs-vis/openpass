/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <exception>
#include <cmath>
#include "TrafficObjectAdapter.h"
#include "Localization/LocalizationCache.h"

//TODO: replace GlobalObject with injected shared_pointer to locator

TrafficObjectAdapter::TrafficObjectAdapter(OWL::Interfaces::WorldData& worldData,
        World::Localization::Cache& localizationCache,
        OWL::Primitive::AbsPosition position,
        OWL::Primitive::Dimension dimension, OWL::Primitive::AbsOrientation orientation) :
    WorldObjectAdapter{worldData.AddStationaryObject(static_cast<void*>(this))},
    locator{baseTrafficObject, worldData, localizationCache}
{
    baseTrafficObject.SetReferencePointPosition(position);
    baseTrafficObject.SetDimension(dimension);
    baseTrafficObject.SetAbsOrientation(orientation);
    InitLaneDirection(orientation.yaw);
    Locate();
}

ObjectTypeOSI TrafficObjectAdapter::GetType() const
{
    return ObjectTypeOSI::Object;
}

double TrafficObjectAdapter::GetDistanceToStartOfRoad() const
{
    return WorldObjectInterface::GetDistanceToStartOfRoad();
}

void TrafficObjectAdapter::InitLaneDirection(double hdg)
{
    laneDirection = GetYaw() - hdg;
}

double TrafficObjectAdapter::GetDistanceToStartOfRoad(MeasurementPoint mp) const
{
    if (mp == MeasurementPoint::Front)
    {
        return GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadEnd);
    }
    if (mp == MeasurementPoint::Rear)
    {
        return GetBaseTrafficObject().GetDistance(OWL::MeasurementPoint::RoadStart);
    }
    if (mp == MeasurementPoint::Reference)
    {
        auto value = GetBaseTrafficObject().GetRoadCoordinate().s;
        return value;
    }

    throw std::invalid_argument("measurement point not within valid bounds");
}

double TrafficObjectAdapter::GetVelocity(VelocityScope velocityScope) const
{
    //TrafficObjects don't move
    return 0.0;
}

double TrafficObjectAdapter::GetLaneDirection() const
{
    return laneDirection;
}

double TrafficObjectAdapter::GetLaneRemainder(Side side) const
{
    if (remainders.empty())
    {
        // Update only on request
        remainders = locator.GetLaneRemainders();
    }

    assert(!remainders.empty());
    return side == Side::Left ? remainders.begin()->left : remainders.rbegin()->right;
}

GlobalRoadPosition TrafficObjectAdapter::GetBoundaryPoint(Side side) const
{
    if (boundaryPoints.empty())
    {
        // Update only on request
        boundaryPoints = locator.GetBoundaryPoints();
    }
    assert(!boundaryPoints.empty());
    return side == Side::Left ? *boundaryPoints.begin() : *boundaryPoints.rbegin();
}

bool TrafficObjectAdapter::Locate()
{
    // reset on-demand values
    remainders.clear();
    boundaryPoints.clear();

    locateResult = locator.Locate(GetBoundingBox2D(), GetLength() + GetWidth());
    if (!locateResult.isLocalizable)
    {
        return false;
    }

    baseTrafficObject.SetRoadCoordinate(locateResult.globalRoadPosition.roadPosition);
    return true;
}

void TrafficObjectAdapter::Unlocate()
{
    locator.Unlocate();
}

TrafficObjectAdapter::~TrafficObjectAdapter()
{
    Unlocate();
}
