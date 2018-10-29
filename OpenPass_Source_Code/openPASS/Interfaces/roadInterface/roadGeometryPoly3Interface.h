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
//! @file  roadGeometryPoly3Interface.h
//! @brief This file contains a interface representing a road form defined via
//!        a cubic polynomial as a RoadGeometry.
//-----------------------------------------------------------------------------

#ifndef ROADGEOMETRYPOLY3INTERFACE
#define ROADGEOMETRYPOLY3INTERFACE

#include "roadInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a road form defined via a cubic polynomial as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometryPoly3Interface : public virtual RoadGeometryInterface
{
public:
    RoadGeometryPoly3Interface() = default;
    RoadGeometryPoly3Interface(const RoadGeometryPoly3Interface&) = delete;
    RoadGeometryPoly3Interface(RoadGeometryPoly3Interface&&) = delete;
    RoadGeometryPoly3Interface& operator=(const RoadGeometryPoly3Interface&) = delete;
    RoadGeometryPoly3Interface& operator=(RoadGeometryPoly3Interface&&) = delete;
    virtual ~RoadGeometryPoly3Interface() = default;

    //-----------------------------------------------------------------------------
    //! Calculates the x/y coordinates as vector. Wrapper for RoadGeometry:GetCoordLine
    //! if all 4 factors (a, b, c, and d) are 0.
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
    //! Calculates the curvature. Wrapper for RoadGeometry:GetCurvatureLine
    //! if all 4 factors (a, b, c, and d) are 0.
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
    //! Calculates the direction. Wrapper for RoadGeometry:GetDirLine if all 4 factors
    //! (a, b, c, and d) are 0.
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

#endif // ROADGEOMETRYPOLY3INTERFACE

