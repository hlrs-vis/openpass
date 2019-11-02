/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  GeometryConverter.h
//! @brief This file contains the converter of the scenery geometry.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include <list>
#include "Interfaces/sceneryInterface.h"
#include "WorldData.h"
#include "Interfaces/worldInterface.h"

//-----------------------------------------------------------------------------
//! Class for the convertion of the road geometries in a section. First, the roads,
//! lane sections and lanes have to be converted using SceneryConverter, which then
//! provides the mappings from OpenDrive to OSI of those elements.
//-----------------------------------------------------------------------------
class GeometryConverter
{
public:
    GeometryConverter(SceneryInterface *scenery,
                      OWL::Interfaces::WorldData& worldData,
                      const CallbackInterface *callbacks);

    GeometryConverter(const GeometryConverter&) = delete;
    GeometryConverter(GeometryConverter&&) = delete;
    GeometryConverter& operator=(const GeometryConverter&) = delete;
    GeometryConverter& operator=(GeometryConverter&&) = delete;
    virtual ~GeometryConverter() = default;

    //-----------------------------------------------------------------------------
    //! Converts OpenDrive road geometries to OSI
    //!
    //! Subdivides each OpenDrive road into OSI section objects.
    //! Each section corresponds to an OpenDrive geometry (limited
    //! to the borders of an OpenDrive laneSection.
    //!
    //! Notes:
    //! - OpenDrive center lanes are skipped (width=0 by convention)
    //!
    //! @param[in]  scenery        Scenery with the OpenDrive roads
    //! @return                    False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool Convert();

protected:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    //-----------------------------------------------------------------------------
    //! Calculates height coordinate according to OpenDrive elevation profiles.
    //!
    //! @param[in]  road           OpenDrive road data structure
    //! @param[in]  offset         Absolute offset on reference line within road
    //! @return                    height coordinate
    //-----------------------------------------------------------------------------
    double CalculateCoordZ(RoadInterface *road, double offset);

    //-----------------------------------------------------------------------------
    //! Calculates slope according to OpenDrive elevation profiles.
    //!
    //! @param[in]  road           OpenDrive road data structure
    //! @param[in]  offset         Absolute offset on reference line within road
    //! @return                    slope
    //-----------------------------------------------------------------------------
    double CalculateSlope(RoadInterface *road, double offset);

    //-----------------------------------------------------------------------------
    //! Iterates over all OpenDrive Roads to calculate Geometries.
    //!
    //! This function is a part of the Convert function.
    //! Iterates over all roads to calculate Geometries
    //!
    //! Notes:
    //! - OpenDrive center lanes are skipped (width=0 by convention)
    //!
    //! @return                    False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool CalculateRoads();

    //-----------------------------------------------------------------------------
    //! Iterates over all geometries to calculate the geometry.
    //!
    //! This function is a part of the Convert function.
    //! Iterates over all geometries to calculate geometries and lane lengths.
    //!
    //! @param[in]  roadSectionStart       s coordinate of current roadSectionStart
    //! @param[in]  roadSectionNextStart   s coordinate of next roadSectionStart
    //! @param[in]  road                   Pointer containing the road
    //! @param[in]  roadLanes              Map of lanes per Road
    //! @return                     False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool CalculateGeometries(double roadSectionStart,
                             double roadSectionNextStart,
                             RoadInterface *road,
                             std::map<int, RoadLaneInterface*> &roadLanes);

    //-----------------------------------------------------------------------------
    //! Calculates points if section is affected by geometry.
    //!
    //! This function is a part of the Convert function.
    //! Calculates points if section is affected by geometry.
    //!
    //! @param[in]  roadSectionStart       s coordinate of current roadSectionStart
    //! @param[in]  roadSectionNextStart   s coordinate of next roadSectionStart
    //! @param[in]  road                   Pointer containing the road
    //! @param[in]  roadLanes              Map of lanes per Road
    //! @param[in]  roadGeometry           Pointer containing the roadGeometry
    //! @param[out] roadGeometryStart      s coordinate of roadGeometryStart
    //! @param[out] roadGeometryEnd        s coordinate of roadGeometryEnd
    //! @return                    False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool CalculateGeometry(double roadSectionStart,
                           double roadSectionNextStart,
                           RoadInterface *road,
                           std::map<int, RoadLaneInterface*> &roadLanes,
                           RoadGeometryInterface *roadGeometry,
                           double &roadGeometryStart,
                           double &roadGeometryEnd);

    //-----------------------------------------------------------------------------
    //! Allocates lane geometry and calculates points.
    //!
    //! This function is a part of the Convert function.
    //! Calculates the geometry and section start and end points.
    //! Then allocates lane geometry and calculates geometry points.
    //!
    //! @param[in]  roadSectionStart       s coordinate of current roadSectionStart
    //! @param[in]  roadGeometryStart      s coordinate of roadGeometryStart
    //! @param[in]  roadSectionNextStart   s coordinate of next roadSectionStart
    //! @param[in]  roadGeometryEnd        s coordinate of roadGeometryEnd
    //! @param[in]  roadGeometryLength     Length of the roadGeometry
    //! @param[in]  roadLanes              Map of lanes per Road
    //! @param[in]  road                   Pointer containing the road
    //! @param[in]  roadGeometry           Pointer containing the roadGeometry
    //! @return                    False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool CalculatePointsOfAffectedGeometry(double roadSectionStart,
                                           double roadGeometryStart,
                                           double roadSectionNextStart,
                                           double roadGeometryEnd,
                                           double roadGeometryLength,
                                           std::map<int, RoadLaneInterface *> &roadLanes,
                                           RoadInterface *road,
                                           RoadGeometryInterface *roadGeometry);

    //-----------------------------------------------------------------------------
    //! Calculates geometryOffSetStart and SectionOffsetStart.
    //!
    //! This function is a part of the Convert function.
    //! Calculates geometryOffSetStart and SectionOffsetStart.
    //!
    //! @param[in]  roadSectionStart       s coordinate of current roadSectionStart
    //! @param[in]  roadGeometryStart      s coordinate of roadGeometryStart
    //!
    //! @param[out] geometryOffsetStart    Offset to the start point of the geometry
    //! @param[out] sectionOffsetStart     Offset to the start point of the section
    //-----------------------------------------------------------------------------
    void CalculateGeometryOffsetStart(double roadSectionStart,
                                      double roadGeometryStart,
                                      double* geometryOffsetStart,
                                      double* sectionOffsetStart);

    //-----------------------------------------------------------------------------
    //! Calculates geometryOffSetEnd and SectionOffsetEnd.
    //!
    //! This function is a part of the Convert function.
    //! Calculates geometryOffSetEnd and SectionOffsetEnd.
    //!
    //! @param[in]  roadSectionStart       s coordinate of current roadSectionStart
    //! @param[in]  roadSectionNextStart   s coordinate of next roadSectionStart
    //! @param[in]  roadGeometryStart      s coordinate of roadGeometryStart
    //! @param[in]  roadGeometryEnd        s coordinate of roadGeometryEnd
    //! @param[in]  roadGeometryLength     Length of the roadGeometry
    //!
    //! @param[out] geometryOffsetEnd      Offset to the end point of the geometry
    //! @param[out] sectionOffsetEnd       Offset to the end point of the section
    //-----------------------------------------------------------------------------
    void CalculateGeometryOffsetEnd(double roadSectionStart,
                                    double roadSectionNextStart,
                                    double roadGeometryStart,
                                    double roadGeometryEnd,
                                    double roadGeometryLength,
                                    double* geometryOffsetEnd,
                                    double* sectionOffsetEnd);

    //-----------------------------------------------------------------------------
    //! Calculates points for left and right lanes.
    //!
    //! This function is a part of the Convert function.
    //! Calculates points for left and right lanes.
    //!
    //! @param[in]  geometryOffsetStart    Offset to the start point of the geometry
    //! @param[in]  geometryOffsetEnd      Offset to the end point of the geometry
    //! @param[in]  numberLaneGeomPoints   Number of geometry point in the lane
    //! @param[in]  roadLanes              Map of lanes per Road
    //! @param[in]  road                   Pointer containing the road
    //! @param[in]  roadGeometry           Pointer containing the roadGeometry
    //! @param[in]  sectionOffsetStart     Offset to the start point of the section
    //! @param[out] sectionOffsetEnd       Offset to the end point of the section
    //! @param[in]  roadGeometryStart      s coordinate of roadGeometryStart
    //! @param[in]  roadSectionStart       s coordinate of current roadSectionStart
    //-----------------------------------------------------------------------------
    bool CalculatePoints(double geometryOffsetStart,
                         double geometryOffsetEnd,
                         int numberLaneGeomPoints,
                         std::map<int, RoadLaneInterface*> &roadLanes,
                         RoadInterface *road,
                         RoadGeometryInterface *roadGeometry,
                         double sectionOffsetStart,
                         double sectionOffsetEnd,
                         double roadGeometryStart,
                         double roadSectionStart);


    //-----------------------------------------------------------------------------
    //! Fills OSI lanes according to OpenDrive geometry.
    //!
    //! @param[in]  side                 Side of road (1: left, -1: right)
    //! @param[in]  lanes                OpenDrive road lanes
    //! @param[in]  road                 OpenDrive road data structure
    //! @param[in]  roadGeometry         OpenDrive geometric information
    //! @param[in]  geometryOffset       Offset within geometry
    //! @param[in]  sectionOffsetStart   Start offset within lane
    //! @param[in]  sectionOffsetEnd     End offset within lane
    //! @param[in]  roadOffset           Absolute offset within lane
    //! @param[in]  roadSectionStart     Start offset of lane section within road
    //! @param[in]  index                Index within lane memory
    //! @return                          True
    //-----------------------------------------------------------------------------
    bool CalculateLanes(double side,
                        std::map<int, RoadLaneInterface *> &roadLanes,
                        RoadInterface *road,
                        RoadGeometryInterface *roadGeometry,
                        double geometryOffset,
                        double sectionOffsetStart,
                        double sectionOffsetEnd,
                        double roadOffset,
                        double roadSectionStart,
                        int index);


    //-----------------------------------------------------------------------------
    //! Calculates the width of the provided lane.
    //!
    //! @param[in]  roadLane            OpenDrive road lane (for width information)
    //! @param[in]  sectionOffset       Offset within the OpenDrive section
    //! @return                         Lane width, 0.0 if no width was specified
    //-----------------------------------------------------------------------------
    double CalculateLaneWidth(const RoadLaneInterface* roadLane, double sectionOffset);

    double CalculateLaneOffset(RoadInterface* road, double roadPosition);

    //-----------------------------------------------------------------------------
    //! Calculates the slope of a lane at a given position
    //!
    //! @param[in] elevation        elevation input data (has to match given position)
    //! @param[in] position         position w.r.t. start of section
    //! @return                     width at given position
    //-----------------------------------------------------------------------------
    double CalculateSlopeAtRoadPosition(const RoadElevation* elevation, double position);

    double CalculateOffsetAtRoadPosition(const RoadLaneOffset* roadOffset, double position);

    //-----------------------------------------------------------------------------
    //! Calculates the width of a lane at a given position
    //!
    //! @param[in] width            width input data (has to match given position)
    //! @param[in] position         position w.r.t. start of section
    //! @return                     width at given position
    //-----------------------------------------------------------------------------
    double CalculateWidthAtSectionPosition(const RoadLaneWidth* width, double position);

    //-----------------------------------------------------------------------------
    //! Get the RoadElevation which is relevant for the given position
    //!
    //! @param[in] sectionOffset    position w.r.t. start of section
    //! @param[in] roadLane         the RoadLaneInterface input data
    //! @return                     relevant RoadElevation
    //-----------------------------------------------------------------------------
    const RoadElevation* GetRelevantRoadElevation(double sectionOffset, RoadInterface *road);

    //-----------------------------------------------------------------------------
    //! Get the RoadLaneWidth which is relevant for the given position
    //!
    //! @param[in] sectionOffset    position w.r.t. start of section
    //! @param[in] roadLane         the RoadLaneInterface input data
    //! @return                     relevant RoadLaneWidth
    //-----------------------------------------------------------------------------
    const RoadLaneWidth* GetRelevantRoadLaneWidth(double sectionOffset, const RoadLaneInterface* roadLane);

    const RoadLaneOffset* GetRelevantRoadLaneOffset(double roadOffset, RoadInterface* road);

    const RoadLaneRoadMark* GetRelevantRoadLaneRoadMark(double sectionOffset, const RoadLaneInterface* roadLane);

    //-----------------------------------------------------------------------------
    //! Tests if the provided values' difference is smaller than EPS
    //!
    //! @param[in] valueA           input value A
    //! @param[in] valueB           input value B
    //! @return                     true, if valueA and valueB are (approximately) the same, false otherwise
    //-----------------------------------------------------------------------------
    bool IsEqual(const double valueA, const double valueB);

    //-----------------------------------------------------------------------------
    //! Convert OpenDRIVE road mark types to OSI lane boundary types
    //!
    //! @param[in] type                 OpenDRIVE road mark type
    //! @return                         OSI lane boundary type
    //-----------------------------------------------------------------------------
    osi3::LaneBoundary::Classification::Type ConvertRoadLaneRoadMarkToOsiLaneBoundaryType(RoadLaneRoadMarkType type);

    //-----------------------------------------------------------------------------
    //! Convert OpenDRIVE road mark colors to OSI lane boundary colors
    //!
    //! @param[in] color                OpenDRIVE road mark color
    //! @return                         OSI lane boundary color
    //-----------------------------------------------------------------------------
    osi3::LaneBoundary::Classification::Color ConvertRoadLaneRoadMarkColorToOsiLaneBoundaryColor(RoadLaneRoadMarkColor color);


    SceneryInterface *scenery;

    OWL::Interfaces::WorldData& worldData;

    constexpr static const double SAMPLING_RATE = 3.0; // 1m sampling rate of reference line
    constexpr static const double EPS = 1e-3;   // epsilon value for geometric comparisons

    const CallbackInterface *callbacks;
};

