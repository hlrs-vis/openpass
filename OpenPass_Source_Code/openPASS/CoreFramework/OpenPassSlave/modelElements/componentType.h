/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  componentType.h
//! @brief This file contains the internal representation of the type of a
//!        model component as given by the configuration.
//-----------------------------------------------------------------------------

#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

#include <map>
#include "modelInterface.h"
#include "parameters.h"

namespace SimulationSlave
{

class ComponentType
{
public:
    ComponentType(int id,
                  bool isInit,
                  int priority,
                  int offsetTime,
                  int responseTime,
                  int cycleTime,
                  const std::string &modelLibrary);
    ComponentType(const ComponentType&) = delete;
    ComponentType(ComponentType&&) = delete;
    ComponentType& operator=(const ComponentType&) = delete;
    ComponentType& operator=(ComponentType&&) = delete;
    virtual ~ComponentType() = default;

    bool AddInputLink(int inputRef, int linkId);
    bool AddOutputLink(int outputRef, int linkId);
    bool AddObservationLink(int observationRef, int linkId);
    int GetId()
    {
        return id;
    }

    std::map<int, int> &GetInputLinks()
    {
        return inputs;
    }

    std::map<int, int> &GetOutputLinks()
    {
        return outputs;
    }

    std::map<int, int> &GetObservationLinks()
    {
        return observations;
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

    SimulationCommon::ModelParameters &GetModelParameters()
    {
        return modelParameters;
    }

private:
    int id;
    bool isInit;
    int priority;
    int offsetTime;
    int responseTime;
    int cycleTime;
    std::string modelLibrary;
    std::map<int, int> inputs;
    std::map<int, int> outputs;
    std::map<int, int> observations;
    SimulationCommon::ModelParameters modelParameters;
};

} // namespace SimulationSlave

#endif // COMPONENTTYPE_H
