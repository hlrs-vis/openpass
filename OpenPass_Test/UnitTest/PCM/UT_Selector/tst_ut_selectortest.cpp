/*********************************************************************
* Copyright (c) 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "tst_ut_selectortest.h"

void UT_SelectorTest::testCase_perform_data()
{
    QTest::addColumn<double>("driverThrottle");
    QTest::addColumn<double>("driverBrake");
    QTest::addColumn<double>("driverSteering");

    QTest::addColumn<bool>("brakeAssistActive");
    QTest::addColumn<double>("brakeAssistBrake");

    QTest::addColumn<bool>("laneAssistActive");
    QTest::addColumn<std::vector<double> >("laneAssistBrakeSuperpose");

    QTest::addColumn<bool>("evasiveSteeringActive");
    QTest::addColumn<double>("evasiveSteeringAngle");

    QTest::addColumn<bool>("collisionOccured");

    QTest::addColumn<double>("resultingThrottle");
    QTest::addColumn<double>("resultingBrake");
    QTest::addColumn<double>("resultingSteering");
    QTest::addColumn<std::vector<double>>("resultingBrakeSuperpose");

    QTest::newRow("collision occured +  all systems active") << 1.0 << 0.2 << 0.5
                                                             << true << 0.9
                                                             << true
                                                             << std::vector<double> {0.2, 0.0, 0.2, 0.0}
                                                             << true << 0.7
                                                             << true
                                                             << 0.0 << 1.0 << 0.5
                                                             << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("no system active") << 1.0 << 0.0 << 0.0
                                      << false << 0.0
                                      << false
                                      << std::vector<double> {0.0, 0.0, 0.0, 0.0}
                                      << false << 0.0
                                      << false
                                      << 1.0 << 0.0 << 0.0
                                      << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("evasive assist system active") << 1.0 << 0.2 << 0.5
                                                  << false << 0.0
                                                  << false
                                                  << std::vector<double> {0.0, 0.0, 0.0, 0.0}
                                                  << true << 0.4
                                                  << false
                                                  << 0.0 << 0.0 << 0.4
                                                  << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("brake assist system active") << 1.0 << 0.0 << 0.5
                                                << true << 0.8
                                                << false
                                                << std::vector<double> {0.0, 0.0, 0.0, 0.0}
                                                << false << 0.0
                                                << false
                                                << 0.0 << 0.8 << 0.5
                                                << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("lane assist system active") << 1.0 << 0.2 << 0.5
                                               << false << 0.0
                                               << true
                                               << std::vector<double> {0.2, 0.0, 0.2, 0.0}
                                               << false << 0.0
                                               << false
                                               << 1.0 << 0.2 << 0.5
                                               << std::vector<double> {0.2, 0.0, 0.2, 0.0};

    QTest::newRow("evasive + brake assist system active") << 1.0 << 0.2 << 0.5
                                                          << true << 0.8
                                                          << false
                                                          << std::vector<double> {0.0, 0.0, 0.0, 0.0}
                                                          << true << 0.7
                                                          << false
                                                          << 0.0 << 0.0 << 0.7
                                                          << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("evasive + lane assist system active") << 1.0 << 0.2 << 0.5
                                                         << false << 0.0
                                                         << true
                                                         << std::vector<double> {0.2, 0.0, 0.2, 0.0}
                                                         << true << 0.7
                                                         << false
                                                         << 0.0 << 0.0 << 0.7
                                                         << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("brake + lane assist system active") << 1.0 << 0.2 << 0.5
                                                       << true << 0.8
                                                       << true
                                                       << std::vector<double> {0.2, 0.0, 0.2, 0.0}
                                                       << false << 0.0
                                                       << false
                                                       << 0.0 << 0.8 << 0.5
                                                       << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("all systems active") << 1.0 << 0.2 << 0.5
                                        << true << 0.9
                                        << true
                                        << std::vector<double> {0.2, 0.0, 0.2, 0.0}
                                        << true << 0.7
                                        << false
                                        << 0.0 << 0.0 << 0.7
                                        << std::vector<double> {0.0, 0.0, 0.0, 0.0};
}

void UT_SelectorTest::testCase_perform()
{
    QFETCH(double, driverThrottle);
    QFETCH(double, driverBrake);
    QFETCH(double, driverSteering);

    QFETCH(bool, brakeAssistActive);
    QFETCH(double, brakeAssistBrake);

    QFETCH(bool, laneAssistActive);
    QFETCH(std::vector<double>, laneAssistBrakeSuperpose);

    QFETCH(bool, evasiveSteeringActive);
    QFETCH(double, evasiveSteeringAngle);

    QFETCH(bool, collisionOccured);

    QFETCH(double, resultingThrottle);
    QFETCH(double, resultingBrake);
    QFETCH(double, resultingSteering);
    QFETCH(std::vector<double>, resultingBrakeSuperpose);

    Selector selector;
    selector.Perform(driverThrottle,
                     driverBrake,
                     driverSteering,
                     brakeAssistActive,
                     brakeAssistBrake,
                     laneAssistActive,
                     laneAssistBrakeSuperpose,
                     evasiveSteeringActive,
                     evasiveSteeringAngle,
                     collisionOccured);

    double throttle = selector.GetResultingThrottle();
    double brake = selector.GetResultingBrake();
    double steering = selector.GetResultingSteering();
    std::vector<double> brakeSuperpose = selector.GetResultingBrakeSuperpose();

    if (qFuzzyIsNull(throttle) || qFuzzyIsNull(resultingThrottle))
    {
        QCOMPARE(throttle + 1 , resultingThrottle + 1);
    }
    else
    {
        QCOMPARE(throttle, resultingThrottle);
    }

    if (qFuzzyIsNull(brake) || qFuzzyIsNull(resultingBrake))
    {
        QCOMPARE(brake + 1 , resultingBrake + 1);
    }
    else
    {
        QCOMPARE(brake, resultingBrake);
    }

    if (qFuzzyIsNull(steering) || qFuzzyIsNull(resultingSteering))
    {
        QCOMPARE(steering + 1 , resultingSteering + 1);
    }
    else
    {
        QCOMPARE(steering, resultingSteering);
    }

    for (size_t i = 0; i < brakeSuperpose.size(); ++i)
    {
        if (qFuzzyIsNull(brakeSuperpose.at(i)) || qFuzzyIsNull(resultingBrakeSuperpose.at(i)))
        {
            QCOMPARE(brakeSuperpose.at(i) + 1 , resultingBrakeSuperpose.at(i) + 1);
        }
        else
        {
            QCOMPARE(brakeSuperpose.at(i), resultingBrakeSuperpose.at(i));
        }
    }
}

void UT_SelectorTest::testCase_perform_sequence_data()
{
    QTest::addColumn<int>("numberOfTimeSteps");

    QTest::addColumn<std::vector<double>>("driverThrottle");
    QTest::addColumn<std::vector<double>>("driverBrake");
    QTest::addColumn<std::vector<double>>("driverSteering");

    QTest::addColumn<std::vector<bool>>("brakeAssistActive");
    QTest::addColumn<std::vector<double>>("brakeAssistBrake");

    QTest::addColumn<std::vector<bool>>("laneAssistActive");
    QTest::addColumn<std::vector<std::vector<double>>>("laneAssistBrakeSuperpose");

    QTest::addColumn<std::vector<bool>>("evasiveSteeringActive");
    QTest::addColumn<std::vector<double>>("evasiveSteeringAngle");

    QTest::addColumn<std::vector<bool>>("collisionOccured");

    QTest::addColumn<double>("resultingThrottle");
    QTest::addColumn<double>("resultingBrake");
    QTest::addColumn<double>("resultingSteering");
    QTest::addColumn<std::vector<double>>("resultingBrakeSuperpose");

    QTest::newRow("driver drives")
            << 2

            << std::vector<double> {1.0, 1.0}
            << std::vector<double> {0.0, 0.0}
            << std::vector<double> {0.0, 0.0}

            << std::vector<bool> {false, false}
            << std::vector<double> {0.0, 0.0}

            << std::vector<bool> {false, false}
            << std::vector<std::vector<double>>
    {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}

            << std::vector<bool> {false, false}
            << std::vector<double> {0.0, 0.0}

            << std::vector<bool> {false, false}

            << 1.0 << 0.0 << 0.0
            << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("collision occures then driver drives, all systems active")
            << 2

            << std::vector<double> {1.0, 0.0}
            << std::vector<double> {0.0, 0.2}
            << std::vector<double> {0.7, 0.3}

            << std::vector<bool> {false, true}
            << std::vector<double> {0.0, 0.5}

            << std::vector<bool> {false, true}
            << std::vector<std::vector<double>>
    {{0.0, 0.0, 0.0, 0.0}, {0.2, 0.0, 0.2, 0.0}}

            << std::vector<bool> {false, true}
            << std::vector<double> {0.0, 0.6}

            << std::vector<bool> {true, false}

            << 0.0 << 1.0 << 0.7
            << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("brake system active then no system active")
            << 2

            << std::vector<double> {1.0, 0.0}
            << std::vector<double> {0.0, 0.2}
            << std::vector<double> {0.7, 0.3}

            << std::vector<bool> {true, false}
            << std::vector<double> {0.5, 0.0}

            << std::vector<bool> {false, false}
            << std::vector<std::vector<double>>
    {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}

            << std::vector<bool> {false, false}
            << std::vector<double> {0.0, 0.0}

            << std::vector<bool> {false, false}

            << 0.0 << 0.2 << 0.3
            << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("evasive system active then no system active")
            << 2

            << std::vector<double> {1.0, 0.0}
            << std::vector<double> {0.0, 0.2}
            << std::vector<double> {0.7, 0.3}

            << std::vector<bool> {false, false}
            << std::vector<double> {0.0, 0.0}

            << std::vector<bool> {false, false}
            << std::vector<std::vector<double>>
    {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}

            << std::vector<bool> {true, false}
            << std::vector<double> {0.8, 0.0}

            << std::vector<bool> {false, false}

            << 0.0 << 0.2 << 0.3
            << std::vector<double> {0.0, 0.0, 0.0, 0.0};

    QTest::newRow("lane system active then no system active")
            << 2

            << std::vector<double> {1.0, 0.0}
            << std::vector<double> {0.0, 0.2}
            << std::vector<double> {0.7, 0.3}

            << std::vector<bool> {false, false}
            << std::vector<double> {0.0, 0.0}

            << std::vector<bool> {true, false}
            << std::vector<std::vector<double>>
    {{0.2, 0.0, 0.2, 0.0}, {0.0, 0.0, 0.0, 0.0}}

            << std::vector<bool> {false, false}
            << std::vector<double> {0.0, 0.0}

            << std::vector<bool> {false, false}

            << 0.0 << 0.2 << 0.3
            << std::vector<double> {0.0, 0.0, 0.0, 0.0};
}

void UT_SelectorTest::testCase_perform_sequence()
{
    QFETCH(int, numberOfTimeSteps);

    QFETCH(std::vector<double>, driverThrottle);
    QFETCH(std::vector<double>, driverBrake);
    QFETCH(std::vector<double>, driverSteering);

    QFETCH(std::vector<bool>, brakeAssistActive);
    QFETCH(std::vector<double>, brakeAssistBrake);

    QFETCH(std::vector<bool>, laneAssistActive);
    QFETCH(std::vector<std::vector<double>>, laneAssistBrakeSuperpose);

    QFETCH(std::vector<bool>, evasiveSteeringActive);
    QFETCH(std::vector<double>, evasiveSteeringAngle);

    QFETCH(std::vector<bool>, collisionOccured);

    QFETCH(double, resultingThrottle);
    QFETCH(double, resultingBrake);
    QFETCH(double, resultingSteering);
    QFETCH(std::vector<double>, resultingBrakeSuperpose);

    Selector selector;

    for (int i = 0; i < numberOfTimeSteps; ++i)
    {
        selector.Perform(driverThrottle.at(i),
                         driverBrake.at(i),
                         driverSteering.at(i),
                         brakeAssistActive.at(i),
                         brakeAssistBrake.at(i),
                         laneAssistActive.at(i),
                         laneAssistBrakeSuperpose.at(i),
                         evasiveSteeringActive.at(i),
                         evasiveSteeringAngle.at(i),
                         collisionOccured.at(i));
    }

    double throttle = selector.GetResultingThrottle();
    double brake = selector.GetResultingBrake();
    double steering = selector.GetResultingSteering();
    std::vector<double> brakeSuperpose = selector.GetResultingBrakeSuperpose();

    if (qFuzzyIsNull(throttle) || qFuzzyIsNull(resultingThrottle))
    {
        QCOMPARE(throttle + 1 , resultingThrottle + 1);
    }
    else
    {
        QCOMPARE(throttle, resultingThrottle);
    }

    if (qFuzzyIsNull(brake) || qFuzzyIsNull(resultingBrake))
    {
        QCOMPARE(brake + 1 , resultingBrake + 1);
    }
    else
    {
        QCOMPARE(brake, resultingBrake);
    }

    if (qFuzzyIsNull(steering) || qFuzzyIsNull(resultingSteering))
    {
        QCOMPARE(steering + 1 , resultingSteering + 1);
    }
    else
    {
        QCOMPARE(steering, resultingSteering);
    }

    for (size_t i = 0; i < brakeSuperpose.size(); ++i)
    {
        if (qFuzzyIsNull(brakeSuperpose.at(i)) || qFuzzyIsNull(resultingBrakeSuperpose.at(i)))
        {
            QCOMPARE(brakeSuperpose.at(i) + 1 , resultingBrakeSuperpose.at(i) + 1);
        }
        else
        {
            QCOMPARE(brakeSuperpose.at(i), resultingBrakeSuperpose.at(i));
        }
    }
}
