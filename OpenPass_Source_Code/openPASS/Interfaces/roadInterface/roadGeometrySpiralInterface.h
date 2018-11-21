/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadGeometrySpiralInterface.h
//! @brief This file contains a interface representing a spiral road (clothoid)
//!        as a RoadGeometry.
//-----------------------------------------------------------------------------

#ifndef ROADGEOMETRYSPIRALINTERFACE
#define ROADGEOMETRYSPIRALINTERFACE

#include "roadInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a spiral road (clothoid) as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometrySpiralInterface : public virtual RoadGeometryInterface
{
public:
    RoadGeometrySpiralInterface() = default;
    RoadGeometrySpiralInterface(const RoadGeometrySpiralInterface&) = delete;
    RoadGeometrySpiralInterface(RoadGeometrySpiralInterface&&) = delete;
    RoadGeometrySpiralInterface& operator=(const RoadGeometrySpiralInterface&) = delete;
    RoadGeometrySpiralInterface& operator=(RoadGeometrySpiralInterface&&) = delete;
    virtual ~RoadGeometrySpiralInterface() = default;


    //-----------------------------------------------------------------------------
    //! Calculates the x/y coordinates as vector. Wrapper for RoadGeometry:GetCoordLine,
    //! if start and end curvature of the spiral are 0, for RoadGeometry:GetCoordArc
    //! if they are equal and to FullCoord otherwise.
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
    //! Calculates the curvature. Wrapper for RoadGeometry:GetCurvatureLine,
    //! if start and end curvature of the spiral are 0, for RoadGeometry:GetCurvatureArc
    //! if they are equal and to FullCurvature otherwise.
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
    //! Calculates the direction. Wrapper for RoadGeometry:GetDirLine,
    //! if start and end curvature of the spiral are 0, for RoadGeometry:GetDirArc
    //! if they are equal and to FullDir otherwise.
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
#endif // ROADGEOMETRYSPIRALINTERFACE

