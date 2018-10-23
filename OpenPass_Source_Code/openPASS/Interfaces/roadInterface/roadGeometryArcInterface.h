/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadGeometryArcInterface.h
//! @brief This file contains a interface representing a road arc as a
//!        RoadGeometry.
//-----------------------------------------------------------------------------

#ifndef ROADGEOMETRYARCINTERFACE
#define ROADGEOMETRYARCINTERFACE

#include "roadInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a road arc as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometryArcInterface : public virtual RoadGeometryInterface
{
public:
    RoadGeometryArcInterface() = default;
    RoadGeometryArcInterface(const RoadGeometryArcInterface&) = delete;
    RoadGeometryArcInterface(RoadGeometryArcInterface&&) = delete;
    RoadGeometryArcInterface& operator=(const RoadGeometryArcInterface&) = delete;
    RoadGeometryArcInterface& operator=(RoadGeometryArcInterface&&) = delete;
    virtual ~RoadGeometryArcInterface() = default;

    //-----------------------------------------------------------------------------
    //! Calculates the x/y coordinates as vector. Wrapper for RoadGeometry:GetCoordArc
    //! (or RoadGeometry:GetCoordLine, if curvature is 0).
    //!
    //! @param[in]  side                side of road (1: left, -1: right)
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes
    //! @param[in]  laneOffset          lane offset
    //! @param[in]  laneWidth           width of lane
    //! @param[in]  corner              position in lane (1: left, 0: middle, -1: right)
    //! @return                         vector with the x/y coordinates
    //-----------------------------------------------------------------------------
    virtual Common::Vector2d GetCoord(double side,
                                      double geometryOffset,
                                      double previousWidth,
                                      double laneOffset,
                                      double laneWidth,
                                      int corner);

    //-----------------------------------------------------------------------------
    //! Calculates the curvature. Wrapper for RoadGeometry:GetCurvatureArc (or
    //! RoadGeometry:GetCurvatureLine, if curvature is 0).
    //!
    //! @param[in]  side                side of road (1: left, -1: right)
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes
    //! @param[in]  laneOffset          lane offset
    //! @param[in]  laneWidth           width of lane
    //! @return                         curvature
    //-----------------------------------------------------------------------------
    virtual double GetCurvature(double side,
                                double geometryOffset,
                                double previousWidth,
                                double laneOffset,
                                double laneWidth);

    //-----------------------------------------------------------------------------
    //! Calculates the direction. Wrapper for RoadGeometry:GetDirArc (or
    //! RoadGeometry:GetDirLine, if curvature is 0).
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @return                         direction
    //-----------------------------------------------------------------------------
    virtual double GetDir(double side,
                          double geometryOffset,
                          double previousWidth,
                          double laneOffset,
                          double laneWidth);
};

#endif // ROADGEOMETRYARCINTERFACE

