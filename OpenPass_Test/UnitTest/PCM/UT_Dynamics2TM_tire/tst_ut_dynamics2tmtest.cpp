/*********************************************************************
* Copyright (c) 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "tst_ut_dynamics2tmtest.h"

UT_Dynamics2TMTest::UT_Dynamics2TMTest()
{
}

UT_Dynamics2TMTest::~UT_Dynamics2TMTest()
{
}

void UT_Dynamics2TMTest::initTestCase()
{
}

void UT_Dynamics2TMTest::testCase1_data()
{
    QTest::addColumn<double>("forceTireVerticalStatic");
    QTest::addColumn<double>("F_max");
    QTest::addColumn<double>("F_slide");
    QTest::addColumn<double>("s_max");
    QTest::addColumn<double>("r_tire");
    QTest::addColumn<double>("slip_X");
    QTest::addColumn<double>("slip_Y");

    QTest::addColumn<double>("force_Ref_X");
    QTest::addColumn<double>("force_Ref_Y");

    QTest::newRow("NULL") << 2500.0 // forceTireVerticalStatic
                          << 1000.0 // F_max
                          << 500.0 // F_slide
                          << 0.1 // s_max
                          << 0.2 // r_tire
                          << 1.0 // slip_X
                          << 0.0 // slip_Y
                          << 500.0 // force_Ref_X
                          << 0.0; // force_Ref_Y

    QTest::newRow("NULL") << 2500.0 // forceTireVerticalStatic
                          << 1000.0 // F_max
                          << 500.0 // F_slide
                          << 0.1 // s_max
                          << 0.2 // r_tire
                          << 0.0 // slip_X
                          << 0.1 // slip_Y
                          << 0.0 // force_Ref_X
                          << -1000.0; // force_Ref_Y

}

void UT_Dynamics2TMTest::testCase1()
{
    QFETCH(double, forceTireVerticalStatic);
    QFETCH(double, F_max);
    QFETCH(double, F_slide);
    QFETCH(double, s_max);
    QFETCH(double, r_tire);
    QFETCH(double, slip_X);
    QFETCH(double, slip_Y);
    QFETCH(double, force_Ref_X);
    QFETCH(double, force_Ref_Y);

    Tire* tire = new Tire(forceTireVerticalStatic, F_max, F_slide, s_max, r_tire, 1.0);
    //double slipTire_x = tire->GetLongSlip(torqueTireX[i], velocityTire.x, timeStep);
    //double slipTire_y = CalcSlipY(velocityTire.x / tire->radius, velocityTire.y, tire->radius);

    double slip_Abs = sqrt(slip_X*slip_X + slip_Y*slip_Y);
    double forceAbs = tire->GetForce(slip_Abs);
    double force_X = forceAbs*cos(atan2(slip_Y,slip_X));
    double force_Y = forceAbs*sin(atan2(-slip_Y,slip_X));

    //double forceFric = tire->GetRollFriction(velocityTire.x, forceTireVerticalAdd[i]);

    if(qFuzzyIsNull(force_X) || qFuzzyIsNull(force_Ref_X)){
        QCOMPARE(force_X + 1, force_Ref_X + 1);
    }else{
        QCOMPARE(force_X, force_Ref_X);
    }
    if(qFuzzyIsNull(force_Y) || qFuzzyIsNull(force_Ref_Y)){
        QCOMPARE(force_Y + 1, force_Ref_Y + 1);
    }else{
        QCOMPARE(force_Y, force_Ref_Y);
    }


    delete (tire);

}

void UT_Dynamics2TMTest::cleanupTestCase()
{
    QVERIFY(true);
}
