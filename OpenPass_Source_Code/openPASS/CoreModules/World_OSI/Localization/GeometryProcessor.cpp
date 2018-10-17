/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "GeometryProcessor.h"

#include <cmath>

bool OWL::GeometryProcessor::Match(const Common::Vector2d &point) const
{
    return PointQuery::IsWithin(laneGeometryElement, point);
}

double OWL::GeometryProcessor::getS(const Common::Vector2d &point) const
{
    const Common::Vector2d vectorPointToMiddle = getVectorPointToReference(point);
    return laneGeometryElement.joints.current.projectionAxes.sOffset + getProjectedVectorLength(vectorPointToMiddle, vectorAlongMiddle);
}

double OWL::GeometryProcessor::getT(const Common::Vector2d &point) const
{
    const Common::Vector2d vectorPointToMiddle = getVectorPointToReference(point);

    if (std::abs(vectorLeftToMiddle.x) < 1e-6 &&
            std::abs(vectorLeftToMiddle.y) < 1e-6) {
        return getProjectedVectorLength(vectorPointToMiddle, getOrthogonalMiddleVector());
    }

    return getProjectedVectorLength(vectorPointToMiddle, vectorLeftToMiddle);
}

double OWL::GeometryProcessor::getYaw(double hdg) const {
    return std::fmod(hdg - laneGeometryElement.joints.current.projectionAxes.sHdg, M_PI);
}

double OWL::GeometryProcessor::getProjectedVectorLength(const Common::Vector2d &vectorToProject, const Common::Vector2d &directionVector) const
{
    return vectorToProject.Dot(directionVector) / directionVector.Length();
}

const Common::Vector2d OWL::GeometryProcessor::getVectorPointToReference(Common::Vector2d point) const
{
    const Common::Vector2d refCoord = laneGeometryElement.joints.current.points.reference;
    return point - refCoord;
}

const Common::Vector2d OWL::GeometryProcessor::getOrthogonalMiddleVector() const
{
    Common::Vector2d vectorForRotation(vectorAlongMiddle);
    vectorForRotation.Rotate(M_PI_2);
    return vectorForRotation;
}
