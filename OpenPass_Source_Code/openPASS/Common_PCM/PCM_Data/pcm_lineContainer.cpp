/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "pcm_lineContainer.h"

PCM_LineContainer::~PCM_LineContainer()
{
    for (std::pair<int, PCM_Line *> pcmLinePair : lineMap)
    {
        delete pcmLinePair.second;
    }
    lineMap.clear();
}

bool PCM_LineContainer::AddPCM_Line(PCM_Line *line)
{
    bool success = lineMap.emplace(std::make_pair(line->GetId(), line)).second;
    return success;
}

const std::map<int, PCM_Line *> *PCM_LineContainer::GetLineMap() const
{
    return &lineMap;
}

bool PCM_LineContainer::IsLinePresent(int id) const
{
    std::map<int, PCM_Line *>::const_iterator lineIterator = lineMap.find(id);

    if (lineIterator != lineMap.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

PCM_Line *PCM_LineContainer::GetLineById(int id) const
{
    std::map<int, PCM_Line *>::const_iterator lineIterator = lineMap.find(id);

    if (lineIterator != lineMap.end())
    {
        return lineIterator->second;
    }
    else
    {
        return nullptr;
    }
}

PCM_LineSegment PCM_LineContainer::GetNearestLineSegment(const PCM_Point *point,
                                                         double viewDirection, double range) const
{
    PCM_LineSegment minLineSegment(nullptr, nullptr);

    if (point == nullptr)
    {
        return minLineSegment;
    }

    double minDistance = INFINITY;

    for (std::pair<int, const PCM_Line *> pcmLinePair : lineMap)
    {
        const PCM_Line *line = pcmLinePair.second;

        PCM_LineSegment lineSegment = line->GetNearestLineSegment(point, viewDirection, range);

        double distance = lineSegment.CalcDistanceFromPoint(point, viewDirection, range);

        if ((minDistance > distance) && (!std::isinf(distance)))
        {
            minDistance = distance;
            minLineSegment = lineSegment;
        }
    }

    return minLineSegment;
}

PCM_Point PCM_LineContainer::GetNearestPoint(const PCM_Point *point, double viewDirection,
                                             double range) const
{
    PCM_LineSegment minLineSegment = GetNearestLineSegment(point, viewDirection, range);
    return minLineSegment.GetNearestPointFromPoint(point, viewDirection, range);
}
