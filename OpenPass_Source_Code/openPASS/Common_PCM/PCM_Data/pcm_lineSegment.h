/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef PCM_LINESEGMENT_H
#define PCM_LINESEGMENT_H

#include "pcm_point.h"
#include "pcm_helper.h"

/*!
 * \brief The PCM_LineSegment class
 * This class is responsible to manage a line segment which consists of exactly two points.
 */
class PCM_LineSegment
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    PCM_LineSegment() = default;
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     firstPoint     first pcm point
    //! @param[in]     secondPoint    second pcm point
    //-----------------------------------------------------------------------------
    PCM_LineSegment(const PCM_Point *firstPoint, const PCM_Point *secondPoint);

    // removing operators
    PCM_LineSegment(const PCM_LineSegment &) = default;
    PCM_LineSegment(PCM_LineSegment &&) = default;
    PCM_LineSegment &operator=(const PCM_LineSegment &) = default;
    PCM_LineSegment &operator=(PCM_LineSegment &&) = default;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    virtual ~PCM_LineSegment() = default;

    //-----------------------------------------------------------------------------
    //! Retrieve the pointer to the first point of the line segment.
    //!
    //! @return                     first point of line segment
    //-----------------------------------------------------------------------------
    const PCM_Point *GetFirstPoint() const;

    //-----------------------------------------------------------------------------
    //! Retrieve the pointer to the second point of the line segment.
    //!
    //! @return                     second point of line segment
    //-----------------------------------------------------------------------------
    const PCM_Point *GetSecondPoint() const;

    //-----------------------------------------------------------------------------
    //! Set the pointer of the first point of the line segment.
    //!
    //! @return                     first point of line segment
    //-----------------------------------------------------------------------------
    void SetFirstPoint(const PCM_Point *point);

    //-----------------------------------------------------------------------------
    //! Set the pointer of the second point of the line segment.
    //!
    //! @return                     second point of line segment
    //-----------------------------------------------------------------------------
    void SetSecondPoint(const PCM_Point *point);

    //-----------------------------------------------------------------------------
    //! Calculate the nearest point on a line segment from a pcm point either in a
    //! specific viewDirection, within a range or none.
    //!
    //! @param[in]    point         pcm Point from where to calculate nearest point
    //! @param[in]    viewDirection viewDirection (INFINITY if no viewDirection is set in
    //!                             in function)
    //! @param[in]    range         range (INFINITY if no range is set in
    //!                             in function)
    //! @return                     distance from point to line line segment,
    //!                             INFINITY if line is not in viewDirection or range
    //!                             from point.
    //-----------------------------------------------------------------------------
    PCM_Point GetNearestPointFromPoint(const PCM_Point *point,
                                       double viewDirection = INFINITY,
                                       double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Calculate the distance from a pcm point to a pcm line segment either in a
    //! specific viewDirection, within a range or none.
    //!
    //! @param[in]    point         pcm Point from where to calculate distance
    //! @param[in]    viewDirection viewDirection (INFINITY if no viewDirection is set in
    //!                             in function)
    //! @param[in]    range         range (INFINITY if no range is set in
    //!                             in function)
    //! @return                     distance from point to line line segment,
    //!                             INFINITY if line is not in viewDirection or range
    //!                             from point.
    //-----------------------------------------------------------------------------
    double CalcDistanceFromPoint(const PCM_Point *point,
                                 double viewDirection = INFINITY,
                                 double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Calculate the viewDirection from a pcm point to a pcm line in minimal distance.
    //!
    //! @param[in]     point        pcm Point from where to calculate viewDirection
    //! @return                     distance from point to line,
    //!                             INFINITY if point or lineSegment == nullptr
    //-----------------------------------------------------------------------------
    double CalcViewDirectionFromPoint(const PCM_Point *point) const;

    //-----------------------------------------------------------------------------
    //! Calculate the absolute angle of a line segment (two points).
    //! The angle is defined between [-pi/2,+pi/2] to avoid the ambiguousness of the
    //! direction of a line (a line has no direction).
    //!
    //! @return                     angle of line segment
    //-----------------------------------------------------------------------------
    double CalculateAngle() const;

private:
    //-----------------------------------------------------------------------------
    //! Calculate the distance from a pcm point to a pcm line in a specific viewDirection.
    //!
    //! @param[in]    point         pcm Point from where to calculate distance
    //! @param[in]    viewDirection viewDirection to calculate the distance
    //! @return                     distance from point to line in specific viewDirection,
    //!                             INFINITY if line is not in viewDirection from point
    //-----------------------------------------------------------------------------
    double CalcDistanceFromPointInViewDirection(const PCM_Point *point, double viewDirection) const;

    //-----------------------------------------------------------------------------
    //! Calculate the nearest point on the line segment to the input point.
    //!
    //! @param[in]    point         point from where to calculate
    //! @return                     nearest point on line segment to input point
    //-----------------------------------------------------------------------------
    PCM_Point CalcNearestPointFromPoint(const PCM_Point *point) const;

    //-----------------------------------------------------------------------------
    //! Calculate the intersection point on the line segment from point in direction.
    //!
    //! @param[in]    point         point
    //! @param[in]    direction     direction
    //! @return                     intersetction point
    //-----------------------------------------------------------------------------
    PCM_Point CalcIntersectionPointInDirection(const PCM_Point *point,
                                               double direction) const;

    //-----------------------------------------------------------------------------
    //! Calculate the nearest point on the line segment to the input point in a
    //! view range.
    //!
    //! @param[in]    point         point from where to calculate
    //! @param[in]    viewDirection viewDirection to calculate the nearest point
    //! @param[in]    range         range
    //! @return                     nearest point on line segment to input point
    //-----------------------------------------------------------------------------
    PCM_Point CalcNearestPointFromPointInViewRange(const PCM_Point *point, double viewDirection,
                                                   double range) const;

    //-----------------------------------------------------------------------------
    //! Checks if given point or points from line segment are valid.
    //!
    //! @return                     true if all points are not nullptr -->valid
    //!                             false if any point is nullptr --> not valid
    //-----------------------------------------------------------------------------
    bool CheckAllPointsValid(const PCM_Point *point) const;

    //-----------------------------------------------------------------------------
    //! Checks if point is valid.
    //!
    //! @return                     true if point is nullptr -->valid
    //!                             false if any point is nullptr --> not valid
    //-----------------------------------------------------------------------------
    bool CheckPointValid(const PCM_Point *point) const;

    //-----------------------------------------------------------------------------
    //! Checks if points from line segment are valid.
    //!
    //! @return                     true if all points are not nullptr -->valid
    //!                             false if any point is nullptr --> not valid
    //-----------------------------------------------------------------------------
    bool CheckLineSegValid() const;

    const PCM_Point *firstPoint = nullptr;  //!< first point
    const PCM_Point *secondPoint = nullptr; //!< second point
};

#endif // PCM_LINESEGMENT_H
