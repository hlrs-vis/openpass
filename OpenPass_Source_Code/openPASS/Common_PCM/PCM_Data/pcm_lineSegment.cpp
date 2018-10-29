/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  pcm_lineSegment.h
//! @brief class to store a line segment which consists of two points
//!
//! This class is responsible to manage a pcm lineSegment.
//-----------------------------------------------------------------------------

#include "pcm_lineSegment.h"
#include "vector2d.h"
#include "commonTools.h"

PCM_LineSegment::PCM_LineSegment(const PCM_Point &firstPoint, const PCM_Point &secondPoint):
    firstPoint(firstPoint),
    secondPoint(secondPoint)
{}

const PCM_Point PCM_LineSegment::GetFirstPoint() const
{
    return firstPoint;
}

const PCM_Point PCM_LineSegment::GetSecondPoint() const
{
    return secondPoint;
}

void PCM_LineSegment::SetFirstPoint(const PCM_Point point)
{
    firstPoint = point;
}

void PCM_LineSegment::SetSecondPoint(const PCM_Point point)
{
    secondPoint = point;
}

PCM_Point PCM_LineSegment::GetNearestPointFromPoint(const PCM_Point *point, double viewAngle,
                                                    double range) const
{
    if (!CheckAllPointsValid(point)) {
        return PCM_Point();
    }

    PCM_Point nearestPoint;

    if (std::isinf(viewAngle)) { // check if viewing direction is infinity --> no specific viewing direction
        nearestPoint = CalcNearestPointFromPoint(point);
    } else if ((qFuzzyIsNull(range)) // check if range is 0 --> no range, just direct viewing line
               || (std::isinf(range))) { //Check if range is INFINITY --> no range, just direct viewing line
        nearestPoint = CalcIntersectionPointInDirection(point, viewAngle);
    } else if ((2 * M_PI - range) <
               std::numeric_limits<double>::lowest()) { // check if range > 2PI -->no specific viewing direction
        nearestPoint = CalcNearestPointFromPoint(point);
    } else { // calculate min distance in range about the viewing direction
        nearestPoint = CalcNearestPointFromPointInViewRange(point, viewAngle, range);
    }

    return nearestPoint;
}

double PCM_LineSegment::CalcDistanceFromPoint(const PCM_Point *point, double viewAngle,
                                              double range) const
{
    if (!CheckAllPointsValid(point)) {
        return INFINITY;
    }

    PCM_Point nearestPoint = GetNearestPointFromPoint(point, viewAngle, range);

    double distance = PCM_Helper::CalcDistanceBetweenPoints(point, &nearestPoint);

    return distance;
}

double PCM_LineSegment::CalcDistanceFromPointInViewDirection(const PCM_Point *point,
                                                             double viewAngle) const
{
    if (!CheckAllPointsValid(point)) {
        return INFINITY;
    }

    if (std::isinf(viewAngle)) {
        return INFINITY;
    }

    Common::Vector2d firstPointVec = PCM_Helper::TransformPointToSourcePointCoord(
                                         &firstPoint, point, viewAngle);
    Common::Vector2d secondPointVec = PCM_Helper::TransformPointToSourcePointCoord(
                                          &secondPoint, point, viewAngle);

    double distance = INFINITY;

    //To Check quadrant signing (I,II - positiv; III,IV - negative)
    double signingChange = INFINITY;

    if ((firstPointVec.x > 0) || (secondPointVec.x > 0)) {
        if ((qFuzzyIsNull(firstPointVec.y))
                || (qFuzzyIsNull(secondPointVec.y))) { // Check if any point is on x-axis
            // Check if both points are on x-axis
            if (qFuzzyCompare(firstPointVec.y + 1, secondPointVec.y + 1)) { // +1 for comparing, 0==0 will fail
                if ((firstPointVec.x > 0) && (secondPointVec.x > 0)) {
                    // take minimal x value for minimal distance
                    distance = std::min(firstPointVec.x, secondPointVec.x);
                } else {
                    // one point is left, one is right --> minimal is 0
                    distance = 0;
                }
            } else {
                // if any y is 0, then x-axis is cut
                signingChange = -1;
            }
        } else {
            // if change in signing, than x-axis is cut by line
            signingChange = firstPointVec.y / secondPointVec.y;
        }
    }

    // Check for change in signing and calculate the intersection on the x-axis
    if (signingChange < 0) {
        double x1 = firstPointVec.x;
        double x2 = secondPointVec.x;
        double y1 = firstPointVec.y;
        double y2 = secondPointVec.y;
        // Two-Point Form of linear equation
        // (y2-y)/(x2-x) = (y2-y1)/(x2-x1)
        // looking for x when y = 0
        double x = x2 - (x2 - x1) / (y2 - y1) * y2;
        // Check if distance is calculated in the opposite direction
        if ((x > 0) || (qFuzzyIsNull(x))) {
            distance = x;
        }
    }

    return distance;
}

double PCM_LineSegment::CalcViewDirectionFromPoint(const PCM_Point *point) const
{
    if (!CheckAllPointsValid(point)) {
        return INFINITY;
    }

    PCM_Point nearestPoint = CalcNearestPointFromPoint(point);

    if (!CheckPointValid(&nearestPoint)) {
        return INFINITY;
    }

    Common::Vector2d pointVec = PCM_Helper::TransformPointToVector2d(point);
    Common::Vector2d nearestPointVec = PCM_Helper::TransformPointToVector2d(&nearestPoint);
    Common::Vector2d resultVec = nearestPointVec - pointVec;

    double angle = resultVec.Angle();

    return angle;
}

double PCM_LineSegment::CalculateAngle() const
{
    if (!CheckLineSegValid()) {
        return INFINITY;
    }

    Common::Vector2d firstPointVec = PCM_Helper::TransformPointToVector2d(&firstPoint);
    Common::Vector2d secondPointVec = PCM_Helper::TransformPointToVector2d(&secondPoint);

    Common::Vector2d line = firstPointVec - secondPointVec;

    // angle [-pi,+pi]
    double angle = line.Angle();

    // angle [-pi/2,+pi/2] !
    if (fabs(angle) > M_PI_2) {
        if (angle > 0) {
            angle -= M_PI;
        } else {
            angle += M_PI;
        }
    }

    return angle;
}

double PCM_LineSegment::CalculateLength() const
{
    if (!CheckLineSegValid()) {
        return INFINITY;
    }

    bool xINF = (std::isinf(firstPoint.GetX()) || std::isinf(secondPoint.GetX()));
    bool yINF = (std::isinf(firstPoint.GetY()) || std::isinf(secondPoint.GetY()));
    bool zINF = (std::isinf(firstPoint.GetZ()) || std::isinf(secondPoint.GetZ()));

    if (xINF && yINF && zINF)
        return INFINITY;

    double sum = 0;
    if (!xINF)
        sum += (firstPoint.GetX() - secondPoint.GetX()) * (firstPoint.GetX() - secondPoint.GetX());
    if (!yINF)
        sum += (firstPoint.GetY() - secondPoint.GetY()) * (firstPoint.GetY() - secondPoint.GetY());
    if (!zINF)
        sum += (firstPoint.GetZ() - secondPoint.GetZ()) * (firstPoint.GetZ() - secondPoint.GetZ());

    return sqrt(sum);
}

PCM_Point PCM_LineSegment::CalcNearestPointFromPoint(const PCM_Point *point) const
{
    if (!CheckAllPointsValid(point)) {
        return PCM_Point();
    }

    Common::Vector2d pointVec = PCM_Helper::TransformPointToVector2d(point);
    Common::Vector2d firstPointVec = PCM_Helper::TransformPointToVector2d(&firstPoint);
    Common::Vector2d secondPointVec = PCM_Helper::TransformPointToVector2d(&secondPoint);

    Common::Vector2d lineSegVector = secondPointVec - firstPointVec;

    //Transformation of coordinates to firstPoint coordination system
    //lineSegVector is already in firstPoint coordination system
    pointVec.Translate(firstPointVec * (-1));

    double angle = lineSegVector.Angle();

    // rotate so lineSegVector is on the X-axis in positive viewAngle with
    // first point in (0|0)
    lineSegVector.Rotate(-angle);
    pointVec.Rotate(-angle);

    PCM_Point nearestPoint;
    if (pointVec.x < 0) { // point is left outside lineSegVector range
        nearestPoint = firstPoint;
    } else if (pointVec.x > lineSegVector.x) { //point is right outside lineSegVector range
        nearestPoint = secondPoint;
    } else {
        // x = pointVec.x (pointVec.x is perpendicular to lineSegVector)
        // y = 0 (is on lineSegVector)
        Common::Vector2d nearestPointVec(pointVec.x, 0);
        // back transformation
        nearestPoint = PCM_Helper::TransformPointFromSourcePointCoord(nearestPointVec,
                                                                      &firstPoint,
                                                                      angle);
    }

    return nearestPoint;
}

PCM_Point PCM_LineSegment::CalcIntersectionPointInDirection(const PCM_Point *point,
                                                            double direction) const
{
    if (!CheckAllPointsValid(point)) {
        return PCM_Point();
    }

    if (std::isinf(direction)) {
        return PCM_Point();
    }

    double intersectionX = CalcDistanceFromPointInViewDirection(point, direction);

    PCM_Point intersectionPoint;
    if (!std::isinf(intersectionX)) {
        Common::Vector2d intersectionPointVec(intersectionX, 0);
        intersectionPoint = PCM_Helper::TransformPointFromSourcePointCoord(
                                intersectionPointVec,
                                point,
                                direction);
    }

    return intersectionPoint;
}

PCM_Point PCM_LineSegment::CalcNearestPointFromPointInViewRange(const PCM_Point *point,
                                                                double viewAngle, double range) const
{
    if (!CheckAllPointsValid(point)) {
        return PCM_Point();
    }

    if ((std::isinf(viewAngle) || (std::isinf(range)))) {
        return PCM_Point();
    }

    Common::Vector2d firstPointVec = PCM_Helper::TransformPointToSourcePointCoord(
                                         &firstPoint, point, viewAngle);
    Common::Vector2d secondPointVec = PCM_Helper::TransformPointToSourcePointCoord(
                                          &secondPoint, point, viewAngle);

    const double rangeUpperLimit = range / 2;
    const double rangeLowerLimit = -range / 2;

    double angleFirstVec = firstPointVec.Angle();
    double angleSecondVec = secondPointVec.Angle();

    bool firstVecIsInRange = false;
    bool secondVecIsInRange = false;

    if ((angleFirstVec < rangeUpperLimit) && (angleFirstVec > rangeLowerLimit)) {
        firstVecIsInRange = true;
    }

    if ((angleSecondVec < rangeUpperLimit) && (angleSecondVec > rangeLowerLimit)) {
        secondVecIsInRange = true;
    }

    const PCM_Point origin(-1, 0, 0, 0);
    const PCM_Point transFirstPoint = PCM_Helper::TransformVector2dToPoint(firstPointVec);
    const PCM_Point transSecondPoint = PCM_Helper::TransformVector2dToPoint(secondPointVec);
    PCM_LineSegment transLineSeg(transFirstPoint, transSecondPoint);

    PCM_Point intersectionPointUpper = transLineSeg.CalcIntersectionPointInDirection(
                                           &origin, rangeUpperLimit);
    bool intersecUpperValid = CheckPointValid(&intersectionPointUpper);

    PCM_Point intersectionPointLower = transLineSeg.CalcIntersectionPointInDirection(
                                           &origin, rangeLowerLimit);
    bool intersecLowerValid = CheckPointValid(&intersectionPointLower);

    PCM_Point transNearestPoint;

    // points of line segment are in range
    if (firstVecIsInRange && secondVecIsInRange) {
        // Check if line segment intersects range (can happen if range > PI)
        // points are in range, but line is out of range
        if (intersecUpperValid && intersecLowerValid) {
            // intersection point with minimal distance to origin is distance
            double distanceUpper = PCM_Helper::TransformPointToVector2d(&intersectionPointUpper).Length();
            double distanceLower = PCM_Helper::TransformPointToVector2d(&intersectionPointLower).Length();
            if (distanceUpper < distanceLower) {
                transNearestPoint = intersectionPointUpper;
            } else {
                transNearestPoint = intersectionPointLower;
            }

        } else { // whole line segment is in range --> calculate as common distance from point
            transNearestPoint = transLineSeg.CalcNearestPointFromPoint(&origin);
        }
    } else { // one or none point is in range --> line segment intersects range
        //resulting inner intersection line segment has to be determined if possible
        PCM_LineSegment intersectionLine;
        PCM_Point intersectionPoint;
        PCM_Point pointInRange;

        // Both range borders are intersected --> intersection line segment is in
        // between both intersection points
        if (intersecUpperValid && intersecLowerValid) {
            intersectionLine.SetFirstPoint(intersectionPointUpper);
            intersectionLine.SetSecondPoint(intersectionPointLower);
        } else if (intersecUpperValid || intersecLowerValid) {
            // if exactly one intersection occurs
            // Find point which intersects
            if (intersecUpperValid) {
                intersectionPoint = intersectionPointUpper;
            } else {
                intersectionPoint = intersectionPointLower;
            }

            // Find point in range
            if (firstVecIsInRange) {
                pointInRange = transFirstPoint;
            } else {
                pointInRange = transSecondPoint;
            }

            // Build intersection line segment
            intersectionLine.SetFirstPoint(intersectionPoint);
            intersectionLine.SetSecondPoint(pointInRange);
        }

        // Calculate nearest point to intersection line
        // If intersection points where not set before --> line segment is not in range
        // nearest point.x = INFINITY
        transNearestPoint = intersectionLine.CalcNearestPointFromPoint(&origin);
    }

    PCM_Point nearestPoint = PCM_Helper::TransformPointFromSourcePointCoord(
                                 PCM_Helper::TransformPointToVector2d(&transNearestPoint),
                                 point, viewAngle);

    return nearestPoint;
}

PCM_LineSegment PCM_LineSegment::CalcSubLineSegmentInViewRange(const PCM_Point *point, double viewAngle, double range) const
{
    double viewRangeRight = CommonHelper::ConvertAngleToPi(viewAngle - range/2.0);
    double viewRangeLeft = CommonHelper::ConvertAngleToPi(viewAngle + range/2.0);

    double viewAngleToFirstPoint = PCM_Helper::CalcAngleBetweenPoints(point, &firstPoint);
    double viewAngleToSecondPoint = PCM_Helper::CalcAngleBetweenPoints(point, &secondPoint);
    viewAngleToFirstPoint = CommonHelper::ConvertAngleToPi(viewAngleToFirstPoint);
    viewAngleToSecondPoint = CommonHelper::ConvertAngleToPi(viewAngleToSecondPoint);

    // check if end points of the line segment is within the view range
    bool firstPointInViewRange  = CommonHelper::Angle_In_Range(viewAngleToFirstPoint,  viewRangeRight, viewRangeLeft);
    bool secondPointInViewRange = CommonHelper::Angle_In_Range(viewAngleToSecondPoint, viewRangeRight, viewRangeLeft);

    if(firstPointInViewRange && secondPointInViewRange)
        return PCM_LineSegment(firstPoint, secondPoint);

    PCM_Point linePointInViewRangeRight = CalcIntersectionPointInDirection(point, viewRangeRight);
    PCM_Point linePointInViewRangeLeft = CalcIntersectionPointInDirection(point, viewRangeLeft);

    if(!firstPointInViewRange && !secondPointInViewRange)
    {
        return PCM_LineSegment(linePointInViewRangeLeft, linePointInViewRangeRight);
    }

    if(!firstPointInViewRange && secondPointInViewRange)
    {
        return PCM_LineSegment(PCM_Helper::CheckPointValid(&linePointInViewRangeRight) ? linePointInViewRangeRight : linePointInViewRangeLeft, secondPoint);
    }

    // firstPointInViewRange && !secondPointInViewRange
    return PCM_LineSegment(firstPoint, PCM_Helper::CheckPointValid(&linePointInViewRangeRight) ? linePointInViewRangeRight : linePointInViewRangeLeft);
}

bool PCM_LineSegment::CheckAllPointsValid(const PCM_Point *point) const
{
    bool valid = true;
    if (!(CheckPointValid(point)) || (!CheckLineSegValid())) {
        valid = false;
    }
    return valid;
}

bool PCM_LineSegment::CheckPointValid(const PCM_Point *point) const
{
    return PCM_Helper::CheckPointValid(point);
}

bool PCM_LineSegment::CheckLineSegValid() const
{
    bool valid = true;
    if ((!CheckPointValid(&firstPoint)) || (!CheckPointValid(&secondPoint))) {
        valid = false;
    }
    return valid;
}
