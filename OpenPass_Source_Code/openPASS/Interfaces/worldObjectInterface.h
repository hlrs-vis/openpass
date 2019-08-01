/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include "Common/globalDefinitions.h"
#include "Common/boostGeometryCommon.h"
#include <unordered_map>

class WorldObjectInterface;

enum class VelocityScope
{
    Absolute,
    Lateral,
    Longitudinal,
    DirectionX,
    DirectionY
};

class WorldObjectInterface
{
public:
    virtual ObjectTypeOSI GetType() const = 0;

    virtual double GetPositionX() const = 0;
    virtual double GetPositionY() const = 0;

    /// \brief Retrieves width of the boundary box
    /// \return width
    virtual double GetWidth() const = 0;

    /// \brief Retrieves length of the boundary box
    /// \return length
    virtual double GetLength() const = 0;

    virtual double GetHeight() const = 0;

    /// \brief Retrieves yaw angle w.r.t. x-axis
    /// \return yaw
    virtual double GetYaw() const = 0;

    /// \brief  Get unique id of an object
    /// \return id
    virtual int GetId() const = 0;

    virtual const polygon_t& GetBoundingBox2D() const = 0;

    virtual double GetDistanceToStartOfRoad() const
    {
        return GetDistanceToStartOfRoad(MeasurementPoint::Front);
    }

    virtual double GetLaneDirection() const = 0;

    /// \brief  Get distance w.r.t. the specified measurement point
    /// \return distance
    virtual double GetDistanceToStartOfRoad(MeasurementPoint mp) const = 0;

    virtual double GetDistanceReferencePointToLeadingEdge() const = 0;

    virtual double GetVelocity() const
    {
        return GetVelocity(VelocityScope::Absolute);
    }

    /// \brief  Get velocity w.r.t. the specified direction
    /// \return velocity
    virtual double GetVelocity(VelocityScope velocityScope) const = 0;

    virtual double GetAcceleration() const = 0;

    /// \brief Distance w.r.t. to the lane boundary for the outmost boundary point
    /// \return distance in m
    virtual double GetLaneRemainder(Side) const = 0;

    /// \brief Gets the coordinate of the outmost boundary point for a given side
    /// \param side Side of interest
    /// \return Position with respect to a lane
    virtual GlobalRoadPosition GetBoundaryPoint(Side side) const = 0;

    virtual bool Locate() = 0;
    virtual void Unlocate() = 0;

    // object is not inteded to be copied or assigned
    WorldObjectInterface() = default;
    WorldObjectInterface(const WorldObjectInterface&) = delete;
    WorldObjectInterface(WorldObjectInterface&&) = delete;
    WorldObjectInterface& operator=(const WorldObjectInterface&) = delete;
    WorldObjectInterface& operator=(WorldObjectInterface&&) = delete;
    virtual ~WorldObjectInterface() = default;
};

