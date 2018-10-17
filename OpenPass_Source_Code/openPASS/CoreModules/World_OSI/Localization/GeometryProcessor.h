/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

#include "OWL/DataTypes.h"
#include "PointQuery.h"
#include "vector2d.h"

namespace OWL {

class GeometryProcessor
{

const Primitive::LaneGeometryElement& laneGeometryElement;
const Common::Vector2d vectorAlongMiddle;
const Common::Vector2d vectorLeftToMiddle;

public:
GeometryProcessor(const Primitive::LaneGeometryElement& laneGeometryElement) :
    laneGeometryElement{laneGeometryElement},
    vectorAlongMiddle{laneGeometryElement.joints.current.projectionAxes.s},
    vectorLeftToMiddle{laneGeometryElement.joints.current.projectionAxes.t}
{}

bool Match(const Common::Vector2d &point) const;

Primitive::RoadCoordinate GetRoadCoordinate(const Common::Vector2d &point, double hdg) const {
    const auto s = getS(point);
    const auto t = getT(point);
    const auto yaw = getYaw(hdg);
    return {s, t, yaw};
}

private:
double getS(const Common::Vector2d &point) const;
double getT(const Common::Vector2d &point) const;
double getYaw(double hdg) const;
double getProjectedVectorLength(const Common::Vector2d &vectorToProject,
                                const Common::Vector2d &directionVector) const;

const Common::Vector2d getVectorPointToReference(Common::Vector2d point) const;
const Common::Vector2d getOrthogonalMiddleVector() const;

};

} // namespace OWL
