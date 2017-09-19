/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  road.h
//! @brief This file contains the internal representation of a road in a
//!        scenery.
//-----------------------------------------------------------------------------

#ifndef ROAD_H
#define ROAD_H

#ifndef M_PI
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#endif

#include <cassert>
#include <cmath>
#include <algorithm>
#include <list>
#include <map>
#include "vector2d.h"
#include "worldInterface.h"
#include "roadInterface.h"
#include "log.h"

class Road;
class RoadLane;
class RoadLaneSection;

//-----------------------------------------------------------------------------
//! Class representing a link to a road.
//-----------------------------------------------------------------------------
class RoadLink : public RoadLinkInterface
{
public:
    RoadLink(RoadLinkType type,
             RoadLinkElementType elementType,
             const std::string &elementId,
             ContactPointType contactPoint,
             RoadLinkDirectionType direction,
             RoadLinkSideType side) :
        type(type),
        elementType(elementType),
        elementId(elementId),
        contactPoint(contactPoint),
        direction(direction),
        side(side)
    {}
    RoadLink(const RoadLink&) = delete;
    RoadLink(RoadLink&&) = delete;
    RoadLink& operator=(const RoadLink&) = delete;
    RoadLink& operator=(RoadLink&&) = delete;
    virtual ~RoadLink() = default;

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkType of the RoadLink.
    //!
    //! @return                         RoadLinkType of the RoadLink
    //-----------------------------------------------------------------------------
    RoadLinkType GetType() const
    {
        return type;
    }

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkElementType of the RoadLink.
    //!
    //! @return                         RoadLinkElementType of the RoadLink
    //-----------------------------------------------------------------------------
    RoadLinkElementType GetElementType() const
    {
        return elementType;
    }

    //-----------------------------------------------------------------------------
    //! Returns the ID of the RoadLink.
    //!
    //! @return                         ID of the RoadLink
    //-----------------------------------------------------------------------------
    const std::string &GetElementId() const
    {
        return elementId;
    }

    //-----------------------------------------------------------------------------
    //! Returns the ContactPointType of the RoadLink.
    //!
    //! @return                         ContactPointType of the RoadLink
    //-----------------------------------------------------------------------------
    ContactPointType GetContactPoint() const
    {
        return contactPoint;
    }

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkDirectionType of the RoadLink.
    //!
    //! @return                         RoadLinkDirectionType of the RoadLink
    //-----------------------------------------------------------------------------
    RoadLinkDirectionType GetDirection() const
    {
        return direction;
    }

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkSideType of the RoadLink.
    //!
    //! @return                         RoadLinkSideType of the RoadLink
    //-----------------------------------------------------------------------------
    RoadLinkSideType GetSide() const
    {
        return side;
    }

private:
    RoadLinkType type;
    RoadLinkElementType elementType;
    std::string elementId;
    ContactPointType contactPoint;
    RoadLinkDirectionType direction;
    RoadLinkSideType side;
};

//-----------------------------------------------------------------------------
//! Class representing a road lane.
//-----------------------------------------------------------------------------
class RoadLane : public RoadLaneInterface
{
public:
    RoadLane(RoadLaneSectionInterface *laneSection,
             int id,
             RoadLaneTypeType type) :
        laneSection(laneSection),
        id(id),
        type(type)
    {}
    RoadLane(const RoadLane&) = delete;
    RoadLane(RoadLane&&) = delete;
    RoadLane& operator=(const RoadLane&) = delete;
    RoadLane& operator=(RoadLane&&) = delete;
    virtual ~RoadLane();

    //-----------------------------------------------------------------------------
    //! Adds a new polynomial calculating the width of a lane to a road lane.
    //!
    //! @param[in]  sOffset             Offset relative to the preceding lane section
    //! @param[in]  a                   Constant factor from the polynomial
    //! @param[in]  b                   Linear factor from the polynomial
    //! @param[in]  c                   Quadratic factor from the polynomial
    //! @param[in]  d                   Cubic factor from the polynomial
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddWidth(double sOffset,
                  double a,
                  double b,
                  double c,
                  double d);

    //-----------------------------------------------------------------------------
    //! Adds the ID of a successor lane to a road lane.
    //!
    //! @param[in]  id                  ID of a successor lane
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddSuccessor(int id);

    //-----------------------------------------------------------------------------
    //! Adds the ID of a predecessor lane to a road lane.
    //!
    //! @param[in]  id                  ID of a predecessor lane
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddPredecessor(int id);

    //-----------------------------------------------------------------------------
    //! Returns the ID of the road lane.
    //!
    //! @return                         ID of the road lane
    //-----------------------------------------------------------------------------
    int GetId() const
    {
        return id;
    }

    //-----------------------------------------------------------------------------
    //! Returns the RoadLaneTypeType of the road lane.
    //!
    //! @return                         RoadLaneTypeType of the road lane
    //-----------------------------------------------------------------------------
    RoadLaneTypeType GetType() const
    {
        return type;
    }

    //-----------------------------------------------------------------------------
    //! Returns the widths of the road lane.
    //!
    //! @return                         RoadLaneTypeType of the road lane
    //-----------------------------------------------------------------------------
    std::list< RoadLaneWidth*> &GetWidths()
    {
        return widths;
    }

    //-----------------------------------------------------------------------------
    //! Returns the successors of a road lane, can have at most one element.
    //!
    //! @return                         Successors of the road lane
    //-----------------------------------------------------------------------------
    const std::list<int> &GetSuccessor() const
    {
        return successor;
    }

    //-----------------------------------------------------------------------------
    //! Returns the predecessors of a road lane, can have at most one element.
    //!
    //! @return                         Predecessors of the road lane
    //-----------------------------------------------------------------------------
    const std::list<int> &GetPredecessor() const
    {
        return predecessor;
    }

    //-----------------------------------------------------------------------------
    //! Sets the flag, if the road lane is in the reference direction or not.
    //!
    //! @param[in]  inDirection         Flag, if the road lane is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    void SetInDirection(bool inDirection)
    {
        this->inDirection = inDirection;
    }

    //-----------------------------------------------------------------------------
    //! Returns if the road lane is in the reference direction or not.
    //!
    //! @return                         Flag, if the road lane is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    bool GetInDirection() const
    {
        return inDirection;
    }

    //-----------------------------------------------------------------------------
    //! Returns the RoadLaneSection to which the road lane belongs.
    //!
    //! @return                         RoadLaneSection to which the road lane belongs
    //-----------------------------------------------------------------------------
    RoadLaneSectionInterface *GetLaneSection()
    {
        return laneSection;
    }

private:
    RoadLaneSectionInterface *laneSection;
    int id;
    RoadLaneTypeType type;
    // using lists to indicate empty predecessor/successor
    std::list<RoadLaneWidth*> widths;
    std::list<int> predecessor;
    std::list<int> successor;
    bool inDirection = true;
};

//-----------------------------------------------------------------------------
//! Class representing a road lane section that can contain several road lanes.
//-----------------------------------------------------------------------------
class RoadLaneSection : public RoadLaneSectionInterface
{
public:
    RoadLaneSection(RoadInterface *road,
                    double start) :
        road(road),
        start(start)
    {}
    RoadLaneSection(const RoadLaneSection&) = delete;
    RoadLaneSection(RoadLaneSection&&) = delete;
    RoadLaneSection& operator=(const RoadLaneSection&) = delete;
    RoadLaneSection& operator=(RoadLaneSection&&) = delete;
    virtual ~RoadLaneSection();


    //-----------------------------------------------------------------------------
    //! Adds and returns a road lane.
    //!
    //! @param[in]  id                  ID of the lane
    //! @param[in]  type                RoadLaneTypeType of the lane
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    RoadLane *AddRoadLane(int id,
                          RoadLaneTypeType type);

    //-----------------------------------------------------------------------------
    //! Returns the stored road lanes as a mapping from their respective IDs.
    //!
    //! @return                         Stored road lanes as a mapping from their
    //!                                 respective IDs
    //-----------------------------------------------------------------------------
    std::map<int, RoadLaneInterface*> &GetLanes()
    {
        return lanes;
    }

    //-----------------------------------------------------------------------------
    //! Returns the starting offset of the road lane section.
    //!
    //! @return                         Starting offset of the road lane section
    //-----------------------------------------------------------------------------
    double GetStart() const
    {
        return start;
    }

    //-----------------------------------------------------------------------------
    //! Sets the flag, if the lane section is in the reference direction or not.
    //!
    //! @param[in]  inDirection         Flag, if the road lane is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    void SetInDirection(bool inDirection)
    {
        this->inDirection = inDirection;
    }

    //-----------------------------------------------------------------------------
    //! Sets the index offset for the road lanes in this section.
    //!
    //! @param[in]  laneIndexOffset     Index offset for the road lanes in this section
    //-----------------------------------------------------------------------------
    void SetLaneIndexOffset(int laneIndexOffset)
    {
        this->laneIndexOffset = laneIndexOffset;
    }

    //-----------------------------------------------------------------------------
    //! Sets the ID.
    //!
    //! @param[in]  Id             ID
    //-----------------------------------------------------------------------------
    void SetId(int Id)
    {
        this->Id = Id;
    }

    //-----------------------------------------------------------------------------
    //! Returns if the lane section is in the reference direction or not.
    //!
    //! @return                         Flag, if the lane section is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    bool GetInDirection() const
    {
        return inDirection;
    }

    //-----------------------------------------------------------------------------
    //! Returns the index offset for the road lanes in this section.
    //!
    //! @return                         Index offset for the road lanes in this section
    //-----------------------------------------------------------------------------
    int GetLaneIndexOffset() const
    {
        return laneIndexOffset;
    }

    //-----------------------------------------------------------------------------
    //! Returns the ID.
    //!
    //! @return                         ID
    //-----------------------------------------------------------------------------
    int GetId() const
    {
        return Id;
    }

    //-----------------------------------------------------------------------------
    //! Returns the road from which this section is a part of.
    //!
    //! @return                         Road from which this section is a part of
    //-----------------------------------------------------------------------------
    RoadInterface *GetRoad()
    {
        return road;
    }

private:
    RoadInterface *road;
    const double start;
    std::map<int, RoadLaneInterface*> lanes; // use map for sorted ids
    bool inDirection = true;
    int laneIndexOffset = 0;
    int Id;
};

//-----------------------------------------------------------------------------
//! Abstract class representing the geometry of a road, i.e. the layout of the
//! road's reference line in the in the x/y plane.
//-----------------------------------------------------------------------------
class RoadGeometry : public RoadGeometryInterface
{
public:
    RoadGeometry(double s,
                 double x,
                 double y,
                 double hdg,
                 double length) :
        s(s),
        x(x),
        y(y),
        hdg(hdg),
        length(length)
    {}
    RoadGeometry(const RoadGeometry&) = delete;
    RoadGeometry(RoadGeometry&&) = delete;
    RoadGeometry& operator=(const RoadGeometry&) = delete;
    RoadGeometry& operator=(RoadGeometry&&) = delete;
    virtual ~RoadGeometry() = default;

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
    double GetS() const
    {
        return s;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the initial direction of the geometry section
    //!
    //! @return                         initial direction of geometry
    //-----------------------------------------------------------------------------
    double GetHdg() const
    {
        return hdg;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the length of the geometry section
    //!
    //! @return                         length of geometry section
    //-----------------------------------------------------------------------------
    double GetLength() const
    {
        return length;
    }

protected:
    // shared functionality

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

//-----------------------------------------------------------------------------
//! Class representing a road line as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometryLine : public RoadGeometry
{
public:
    RoadGeometryLine(double s,
                     double x,
                     double y,
                     double hdg,
                     double length) :
        RoadGeometry(s, x, y, hdg, length)
    {}
    RoadGeometryLine(const RoadGeometryLine&) = delete;
    RoadGeometryLine(RoadGeometryLine&&) = delete;
    RoadGeometryLine& operator=(const RoadGeometryLine&) = delete;
    RoadGeometryLine& operator=(RoadGeometryLine&&) = delete;
    virtual ~RoadGeometryLine() = default;

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

//-----------------------------------------------------------------------------
//! Class representing a road arc as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometryArc : public RoadGeometry
{
public:
    RoadGeometryArc(double s,
                    double x,
                    double y,
                    double hdg,
                    double length,
                    double curvature) :
        RoadGeometry(s, x, y, hdg, length),
        curvature(curvature)
    {}
    RoadGeometryArc(const RoadGeometryArc&) = delete;
    RoadGeometryArc(RoadGeometryArc&&) = delete;
    RoadGeometryArc& operator=(const RoadGeometryArc&) = delete;
    RoadGeometryArc& operator=(RoadGeometryArc&&) = delete;
    virtual ~RoadGeometryArc() = default;

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

    //-----------------------------------------------------------------------------
    //! Returns the stored curvature.
    //!
    //! @return                         curvature
    //-----------------------------------------------------------------------------
    double GetCurvature() const
    {
        return curvature;
    }

private:
    double curvature;
};

//-----------------------------------------------------------------------------
//! Class representing a spiral road (clothoid) as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometrySpiral : public RoadGeometry
{
public:
    RoadGeometrySpiral(double s,
                       double x,
                       double y,
                       double hdg,
                       double length,
                       double curvStart,
                       double curvEnd) :
        RoadGeometry(s, x, y, hdg, length),
        curvStart(curvStart),
        curvEnd(curvEnd)
    {}
    RoadGeometrySpiral(const RoadGeometrySpiral&) = delete;
    RoadGeometrySpiral(RoadGeometrySpiral&&) = delete;
    RoadGeometrySpiral& operator=(const RoadGeometrySpiral&) = delete;
    RoadGeometrySpiral& operator=(RoadGeometrySpiral&&) = delete;
    virtual ~RoadGeometrySpiral() = default;


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

    //-----------------------------------------------------------------------------
    //! Returns the curvature at the start of the spiral.
    //!
    //! @return                         curvature at the start of the spiral
    //-----------------------------------------------------------------------------
    double GetCurvStart() const
    {
        return curvStart;
    }

    //-----------------------------------------------------------------------------
    //! Returns the curvature at the end of the spiral.
    //!
    //! @return                         curvature at the end of the spiral
    //-----------------------------------------------------------------------------
    double GetCurvEnd() const
    {
        return curvEnd;
    }

private:

    //-----------------------------------------------------------------------------
    //! Calculates the x/y coordinates as vector. Only valid if the start and end
    //! curvature of the spiral are either both positive or negative.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @param[in]  corner              position in lane (1: left, 0: middle, -1: right)
    //! @return                         vector with the x/y coordinates
    //-----------------------------------------------------------------------------
    Common::Vector2d HalfCoord(double side,
                               double geometryOffset,
                               double previousWidth,
                               double laneOffset,
                               double laneWidth,
                               int corner);

    //-----------------------------------------------------------------------------
    //! Calculates the x/y coordinates as vector. Returns an empty vector, if start
    //! and end curvature of the spiral have different signs.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @param[in]  corner              position in lane (1: left, 0: middle, -1: right)
    //! @return                         vector with the x/y coordinates, empty vector,
    //!                                 if start and end curvature of the spiral have
    //!                                 different signs
    //-----------------------------------------------------------------------------
    Common::Vector2d FullCoord(double side,
                               double geometryOffset,
                               double previousWidth,
                               double laneOffset,
                               double laneWidth,
                               int corner);

    //-----------------------------------------------------------------------------
    //! Calculates the curvature. Only valid if the start and end
    //! curvature of the spiral are either both positive or negative.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @return                         curvature
    //-----------------------------------------------------------------------------
    double HalfCurvature(double side,
                         double geometryOffset,
                         double previousWidth,
                         double laneOffset,
                         double laneWidth);

    //-----------------------------------------------------------------------------
    //! Calculates the curvature. Returns 0.0, if start
    //! and end curvature of the spiral have different signs.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @return                         curvature, 0.0, if start and end curvature
    //!                                 of the spiral have different signs
    //-----------------------------------------------------------------------------
    double FullCurvature(double side,
                         double geometryOffset,
                         double previousWidth,
                         double laneOffset,
                         double laneWidth);

    //-----------------------------------------------------------------------------
    //! Calculates the direction. Only valid if the start and end
    //! curvature of the spiral are either both positive or negative.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @return                         direction
    //-----------------------------------------------------------------------------
    double HalfDir(double side,
                   double geometryOffset,
                   double previousWidth,
                   double laneOffset,
                   double laneWidth);

    //-----------------------------------------------------------------------------
    //! Calculates the direction. Returns 0.0, if start
    //! and end curvature of the spiral have different signs.
    //!
    //! @param[in]  side                side of road (1: left, -1: right); unused
    //! @param[in]  geometryOffset      offset within geometry section
    //! @param[in]  previousWidth       sum of widths of inner lanes; unused
    //! @param[in]  laneOffset          lane offset; unused
    //! @param[in]  laneWidth           width of lane; unused
    //! @return                         direction
    //-----------------------------------------------------------------------------
    double FullDir(double side,
                   double geometryOffset,
                   double previousWidth,
                   double laneOffset,
                   double laneWidth);

    double curvStart;
    double curvEnd;
};

//-----------------------------------------------------------------------------
//! Class representing a road form defined via a cubic polynomial as a RoadGeometry.
//-----------------------------------------------------------------------------
class RoadGeometryPoly3 : public RoadGeometry
{
public:
    RoadGeometryPoly3(double s,
                      double x,
                      double y,
                      double hdg,
                      double length,
                      double a,
                      double b,
                      double c,
                      double d) :
        RoadGeometry(s, x, y, hdg, length),
        a(a),
        b(b),
        c(c),
        d(d)
    {}
    RoadGeometryPoly3(const RoadGeometryPoly3&) = delete;
    RoadGeometryPoly3(RoadGeometryPoly3&&) = delete;
    RoadGeometryPoly3& operator=(const RoadGeometryPoly3&) = delete;
    RoadGeometryPoly3& operator=(RoadGeometryPoly3&&) = delete;
    virtual ~RoadGeometryPoly3() = default;

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

    //-----------------------------------------------------------------------------
    //! Returns the constant factor of the polynomial.
    //!
    //! @return                         constant factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetA() const
    {
        return a;
    }

    //-----------------------------------------------------------------------------
    //! Returns the linear factor of the polynomial.
    //!
    //! @return                         linear factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetB() const
    {
        return b;
    }

    //-----------------------------------------------------------------------------
    //! Returns the quadratic factor of the polynomial.
    //!
    //! @return                         quadratic factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetC() const
    {
        return c;
    }

    //-----------------------------------------------------------------------------
    //! Returns the cubic factor of the polynomial.
    //!
    //! @return                         cubic factor of the polynomial
    //-----------------------------------------------------------------------------
    double GetD() const
    {
        return d;
    }

private:
    double a;
    double b;
    double c;
    double d;
};

//-----------------------------------------------------------------------------
//! Class representing a road.
//-----------------------------------------------------------------------------
class Road :public RoadInterface
{
public:
    Road(const std::string &id) :
        id(id)
    {}
    Road(const Road&) = delete;
    Road(Road&&) = delete;
    Road& operator=(const Road&) = delete;
    Road& operator=(Road&&) = delete;
    virtual ~Road();

    //-----------------------------------------------------------------------------
    //! Adds a line geometry to a road by creating a new RoadGeometryLine object and
    //! adding it to the stored list of geometries.
    //!
    //! @param[in]  s                   start position s-coordinate
    //! @param[in]  x                   start position x inertial
    //! @param[in]  y                   start position y inertial
    //! @param[in]  hdg                 start orientation (inertial heading)
    //! @param[in]  length              length of the element's reference line
    //! @return                         false if an error has occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddGeometryLine(double s,
                         double x,
                         double y,
                         double hdg,
                         double length);

    //-----------------------------------------------------------------------------
    //! Adds an arc geometry to a road by creating a new RoadGeometryArc object and
    //! adding it to the stored list of geometries.
    //!
    //! @param[in]  s                   start position s-coordinate
    //! @param[in]  x                   start position x inertial
    //! @param[in]  y                   start position y inertial
    //! @param[in]  hdg                 start orientation (inertial heading)
    //! @param[in]  length              length of the element's reference line
    //! @param[in]  curvature           constant curvature throughout the element
    //! @return                         false if an error has occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddGeometryArc(double s,
                        double x,
                        double y,
                        double hdg,
                        double length,
                        double curvature);

    //-----------------------------------------------------------------------------
    //! Adds a spiral geometry to a road by creating a new RoadGeometrySpiral object and
    //! adding it to the stored list of geometries.
    //!
    //! @param[in]  s                   start position s-coordinate
    //! @param[in]  x                   start position x inertial
    //! @param[in]  y                   start position y inertial
    //! @param[in]  hdg                 start orientation (inertial heading)
    //! @param[in]  length              length of the element's reference line
    //! @param[in]  curvStart           curvature at the start of the element
    //! @param[in]  curvEnd             curvature at the end of the element
    //! @return                         false if an error has occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddGeometrySpiral(double s,
                           double x,
                           double y,
                           double hdg,
                           double length,
                           double curvStart,
                           double curvEnd);

    //-----------------------------------------------------------------------------
    //! Adds a cubic polynomial geometry to a road by creating a new RoadGeometryPoly3
    //! object and adding it to the stored list of geometries.
    //!
    //! @param[in]  s                   start position s-coordinate
    //! @param[in]  x                   start position x inertial
    //! @param[in]  y                   start position y inertial
    //! @param[in]  hdg                 start orientation (inertial heading)
    //! @param[in]  length              length of the element's reference line
    //! @param[in]  a                   constant factor of the polynomial
    //! @param[in]  b                   linear factor of the polynomial
    //! @param[in]  c                   quadratic factor of the polynomial
    //! @param[in]  d                   cubic factor of the polynomial
    //! @return                         false if an error has occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddGeometryPoly3(double s,
                          double x,
                          double y,
                          double hdg,
                          double length,
                          double a,
                          double b,
                          double c,
                          double d);

    //-----------------------------------------------------------------------------
    //! Adds an elevation profile defined via a cubic polynomial to a road by creating
    //! a new RoadElevation object and adding it to the stored list of elevations.
    //!
    //! @param[in]  s                   start position s-coordinate
    //! @param[in]  a                   constant factor of the polynomial
    //! @param[in]  b                   linear factor of the polynomial
    //! @param[in]  c                   quadratic factor of the polynomial
    //! @param[in]  d                   cubic factor of the polynomial
    //! @return                         false if an error has occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddElevation(double s,
                      double a,
                      double b,
                      double c,
                      double d);

    //-----------------------------------------------------------------------------
    //! Adds a lane offset defined via a cubic polynomial to a road by creating a new
    //! RoadLaneOffset object and adding it to the stored list of lane offsets.
    //!
    //! @param[in]  s                   start position s-coordinate
    //! @param[in]  a                   constant factor of the polynomial
    //! @param[in]  b                   linear factor of the polynomial
    //! @param[in]  c                   quadratic factor of the polynomial
    //! @param[in]  d                   cubic factor of the polynomial
    //! @return                         false if an error has occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddLaneOffset(double s,
                       double a,
                       double b,
                       double c,
                       double d);

    //-----------------------------------------------------------------------------
    //! Adds a new link to a road by creating a new RoadLink object and adding it
    //! to the stored list of links.
    //!
    //! @param[in]  type                type of link
    //! @param[in]  elementType         element type of the link
    //! @param[in]  elementId           ID of the linked element
    //! @param[in]  contactPoint        contact point type of link on the linked element
    //! @param[in]  direction           orientation of the lanes to link; only relevant
    //!                                 for a neighbor link type
    //! @param[in]  side                at which side the neighboring road lies; only
    //!                                 relevant for a neighbor link type
    //! @return                         false if an error has occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool AddLink(RoadLinkType type,
                 RoadLinkElementType elementType,
                 const std::string &elementId,
                 ContactPointType contactPoint,
                 RoadLinkDirectionType direction,
                 RoadLinkSideType side);

    //-----------------------------------------------------------------------------
    //! Adds a new lane section to a road by creating a new RoadLaneSection object
    //! and adding it to the stored list of lane sections.
    //!
    //! @param[in]  start               start position s-coordinate
    //! @return                         created road lane section
    //-----------------------------------------------------------------------------
    RoadLaneSection *AddRoadLaneSection(double start);

    //-----------------------------------------------------------------------------
    //! Returns the ID of the road.
    //!
    //! @return                         ID of the road
    //-----------------------------------------------------------------------------
    const std::string &GetId() const
    {
        return id;
    }

    //-----------------------------------------------------------------------------
    //! Returns the stored list of elevation profiles.
    //!
    //! @return                         list of elevation profiles
    //-----------------------------------------------------------------------------
    std::list<RoadElevation*> &GetElevations()
    {
        return elevations;
    }

    //-----------------------------------------------------------------------------
    //! Returns the stored list of lane offsets.
    //!
    //! @return                         list of lane offsets
    //-----------------------------------------------------------------------------
    std::list<RoadLaneOffset*> &GetLaneOffsets()
    {
        return laneOffsets;
    }

    //-----------------------------------------------------------------------------
    //! Returns the stored list of road geometries.
    //!
    //! @return                         list of road geometries
    //-----------------------------------------------------------------------------
    std::list<RoadGeometryInterface*> &GetGeometries()
    {
        return geometries;
    }

    //-----------------------------------------------------------------------------
    //! Returns the stored list of road links.
    //!
    //! @return                         list of road links
    //-----------------------------------------------------------------------------
    std::list<RoadLinkInterface*> &GetRoadLinks()
    {
        return links;
    }

    //-----------------------------------------------------------------------------
    //! Returns the stored list of lane sections.
    //!
    //! @return                         list of lane sections
    //-----------------------------------------------------------------------------
    std::list<RoadLaneSectionInterface*> &GetLaneSections()
    {
        return laneSections;
    }

    //-----------------------------------------------------------------------------
    //! Sets the flag, if the road is in the reference direction or not.
    //!
    //! @param[in]  inDirection         Flag, if the road is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    void SetInDirection(bool inDirection)
    {
        this->inDirection = inDirection;
    }

    //-----------------------------------------------------------------------------
    //! Returns if the road is in the reference direction or not.
    //!
    //! @return                         Flag, if the road is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    bool GetInDirection() const
    {
        return inDirection;
    }

private:
    const std::string id;
    std::list<RoadElevation*> elevations;
    std::list<RoadLaneOffset*> laneOffsets;
    std::list<RoadGeometryInterface*> geometries;
    std::list<RoadLinkInterface*> links;
    std::list<RoadLaneSectionInterface*> laneSections;
    bool inDirection = true;
};

#endif // ROAD_H
