/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  parameters.h
//! @brief This file contains the interface of the internal representation of
//!        configuration parameters.
//-----------------------------------------------------------------------------

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <vector>
#include <map>
#include "parameterInterface.h"

namespace SimulationCommon
{

class Parameters : public ParameterInterface
{
public:
    Parameters() = default;
    Parameters(const Parameters&) = delete;
    Parameters(Parameters&&) = delete;
    Parameters& operator=(const Parameters&) = delete;
    Parameters& operator=(Parameters&&) = delete;
    virtual ~Parameters();

    bool AddParameterDouble(int name, double value);
    bool AddParameterInt(int name, int value);
    bool AddParameterBool(int name, bool value);
    bool AddParameterString(int name, const std::string &value);
    bool AddParameterDoubleVector(int name, const std::vector<double> *value);
    bool AddParameterIntVector(int name, const std::vector<int> *value);
    bool AddParameterBoolVector(int name, const std::vector<bool> *value);

    virtual const std::map<int, double> &GetParametersDouble() const;
    virtual const std::map<int, int> &GetParametersInt() const;
    virtual const std::map<int, bool> &GetParametersBool() const;
    virtual const std::map<int, const std::string> &GetParametersString() const;
    virtual const std::map<int, const std::vector<double>*> &GetParametersDoubleVector() const;
    virtual const std::map<int, const std::vector<int>*> &GetParametersIntVector() const;
    virtual const std::map<int, const std::vector<bool>*> &GetParametersBoolVector() const;

protected:
    std::map<int, double> parametersDouble;
    std::map<int, int> parametersInt;
    std::map<int, bool> parametersBool;
    std::map<int, const std::string> parametersString;
    std::map<int, const std::vector<double>*> parametersDoubleVector;
    std::map<int, const std::vector<int>*> parametersIntVector;
    std::map<int, const std::vector<bool>*> parametersBoolVector;
};

class ModelParameters : public Parameters
{};

class SpawnPointParameters : public Parameters
{};

class ObservationParameters : public Parameters
{};

} // namespace SimulationCommon

#endif // PARAMETERS_H
