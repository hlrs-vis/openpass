/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadGeometryLineInterface.h
//! @brief This file contains a interface representing a road line as a
//!        RoadGeometry.
//-----------------------------------------------------------------------------

#ifndef ROADGEOMETRYLINEINTERFACE
#define ROADGEOMETRYLINEINTERFACE

#include "roadInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a road line as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometryLineInterface : public virtual RoadGeometryInterface
{
public:
    RoadGeometryLineInterface() = default;
    RoadGeometryLineInterface(const RoadGeometryLineInterface&) = delete;
    RoadGeometryLineInterface(RoadGeometryLineInterface&&) = delete;
    RoadGeometryLineInterface& operator=(const RoadGeometryLineInterface&) = delete;
    RoadGeometryLineInterface& operator=(RoadGeometryLineInterface&&) = delete;
    virtual ~RoadGeometryLineInterface() = default;

    //-----------------------------------------------------------------------------
    //! Calculates the x/y coordinates as vector. Wrapper for RoadGeometry:GetCoordLine.
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
    //! Calculates the curvature. Wrapper for RoadGeometry:GetCurvatureLine.
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
    //! Calculates the direction. Wrapper for RoadGeometry:GetDirLine.
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

#endif // ROADGEOMETRYLINEINTERFACE

