/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "PolygonSampler.h"

namespace World {
namespace Localization {


std::list<OWL::SearchablePoint> PolygonSampler::sample()
{
    std::list<OWL::SearchablePoint> sampledPoints;

    std::vector<point_t> const& points = polygon.outer();

    assert(points.size() == 5); //polygon has to be a rectangle

    for (unsigned polygonPosition = 1; polygonPosition < points.size(); ++polygonPosition)
    {
        OWL::SearchablePoint cornerPoint = GenerateCornerPoint(polygonPosition - 1, points[polygonPosition - 1]);
        sampledPoints.push_back(cornerPoint);

        if (polygonPosition == 1)
        {
            std::list<OWL::SearchablePoint> sampledSegmentPoints = GenerateEdgePoints(points[polygonPosition - 1],
                    points[polygonPosition], OWL::PointType::EdgeLeft);
            sampledPoints.insert(sampledPoints.end(), sampledSegmentPoints.begin(), sampledSegmentPoints.end());
        }
        else
            if (polygonPosition == 3)
            {
                std::list<OWL::SearchablePoint> sampledSegmentPoints = GenerateEdgePoints(points[polygonPosition - 1],
                        points[polygonPosition], OWL::PointType::EdgeRight);
                sampledPoints.insert(sampledPoints.end(), sampledSegmentPoints.begin(), sampledSegmentPoints.end());
            }
    }

    return sampledPoints;
}

int PolygonSampler::numberOfAdditionalPoints(const point_t startPoint, const point_t endPoint)
{
    const double epsilon = 1e-9;
    double distance = bg::distance(startPoint, endPoint);

    return std::floor((distance - epsilon) / samplingWidth);
}

std::vector<point_t> PolygonSampler::generateAdditionalPoints(const point_t& startPoint, const point_t& endPoint,
        const unsigned int numberOfAdditionalPoints)
{
    std::vector<point_t> additionalPoints;

    if (numberOfAdditionalPoints == 0)
    {
        return additionalPoints;
    }

    point_t delta = endPoint;
    bg::subtract_point(delta, startPoint);

    bg::divide_value(delta, numberOfAdditionalPoints + 1);

    for (unsigned int i = 0; i < numberOfAdditionalPoints; i++)
    {
        point_t sampledPoint = delta;
        bg::multiply_value(sampledPoint, i + 1);
        bg::add_point(sampledPoint, startPoint);
        additionalPoints.push_back(std::move(sampledPoint));
    }

    return additionalPoints;
}


OWL::SearchablePoint PolygonSampler::GenerateCornerPoint(const int polygonPosition, const point_t& polygonPoint)
{
    if (polygonPosition >= 0 && polygonPosition <= 4)
    {
        OWL::SearchablePoint singleSegmentPoint {{
                polygonPoint.x(),
                polygonPoint.y(),
                0.0
            },
            polygonPosToPointTypeMapping.at(polygonPosition)};

        return singleSegmentPoint;
    }

    throw std::out_of_range("Invalid corner point");
}

std::list<OWL::SearchablePoint> PolygonSampler::GenerateEdgePoints(const point_t& startPoint, const point_t& endPoint,
        OWL::PointType edgeType)
{
    std::list<OWL::SearchablePoint> result;

    int additionalPoints = numberOfAdditionalPoints(startPoint, endPoint);
    // maybe implement something similar to this: http://stackoverflow.com/questions/27041660/inserting-a-stdvector-as-returned-from-a-function
    std::vector<point_t> sampledSegmentPoints = generateAdditionalPoints(startPoint, endPoint, additionalPoints);

    for (const auto& segmentPoint : sampledSegmentPoints)
    {
        result.push_back(OWL::SearchablePoint{{
                segmentPoint.x(),
                segmentPoint.y(),
                0.0
            },
            edgeType});

    }
    return result;
}

}
}
