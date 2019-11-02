/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadGeometryInterface.h
//! @brief This file contains a abstract class representing the geometry of a
//!        road, i.e. the layout of the road's reference line in the in the x/y
//!        plane.
//-----------------------------------------------------------------------------

#ifndef ROADGEOMETRYINTERFACE
#define ROADGEOMETRYINTERFACE

#include "Common/vector2d.h"
#include "Interfaces/roadInterface/roadInterface.h"

//-----------------------------------------------------------------------------
//! Abstract class representing the geometry of a road, i.e. the layout of the
//! road's reference line in the in the x/y plane.
//-----------------------------------------------------------------------------
class RoadGeometryInterface
{
public:
    RoadGeometryInterface() = default;
    RoadGeometryInterface(const RoadGeometryInterface&) = delete;
    RoadGeometryInterface(RoadGeometryInterface&&) = delete;
    RoadGeometryInterface& operator=(const RoadGeometryInterface&) = delete;
    RoadGeometryInterface& operator=(RoadGeometryInterface&&) = delete;
    virtual ~RoadGeometryInterface() = default;

    //-----------------------------------------------------------------------------
    //! Calculates the x/y coordinates as vector.
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
                                      int corner) = 0;

    //-----------------------------------------------------------------------------
    //! Calculates the curvature.
    //!
    //! @param[in]  side                side of road (1: left, -1, right)
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
                                double laneWidth) = 0;

    //-----------------------------------------------------------------------------
    //! Calculates the direction.
    //!
    //! @param[in]  side                side of road (1: left, -1, right)
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes
    //! @param[in]  laneOffset          lane offset
    //! @param[in]  laneWidth           width of lane
    //! @return                         direction
    //-----------------------------------------------------------------------------
    virtual double GetDir(double side,
                          double geometryOffset,
                          double previousWidth,
                          double laneOffset,
                          double laneWidth) = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the offset within the OpenDrive road.
    //!
    //! @return                         offset
    //-----------------------------------------------------------------------------
    virtual double GetS() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the initial direction of the geometry section
    //!
    //! @return                         initial direction of geometry
    //-----------------------------------------------------------------------------
    virtual double GetHdg() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the length of the geometry section
    //!
    //! @return                         length of geometry section
    //-----------------------------------------------------------------------------
    virtual double GetLength() const = 0;

    //-----------------------------------------------------------------------------
    //! Gets a vector representing a linear road in a x/y coordinate system.
    //!
    //! @param[in]  side                side of road (1: left, -1: right)
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes
    //! @param[in]  laneOffset          lane offset
    //! @param[in]  laneWidth           width of lane
    //! @param[in]  corner              position in lane (1: left, 0: middle, -1: right)
    //! @return                         vector with the x/y coordinates
    //-----------------------------------------------------------------------------
    Common::Vector2d GetCoordLine(double side,
                                  double geometryOffset,
                                  double previousWidth,
                                  double laneOffset,
                                  double laneWidth,
                                  int corner);

    //-----------------------------------------------------------------------------
    //! Returns the curvature of a line, i.e. 0.0.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @return                         0.0
    //-----------------------------------------------------------------------------
    double GetCurvatureLine(double side,
                            double geometryOffset,
                            double previousWidth,
                            double laneOffset,
                            double laneWidth);

    //-----------------------------------------------------------------------------
    //! Returns the direction of the line, i.e. the start orientation.
    //!
    //! @param[in]  geometryOffset      offset within geometry section; unused
    //! @return                         direction
    //-----------------------------------------------------------------------------
    double GetDirLine(double geometryOffset);

    //-----------------------------------------------------------------------------
    //! Gets a vector representing an arc in a polar coordinate system.
    //!
    //! @param[in]  side                side of road (1: left, -1: right)
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes
    //! @param[in]  laneOffset          lane offset
    //! @param[in]  laneWidth           width of lane
    //! @param[in]  curvature           curvature of the arc
    //! @param[in]  corner              position in lane (1: left, 0: middle, -1: right)
    //! @return                         vector with the x/y coordinates
    //-----------------------------------------------------------------------------
    Common::Vector2d GetCoordArc(double side,
                                 double geometryOffset,
                                 double previousWidth,
                                 double laneOffset,
                                 double laneWidth,
                                 double curvature,
                                 int corner);

    //-----------------------------------------------------------------------------
    //! Returns the curvature of an arc.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section; unused
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @param[in]  curvature           curvature of the arc
    //! @return                         curvature
    //-----------------------------------------------------------------------------
    double GetCurvatureArc(double side,
                           double geometryOffset,
                           double previousWidth,
                           double laneOffset,
                           double laneWidth,
                           double curvature);

    //-----------------------------------------------------------------------------
    //! Returns the direction of an arc, i.e. the initial heading plus the fraction
    //! of the radius due to the curvature.
    //!
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  curvature           curvature of the arc
    //! @return                         direction of the arc
    //-----------------------------------------------------------------------------
    double GetDirArc(double geometryOffset,
                     double curvature);

    double s;
    double x;
    double y;
    double hdg;
    double length;
};

#endif // ROADGEOMETRYINTERFACE

