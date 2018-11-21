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
    QTest::addColumn<double>("weight");
    QTest::addColumn<double>("P_engine");
    QTest::addColumn<double>("T_brakeLimit");

    QTest::addColumn<double>("x_wheelbase");
    QTest::addColumn<double>("x_COG");
    QTest::addColumn<double>("y_track");
    QTest::addColumn<double>("y_COG");

    QTest::addColumn<double>("vel");
    QTest::addColumn<double>("r_tire");
    QTest::addColumn<double>("F_max");
    QTest::addColumn<double>("F_slide");
    QTest::addColumn<double>("s_max");

    QTest::addColumn<double>("angleSlide");
    QTest::addColumn<double>("rateYaw");

    QTest::addColumn<double>("timeStep");
    QTest::addColumn<double>("throttlePedal");
    QTest::addColumn<double>("brakePedal");
    QTest::addColumn<double>("angleTireFront");

    QTest::addColumn<double>("forceTotalXY_X");
    QTest::addColumn<double>("forceTotalXY_Y");
    QTest::addColumn<double>("momentTotalZ");


    QTest::newRow("NULL") << 1000.0 // mass
                          << 100000.0 // power
                          << -10000.0 // brake

                          << 4.0 // wheelbase
                          << 0.0 // COG_x
                          << 2.0 // track
                          << 0.0 // COG_y

                          << 10.0 // speed
                          << 0.2 // tire radius
                          << 10000.0 // tire max. force
                          << 8000.0 // tire slide force
                          << 0.1 // tire max. slip

                          << 0.0 // car slide angle
                          << 0.0 // yaw rate

                          << 0.01 // diff. time step
                          << 0.1 // throttle
                          << 0.0 // brake
                          << 0.0 // steer

                          << 859.35580000 // F_x
                          << 0.0 // F_y
                          << 0.0; // M_z

    QTest::newRow("NULL") << 1000.0 // mass
                          << 100000.0 // power
                          << -10000.0 // brake

                          << 4.0 // wheelbase
                          << 0.0 // COG_x
                          << 2.0 // track
                          << 0.0 // COG_y

                          << 100.0 // speed
                          << 0.2 // tire radius
                          << 10000.0 // tire max. force
                          << 8000.0 // tire slide force
                          << 0.1 // tire max. slip

                          << M_PI_2 // car slide angle
                          << 0.0 // yaw rate

                          << 0.01 // diff. time step
                          << 0.0 // throttle
                          << 0.0 // brake
                          << 0.0 // steer

                          << 0.0 // F_x
                          << -36254.42 // F_y
                          << 0.0; // M_z

    QTest::newRow("NULL") << 1000.0 // mass
                          << 100000.0 // power
                          << -10000.0 // brake

                          << 4.0 // wheelbase
                          << 0.0 // COG_x
                          << 2.0 // track
                          << 0.0 // COG_y

                          << 100.0 // speed
                          << 0.2 // tire radius
                          << 10000.0 // tire max. force
                          << 8000.0 // tire slide force
                          << 0.1 // tire max. slip

                          << 0.0 // car slide angle
                          << 0.0 // yaw rate

                          << 0.01 // diff. time step
                          << 1.0 // throttle
                          << 0.0 // brake
                          << 0.0 // steer

                          << -3352.52 // F_x
                          << 0.0 // F_y
                          << 0.0; // M_z


    QTest::newRow("NULL") << 1000.0 // mass
                          << 100000.0 // power
                          << -10000.0 // brake

                          << 4.0 // wheelbase
                          << 0.0 // COG_x
                          << 2.0 // track
                          << 0.0 // COG_y

                          << 100.0 // speed
                          << 0.2 // tire radius
                          << 10000.0 // tire max. force
                          << 8000.0 // tire slide force
                          << 0.1 // tire max. slip

                          << 0.0 // car slide angle
                          << 0.0 // yaw rate

                          << 0.01 // diff. time step
                          << 0.0 // throttle
                          << 1.0 // brake
                          << 0.0 // steer

                          << -36352.52000000 // F_x
                          << 0.0 // F_y
                          << 0.0; // M_z

}

void UT_Dynamics2TMTest::testCase1()
{
    QFETCH(double, weight);
    QFETCH(double, P_engine);
    QFETCH(double, T_brakeLimit);

    QFETCH(double, x_wheelbase);
    QFETCH(double, x_COG);
    QFETCH(double, y_track);
    QFETCH(double, y_COG);

    QFETCH(double, vel);
    QFETCH(double, r_tire);
    QFETCH(double, F_max);
    QFETCH(double, F_slide);
    QFETCH(double, s_max);

    QFETCH(double, angleSlide);
    QFETCH(double, rateYaw);

    QFETCH(double, timeStep);
    QFETCH(double, throttlePedal);
    QFETCH(double, brakePedal);
    QFETCH(double, angleTireFront);

    QFETCH(double, forceTotalXY_X);
    QFETCH(double, forceTotalXY_Y);
    QFETCH(double, momentTotalZ);

    VehicleSimpleTT* vehicle = new VehicleSimpleTT();

    vehicle->InitSetEngine(weight, P_engine, T_brakeLimit);
    vehicle->InitSetGeometry(x_wheelbase, x_COG, y_track, y_COG);

    vehicle->InitSetTire(vel*std::cos(angleSlide), F_max, F_slide, s_max, r_tire, 1.0);
    Common::Vector2d vVel(vel*std::cos(angleSlide), vel*std::sin(angleSlide));

    vehicle->SetVelocity(vVel, rateYaw);
    std::vector<double> brkSuper = {0.0, 0.0, 0.0, 0.0};
    std::vector<double> vertForce = {
        vehicle->forceTireVerticalStatic[0],
        vehicle->forceTireVerticalStatic[1],
        vehicle->forceTireVerticalStatic[2],
        vehicle->forceTireVerticalStatic[3]};

    vehicle->DriveTrain(throttlePedal, brakePedal, brkSuper);
    vehicle->ForceLocal(timeStep, angleTireFront, vertForce);
    vehicle->ForceGlobal();


    if(qFuzzyIsNull(vehicle->forceTotalXY.x) || qFuzzyIsNull(forceTotalXY_X)){
        QCOMPARE((float)vehicle->forceTotalXY.x + 1, (float)forceTotalXY_X + 1);
    }else{
        QCOMPARE((float)vehicle->forceTotalXY.x, (float)forceTotalXY_X);
    }

    if(qFuzzyIsNull(vehicle->forceTotalXY.y) || qFuzzyIsNull(forceTotalXY_Y)){
        QCOMPARE((float)vehicle->forceTotalXY.y + 1, (float)forceTotalXY_Y + 1);
    }else{
        QCOMPARE((float)vehicle->forceTotalXY.y, (float)forceTotalXY_Y);
    }

    if(qFuzzyIsNull(vehicle->momentTotalZ) || qFuzzyIsNull(momentTotalZ)){
        QCOMPARE((float)vehicle->momentTotalZ + 1, (float)momentTotalZ + 1);
    }else{
        QCOMPARE((float)vehicle->momentTotalZ, (float)momentTotalZ);
    }

    delete (vehicle);

}

void UT_Dynamics2TMTest::cleanupTestCase()
{
    QVERIFY(true);
}
