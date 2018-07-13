/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <algorithm>
#include <cmath>

#include "WorldObjectAdapter.h"

namespace WorldObjectCommon {

double GetFrontDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge)
{
    auto l_front = distanceReferencePointToLeadingEdge;
    auto l_rear = distanceReferencePointToLeadingEdge - length;
    auto w = width/2;

    auto s1 = l_front * cos(hdg) - w * sin(hdg);
    auto s2 = l_front * cos(hdg) + w * sin(hdg);
    auto s3 = l_rear  * cos(hdg) - w * sin(hdg);
    auto s4 = l_rear  * cos(hdg) + w * sin(hdg);

    return std::max({s1, s2, s3, s4});
}

double GetRearDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge)
{
    auto l_front = distanceReferencePointToLeadingEdge;
    auto l_rear = distanceReferencePointToLeadingEdge - length;
    auto w = width/2;

    auto s1 = l_front * cos(hdg) - w * sin(hdg);
    auto s2 = l_front * cos(hdg) + w * sin(hdg);
    auto s3 = l_rear  * cos(hdg) - w * sin(hdg);
    auto s4 = l_rear  * cos(hdg) + w * sin(hdg);

    return std::abs(std::min({s1, s2, s3, s4}));
}
} // namespace WorldObjectCommon
