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

#include "ConditionInterface.h"
#include "Interfaces/parameterInterface.h"

class SimulationTimeCondition : public ConditionInterface
{
public:
    SimulationTimeCondition(const ParameterInterface &parameters);
    virtual ~SimulationTimeCondition() = default;

    virtual bool IsMet(double value) const override;

    SimulationTimeCondition() = delete;
    SimulationTimeCondition(const SimulationTimeCondition&) = delete;
    SimulationTimeCondition(SimulationTimeCondition&&) = delete;
    SimulationTimeCondition& operator=(const SimulationTimeCondition&) = delete;
    SimulationTimeCondition& operator=(SimulationTimeCondition&&) = delete;

private:
    Rule rule;
    double targetValue;
};
