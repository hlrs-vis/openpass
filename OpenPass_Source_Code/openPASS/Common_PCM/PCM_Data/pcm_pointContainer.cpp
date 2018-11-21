/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_pointContainer.h"

PCM_PointContainer::~PCM_PointContainer()
{
    for (std::pair<int, const PCM_Point *> pcmPointPair : pointMap)
    {
        delete pcmPointPair.second;
    }

    pointMap.clear();
}

bool PCM_PointContainer::AddPCM_Point(const PCM_Point *point)
{
    bool success = pointMap.emplace(std::make_pair(point->GetId(), point)).second;
    return success;
}

const std::map<int, const PCM_Point *> *PCM_PointContainer::GetPointMap() const
{
    return &pointMap;
}

bool PCM_PointContainer::IsPointPresent(int id) const
{
    std::map<int, const PCM_Point *>::const_iterator pointIterator = pointMap.find(id);
    if (pointIterator != pointMap.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

const PCM_Point *PCM_PointContainer::GetPointById(int id) const
{
    std::map<int, const PCM_Point *>::const_iterator pointIterator = pointMap.find(id);

    if (pointIterator != pointMap.end())
    {
        return pointIterator->second;
    }
    else
    {
        return nullptr;
    }
}

PCM_LineSegment PCM_PointContainer::GetNearestLineSegment(const PCM_Point *point,
                                                          double viewAngle, double range,
                                                          bool calculateSubLine) const
{
    PCM_LineSegment minLineSegment;

    if (point == nullptr)
    {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    const PCM_Point *previousPoint = nullptr;

    for (std::pair<int, const PCM_Point *> pcmPointPair : pointMap)
    {
        const PCM_Point *currentPoint = pcmPointPair.second;
        // skip first point
        if (previousPoint != nullptr)
        {
            PCM_LineSegment lineSegment(*previousPoint, *currentPoint);

            if(calculateSubLine)
                lineSegment = lineSegment.CalcSubLineSegmentInViewRange(point, viewAngle, range);

//            // debug
//            double fullLength = fullLineSegment.CalculateLength();
//            double subLength = lineSegment.CalculateLength();
//            if(subLength < fullLength)
//                qDebug("GetNearestLineSegment: SubLineSegment %f / %f", subLength, fullLength);
//                LOGINFO(QString().sprintf("GetNearestLineSegment: SubLineSegment %f / %f", subLength, fullLength).toStdString());

//            PCM_LineSegment lineSegment(previousPoint, currentPoint);
            double distance = lineSegment.CalcDistanceFromPoint(point, viewAngle, range);

            if ((minDistance > distance) && (!std::isinf(distance)))
            {
                minDistance = distance;
                minLineSegment = lineSegment;
            }
        }
        previousPoint = currentPoint;
    }

    return minLineSegment;
}

//std::vector<PCM_LineSegment> PCM_PointContainer::GetLineSegments(double distanceMax,
//                                                          double viewAngle, double range) const

PCM_Point PCM_PointContainer::GetNearestPoint(const PCM_Point *point, double viewAngle,
                                              double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegment(point, viewAngle, range);
    return minLineSegment.GetNearestPointFromPoint(point, viewAngle, range);
}
