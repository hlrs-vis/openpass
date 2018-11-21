/*********************************************************************
* Copyright (c) 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "tst_ut_pcm_linesegmenttest.h"

void UT_PCM_LineSegmentTest::testCase_nearestPointFromPoint_data()
{
    QTest::addColumn<double>("sourcePointX");
    QTest::addColumn<double>("sourcePointY");
    QTest::addColumn<double>("nearestPointX");
    QTest::addColumn<double>("nearestPointY");

    QTest::newRow("point in line segment") << 5.0 << 5.0 << 0.0 << 5.0;
    QTest::newRow("point out of line segment up") << 5.0 << 12.0 << 0.0 << 10.0;
    QTest::newRow("point out of line segment down") << 5.0 << -1.0 << 0.0 << 0.0;
    QTest::newRow("point on line segment") << 0.0 << 5.0 << 0.0 << 5.0;
}

void UT_PCM_LineSegmentTest::testCase_nearestPointFromPoint()
{
    QFETCH(double, sourcePointX);
    QFETCH(double, sourcePointY);
    QFETCH(double, nearestPointX);
    QFETCH(double, nearestPointY);

    PCM_Point sourcePoint(-1, sourcePointX, sourcePointY, 0);

    PCM_Point firstPoint(1, 0, 0, 0);
    PCM_Point secondPoint(2, 0, 10, 0);
    PCM_LineSegment lineSegment(firstPoint, secondPoint);

    PCM_Point nearestPoint = lineSegment.GetNearestPointFromPoint(&sourcePoint);

    if (qFuzzyIsNull(nearestPoint.GetX()) || qFuzzyIsNull(nearestPointX))
    {
        QCOMPARE(nearestPoint.GetX() + 1 , nearestPointX + 1);
    }
    else
    {
        QCOMPARE(nearestPoint.GetX(), nearestPointX);
    }

    if (qFuzzyIsNull(nearestPoint.GetY()) || qFuzzyIsNull(nearestPointY))
    {
        QCOMPARE(nearestPoint.GetY() + 1 , nearestPointY + 1);
    }
    else
    {
        QCOMPARE(nearestPoint.GetY(), nearestPointY);
    }
}

void UT_PCM_LineSegmentTest::testCase_nearestPointInDirection_data()
{
    QTest::addColumn<double>("sourcePointX");
    QTest::addColumn<double>("sourcePointY");
    QTest::addColumn<double>("direction");
    QTest::addColumn<double>("nearestPointX");
    QTest::addColumn<double>("nearestPointY");

    double backDirection = CommonHelper::ConvertagentViewDirectionToRadian(
                               AgentViewDirection::back);

    double leftDirection = CommonHelper::ConvertagentViewDirectionToRadian(
                               AgentViewDirection::left);

    QTest::newRow("point in line segment") << 5.0 << 5.0
                                           << backDirection
                                           << 0.0 << 5.0;
    QTest::newRow("point out of line segment up") << 5.0 << 12.0
                                                  << backDirection
                                                  << INFINITY << INFINITY;
    QTest::newRow("point out of line segment down") << 5.0 << -1.0
                                                    << backDirection
                                                    << INFINITY << INFINITY;
    QTest::newRow("point on line segment") << 0.0 << 5.0
                                           << backDirection
                                           << 0.0 << 5.0;
    QTest::newRow("point on line segment (in same direction)") << 0.0 << 5.0
                                                               << leftDirection
                                                               << 0.0 << 5.0;

    QTest::newRow("point down line segment (in same direction)") << 0.0 << -1.0
                                                                 << leftDirection
                                                                 << 0.0 << 0.0;
}

void UT_PCM_LineSegmentTest::testCase_nearestPointInDirection()
{
    QFETCH(double, sourcePointX);
    QFETCH(double, sourcePointY);
    QFETCH(double, direction);
    QFETCH(double, nearestPointX);
    QFETCH(double, nearestPointY);

    PCM_Point sourcePoint(-1, sourcePointX, sourcePointY, 0);

    PCM_Point firstPoint(1, 0, 0, 0);
    PCM_Point secondPoint(2, 0, 10, 0);
    PCM_LineSegment lineSegment(firstPoint, secondPoint);

    PCM_Point nearestPoint = lineSegment.GetNearestPointFromPoint(&sourcePoint, direction);

    if (qIsInf(nearestPoint.GetX()) && qIsInf(nearestPointX))
    {
        QVERIFY(true);
    }
    else
    {
        if (qFuzzyIsNull(nearestPoint.GetX()) || qFuzzyIsNull(nearestPointX))
        {
            QCOMPARE(nearestPoint.GetX() + 1 , nearestPointX + 1);
        }
        else
        {
            QCOMPARE(nearestPoint.GetX(), nearestPointX);
        }
    }


    if (qIsInf(nearestPoint.GetY()) && qIsInf(nearestPointY))
    {
        QVERIFY(true);
    }
    else
    {
        if (qFuzzyIsNull(nearestPoint.GetY()) || qFuzzyIsNull(nearestPointY))
        {
            QCOMPARE(nearestPoint.GetY() + 1 , nearestPointY + 1);
        }
        else
        {
            QCOMPARE(nearestPoint.GetY(), nearestPointY);
        }
    }
}

void UT_PCM_LineSegmentTest::testCase_nearestPointInRange_data()
{
    QTest::addColumn<double>("sourcePointX");
    QTest::addColumn<double>("sourcePointY");
    QTest::addColumn<double>("direction");
    QTest::addColumn<double>("range");
    QTest::addColumn<double>("nearestPointX");
    QTest::addColumn<double>("nearestPointY");

    double backDirection = CommonHelper::ConvertagentViewDirectionToRadian(
                               AgentViewDirection::back);
    double leftDirection = CommonHelper::ConvertagentViewDirectionToRadian(
                               AgentViewDirection::left);

    QTest::newRow("point in line segment") << 5.0 << 5.0
                                           << backDirection << M_PI
                                           << 0.0 << 5.0;
    QTest::newRow("point in line segment (2)") << 5.0 << 6.0
                                               << backDirection << 2 * M_PI
                                               << 0.0 << 6.0;
    QTest::newRow("point out of line segment up") << 5.0 << 12.0
                                                  << backDirection << M_PI
                                                  << 0.0 << 10.0;
    QTest::newRow("point out of line segment up (2)") << 5.0 << 13.0
                                                      << backDirection << 0.0
                                                      << INFINITY << INFINITY;
    QTest::newRow("point out of line segment down") << 5.0 << -1.0
                                                    << backDirection << M_PI
                                                    << 0.0 << 0.0;
    QTest::newRow("point on line segment") << 0.0 << 5.0
                                           << backDirection << M_PI
                                           << 0.0 << 5.0;
    QTest::newRow("point on line segment (in same direction)") << 0.0 << 5.0
                                                               << leftDirection << M_PI
                                                               << 0.0 << 5.0;
    QTest::newRow("point down line segment (in same direction)") << 0.0 << -1.0
                                                                 << leftDirection << M_PI
                                                                 << 0.0 << 0.0;
    QTest::newRow("point down line segment (range tangenting)") << 5.0 << -5.0
                                                                << backDirection << M_PI
                                                                << 0.0 << 0.0;
    QTest::newRow("point behind line segment (range greater 180)") << -5.0 << 5.0
                                                                   << backDirection << 1.5 * M_PI
                                                                   << 0.0 << 5.0;
    QTest::newRow("point behind line segment (range greater 180) (2))") << -1.0 << 5.0
                                                                        << backDirection << 1.5 * M_PI
                                                                        << 0.0 << 6.0;
}

void UT_PCM_LineSegmentTest::testCase_nearestPointInRange()
{
    QFETCH(double, sourcePointX);
    QFETCH(double, sourcePointY);
    QFETCH(double, direction);
    QFETCH(double, range);
    QFETCH(double, nearestPointX);
    QFETCH(double, nearestPointY);

    PCM_Point sourcePoint(-1, sourcePointX, sourcePointY, 0);

    PCM_Point firstPoint(1, 0, 0, 0);
    PCM_Point secondPoint(2, 0, 10, 0);
    PCM_LineSegment lineSegment(firstPoint, secondPoint);

    PCM_Point nearestPoint = lineSegment.GetNearestPointFromPoint(&sourcePoint, direction, range);

    if (qIsInf(nearestPoint.GetX()) && qIsInf(nearestPointX))
    {
        QVERIFY(true);
    }
    else
    {
        if (qFuzzyIsNull(nearestPoint.GetX()) || qFuzzyIsNull(nearestPointX))
        {
            QCOMPARE(nearestPoint.GetX() + 1 , nearestPointX + 1);
        }
        else
        {
            QCOMPARE(nearestPoint.GetX(), nearestPointX);
        }
    }


    if (qIsInf(nearestPoint.GetY()) && qIsInf(nearestPointY))
    {
        QVERIFY(true);
    }
    else
    {
        if (qFuzzyIsNull(nearestPoint.GetY()) || qFuzzyIsNull(nearestPointY))
        {
            QCOMPARE(nearestPoint.GetY() + 1 , nearestPointY + 1);
        }
        else
        {
            QCOMPARE(nearestPoint.GetY(), nearestPointY);
        }
    }
}
