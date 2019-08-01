/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "parameters.h"
#include "log.h"
#include <memory>

namespace SimulationCommon {

template <typename Map, typename Value >
bool CheckedInsert(Map& map, std::string name, Value& value)
{
    auto inserted = std::get<1>(map.try_emplace(name, value));
    if (!inserted)
    {
        LOG_INTERN(LogLevel::Error) << "trying to insert parameter " + name + " twice, must be unique";
    }
    return inserted;
}

bool Parameters::AddParameterDouble(std::string name, double value)
{
    return CheckedInsert(parametersDouble, name, value);
}

bool Parameters::AddParameterInt(std::string name, int value)
{
    return CheckedInsert(parametersInt, name, value);
}

bool Parameters::AddParameterBool(std::string name, bool value)
{
    return CheckedInsert(parametersBool, name, value);
}

bool Parameters::AddParameterDoubleVector(std::string name, const std::vector<double> value)
{
    return CheckedInsert(parametersDoubleVector, name, value);
}

bool Parameters::AddParameterIntVector(std::string name, const std::vector<int> value)
{
    return CheckedInsert(parametersIntVector, name, value);
}

bool Parameters::AddParameterBoolVector(std::string name, const std::vector<bool> value)
{
    return CheckedInsert(parametersBoolVector, name, value);
}

bool Parameters::AddParameterString(std::string name, const std::string& value)
{
    return CheckedInsert(parametersString, name, value);
}

bool Parameters::AddParameterStringVector(std::string name, const std::vector<std::string> value)
{
    return CheckedInsert(parametersStringVector, name, value);
}

bool Parameters::AddParameterNormalDistribution(std::string name,
        const StochasticDefintions::NormalDistributionParameter value)
{
    return CheckedInsert(parametersNormalDistribution, name, value);
}

ParameterInterface& Parameters::InitializeListItem(std::string key)
{
    auto& element = parameterLists[key];
    element.emplace_back(std::make_shared<ModelParameters>());
    return *element.back().get();
}

} // namespace SimulationCommon
