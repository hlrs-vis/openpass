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

#include "WorldData.h"
#include "Interfaces/worldObjectInterface.h"

class WorldObjectAdapter : public virtual WorldObjectInterface
{
public:
    const polygon_t& GetBoundingBox2D() const;
    double GetHeight() const;
    int GetId() const;
    double GetLength() const;
    double GetPositionX() const;
    double GetPositionY() const;
    double GetPositionLateral() const;
    double GetWidth() const;
    double GetYaw() const;
    double GetDistanceReferencePointToLeadingEdge() const;
    double GetAcceleration() const;
    const OWL::Interfaces::WorldObject& GetBaseTrafficObject() const;

    virtual double GetLaneRemainder(Side) const = 0;
    virtual double GetLaneDirection() const = 0;


protected:
    OWL::Interfaces::WorldObject& baseTrafficObject;

    double s{0.0};
    mutable bool boundingBoxNeedsUpdate{true};

private:
    const polygon_t CalculateBoundingBox() const;

    mutable polygon_t boundingBox;

public:
    // object is not inteded to be copied or assigned
    WorldObjectAdapter(OWL::Interfaces::WorldObject& baseTrafficObject);
    WorldObjectAdapter(const WorldObjectAdapter&) = delete;
    WorldObjectAdapter(WorldObjectAdapter&&) = delete;
    WorldObjectAdapter& operator=(const WorldObjectAdapter&) = delete;
    WorldObjectAdapter& operator=(WorldObjectAdapter&&) = delete;
    virtual ~WorldObjectAdapter() = default;
};

namespace WorldObjectCommon {

double GetFrontDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge);
double GetRearDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge);

} // namespace WorldObjectCommon

