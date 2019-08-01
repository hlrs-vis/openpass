/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
//-----------------------------------------------------------------------------
//! @file
//! @brief PolygonSampler

#pragma once

#include "Common/boostGeometryCommon.h"
#include "OWL/DataTypes.h"

namespace World {
namespace Localization {

class PolygonSampler
{
private:
    const polygon_t polygon;
    const double samplingWidth;

    const std::map<int, OWL::PointType> polygonPosToPointTypeMapping {{0, OWL::PointType::CornerRightRear},
                                                                      {1, OWL::PointType::CornerLeftRear},
                                                                      {2, OWL::PointType::CornerLeftFront},
                                                                      {3, OWL::PointType::CornerRightFront},
                                                                      {4, OWL::PointType::CornerRightRear}};

public:
    PolygonSampler(polygon_t polygon, const double samplingWidth) : polygon{polygon}, samplingWidth{samplingWidth} {}

    std::list<OWL::SearchablePoint> sample();

private:
    int numberOfAdditionalPoints(const point_t startPoint, const point_t endPoint);
    OWL::SearchablePoint GenerateCornerPoint(const int polygonPosition, const point_t& polygonPoint);
    std::list<OWL::SearchablePoint> GenerateEdgePoints(const point_t& startPoint, const point_t& endPoint, OWL::PointType edgeType);

public:
    static std::vector<point_t> generateAdditionalPoints(const point_t& startPoint, const point_t& endPoint, const unsigned int numberOfAdditionalPoints);

};


} // namespace Localization
} // namespace World


