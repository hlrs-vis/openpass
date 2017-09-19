/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "parameters.h"
#include "log.h"

namespace SimulationCommon
{

Parameters::~Parameters()
{
    for(std::pair<const int, const std::vector<double>*> &item : parametersDoubleVector)
    {
        delete item.second;
    }
    parametersDoubleVector.clear();

    for(std::pair<const int, const std::vector<int>*> &item : parametersIntVector)
    {
        delete item.second;
    }
    parametersIntVector.clear();

    for(std::pair<const int, const std::vector<bool>*> &item : parametersBoolVector)
    {
        delete item.second;
    }
    parametersBoolVector.clear();
}

bool Parameters::AddParameterDouble(int name, double value)
{
    if(!parametersDouble.insert({name, value}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "parameter names must be unique";
        return false;
    }

    return true;
}

bool Parameters::AddParameterInt(int name, int value)
{
    if(!parametersInt.insert({name, value}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "parameter names must be unique";
        return false;
    }

    return true;
}

bool Parameters::AddParameterBool(int name, bool value)
{
    if(!parametersBool.insert({name, value}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "parameter names must be unique";
        return false;
    }

    return true;
}

bool Parameters::AddParameterDoubleVector(int name, const std::vector<double> *value)
{
    if(!parametersDoubleVector.insert({name, value}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "parameter names must be unique";
        return false;
    }

    return true;
}

bool Parameters::AddParameterIntVector(int name, const std::vector<int> *value)
{
    if(!parametersIntVector.insert({name, value}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "parameter names must be unique";
        return false;
    }

    return true;
}

bool Parameters::AddParameterBoolVector(int name, const std::vector<bool> *value)
{
    if(!parametersBoolVector.insert({name, value}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "parameter names must be unique";
        return false;
    }

    return true;
}

bool Parameters::AddParameterString(int name, const std::string &value)
{
    if(!parametersString.insert({name, value}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "parameter names must be unique";
        return false;
    }

    return true;
}

const std::map<int, double> &Parameters::GetParametersDouble() const
{
    return parametersDouble;
}

const std::map<int, int> &Parameters::GetParametersInt() const
{
    return parametersInt;
}

const std::map<int, bool> &Parameters::GetParametersBool() const
{
    return parametersBool;
}

const std::map<int, const std::string> &Parameters::GetParametersString() const
{
    return parametersString;
}

const std::map<int, const std::vector<double>*> &Parameters::GetParametersDoubleVector() const
{
    return parametersDoubleVector;
}

const std::map<int, const std::vector<int>*> &Parameters::GetParametersIntVector() const
{
    return parametersIntVector;
}

const std::map<int, const std::vector<bool>*> &Parameters::GetParametersBoolVector() const
{
    return parametersBoolVector;
}

} // namespace SimulationCommon
