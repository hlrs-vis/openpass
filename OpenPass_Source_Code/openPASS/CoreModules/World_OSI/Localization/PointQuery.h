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

#include "OWL/DataTypes.h"
#include "Common/vector2d.h"

namespace World {
namespace Localization {

class PointQuery
{
private:
    static constexpr double EDGE_DETECTION_THRESHOLD = 1e-6;

public:
    /// Check if point lies within (or at the boundaries) of a given geometry element
    ///
    /// \param[in] element  element object
    /// \param[in] point    2D point
    /// \return true, if point is within (or at the boundaries) of the element
    ///
    static bool IsWithin(const OWL::Primitive::LaneGeometryElement& element, const Common::Vector2d& point);

private:

    /*
       currentJoint        |    nextJoint
                          |
              A           |
    left      #-------____|__      B
             /   P        |  ------#
            /    #        |         \
           /              |          \
    right #---------------|-----------#
          C               |            D
                          |
    */
    static bool WithinQuadrilateral(const Common::Vector2d& A,
                                    const Common::Vector2d& B,
                                    const Common::Vector2d& C,
                                    const Common::Vector2d& D,
                                    const Common::Vector2d& P);


    static bool WithinBarycentricCoords(double dot00,
                                        double dot02,
                                        double dot01,
                                        double dot11,
                                        double dot12);

    static bool OnEdge(const Common::Vector2d& A,
                       const Common::Vector2d& B,
                       const Common::Vector2d& P);
};

} // namespace Localization
} // namespace World
