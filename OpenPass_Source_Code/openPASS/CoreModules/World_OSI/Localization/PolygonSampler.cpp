/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "PolygonSampler.h"

namespace World {
namespace Localization {

std::vector<point_t> PolygonSampler::sample()
{
    std::vector<point_t> sampledPoints;

    // ring is a vector
    std::vector<point_t> const& points = polygon.outer();
    for (size_t i = 1; i < points.size(); ++i)
    {
        int additionalPoints = numberOfAdditionalPoints(points[i-1], points[i]);
        // maybe implement something similar to this: http://stackoverflow.com/questions/27041660/inserting-a-stdvector-as-returned-from-a-function
        std::vector<point_t> sampledSegmentPoints = generateAdditionalPoints(points[i-1], points[i], additionalPoints);
        sampledPoints.push_back(points[i-1]);
        sampledPoints.insert(sampledPoints.end(), sampledSegmentPoints.begin(), sampledSegmentPoints.end());
    }

    return sampledPoints;
}


int PolygonSampler::numberOfAdditionalPoints(const point_t startPoint, const point_t endPoint)
{
    const double epsilon = 1e-9;
    double distance = bg::distance(startPoint, endPoint);

    return std::floor((distance - epsilon) / samplingWidth);
}

std::vector<point_t> PolygonSampler::generateAdditionalPoints(const point_t& startPoint, const point_t& endPoint, const unsigned int numberOfAdditionalPoints)
{
    std::vector<point_t> additionalPoints;

    if(numberOfAdditionalPoints == 0)
    {
        return additionalPoints;
    }

    point_t delta = endPoint;
    bg::subtract_point(delta, startPoint);

    bg::divide_value(delta, numberOfAdditionalPoints + 1);

    for(unsigned int i = 0; i < numberOfAdditionalPoints; i++)
    {
        point_t sampledPoint = delta;
        bg::multiply_value(sampledPoint, i + 1);
        bg::add_point(sampledPoint, startPoint);
        additionalPoints.push_back(std::move(sampledPoint));
    }

    return additionalPoints;
}

}
}
