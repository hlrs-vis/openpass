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

#include "OWL/DataTypes.h"
#include "PointQuery.h"
#include "Common/vector2d.h"
#include "LaneWalker.h"

namespace World {
namespace Localization {

class GeometryProcessor
{
    const OWL::Primitive::LaneGeometryElement* laneGeometryElement;
    Common::Vector2d vectorAlongMiddle;
    Common::Vector2d vectorLeftToMiddle;

public:
    GeometryProcessor(const OWL::Primitive::LaneGeometryElement* laneGeometryElement) :
        laneGeometryElement{laneGeometryElement},
        vectorAlongMiddle{laneGeometryElement->joints.current.projectionAxes.s},
        vectorLeftToMiddle{laneGeometryElement->joints.current.projectionAxes.t}
    {}

    GeometryProcessor(const LaneWalker& laneWalker)
    {
        if (!laneWalker.IsDepleted())
        {
            laneGeometryElement = &laneWalker.GetLaneGeometryElement();
            vectorAlongMiddle = laneGeometryElement->joints.current.projectionAxes.s;
            vectorLeftToMiddle = laneGeometryElement->joints.current.projectionAxes.t;
        }
    }

    bool Match(const Common::Vector2d& point) const;

    RoadPosition GetRoadCoordinate(const Common::Vector2d& point, double hdg) const
    {
        const auto s = GetS(point);
        const auto t = GetT(point);
        const auto yaw = GetYaw(hdg);
        return {s, t, yaw};
    }

private:
    double GetS(const Common::Vector2d& point) const;
    double GetT(const Common::Vector2d& point) const;
    double GetYaw(double hdg) const;
    double GetProjectedVectorLength(const Common::Vector2d& vectorToProject,
                                    const Common::Vector2d& directionVector) const;

    const Common::Vector2d GetVectorPointToReference(Common::Vector2d point) const;
    const Common::Vector2d GetOrthogonalMiddleVector() const;

};

} // namespace Localization
} // namespace World

