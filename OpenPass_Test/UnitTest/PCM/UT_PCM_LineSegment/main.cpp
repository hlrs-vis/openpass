/*********************************************************************
* Copyright (c) 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <QStringList>
#include <QDir>
#include "tst_ut_pcm_linesegmenttest.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    QStringList testCmd;
    testCmd<<" "<<"-o"<<"QTestPCM_LineSegment_log.txt";
    UT_PCM_LineSegmentTest uT_PCM_LineSegmentTest;
    QTest::qExec(&uT_PCM_LineSegmentTest, testCmd);
}
