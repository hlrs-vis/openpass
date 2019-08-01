/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "GeometryProcessor.h"
#include <cmath>

namespace World {
namespace Localization {

bool GeometryProcessor::Match(const Common::Vector2d& point) const
{
    return PointQuery::IsWithin(*laneGeometryElement, point);
}

double GeometryProcessor::GetS(const Common::Vector2d& point) const
{
    const Common::Vector2d vectorPointToMiddle = GetVectorPointToReference(point);
    return laneGeometryElement->joints.current.projectionAxes.sOffset + GetProjectedVectorLength(vectorPointToMiddle,
            vectorAlongMiddle);
}

double GeometryProcessor::GetT(const Common::Vector2d& point) const
{
    const Common::Vector2d vectorPointToMiddle = GetVectorPointToReference(point);

    if (std::abs(vectorLeftToMiddle.x) < 1e-6 &&
            std::abs(vectorLeftToMiddle.y) < 1e-6)
    {
        return GetProjectedVectorLength(vectorPointToMiddle, GetOrthogonalMiddleVector());
    }

    return GetProjectedVectorLength(vectorPointToMiddle, vectorLeftToMiddle);
}

double GeometryProcessor::GetYaw(double hdg) const
{
    // Updated for direction awareness - might be an issue, when cars try to turn around
    return std::fmod((hdg - laneGeometryElement->joints.current.projectionAxes.sHdg + 3 * M_PI), (2 * M_PI)) - M_PI;
}

double GeometryProcessor::GetProjectedVectorLength(const Common::Vector2d& vectorToProject,
        const Common::Vector2d& directionVector) const
{
    return vectorToProject.Dot(directionVector) / directionVector.Length();
}

const Common::Vector2d GeometryProcessor::GetVectorPointToReference(Common::Vector2d point) const
{
    const Common::Vector2d refCoord = laneGeometryElement->joints.current.points.reference;
    return point - refCoord;
}

const Common::Vector2d GeometryProcessor::GetOrthogonalMiddleVector() const
{
    Common::Vector2d vectorForRotation(vectorAlongMiddle);
    vectorForRotation.Rotate(M_PI_2);
    return vectorForRotation;
}

} // namespace Localization
} // namespace World

