/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <Localization/LaneWalker.h>
#include <cmath>

namespace OWL
{

LaneWalker::LaneWalker(const Interfaces::Lane &lane) :
    lane(lane),
    element(lane.GetLaneGeometryElements().cbegin()),
    endOfElements(lane.GetLaneGeometryElements().cend())
{
}

void LaneWalker::Step()
{
    if (IsDepleted()) {
        return;
    }
    ++element;
}

bool LaneWalker::IsDepleted() const {
    return element == endOfElements;
}

const Interfaces::Lane& LaneWalker::GetLane() const {
    return lane;
}

const Primitive::LaneGeometryElement& LaneWalker::GetLaneGeometryElement() const
{
    if (IsDepleted()) {
        throw std::runtime_error("GetLaneGeometryElement: Cannot return element of depleted list");
    }
    return *(*element);
}

double LaneWalker::GetSOffset() const
{
    if (IsDepleted()) {
        throw std::runtime_error("GetLaneGeometryElement: Cannot return s-offset of depleted list");
    }

    return (*element)->joints.current.projectionAxes.sOffset;
}

} // namespace OWL
