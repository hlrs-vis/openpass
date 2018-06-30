/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH on behalf of BMW AG
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "road.h"
#include "roadElementTypes.h"

extern "C"
{

extern int fresnl(double xxa,
                  double *ssa,
                  double *cca);

}

namespace
{
const double SQRT_PI_2 = std::sqrt(M_PI_2);

} // namespace

RoadLane::~RoadLane()
{
    for(RoadLaneWidth *item : widths)
    {
        delete item;
    }

    for(auto roadMark : roadMarks)
    {
        delete roadMark;
    }
}

bool RoadLane::AddWidth(double sOffset,
                        double a,
                        double b,
                        double c,
                        double d)
{
    RoadLaneWidth *laneWidth = new (std::nothrow) RoadLaneWidth(sOffset,
                                                                a,
                                                                b,
                                                                c,
                                                                d);
    if(!laneWidth)
    {
        return false;
    }

    widths.push_back(laneWidth);

    return true;
}

bool RoadLane::AddSuccessor(int id)
{
    if(!successor.empty())
    {
        LOG_INTERN(LogLevel::Error) << "added more than one successor to road lane.";
        return false;
    }

    successor.push_back(id);

    return true;
}

bool RoadLane::AddPredecessor(int id)
{
    if(!predecessor.empty())
    {
        LOG_INTERN(LogLevel::Error) << "added more than one predecessor to road lane.";
        return false;
    }

    predecessor.push_back(id);

    return true;
}

bool RoadLane::AddRoadMark(double sOffset,
                           RoadLaneRoadDescriptionType type,
                           RoadLaneRoadMarkType roadMark,
                           RoadLaneRoadMarkColor color,
                           RoadLaneRoadMarkLaneChange laneChange)
{
    RoadLaneRoadMark *laneRoadMark = new (std::nothrow) RoadLaneRoadMark(sOffset,
                                                                         type,
                                                                         roadMark,
                                                                         color,
                                                                         laneChange);
    if(!laneRoadMark)
    {
        return false;
    }

    roadMarks.push_back(laneRoadMark);

    return true;
}

RoadLaneSection::~RoadLaneSection()
{
    for(auto &item : lanes)
    {
        delete item.second;
    }
}

RoadLane *RoadLaneSection::AddRoadLane(int id,
                                       RoadLaneTypeType type)
{
    RoadLane *lane = new (std::nothrow) RoadLane(this,
                                                 id,
                                                 type);

    if(!lanes.insert({id, lane}).second)
    {
        delete lane;
        return nullptr;
    }

    return lane;
}

Common::Vector2d RoadGeometry::GetCoordLine(double side,
                                    double geometryOffset,
                                    double previousWidth,
                                    double laneOffset,
                                    double laneWidth,
                                    int corner)
{
    if(length < geometryOffset)
    {
        LOG_INTERN(LogLevel::Warning) << "exceeding length of geometry";
        geometryOffset = length;
    }

    // unrotated road initially pointing to east
    Common::Vector2d offset(geometryOffset, laneOffset);
    if(0 < side) // left side
    {
        if(0 < corner) // left corner
        {
            offset.y += laneWidth + previousWidth;
        }
        else if(0 == corner) // middle corner
        {
            offset.y += laneWidth/2 + previousWidth;
        }
        else // right corner
        {
            offset.y += previousWidth;
        }
    }
    else // right side
    {
        if(0 < corner) // left corner
        {
            offset.y += -previousWidth;
        }
        else if(0 == corner) // middle corner
        {
            offset.y += -laneWidth/2 - previousWidth;
        }
        else // right corner
        {
            offset.y += -laneWidth - previousWidth;
        }
    }
    offset.Rotate(hdg);

    offset.x += x;
    offset.y += y;

    return offset;
}

double RoadGeometry::GetCurvatureLine(double side,
                                      double geometryOffset,
                                      double previousWidth,
                                      double laneOffset,
                                      double laneWidth)
{
    if(length < geometryOffset)
    {
        LOG_INTERN(LogLevel::Warning) << "exceeding length of geometry";
        geometryOffset = length;
    }

    Q_UNUSED(side);
    Q_UNUSED(previousWidth);
    Q_UNUSED(laneOffset);
    Q_UNUSED(laneWidth);
    return 0.0;
}

double RoadGeometry::GetDirLine(double geometryOffset)
{
    Q_UNUSED(geometryOffset);
    return hdg;
}

Common::Vector2d RoadGeometry::GetCoordArc(double side,
                                   double geometryOffset,
                                   double previousWidth,
                                   double laneOffset,
                                   double laneWidth,
                                   double curvature,
                                   int corner)
{
    if(length < geometryOffset)
    {
        LOG_INTERN(LogLevel::Warning) << "exceeding length of geometry";
        geometryOffset = length;
    }

    double radius = 1 / curvature;
    double circumference = 2 * M_PI / curvature;

    // account for geometryOffset beyond circumference
    // fractionRad = fractionCircumference * 2 * PI / circumference
    double fractionRad = fmod(geometryOffset, circumference) * curvature;

    // shift by radius to rotate around center at origin
    Common::Vector2d offset(0, -radius + laneOffset);
    if(0 < side) // left side
    {
        if(0 < corner) // left corner
        {
            offset.y += laneWidth + previousWidth;
        }
        else if(0 == corner) // middle corner
        {
            offset.y += laneWidth/2 + previousWidth;
        }
        else // right corner
        {
            offset.y += previousWidth;
        }
    }
    else // right side
    {
        if(0 < corner) // left corner
        {
            offset.y += -previousWidth;
        }
        else if(0 == corner) // middle corner
        {
            offset.y += -laneWidth/2 - previousWidth;
        }
        else // right corner
        {
            offset.y += -laneWidth - previousWidth;
        }
    }

    offset.Rotate(fractionRad);

    // shift back
    offset.y += radius;

    offset.Rotate(hdg);

    offset.x += x;
    offset.y += y;

    return offset;
}

double RoadGeometry::GetCurvatureArc(double side,
                                     double geometryOffset,
                                     double previousWidth,
                                     double laneOffset,
                                     double laneWidth,
                                     double curvature)
{
    Q_UNUSED(side);
    Q_UNUSED(geometryOffset);
    Q_UNUSED(previousWidth);
    Q_UNUSED(laneOffset);
    Q_UNUSED(laneWidth);

    return curvature;
}

double RoadGeometry::GetDirArc(double geometryOffset,
                               double curvature)
{
    double circumference = 2 * M_PI / curvature;

    // account for geometryOffset beyond circumference
    // fractionRad = fractionCircumference * 2 * PI / circumference
    double fractionRad = fmod(geometryOffset, circumference) * curvature;

    return hdg + fractionRad;
}

Common::Vector2d RoadGeometryLine::GetCoord(double side,
                                    double geometryOffset,
                                    double previousWidth,
                                    double laneOffset,
                                    double laneWidth,
                                    int corner)
{
    return GetCoordLine(side,
                        geometryOffset,
                        previousWidth,
                        laneOffset,
                        laneWidth,
                        corner);
}

double RoadGeometryLine::GetCurvature(double side,
                                      double geometryOffset,
                                      double previousWidth,
                                      double laneOffset,
                                      double laneWidth)
{
    return GetCurvatureLine(side,
                            geometryOffset,
                            previousWidth,
                            laneOffset,
                            laneWidth);
}

double RoadGeometryLine::GetDir(double side,
                                double geometryOffset,
                                double previousWidth,
                                double laneOffset,
                                double laneWidth)
{
    Q_UNUSED(side);
    Q_UNUSED(previousWidth);
    Q_UNUSED(laneOffset);
    Q_UNUSED(laneWidth);

    return GetDirLine(geometryOffset);
}

Common::Vector2d RoadGeometryArc::GetCoord(double side,
                                   double geometryOffset,
                                   double previousWidth,
                                   double laneOffset,
                                   double laneWidth,
                                   int corner)
{
    if(0.0 == curvature)
    {
        return GetCoordLine(side,
                            geometryOffset,
                            previousWidth,
                            laneOffset,
                            laneWidth,
                            corner);
    }

    return GetCoordArc(side,
                       geometryOffset,
                       previousWidth,
                       laneOffset,
                       laneWidth,
                       curvature,
                       corner);
}

double RoadGeometryArc::GetCurvature(double side,
                                     double geometryOffset,
                                     double previousWidth,
                                     double laneOffset,
                                     double laneWidth)
{
    if(0 == curvature)
    {
        return GetCurvatureLine(side,
                                geometryOffset,
                                previousWidth,
                                laneOffset,
                                laneWidth);
    }

    return GetCurvatureArc(side,
                           geometryOffset,
                           previousWidth,
                           laneOffset,
                           laneWidth,
                           curvature);
}

double RoadGeometryArc::GetDir(double side,
                               double geometryOffset,
                               double previousWidth,
                               double laneOffset,
                               double laneWidth)
{
    Q_UNUSED(side);
    Q_UNUSED(previousWidth);
    Q_UNUSED(laneOffset);
    Q_UNUSED(laneWidth);

    if(0.0 == curvature)
    {
        return GetDirLine(geometryOffset);
    }

    return GetDirArc(geometryOffset,
                     curvature);
}

Common::Vector2d RoadGeometrySpiral::HalfCoord(double side,
                                       double geometryOffset,
                                       double previousWidth,
                                       double laneOffset,
                                       double laneWidth,
                                       int corner)
{
    double _curvStart = curvStart;
    double _curvEnd = curvEnd;

    assert(_curvStart != _curvEnd);
    assert((0.0 <= _curvStart && 0.0 <= _curvEnd) ||
           (0.0 >= _curvStart && 0.0 >= _curvEnd));

    if(length < geometryOffset)
    {
        LOG_INTERN(LogLevel::Warning) << "exceeding length of geometry";
        geometryOffset = length;
    }

    if(0.0 <= _curvStart && 0.0 <= _curvEnd)
    {
        if(_curvStart < _curvEnd)
        {
            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            Common::Vector2d start;
            (void)fresnl(distanceStart / a / SQRT_PI_2,
                         &start.y,
                         &start.x);
            start.Scale(a * SQRT_PI_2);

            double distanceOffset = distanceStart + geometryOffset;
            Common::Vector2d offset;
            (void)fresnl(distanceOffset / a / SQRT_PI_2,
                         &offset.y,
                         &offset.x);
            offset.Scale(a * SQRT_PI_2);
            offset.Sub(start);

            double tangentAngle = distanceOffset * distanceOffset / a / a;
            if(0 > _curvEnd)
            {
                tangentAngle = -tangentAngle;
            }

            double normAngle = tangentAngle - M_PI_2;
            normAngle = std::fmod(normAngle, 2 * M_PI);

            // get perpendicular vector
            Common::Vector2d norm(1, 0);
            norm.Rotate(normAngle);

            if(0 < side) // left side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(-laneWidth - previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(-laneWidth/2 - previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(-previousWidth - laneOffset);
                }
            }
            else // right side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(laneWidth/2 + previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(laneWidth + previousWidth - laneOffset);
                }
            }

            offset.Add(norm);
            offset.Rotate(hdg);

            return Common::Vector2d(x + offset.x, y + offset.y);
        }
        else // _curvStart > _curvEnd ("curStart != _curvEnd" guaranteed by checks in caller)
        {
            std::swap(_curvStart, _curvEnd);
            geometryOffset = length - geometryOffset;

            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            Common::Vector2d start;
            (void)fresnl(distanceStart / a / SQRT_PI_2,
                         &start.y,
                         &start.x);
            start.Scale(a * SQRT_PI_2);

            double distanceOffset = distanceStart + geometryOffset;
            Common::Vector2d offset;
            (void)fresnl(distanceOffset / a / SQRT_PI_2,
                         &offset.y,
                         &offset.x);
            offset.Scale(a * SQRT_PI_2);
            offset.Sub(start);

            double tangentAngle = distanceOffset * distanceOffset / a / a;
            if(0 > _curvEnd)
            {
                tangentAngle = -tangentAngle;
            }

            double normAngle = tangentAngle - M_PI_2;
            normAngle = std::fmod(normAngle, 2 * M_PI);

            // get perpendicular vector
            Common::Vector2d norm(1, 0);
            norm.Rotate(normAngle);

            if(0 < side) // left side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(-laneWidth - previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(-laneWidth/2 - previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(-previousWidth - laneOffset);
                }
            }
            else // right side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(laneWidth/2 + previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(laneWidth + previousWidth - laneOffset);
                }
            }

            offset.Add(norm);

            // calculate end point
            Common::Vector2d endOffset;
            (void)fresnl(distanceEnd / a / SQRT_PI_2,
                         &endOffset.y,
                         &endOffset.x);
            endOffset.Scale(a * SQRT_PI_2);
            endOffset.Sub(start);

            // compensate for inverted curvatures
            double tangentAngleEnd = distanceEnd * distanceEnd / a / a;
            if(0 > _curvEnd)
            {
                tangentAngleEnd = -tangentAngleEnd;
            }
            tangentAngleEnd = -tangentAngleEnd + M_PI;

            offset.Sub(endOffset);
            offset.y = -offset.y;
            offset.Rotate(hdg - tangentAngleEnd);

            return Common::Vector2d(x + offset.x, y + offset.y);
        }
    }
    else // 0.0 >= _curvStart && 0.0 >= _curvEnd
    {
        _curvStart = -_curvStart;
        _curvEnd = -_curvEnd;

        if(_curvStart < _curvEnd)
        {
            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            Common::Vector2d start;
            (void)fresnl(distanceStart / a / SQRT_PI_2,
                         &start.y,
                         &start.x);
            start.Scale(a * SQRT_PI_2);

            double distanceOffset = distanceStart + geometryOffset;
            Common::Vector2d offset;
            (void)fresnl(distanceOffset / a / SQRT_PI_2,
                         &offset.y,
                         &offset.x);
            offset.Scale(a * SQRT_PI_2);
            offset.Sub(start);

            double tangentAngle = distanceOffset * distanceOffset / a / a;
            if(0 > _curvEnd)
            {
                tangentAngle = -tangentAngle;
            }

            double normAngle = tangentAngle - M_PI_2;
            normAngle = std::fmod(normAngle, 2 * M_PI);

            // get perpendicular vector
            Common::Vector2d norm(-1, 0);
            norm.Rotate(normAngle);

            if(0 < side) // left side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(-laneWidth - previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(-laneWidth/2 - previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(-previousWidth - laneOffset);
                }
            }
            else // right side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(laneWidth/2 + previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(laneWidth + previousWidth - laneOffset);
                }
            }

            offset.Add(norm);
            offset.y = -offset.y;
            offset.Rotate(hdg);

            return Common::Vector2d(x + offset.x, y + offset.y);
        }
        else // _curvStart > _curvEnd ("curStart != _curvEnd" guaranteed by checks in caller)
        {
            std::swap(_curvStart, _curvEnd);
            geometryOffset = length - geometryOffset;

            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            Common::Vector2d start;
            (void)fresnl(distanceStart / a / SQRT_PI_2,
                         &start.y,
                         &start.x);
            start.Scale(a * SQRT_PI_2);

            double distanceOffset = distanceStart + geometryOffset;
            Common::Vector2d offset;
            (void)fresnl(distanceOffset / a / SQRT_PI_2,
                         &offset.y,
                         &offset.x);
            offset.Scale(a * SQRT_PI_2);
            offset.Sub(start);

            double tangentAngle = distanceOffset * distanceOffset / a / a;
            if(0 > _curvEnd)
            {
                tangentAngle = -tangentAngle;
            }

            double normAngle = tangentAngle - M_PI_2;
            normAngle = std::fmod(normAngle, 2 * M_PI);

            // get perpendicular vector
            Common::Vector2d norm(-1, 0);
            norm.Rotate(normAngle);

            if(0 < side) // left side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(-laneWidth - previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(-laneWidth/2 - previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(-previousWidth - laneOffset);
                }
            }
            else // right side
            {
                if(0 < corner) // left corner
                {
                    norm.Scale(previousWidth - laneOffset);
                }
                else if(0 == corner) // middle corner
                {
                    norm.Scale(laneWidth/2 + previousWidth - laneOffset);
                }
                else // right corner
                {
                    norm.Scale(laneWidth + previousWidth - laneOffset);
                }
            }

            offset.Add(norm);

            // calculate end point
            Common::Vector2d endOffset;
            (void)fresnl(distanceEnd / a / SQRT_PI_2,
                         &endOffset.y,
                         &endOffset.x);
            endOffset.Scale(a * SQRT_PI_2);
            endOffset.Sub(start);

            // compensate for inverted curvatures
            double tangentAngleEnd = distanceEnd * distanceEnd / a / a;
            if(0 > _curvEnd)
            {
                tangentAngleEnd = -tangentAngleEnd;
            }
            tangentAngleEnd = tangentAngleEnd - M_PI;

            offset.Sub(endOffset);
            offset.Rotate(hdg - tangentAngleEnd);

            return Common::Vector2d(x + offset.x, y + offset.y);
        }
    }
}

Common::Vector2d RoadGeometrySpiral::FullCoord(double side,
                                       double geometryOffset,
                                       double previousWidth,
                                       double laneOffset,
                                       double laneWidth,
                                       int corner)
{
    if((0.0 <= curvStart && 0.0 <= curvEnd) ||
            (0.0 >= curvStart && 0.0 >= curvEnd))
    {
        return HalfCoord(side,
                         geometryOffset,
                         previousWidth,
                         laneOffset,
                         laneWidth,
                         corner);
    }

    assert((0.0 > curvStart && 0.0 < curvEnd) ||
           (0.0 < curvStart && 0.0 > curvEnd));

    // one degree of freedom: start position/end position can not be determined
    LOG_INTERN(LogLevel::Warning) << "could not calculate spiral coordinate";

    return Common::Vector2d();
}

double RoadGeometrySpiral::HalfCurvature(double side,
                                         double geometryOffset,
                                         double previousWidth,
                                         double laneOffset,
                                         double laneWidth)
{
    Q_UNUSED(side);
    Q_UNUSED(previousWidth);
    Q_UNUSED(laneOffset);
    Q_UNUSED(laneWidth);

    double _curvStart = curvStart;
    double _curvEnd = curvEnd;

    assert(_curvStart != _curvEnd);
    assert((0.0 <= _curvStart && 0.0 <= _curvEnd) ||
           (0.0 >= _curvStart && 0.0 >= _curvEnd));

    if(length < geometryOffset)
    {
        LOG_INTERN(LogLevel::Warning) << "exceeding length of geometry";
        geometryOffset = length;
    }

    if(0.0 <= _curvStart && 0.0 <= _curvEnd)
    {
        if(_curvStart < _curvEnd)
        {
            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double distanceOffset = distanceStart + geometryOffset;

            // equation const = radiusEnd * distanceEnd = radiusOffset * distanceOffset
            // -> curvatureOffset = 1 / radiusOffset = distanceOffset / (radiusEnd * distanceEnd)
            return distanceOffset / radiusEnd / distanceEnd;
        }
        else // _curvStart > _curvEnd ("curStart != _curvEnd" guaranteed by checks in caller)
        {
            std::swap(_curvStart, _curvEnd);
            geometryOffset = length - geometryOffset;

            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double distanceOffset = distanceStart + geometryOffset;

            // equation const = radiusEnd * distanceEnd = radiusOffset * distanceOffset
            // -> curvatureOffset = 1 / radiusOffset = distanceOffset / (radiusEnd * distanceEnd)
            return distanceOffset / radiusEnd / distanceEnd;
        }
    }
    else // 0.0 >= _curvStart && 0.0 >= _curvEnd
    {
        _curvStart = -_curvStart;
        _curvEnd = -_curvEnd;

        if(_curvStart < _curvEnd)
        {
            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double distanceOffset = distanceStart + geometryOffset;

            // equation const = radiusEnd * distanceEnd = radiusOffset * distanceOffset
            // -> curvatureOffset = 1 / radiusOffset = distanceOffset / (radiusEnd * distanceEnd)
            return -distanceOffset / radiusEnd / distanceEnd;
        }
        else // _curvStart > _curvEnd ("curStart != _curvEnd" guaranteed by checks in caller)
        {
            std::swap(_curvStart, _curvEnd);
            geometryOffset = length - geometryOffset;

            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double distanceOffset = distanceStart + geometryOffset;

            // equation const = radiusEnd * distanceEnd = radiusOffset * distanceOffset
            // -> curvatureOffset = 1 / radiusOffset = distanceOffset / (radiusEnd * distanceEnd)
            return -distanceOffset / radiusEnd / distanceEnd;
        }
    }
}

double RoadGeometrySpiral::FullCurvature(double side,
                                         double geometryOffset,
                                         double previousWidth,
                                         double laneOffset,
                                         double laneWidth)
{
    if((0.0 <= curvStart && 0.0 <= curvEnd) ||
            (0.0 >= curvStart && 0.0 >= curvEnd))
    {
        return HalfCurvature(side,
                             geometryOffset,
                             previousWidth,
                             laneOffset,
                             laneWidth);
    }

    assert((0.0 > curvStart && 0.0 < curvEnd) ||
           (0.0 < curvStart && 0.0 > curvEnd));

    // one degree of freedom: start position/end position can not be determined
    LOG_INTERN(LogLevel::Warning) << "could not calculate spiral curvature";

    return 0.0;
}

double RoadGeometrySpiral::HalfDir(double side,
                                   double geometryOffset,
                                   double previousWidth,
                                   double laneOffset,
                                   double laneWidth)
{
    Q_UNUSED(side);
    Q_UNUSED(previousWidth);
    Q_UNUSED(laneOffset);
    Q_UNUSED(laneWidth);

    double _curvStart = curvStart;
    double _curvEnd = curvEnd;

    assert(_curvStart != _curvEnd);
    assert((0.0 <= _curvStart && 0.0 <= _curvEnd) ||
           (0.0 >= _curvStart && 0.0 >= _curvEnd));

    if(length < geometryOffset)
    {
        LOG_INTERN(LogLevel::Warning) << "exceeding length of geometry";
        geometryOffset = length;
    }

    if(0.0 <= _curvStart && 0.0 <= _curvEnd)
    {
        if(_curvStart < _curvEnd)
        {
            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            double distanceOffset = distanceStart + geometryOffset;
            double tangentAngle = distanceOffset * distanceOffset / a / a;

            return hdg + tangentAngle;
        }
        else // _curvStart > _curvEnd ("curStart != _curvEnd" guaranteed by checks in caller)
        {
            std::swap(_curvStart, _curvEnd);
            geometryOffset = length - geometryOffset;

            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            double distanceOffset = distanceStart + geometryOffset;

            double tangentAngle = distanceOffset * distanceOffset / a / a;

            // compensate for inverted curvatures
            double tangentAngleEnd = distanceEnd * distanceEnd / a / a;

            return hdg + tangentAngleEnd - tangentAngle;
        }
    }
    else // 0.0 >= _curvStart && 0.0 >= _curvEnd
    {
        _curvStart = -_curvStart;
        _curvEnd = -_curvEnd;

        if(_curvStart < _curvEnd)
        {
            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            double distanceOffset = distanceStart + geometryOffset;

            double tangentAngle = distanceOffset * distanceOffset / a / a;

            return hdg - tangentAngle;
        }
        else // _curvStart > _curvEnd ("curStart != _curvEnd" guaranteed by checks in caller)
        {
            std::swap(_curvStart, _curvEnd);
            geometryOffset = length - geometryOffset;

            assert(0.0 != _curvEnd);

            double radiusEnd = 1.0 / _curvEnd;

            // 1. equation (definition of clothoid): const = radiusStart * distanceStart = radiusEnd * distanceEnd
            // 2. equation: length = distanceEnd - distanceStart
            // -> distanceEnd = radiusStart * length / (radiusStart - radiusEnd)
            // -> formed such that equation copes with _curvStart==0 (infinite radiusStart)
            double distanceEnd = length / (1 - radiusEnd * _curvStart);
            assert(length <= distanceEnd);

            double distanceStart = distanceEnd - length;
            double a = std::sqrt(2 * radiusEnd * distanceEnd);

            double distanceOffset = distanceStart + geometryOffset;

            double tangentAngle = distanceOffset * distanceOffset / a / a;

            // compensate for inverted curvatures
            double tangentAngleEnd = distanceEnd * distanceEnd / a / a;

            return hdg - (tangentAngleEnd- tangentAngle);
        }
    }
}

double RoadGeometrySpiral::FullDir(double side,
                                   double geometryOffset,
                                   double previousWidth,
                                   double laneOffset,
                                   double laneWidth)
{
    if((0.0 <= curvStart && 0.0 <= curvEnd) ||
            (0.0 >= curvStart && 0.0 >= curvEnd))
    {
        return HalfDir(side,
                       geometryOffset,
                       previousWidth,
                       laneOffset,
                       laneWidth);
    }

    assert((0.0 > curvStart && 0.0 < curvEnd) ||
           (0.0 < curvStart && 0.0 > curvEnd));

    // one degree of freedom: start position/end position can not be determined
    LOG_INTERN(LogLevel::Warning) << "could not calculate spiral curvature";

    return 0.0;
}

Common::Vector2d RoadGeometrySpiral::GetCoord(double side,
                                      double geometryOffset,
                                      double previousWidth,
                                      double laneOffset,
                                      double laneWidth,
                                      int corner)
{
    if(0.0 == curvStart && 0.0 == curvEnd)
    {
        return GetCoordLine(side,
                            geometryOffset,
                            previousWidth,
                            laneOffset,
                            laneWidth,
                            corner);
    }

    if(curvStart == curvEnd)
    {
        return GetCoordArc(side,
                           geometryOffset,
                           previousWidth,
                           laneOffset,
                           laneWidth,
                           curvStart,
                           corner);
    }

    return FullCoord(side,
                     geometryOffset,
                     previousWidth,
                     laneOffset,
                     laneWidth,
                     corner);
}

double RoadGeometrySpiral::GetCurvature(double side,
                                        double geometryOffset,
                                        double previousWidth,
                                        double laneOffset,
                                        double laneWidth)
{
    if(0.0 == curvStart && 0.0 == curvEnd)
    {
        return GetCurvatureLine(side,
                                geometryOffset,
                                previousWidth,
                                laneOffset,
                                laneWidth);
    }

    if(curvStart == curvEnd)
    {
        return GetCurvatureArc(side,
                               geometryOffset,
                               previousWidth,
                               laneOffset,
                               laneWidth,
                               curvStart);
    }

    return FullCurvature(side,
                         geometryOffset,
                         previousWidth,
                         laneOffset,
                         laneWidth);
}

double RoadGeometrySpiral::GetDir(double side,
                                  double geometryOffset,
                                  double previousWidth,
                                  double laneOffset,
                                  double laneWidth)
{
    if(0.0 == curvStart && 0.0 == curvEnd)
    {
        return GetDirLine(geometryOffset);
    }

    if(curvStart == curvEnd)
    {
        return GetDirArc(geometryOffset,
                         curvStart);
    }

    return FullDir(side,
                   geometryOffset,
                   previousWidth,
                   laneOffset,
                   laneWidth);
}

Common::Vector2d RoadGeometryPoly3::GetCoord(double side,
                                     double geometryOffset,
                                     double previousWidth,
                                     double laneOffset,
                                     double laneWidth,
                                     int corner)
{
    if(0.0 == a && 0.0 == b && 0.0 == c && 0.0 ==  d)
    {
        return GetCoordLine(side,
                            geometryOffset,
                            previousWidth,
                            laneOffset,
                            laneWidth,
                            corner);
    }

    double s = 0.0;
    Common::Vector2d lastPos;
    Common::Vector2d delta;
    Common::Vector2d pos(0.0, a);

    while(s < geometryOffset)
    {
        lastPos = pos;
        pos.x += 1.0;
        pos.y = a + b * pos.x + c * pos.x * pos.x + d * pos.x * pos.x * pos.x;

        delta = pos - lastPos;
        double deltaLength = delta.Length();

        if(0.0 == deltaLength)
        {
            LOG_INTERN(LogLevel::Warning) << "could not calculate road geometry correctly";
            return Common::Vector2d();
        }

        if(s + deltaLength > geometryOffset)
        {
            // rescale last step
            double scale = (geometryOffset - s) / deltaLength;

            delta.Scale(scale);
            deltaLength = geometryOffset - s;
        }

        s += deltaLength;
    }

    Common::Vector2d offset(lastPos + delta);

    Common::Vector2d norm;
    if(0 < geometryOffset)
    {
        norm = delta;
    }
    else // account for start point
    {
        norm.x = 1.0;
    }

    norm.Rotate(-M_PI_2); // pointing to right side
    if(!norm.Norm())
    {
        LOG_INTERN(LogLevel::Error) << "division by 0";
    }

    offset.Add(norm * -laneOffset);

    if(0 < side) // left side
    {
        if(0 < corner) // left corner
        {
            offset.Add(norm * (-laneWidth - previousWidth));
        }
        else if(0 == corner) // middle corner
        {
            offset.Add(norm * (-laneWidth/2 - previousWidth));
        }
        else // right corner
        {
            offset.Add(norm * -previousWidth);
        }
    }
    else // right side
    {
        if(0 < corner) // left corner
        {
            offset.Add(norm * previousWidth);
        }
        else if(0 == corner) // middle corner
        {
            offset.Add(norm * (laneWidth/2 + previousWidth));
        }
        else // right corner
        {
            offset.Add(norm * (laneWidth + previousWidth));
        }
    }

    offset.Rotate(hdg);

    offset.x += x;
    offset.y += y;
    return offset;
}

double RoadGeometryPoly3::GetCurvature(double side,
                                       double geometryOffset,
                                       double previousWidth,
                                       double laneOffset,
                                       double laneWidth)
{
    if(0.0 == a && 0.0 == b && 0.0 == c && 0.0 ==  d)
    {
        return GetCurvatureLine(side,
                                geometryOffset,
                                previousWidth,
                                laneOffset,
                                laneWidth);
    }

    double s = 0.0;
    Common::Vector2d lastPos;
    Common::Vector2d delta;
    Common::Vector2d pos(0.0, a);

    while(s < geometryOffset)
    {
        lastPos = pos;
        pos.x += 1.0;
        pos.y = a + b * pos.x + c * pos.x * pos.x + d * pos.x * pos.x * pos.x;

        delta = pos - lastPos;
        double deltaLength = delta.Length();

        if(0.0 == deltaLength)
        {
            LOG_INTERN(LogLevel::Warning) << "could not calculate road geometry correctly";
            return 0.0;
        }

        if(s + deltaLength > geometryOffset)
        {
            // rescale last step
            double scale = (geometryOffset - s) / deltaLength;

            delta.Scale(scale);
            deltaLength = geometryOffset - s;
        }

        s += deltaLength;
    }

    return 2 * c + 6 * d * (lastPos + delta).x;
}

double RoadGeometryPoly3::GetDir(double side,
                                 double geometryOffset,
                                 double previousWidth,
                                 double laneOffset,
                                 double laneWidth)
{
    Q_UNUSED(side);
    Q_UNUSED(previousWidth);
    Q_UNUSED(laneOffset);
    Q_UNUSED(laneWidth);

    if(0.0 == a && 0.0 == b && 0.0 == c && 0.0 ==  d)
    {
        return GetDirLine(geometryOffset);
    }

    double s = 0.0;
    Common::Vector2d lastPos;
    Common::Vector2d delta;
    Common::Vector2d pos(0.0, a);

    while(s < geometryOffset)
    {
        lastPos = pos;
        pos.x += 1.0;
        pos.y = a + b * pos.x + c * pos.x * pos.x + d * pos.x * pos.x * pos.x;

        delta = pos - lastPos;
        double deltaLength = delta.Length();

        if(0.0 == deltaLength)
        {
            LOG_INTERN(LogLevel::Warning) << "could not calculate road geometry correctly";
            return 0.0;
        }

        if(s + deltaLength > geometryOffset)
        {
            // rescale last step
            double scale = (geometryOffset - s) / deltaLength;

            delta.Scale(scale);
            deltaLength = geometryOffset - s;
        }

        s += deltaLength;
    }

    Common::Vector2d direction;
    if(0 < geometryOffset)
    {
        direction = delta;
    }
    else // account for start point
    {
        direction.x = 1.0;
    }

    direction.Rotate(hdg);
    if(!direction.Norm())
    {
        LOG_INTERN(LogLevel::Error) << "division by 0";
    }

    if(1.0 < direction.y)
    {
        direction.y = 1.0;
    }

    if(-1.0 > direction.y)
    {
        direction.y = -1.0;
    }

    double angle = std::asin(direction.y);

    if(0.0 <= direction.x)
    {
        return angle;
    }
    else
    {
        return M_PI - angle;
    }
}

Road::~Road()
{
    for(RoadElevation *item : elevations)
    {
        delete item;
    }

    for(RoadLaneOffset *item : laneOffsets)
    {
        delete item;
    }

    for(RoadGeometryInterface *item : geometries)
    {
        delete item;
    }

    for(RoadLinkInterface *item : links)
    {
        delete item;
    }

    for(RoadLaneSectionInterface *item : laneSections)
    {
        delete item;
    }

    for(RoadSignalInterface *item : roadSignals)
    {
        delete item;
    }

    for(RoadObjectInterface *item : roadObjects)
    {
        delete item;
    }
}

bool Road::AddGeometryLine(double s,
                           double x,
                           double y,
                           double hdg,
                           double length)
{
    RoadGeometry *roadGeometry = new (std::nothrow) RoadGeometryLine(s,
                                                                     x,
                                                                     y,
                                                                     hdg,
                                                                     length);
    if(!roadGeometry)
    {
        return false;
    }

    geometries.push_back(roadGeometry);

    return true;
}

bool Road::AddGeometryArc(double s,
                          double x,
                          double y,
                          double hdg,
                          double length,
                          double curvature)
{
    RoadGeometry *roadGeometry = new (std::nothrow) RoadGeometryArc(s,
                                                                    x,
                                                                    y,
                                                                    hdg,
                                                                    length,
                                                                    curvature);
    if(!roadGeometry)
    {
        return false;
    }

    geometries.push_back(roadGeometry);

    return true;
}

bool Road::AddGeometrySpiral(double s,
                             double x,
                             double y,
                             double hdg,
                             double length,
                             double curvStart,
                             double curvEnd)
{
    RoadGeometry *roadGeometry = new (std::nothrow) RoadGeometrySpiral(s,
                                                                       x,
                                                                       y,
                                                                       hdg,
                                                                       length,
                                                                       curvStart,
                                                                       curvEnd);
    if(!roadGeometry)
    {
        return false;
    }

    geometries.push_back(roadGeometry);

    return true;
}

bool Road::AddGeometryPoly3(double s,
                            double x,
                            double y,
                            double hdg,
                            double length,
                            double a,
                            double b,
                            double c,
                            double d)
{
    RoadGeometry *roadGeometry = new (std::nothrow) RoadGeometryPoly3(s,
                                                                      x,
                                                                      y,
                                                                      hdg,
                                                                      length,
                                                                      a,
                                                                      b,
                                                                      c,
                                                                      d);
    if(!roadGeometry)
    {
        return false;
    }

    geometries.push_back(roadGeometry);

    return true;
}

bool Road::AddElevation(double s,
                        double a,
                        double b,
                        double c,
                        double d)
{
    RoadElevation *roadElevation = new (std::nothrow) RoadElevation(s,
                                                                    a,
                                                                    b,
                                                                    c,
                                                                    d);
    if(!roadElevation)
    {
        return false;
    }

    elevations.push_back(roadElevation);

    return true;
}

bool Road::AddLaneOffset(double s,
                         double a,
                         double b,
                         double c,
                         double d)
{
    RoadLaneOffset *roadLaneOffset = new (std::nothrow) RoadLaneOffset(s,
                                                                       a,
                                                                       b,
                                                                       c,
                                                                       d);
    if(!roadLaneOffset)
    {
        return false;
    }

    laneOffsets.push_back(roadLaneOffset);

    return true;
}

bool Road::AddLink(RoadLinkType type,
                   RoadLinkElementType elementType,
                   const std::string &elementId,
                   ContactPointType contactPoint,
                   RoadLinkDirectionType direction,
                   RoadLinkSideType side)
{
    RoadLink *roadLink = new (std::nothrow) RoadLink(type,
                                                     elementType,
                                                     elementId,
                                                     contactPoint,
                                                     direction,
                                                     side);
    if(!roadLink)
    {
        return false;
    }

    links.push_back(roadLink);

    return true;
}

RoadLaneSection *Road::AddRoadLaneSection(double start)
{
    RoadLaneSection *laneSection = new (std::nothrow) RoadLaneSection(this,
                                                                      start);
    laneSections.push_back(laneSection);

    return laneSection;
}

// TODO: make void
RoadSignal* Road::AddRoadSignal(const RoadSignalSpecification &signal)
{
    RoadSignal *roadSignal = new (std::nothrow) RoadSignal(this, signal);
    roadSignals.push_back(roadSignal);

    return roadSignal;
}

// TODO: make void
RoadObject* Road::AddRoadObject(const RoadObjectSpecification &object)
{
    RoadObject *roadObject = new (std::nothrow) RoadObject(this, object);
    roadObjects.push_back(roadObject);

    return roadObject;
}


void Road::AddRoadType(const RoadTypeSpecification &info)
{
    roadTypes.push_back(info);
}

RoadTypeInformation Road::GetRoadType(double start)
{
    for(RoadTypeSpecification roadTypeSpec : roadTypes)
    {
        if(roadTypeSpec.s == start) return roadTypeSpec.roadType;
    }

    return RoadTypeInformation::Undefined;
}

