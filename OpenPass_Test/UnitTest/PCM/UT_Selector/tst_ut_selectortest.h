/*********************************************************************
* Copyright (c) 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef TST_UT_SELECTORTEST_H
#define TST_UT_SELECTORTEST_H

#include <QString>
#include <QtTest>

#include "selector.h"

class UT_SelectorTest : public QObject
{
    Q_OBJECT

public:
    UT_SelectorTest() = default;
    ~UT_SelectorTest() = default;

private slots:
    void testCase_perform_data();
    void testCase_perform();

    void testCase_perform_sequence_data();
    void testCase_perform_sequence();
};

#endif // TST_UT_SELECTORTEST_H
