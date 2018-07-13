/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

#include <vector>
#include "boostGeometryCommon.h"

namespace World {
namespace Localization {

class PolygonSampler
{
private:
    const polygon_t polygon;
    const double samplingWidth;

public:
    PolygonSampler(polygon_t polygon, const double samplingWidth) : polygon{polygon}, samplingWidth{samplingWidth} {}

    std::vector<point_t> sample();

private:
    int numberOfAdditionalPoints(const point_t startPoint, const point_t endPoint);

public:
    static std::vector<point_t> generateAdditionalPoints(const point_t& startPoint, const point_t& endPoint, const unsigned int numberOfAdditionalPoints);

};


} // namespace Localization
} // namespace World


