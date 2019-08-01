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
//! @file  ComponentType.h
//! @brief This file contains the internal representation of the type of a
//!        model component as given by the configuration.
//-----------------------------------------------------------------------------

#pragma once

#include <map>
#include "Interfaces/modelInterface.h"
#include "Interfaces/parameterInterface.h"

namespace SimulationSlave
{

class ComponentType
{
public:
    ComponentType(std::string name,
                  bool isInit,
                  int priority,
                  int offsetTime,
                  int responseTime,
                  int cycleTime,
                  const std::string &modelLibrary);
    ComponentType(const ComponentType&) = default;
    ComponentType(ComponentType&&) = delete;
    ComponentType& operator=(const ComponentType&) = default;
    ComponentType& operator=(ComponentType&&) = default;
    ~ComponentType() = default;

    bool AddInputLink(int localLinkId, int channelId);
    bool AddOutputLink(int localLinkId, int channelId);

    const std::map<int, int> &GetInputLinks() const
    {
        return inputs;
    }

    const std::map<int, int> &GetOutputLinks() const
    {
        return outputs;
    }

    bool GetInit() const
    {
        return isInit;
    }

    int GetPriority() const
    {
        return priority;
    }

    int GetOffsetTime() const
    {
        return offsetTime;
    }

    int GetResponseTime() const
    {
        return responseTime;
    }

    int GetCycleTime() const
    {
        return cycleTime;
    }

    std::string GetModelLibrary()
    {
        return modelLibrary;
    }

    ParameterInterface *GetModelParameters()
    {
        return parameters;
    }

    void SetModelParameter(ParameterInterface* parameters)
    {
        this->parameters = parameters;
    }

    std::string GetName() const;

private:
    std::string name = "";
    bool isInit = false;
    int priority = -999;
    int offsetTime = -999;
    int responseTime = -999;
    int cycleTime = -999;
    std::string modelLibrary = "";
    std::map<int, int> inputs;
    std::map<int, int> outputs;
    ParameterInterface* parameters = nullptr;
};

} // namespace SimulationSlave


