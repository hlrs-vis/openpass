/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <cmath>
#include "PointQuery.h"

namespace OWL {

using namespace Common;

bool PointQuery::IsWithin(const Primitive::LaneGeometryElement &element,
                          const Vector2d &point)
{
    if (!element.box.Within(point.x, point.y)) {
        return false;
    }

    return WithinQuadrilateral(element.joints.current.points.left,
                               element.joints.next.points.left,
                               element.joints.current.points.right,
                               element.joints.next.points.right,
                               point);
}

bool PointQuery::WithinQuadrilateral(const Vector2d &A,
                                     const Vector2d &B,
                                     const Vector2d &C,
                                     const Vector2d &D,
                                     const Vector2d &P)
{
    // Triangle 1 (A, B, C)
    const auto BA = A - B;
    const auto BC = C - B;
    const auto BP = P - B;

    auto dot00 = BA.Dot(BA);
    auto dot01 = BA.Dot(BC);
    auto dot02 = BA.Dot(BP);
    auto dot11 = BC.Dot(BC);
    auto dot12 = BC.Dot(BP);

    if (WithinBarycentricCoords(dot00, dot02, dot01, dot11, dot12)) {
        return true;
    }

    // Triangle 2 (C, B, D)
    const auto BD = D - B;

    dot00 = dot11;
    dot02 = dot12;
    dot01 = BC.Dot(BD);
    dot11 = BD.Dot(BD);
    dot12 = BD.Dot(BP);

    if (WithinBarycentricCoords(dot00, dot02, dot01, dot11, dot12)) {
        return true;
    }

    // On an edge
    return OnEdge(A, B, P) || // along left edge
           OnEdge(B, D, P) || // along end of next element
           OnEdge(D, C, P) || // along right edge
           OnEdge(C, A, P) || // along start of current element
           OnEdge(B, C, P);   // along triangular split
}

bool PointQuery::WithinBarycentricCoords(double dot00,
                                         double dot02,
                                         double dot01,
                                         double dot11,
                                         double dot12)
{
    // Compute barycentric coordinates
    const auto denom = (dot00 * dot11 - dot01 * dot01);
    const auto u = (dot11 * dot02 - dot01 * dot12) / denom;
    const auto v = (dot00 * dot12 - dot01 * dot02) / denom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

bool PointQuery::OnEdge(const Vector2d &A,
                        const Vector2d &B,
                        const Vector2d &P)
{
    const auto PA = P - A;
    const auto BA = B - A;

    if (std::abs(PA.x * BA.y - PA.y * BA.x) > EDGE_DETECTION_THRESHOLD) {
        return false;
    }

    if(std::abs(BA.y) < EDGE_DETECTION_THRESHOLD)
    {
        return BA.x > 0 ? A.x <= P.x && P.x <= B.x:
                          B.x <= P.x && P.x <= A.x;
    }
    else
    {
        return BA.y > 0 ? A.y <= P.y && P.y <= B.y:
                          B.y <= P.y && P.y <= A.y;
    }
}

}
