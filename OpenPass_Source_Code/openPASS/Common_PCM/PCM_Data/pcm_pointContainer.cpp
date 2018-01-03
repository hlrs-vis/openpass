/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "pcm_pointContainer.h"

PCM_PointContainer::~PCM_PointContainer()
{
    for (std::pair<int, const PCM_Point *> pcmPointPair : pointMap) {
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

bool PCM_PointContainer::isPointPresent(int id) const
{
    std::map<int, const PCM_Point *>::const_iterator pointIterator = pointMap.find(id);
    if (pointIterator != pointMap.end()) {
        return true;
    } else {
        return false;
    }
}

const PCM_Point *PCM_PointContainer::GetPointById(int id) const
{
    std::map<int, const PCM_Point *>::const_iterator pointIterator = pointMap.find(id);

    if (pointIterator != pointMap.end()) {
        return pointIterator->second;
    } else {
        return nullptr;
    }
}

PCM_LineSegment PCM_PointContainer::GetNearestLineSegment(const PCM_Point *point,
                                                          double viewDirection, double range) const
{
    PCM_LineSegment minLineSegment(nullptr, nullptr);

    if (point == nullptr) {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    const PCM_Point *previousPoint = nullptr;

    for (std::pair<int, const PCM_Point *> pcmPointPair : pointMap) {
        const PCM_Point *currentPoint = pcmPointPair.second;
        // skip first point
        if (previousPoint != nullptr) {
            PCM_LineSegment lineSegment(previousPoint, currentPoint);

            double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

            if ((minDistance > distance) && (!std::isinf(distance))) {
                minDistance = distance;
                minLineSegment = lineSegment;
            }
        }
        previousPoint = currentPoint;
    }

    return minLineSegment;
}

PCM_Point PCM_PointContainer::GetNearestPoint(const PCM_Point *point, double viewDirection,
                                              double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegment(point, viewDirection, range);
    return minLineSegment.GetNearestPointFromPoint(point, viewDirection, range);
}
