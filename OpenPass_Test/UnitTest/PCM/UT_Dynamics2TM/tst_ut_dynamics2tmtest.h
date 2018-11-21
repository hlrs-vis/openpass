/*********************************************************************
* Copyright (c) 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef TST_UT_DYNAMICS2TMTEST_H
#define TST_UT_DYNAMICS2TMTEST_H

#include <QString>
#include <QtTest>
#include <QtGlobal>
#include "dynamics_twotrack_vehicle.h"

class UT_Dynamics2TMTest : public QObject
{
    Q_OBJECT

public:
    UT_Dynamics2TMTest();
    ~UT_Dynamics2TMTest();


private Q_SLOTS:
    void initTestCase();

    void testCase1_data();
    void testCase1();

    void cleanupTestCase();

private:

};

#endif // TST_UT_DYNAMICS2TMTEST_H
