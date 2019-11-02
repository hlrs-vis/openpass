/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <cmath>
#include <utility>
#include "LaneWalker.h"

namespace World {
namespace Localization {

ForwardLaneWalker::ForwardLaneWalker(const OWL::Interfaces::Lane& lane, bool startAtRoadStart) :
    lane(&lane),
    element(lane.GetLaneGeometryElements().cbegin()),
    startOfElements(lane.GetLaneGeometryElements().cbegin()),
    endOfElements(lane.GetLaneGeometryElements().cend())
{
    if(!startAtRoadStart)
    {
        element = lane.GetLaneGeometryElements().cend();
    }
}

void ForwardLaneWalker::Step()
{
    if (IsDepleted())
    {
        return;
    }
    ++element;
}

void ForwardLaneWalker::Rewind(double sOffset)
{
    if (IsDepleted())
    {
        --element;
    }

    while ((GetSOffset() - sOffset > STEPBACKEPSILON) && (element != startOfElements))
    {
        --element;
    }
}

bool ForwardLaneWalker::IsDepleted() const
{
    return element == endOfElements;
}

const OWL::Interfaces::Lane& ForwardLaneWalker::GetLane() const
{
    return *lane;
}

const OWL::Primitive::LaneGeometryElement& ForwardLaneWalker::GetLaneGeometryElement() const
{
    if (IsDepleted())
    {
        throw std::runtime_error("GetLaneGeometryElement: Cannot return element of depleted list");
    }
    return *(*element);
}

double ForwardLaneWalker::GetSOffset() const
{
    if (IsDepleted())
    {
        throw std::runtime_error("GetLaneGeometryElement: Cannot return s-offset of depleted list");
    }

    return (*element)->joints.current.projectionAxes.sOffset;
}

ReverseLaneWalker::ReverseLaneWalker(const OWL::Interfaces::Lane& lane, bool startAtRoadStart) :
    lane(&lane),
    element(lane.GetLaneGeometryElements().crend()),
    startOfElements(lane.GetLaneGeometryElements().crbegin()),
    endOfElements(lane.GetLaneGeometryElements().crend())
{
    if (!startAtRoadStart)
    {
        element = lane.GetLaneGeometryElements().crbegin();
    }
}

void ReverseLaneWalker::Step()
{
    if (IsDepleted())
    {
        return;
    }
    ++element;
}

void ReverseLaneWalker::Rewind(double sOffset)
{
    if (IsDepleted())
    {
        --element;
    }

    while ((sOffset - GetSOffset() > STEPBACKEPSILON) && (element != startOfElements))
    {
        --element;
    }
}

bool ReverseLaneWalker::IsDepleted() const
{
    return element == endOfElements;
}

const OWL::Interfaces::Lane& ReverseLaneWalker::GetLane() const
{
    return *lane;
}

const OWL::Primitive::LaneGeometryElement& ReverseLaneWalker::GetLaneGeometryElement() const
{
    if (IsDepleted())
    {
        throw std::runtime_error("GetLaneGeometryElement: Cannot return element of depleted list");
    }
    return *(*element);
}

double ReverseLaneWalker::GetSOffset() const
{
    if (IsDepleted())
    {
        throw std::runtime_error("GetLaneGeometryElement: Cannot return s-offset of depleted list");
    }

    return (*element)->joints.next.projectionAxes.sOffset;
}

} // namespace Localization
} // namespace World

