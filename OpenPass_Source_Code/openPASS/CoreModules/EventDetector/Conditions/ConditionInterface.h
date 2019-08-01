/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <string>

enum class Rule
{
    LessThan = 0,
    EqualTo,
    GreaterThan
};

class ConditionInterface
{
public:
    ConditionInterface() = default;
    virtual ~ConditionInterface() = default;

    /*!
     * ------------------------------------------------------------------------
     * \brief IsMet returns whether a condition is met based on a double
     *        parameter.
     *
     * \param[in] value the double parameter with which to determine if the
     *            condition is met.
     *
     * \returns true if the condition is met, false otherwise.
     * ------------------------------------------------------------------------
     */
    virtual bool IsMet(double value) const = 0;

    ConditionInterface(const ConditionInterface&) = delete;
    ConditionInterface(ConditionInterface&&) = delete;
    ConditionInterface& operator=(const ConditionInterface&) = delete;
    ConditionInterface& operator=(ConditionInterface&&) = delete;
};
