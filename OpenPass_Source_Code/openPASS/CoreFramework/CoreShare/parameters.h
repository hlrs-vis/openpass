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

//-----------------------------------------------------------------------------
//! @file  Parameters.h
//! @brief This file contains the interface of the internal representation of
//!        configuration parameters.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <list>
#include <map>
#include "Interfaces/parameterInterface.h"

namespace SimulationCommon {

class Parameters : public ParameterInterface
{
public:
    Parameters() = default;
    virtual ~Parameters() override = default;

    Parameters(const ParameterInterface& other) :
        parametersDouble{ other.GetParametersDouble()},
        parametersInt{other.GetParametersInt()},
        parametersBool{other.GetParametersBool()},
        parametersString{other.GetParametersString()},
        parametersDoubleVector{other.GetParametersDoubleVector()},
        parametersIntVector{other.GetParametersIntVector()},
        parametersBoolVector{other.GetParametersBoolVector()},
        parametersStringVector{other.GetParametersStringVector()},
        parametersNormalDistribution{other.GetParametersNormalDistribution()},
        parameterLists{other.GetParameterLists()} {}

    bool AddParameterDouble(std::string name, double value) override;
    bool AddParameterInt(std::string name, int value) override;
    bool AddParameterBool(std::string name, bool value) override;
    bool AddParameterString(std::string name, const std::string& value) override;
    bool AddParameterDoubleVector(std::string name, const std::vector<double> value) override;
    bool AddParameterIntVector(std::string name, const std::vector<int> value) override;
    bool AddParameterBoolVector(std::string name, const std::vector<bool> value) override;
    bool AddParameterStringVector(std::string name, const std::vector<std::string> value) override;
    bool AddParameterNormalDistribution(std::string name,
                                        const StochasticDefintions::NormalDistributionParameter value) override;

    ParameterInterface& InitializeListItem(std::string key) override;

    //NOTE: The primitive getters are in header on purpose

    virtual const std::map<std::string, double>& GetParametersDouble() const override
    {
        return parametersDouble;
    }

    virtual const std::map<std::string, int>& GetParametersInt() const override
    {
        return parametersInt;
    }

    virtual const std::map<std::string, bool>& GetParametersBool() const override
    {
        return parametersBool;
    }

    virtual const std::map<std::string, const std::string>& GetParametersString() const override
    {
        return parametersString;
    }

    virtual const std::map<std::string, const std::vector<double>>& GetParametersDoubleVector() const override
    {
        return parametersDoubleVector;
    }

    virtual const std::map<std::string, const std::vector<int>>& GetParametersIntVector() const override
    {
        return parametersIntVector;
    }

    virtual const std::map<std::string, const std::vector<bool>>& GetParametersBoolVector() const override
    {
        return parametersBoolVector;
    }

    virtual const std::map<std::string, const std::vector<std::string>>& GetParametersStringVector() const override
    {
        return parametersStringVector;
    }

    virtual const std::map<std::string, const StochasticDefintions::NormalDistributionParameter>&
    GetParametersNormalDistribution() const override
    {
        return parametersNormalDistribution;
    }

    virtual const std::map<std::string, ParameterLists>& GetParameterLists() const override
    {
        return parameterLists;
    }

protected:
    std::map<std::string, double> parametersDouble;
    std::map<std::string, int> parametersInt;
    std::map<std::string, bool> parametersBool;
    std::map<std::string, const std::string> parametersString;
    std::map<std::string, const std::vector<double>> parametersDoubleVector;
    std::map<std::string, const std::vector<int>> parametersIntVector;
    std::map<std::string, const std::vector<bool>> parametersBoolVector;
    std::map<std::string, const std::vector<std::string>> parametersStringVector;
    std::map<std::string, const StochasticDefintions::NormalDistributionParameter> parametersNormalDistribution;
    std::map<std::string, ParameterLists> parameterLists;
};

class ModelParameters : public Parameters
{};

class SpawnPointParameters : public Parameters
{};

class ObservationParameters : public Parameters
{};

class WorldParameters : public Parameters
{};

class EventDetectorConditionParameters : public Parameters
{
public:
    enum class ConditionType {
        SimulationTimeCondition = 0
    };
    ConditionType GetType() const
    {
        return conditionType;
    }

    void SetType(const ConditionType conditionType)
    {
        this->conditionType = conditionType;
    }
private:
    ConditionType conditionType;
};

class EventDetectorParameters : public Parameters
{
public:
    std::string GetType() const
    {
        return type;
    }

    void SetType(std::string type)
    {
        this->type = type;
    }

    virtual const std::list<EventDetectorConditionParameters> &GetConditionParameters() const
    {
        return conditionParameters;
    }

    void AddConditionParameters(const EventDetectorConditionParameters conditionParameters)
    {
        this->conditionParameters.push_back(conditionParameters);
    }

private:
    std::string type;
    std::list<EventDetectorConditionParameters> conditionParameters;
};

class ManipulatorParameters : public Parameters
{
public:
    std::string GetType() const
    {
        return type;
    }
    void SetType(const std::string& type)
    {
        this->type = type;
    }

private:
    std::string type;
};

} // namespace SimulationCommon


