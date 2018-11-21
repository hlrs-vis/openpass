/*********************************************************************
* Copyright (c) 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef TST_UT_PCM_LINESEGMENTTEST_H
#define TST_UT_PCM_LINESEGMENTTEST_H

#include <QString>
#include <QtTest>

#include "pcm_lineSegment.h"
#include "commonTools.h"

class UT_PCM_LineSegmentTest : public QObject
{
    Q_OBJECT

public:
    UT_PCM_LineSegmentTest() = default;
    ~UT_PCM_LineSegmentTest() = default;

private slots:
    void testCase_nearestPointFromPoint_data();
    void testCase_nearestPointFromPoint();

    void testCase_nearestPointInDirection_data();
    void testCase_nearestPointInDirection();

    void testCase_nearestPointInRange_data();
    void testCase_nearestPointInRange();
};

#endif // TST_UT_PCM_LINESEGMENTTEST_H
