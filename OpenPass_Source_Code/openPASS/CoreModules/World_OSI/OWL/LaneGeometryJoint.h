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

#include "Common/vector2d.h"
#include "OWL/Primitives.h"

namespace OWL {
namespace Primitive {

/// \brief Correction factors of the s coordinate
///
/// If a road makes a curvature, the s coordinate needs to be
/// scaled to correct for the different lengths of the lanes
/// If the reference lines are not in parallel, points smaller
/// than 0 and larger than maximum needs to be cropped
struct CurvatureCorrection
{
    double scale;   //!< \brief Multiply with scale to correct projection lenghts
    double maximum; //!< \brief Value needs to be within interval [0 maximum]
};

/// \brief Offset between reference line of the road and reference line of the lane
///
/// s coordintes always refer to the reference line of the lane
/// t coordinate is normal to the reference line of the current lane
struct ProjectionAxes
{
    Common::Vector2d s; //!< \brief s coordinate projection axis
    Common::Vector2d t; //!< \brief t coordinate projection axis
    double sOffset;     //!< \brief Offset with respect to the start of the road
    double sHdg;        //!< \brief Heading of the s projection axis
    CurvatureCorrection curvatureCorrection; //!< \sa CurvatureCorrection
};

struct LaneGeometryJoint
{
    /// \brief Tripple describing the sampled points of the lane
    /// \note Left/Right refers to the direction of the road
    struct Points
    {
        Common::Vector2d left;
        Common::Vector2d reference;
        Common::Vector2d right;
    } points;

    double curvature;              //!< \brief Curvature of the reference line at this joint
    ProjectionAxes projectionAxes; //!< \brief Projection axes originating at the reference point
};

} // namespace Primitive
} // namespace OWL
