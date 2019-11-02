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

#include <algorithm>
#include <utility>
#include <limits>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <QFile>

#include "GeometryConverter.h"
#include "Common/vector2d.h"
#include "WorldData.h"

GeometryConverter::GeometryConverter(SceneryInterface *scenery,
                                     OWL::Interfaces::WorldData& worldData,
                                     const CallbackInterface *callbacks) :
    scenery(scenery),
    worldData(worldData),
    callbacks(callbacks)
{}

double GeometryConverter::CalculateCoordZ(RoadInterface *road, double offset)
{
    double coordZ = 0.0;
    const RoadElevation* roadElevation = GetRelevantRoadElevation(offset, road);

    if(roadElevation)
    {
        double ds = offset - roadElevation->GetS();

        coordZ = roadElevation->GetA() +
                 roadElevation->GetB() * ds +
                 roadElevation->GetC() * ds * ds +
                 roadElevation->GetD() * ds * ds * ds;
    }

    return coordZ;
}

double GeometryConverter::CalculateSlope(RoadInterface *road, double offset)
{
    double slope = 0.0;
    const RoadElevation* roadElevation = GetRelevantRoadElevation(offset, road);

    if(roadElevation)
    {
        slope = CalculateSlopeAtRoadPosition(roadElevation, offset);
    }

    return slope;
}

const RoadElevation* GeometryConverter::GetRelevantRoadElevation(double roadOffset, RoadInterface* road)
{
    auto roadLaneIt = road->GetElevations().begin();
    while(road->GetElevations().end() != roadLaneIt)
    {
        if((*roadLaneIt)->GetS() <= roadOffset)
        {
            auto roadLaneNextIt = std::next(roadLaneIt);
            if(road->GetElevations().end() == roadLaneNextIt ||
                    (*roadLaneNextIt)->GetS() > roadOffset)
            {
                break;
            }
        }

        ++roadLaneIt;
    }

    if(roadLaneIt == road->GetElevations().end())
    {
        return nullptr;
    }
    else
    {
      return *roadLaneIt;
    }
}

const RoadLaneWidth* GeometryConverter::GetRelevantRoadLaneWidth(double sectionOffset, const RoadLaneInterface *roadLane)
{
    auto roadLaneIt = roadLane->GetWidths().begin();
    while(roadLane->GetWidths().end() != roadLaneIt)
    {
        if((*roadLaneIt)->GetSOffset() <= sectionOffset)
        {
            auto roadLaneNextIt = std::next(roadLaneIt);
            if(roadLane->GetWidths().end() == roadLaneNextIt ||
                    (*roadLaneNextIt)->GetSOffset() > sectionOffset)
            {
                break;
            }
        }

        ++roadLaneIt;
    }

    if(roadLaneIt == roadLane->GetWidths().end())
    {
        return nullptr;
    }
    else
    {
      return *roadLaneIt;
    }
}

const RoadLaneOffset* GeometryConverter::GetRelevantRoadLaneOffset(double roadOffset, RoadInterface* road)
{
    auto laneOffsetIt = road->GetLaneOffsets().begin();
    while(laneOffsetIt != road->GetLaneOffsets().end())
    {
        auto laneOffsetNextIt = std::next(laneOffsetIt);

        if(road->GetLaneOffsets().end() == laneOffsetNextIt ||
                (*laneOffsetNextIt)->GetS() > roadOffset)
        {
        }

        ++laneOffsetIt;
    }

    auto roadLaneIt = road->GetLaneOffsets().begin();
    while(road->GetLaneOffsets().end() != roadLaneIt)
    {
        if((*roadLaneIt)->GetS() <= roadOffset)
        {
            auto roadLaneNextIt = std::next(roadLaneIt);
            if(road->GetLaneOffsets().end() == roadLaneNextIt ||
                    (*roadLaneNextIt)->GetS() > roadOffset)
            {
                break;
            }
        }

        ++roadLaneIt;
    }

    if(roadLaneIt == road->GetLaneOffsets().end())
    {
        return nullptr;
    }
    else
    {
      return *roadLaneIt;
    }
}

const RoadLaneRoadMark* GeometryConverter::GetRelevantRoadLaneRoadMark(double sectionOffset, const RoadLaneInterface* roadLane)
{
    auto roadMarkIt = roadLane->getRoadMarks().begin();

    while(roadMarkIt != roadLane->getRoadMarks().end())
    {
        if((*roadMarkIt)->getSOffset() <= sectionOffset)
        {
            auto roadMarkNextIt = std::next(roadMarkIt);
            if(roadMarkNextIt == roadLane->getRoadMarks().end() ||
               (*roadMarkNextIt)->getSOffset() > sectionOffset)
            {
                break;
            }
        }

        ++roadMarkIt;
    }

    if(roadMarkIt == roadLane->getRoadMarks().end())
    {
        return nullptr;
    }
    else
    {
      return *roadMarkIt;
    }
}

double GeometryConverter::CalculateLaneWidth(const RoadLaneInterface *roadLane, double sectionOffset)
{
    const RoadLaneWidth* roadLaneWidth = GetRelevantRoadLaneWidth(sectionOffset, roadLane);

    if(!roadLaneWidth)
    {
        LOG(CbkLogLevel::Warning, "no lane width given");
        return 0.0;
    }

    return CalculateWidthAtSectionPosition(roadLaneWidth, sectionOffset);
}

double GeometryConverter::CalculateLaneOffset(RoadInterface* road, double roadPosition)
{
    const RoadLaneOffset* roadLaneOffset = GetRelevantRoadLaneOffset(roadPosition, road);

    if(!roadLaneOffset)
    {
        LOG(CbkLogLevel::Warning, "no lane offset given");
        return 0.0;
    }

    return CalculateOffsetAtRoadPosition(roadLaneOffset, roadPosition);
}

bool GeometryConverter::CalculateLanes(double side,
                                       std::map<int, RoadLaneInterface*>& roadLanes,
                                       RoadInterface* road,
                                       RoadGeometryInterface* roadGeometry,
                                       double geometryOffset,
                                       double sectionOffsetStart,
                                       double sectionOffsetEnd,
                                       double roadOffset,
                                       double roadSectionStart,
                                       int index)
{
    Q_UNUSED(sectionOffsetStart);
    Q_UNUSED(sectionOffsetEnd);
    Q_UNUSED(index);
    double previousWidth = 0.0;
    std::list<RoadLaneInterface*> orderedRoadLanes; // ordered from inner lane to outer lane (only one side of road)

    // (OpenDrive) ids of road lanes given in ascending order
    if(0 < side)
    {
        std::map<int, RoadLaneInterface*>::iterator roadLaneIt;
        for(roadLaneIt = roadLanes.begin();
            roadLanes.end() != roadLaneIt;
            ++roadLaneIt)
        {
            if(0 < roadLaneIt->first)
            {
                orderedRoadLanes.push_back(roadLaneIt->second);
            }
        }
    }
    else
    {
        std::map<int, RoadLaneInterface*>::reverse_iterator roadLaneIt;
        for(roadLaneIt = roadLanes.rbegin();
            roadLanes.rend() != roadLaneIt;
            ++roadLaneIt)
        {
            if(0 > roadLaneIt->first)
            {
                orderedRoadLanes.push_back(roadLaneIt->second);
            }
        }
    }

    for(RoadLaneInterface* roadLane : orderedRoadLanes)
    {
        double sectionOffset = roadOffset - roadSectionStart;
        double laneOffset = CalculateLaneOffset(road, roadOffset);
        double laneWidth = CalculateLaneWidth(roadLane, sectionOffset);

        Common::Vector2d pointLeft, pointCenter, pointRight;

        pointLeft = roadGeometry->GetCoord(side,
                                           geometryOffset,
                                           previousWidth,
                                           laneOffset,
                                           laneWidth,
                                           1);

        pointCenter = roadGeometry->GetCoord(side,
                                             geometryOffset,
                                             previousWidth,
                                             laneOffset,
                                             laneWidth,
                                             0);

        pointRight = roadGeometry->GetCoord(side,
                                            geometryOffset,
                                            previousWidth,
                                            laneOffset,
                                            laneWidth,
                                            -1);

        double curvature = roadGeometry->GetCurvature(side,
                                                      geometryOffset,
                                                      previousWidth,
                                                      laneOffset,
                                                      laneWidth);
        double heading = roadGeometry->GetDir(side,
                                              geometryOffset,
                                              previousWidth,
                                              laneOffset,
                                              laneWidth);

        worldData.AddLaneGeometryPoint(*roadLane,
                                       pointLeft, pointCenter, pointRight,
                                       roadOffset, curvature, heading);

        previousWidth += laneWidth;
    }

    return true;
}

osi3::LaneBoundary::Classification::Type GeometryConverter::ConvertRoadLaneRoadMarkToOsiLaneBoundaryType(RoadLaneRoadMarkType type)
{
    switch (type)
    {
        case  RoadLaneRoadMarkType::None:
            return osi3::LaneBoundary_Classification_Type_TYPE_NO_LINE;

        case  RoadLaneRoadMarkType::Solid:
            return osi3::LaneBoundary_Classification_Type_TYPE_SOLID_LINE;

        case  RoadLaneRoadMarkType::Broken:
            return osi3::LaneBoundary_Classification_Type_TYPE_DASHED_LINE;

        case  RoadLaneRoadMarkType::Solid_Solid:
            return osi3::LaneBoundary_Classification_Type_TYPE_SOLID_LINE;

        case  RoadLaneRoadMarkType::Solid_Broken:
            return osi3::LaneBoundary_Classification_Type_TYPE_SOLID_LINE;

        case  RoadLaneRoadMarkType::Broken_Solid:
            return osi3::LaneBoundary_Classification_Type_TYPE_DASHED_LINE;

        case  RoadLaneRoadMarkType::Broken_Broken:
            return osi3::LaneBoundary_Classification_Type_TYPE_DASHED_LINE;

        case  RoadLaneRoadMarkType::Botts_Dots:
            return osi3::LaneBoundary_Classification_Type_TYPE_BOTTS_DOTS;

        case  RoadLaneRoadMarkType::Grass:
            return osi3::LaneBoundary_Classification_Type_TYPE_GRASS_EDGE;

        case  RoadLaneRoadMarkType::Curb:
            return osi3::LaneBoundary_Classification_Type_TYPE_CURB;

        case RoadLaneRoadMarkType::Undefined:
            return osi3::LaneBoundary_Classification_Type_TYPE_NO_LINE;

    default:
        throw std::invalid_argument("RoadLaneRoadMarkType not supported");
    }
    return osi3::LaneBoundary_Classification_Type_TYPE_UNKNOWN;
}

osi3::LaneBoundary::Classification::Color GeometryConverter::ConvertRoadLaneRoadMarkColorToOsiLaneBoundaryColor(RoadLaneRoadMarkColor color)
{
    switch (color)
    {
        case RoadLaneRoadMarkColor::Undefined:
            return osi3::LaneBoundary_Classification_Color_COLOR_NONE;

        case RoadLaneRoadMarkColor::Standard:
        case RoadLaneRoadMarkColor::White:
            return osi3::LaneBoundary_Classification_Color_COLOR_WHITE;

        case RoadLaneRoadMarkColor::Blue:
            return osi3::LaneBoundary_Classification_Color_COLOR_BLUE;

        case RoadLaneRoadMarkColor::Green:
            return osi3::LaneBoundary_Classification_Color_COLOR_GREEN;

        case RoadLaneRoadMarkColor::Red:
            return osi3::LaneBoundary_Classification_Color_COLOR_RED;

        case RoadLaneRoadMarkColor::Yellow:
            return osi3::LaneBoundary_Classification_Color_COLOR_YELLOW;

    default:
        throw std::invalid_argument("RoadLaneRoadMarkColor not supported");
    }
    return osi3::LaneBoundary_Classification_Color_COLOR_NONE;
}

double GeometryConverter::CalculateWidthAtSectionPosition(const RoadLaneWidth* width, double position)
{
    double ds = position - width->GetSOffset();

    return width->GetA() +
           width->GetB() * ds +
           width->GetC() * ds * ds +
           width->GetD() * ds * ds * ds;
}

double GeometryConverter::CalculateSlopeAtRoadPosition(const RoadElevation* roadElevation, double position)
{
    double ds = position - roadElevation->GetS();

    double deltaZ =     roadElevation->GetB() +
                    2 * roadElevation->GetC() * ds +
                    3 * roadElevation->GetD() * ds * ds;

    return std::atan(deltaZ);
}

double GeometryConverter::CalculateOffsetAtRoadPosition(const RoadLaneOffset* roadOffset, double position)
{
    double ds = position - roadOffset->GetS();

    double deltaT = roadOffset->GetA() +
                    roadOffset->GetB() * ds +
                    roadOffset->GetC() * ds * ds +
                    roadOffset->GetD() * ds * ds *ds;

    return deltaT;
}

bool GeometryConverter::Convert()
{
    //calculate road
    bool status;
    status = CalculateRoads();

    //if CalculateRoads failed
    if(!status) return status;

    return true;
}

bool GeometryConverter::CalculatePoints(double geometryOffsetStart,
                                        double geometryOffsetEnd,
                                        int numberLaneGeomPoints,
                                        std::map<int, RoadLaneInterface*> &roadLanes,
                                        RoadInterface *road,
                                        RoadGeometryInterface *roadGeometry,
                                        double sectionOffsetStart,
                                        double sectionOffsetEnd,
                                        double roadGeometryStart,
                                        double roadSectionStart)
{
    // calculate points
    double geometryOffset = geometryOffsetStart;
    for(int index = 0; index < numberLaneGeomPoints; ++index)
    {
        // account for last sample
        if(geometryOffset > geometryOffsetEnd)
        {
            geometryOffset = geometryOffsetEnd;
        }

        if(!CalculateLanes(1.0, // left lanes
                           roadLanes,
                           road,
                           roadGeometry,
                           geometryOffset,
                           sectionOffsetStart,
                           sectionOffsetEnd,
                           roadGeometryStart + geometryOffset,
                           roadSectionStart,
                           index))
        {
            return false;
        }

        if(!CalculateLanes(-1.0, // right lanes
                           roadLanes,
                           road,
                           roadGeometry,
                           geometryOffset,
                           sectionOffsetStart,
                           sectionOffsetEnd,
                           roadGeometryStart + geometryOffset,
                           roadSectionStart,
                           index))
        {
            return false;
        }

        geometryOffset += SAMPLING_RATE;
    }

    return true;
}

void GeometryConverter::CalculateGeometryOffsetStart(double roadSectionStart,
                                                     double roadGeometryStart,
                                                     double* geometryOffsetStart,
                                                     double* sectionOffsetStart)
{
    // geometry begins within section
    if(roadSectionStart <= roadGeometryStart)
    {
        *geometryOffsetStart = 0.0;
        *sectionOffsetStart = roadGeometryStart - roadSectionStart;
    }
    // geometry begins before section
    else
    {
        *geometryOffsetStart = roadSectionStart - roadGeometryStart;
        *sectionOffsetStart = 0.0;
    }
}

void GeometryConverter::CalculateGeometryOffsetEnd(double roadSectionStart,
                                                   double roadSectionNextStart,
                                                   double roadGeometryStart,
                                                   double roadGeometryEnd,
                                                   double roadGeometryLength,
                                                   double* geometryOffsetEnd,
                                                   double* sectionOffsetEnd)
{
    // geometry ends within section
    if(roadSectionNextStart >= roadGeometryEnd)
    {
        *geometryOffsetEnd = roadGeometryLength;
        *sectionOffsetEnd = roadGeometryEnd - roadSectionStart;
    }
    // geometry ends after section
    else
    {
        *geometryOffsetEnd = roadSectionNextStart - roadGeometryStart;
        *sectionOffsetEnd = roadSectionNextStart;
    }
}

bool GeometryConverter::CalculatePointsOfAffectedGeometry(double roadSectionStart,
                                                          double roadGeometryStart,
                                                          double roadSectionNextStart,
                                                          double roadGeometryEnd,
                                                          double roadGeometryLength,
                                                          std::map<int, RoadLaneInterface *>& roadLanes,
                                                          RoadInterface* road,
                                                          RoadGeometryInterface* roadGeometry)
{
    double geometryOffsetStart;
    double sectionOffsetStart;
    CalculateGeometryOffsetStart(roadSectionStart,
                                 roadGeometryStart,
                                 &geometryOffsetStart,
                                 &sectionOffsetStart);

    double geometryOffsetEnd;
    double sectionOffsetEnd;
    CalculateGeometryOffsetEnd(roadSectionStart,
                               roadSectionNextStart,
                               roadGeometryStart,
                               roadGeometryEnd,
                               roadGeometryLength,
                               &geometryOffsetEnd,
                               &sectionOffsetEnd);

    // allocate memory (account for last sample in even/odd cases)
    int numberLaneGeomPoints = static_cast<int>(std::ceil((geometryOffsetEnd - geometryOffsetStart) / SAMPLING_RATE)) + 1;

/*
    for(auto &roadLaneItem : roadLanes)
    {
        AllocateLaneGeometry(roadLaneItem.second,
                             numberLaneGeomPoints,
                             sectionOffsetStart,
                             sectionOffsetEnd);
    }
*/

    bool status = CalculatePoints(geometryOffsetStart,
                                  geometryOffsetEnd,
                                  numberLaneGeomPoints,
                                  roadLanes,
                                  road,
                                  roadGeometry,
                                  sectionOffsetStart,
                                  sectionOffsetEnd,
                                  roadGeometryStart,
                                  roadSectionStart);

    return status;
}

bool GeometryConverter::CalculateGeometries(double roadSectionStart,
                                            double roadSectionNextStart,
                                            RoadInterface* road,
                                            std::map<int, RoadLaneInterface*>& roadLanes)
{
    bool status;

    std::list<RoadGeometryInterface*> roadGeometries = road->GetGeometries();

    double roadGeometryStart;
    double roadGeometryEnd;

    for(RoadGeometryInterface* roadGeometry: roadGeometries)
    {
        status = CalculateGeometry(roadSectionStart,
                                   roadSectionNextStart,
                                   road,
                                   roadLanes,
                                   roadGeometry,
                                   roadGeometryStart,
                                   roadGeometryEnd);

        if(!status) return status;
    }

    status = true;
    return status;
}

bool GeometryConverter::CalculateGeometry(double roadSectionStart,
                                          double roadSectionNextStart,
                                          RoadInterface* road,
                                          std::map<int, RoadLaneInterface*>& roadLanes,
                                          RoadGeometryInterface* roadGeometry,
                                          double& roadGeometryStart,
                                          double& roadGeometryEnd)
{
    double roadGeometryLength = roadGeometry->GetLength();

    roadGeometryStart = roadGeometry->GetS();
    roadGeometryEnd = roadGeometryStart + roadGeometryLength;

    // if section is affected by geometry
    if(roadSectionStart < roadGeometryEnd &&
            roadSectionNextStart > roadGeometryStart)
    {
        bool status = CalculatePointsOfAffectedGeometry(roadSectionStart,
                                                        roadGeometryStart,
                                                        roadSectionNextStart,
                                                        roadGeometryEnd,
                                                        roadGeometryLength,
                                                        roadLanes,
                                                        road,
                                                        roadGeometry);
        if(status == false) return status;
    }

    return true;
}

bool GeometryConverter::CalculateRoads()
{
    for(auto &roadItem : scenery->GetRoads())
    {
        RoadInterface *road = roadItem.second;

        std::vector<RoadLaneSectionInterface*> roadLaneSections = road->GetLaneSections();

        //for(size_t i = 0; i < numberOfRoadLaneSections; i++)
        //for (const auto roadSection : roadLaneSections)
        for (auto roadLaneSectionIt = roadLaneSections.begin();
             roadLaneSectionIt != roadLaneSections.end();
             roadLaneSectionIt++)
        {
            RoadLaneSectionInterface* roadSection = *roadLaneSectionIt;
            std::map<int, RoadLaneInterface*>& roadLanes = roadSection->GetLanes();

            if(!roadLanes.empty())
            {
                double roadSectionStart = roadSection->GetStart();
                double roadSectionNextStart = std::numeric_limits<double>::max();

                if (std::next(roadLaneSectionIt) != roadLaneSections.end()) // if not a single element in the list
                {
                    roadSectionNextStart = (*std::next(roadLaneSectionIt))->GetStart();
                }

                // collect geometry sections
                bool status;
                status = CalculateGeometries(roadSectionStart,
                                             roadSectionNextStart,
                                             road,
                                             roadLanes);

                if(status == false) return status;
            } // if lanes are not empty
        }
    }

    return true;
}

bool GeometryConverter::IsEqual(const double valueA, const double valueB)
{
    return std::abs(valueA - valueB) < EPS;
}
