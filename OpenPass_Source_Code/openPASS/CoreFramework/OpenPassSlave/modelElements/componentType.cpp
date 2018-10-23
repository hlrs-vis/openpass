/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <algorithm>
#include <iostream>
#include "componentType.h"
#include "modelLibrary.h"
#include "parameters.h"
#include "log.h"

namespace SimulationSlave
{

ComponentType::ComponentType(int id,
                             bool isInit,
                             int priority,
                             int offsetTime,
                             int responseTime,
                             int cycleTime,
                             const std::string &modelLibrary) :
    id(id),
    isInit(isInit),
    priority(priority),
    offsetTime(offsetTime),
    responseTime(responseTime),
    cycleTime(cycleTime),
    modelLibrary(modelLibrary)
{}

bool ComponentType::AddInputLink(int inputRef, int linkId)
{
    if(!inputs.insert({linkId, inputRef}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "input must be unique";
        return false;
    }

    return true;
}

bool ComponentType::AddOutputLink(int outputRef, int linkId)
{
    if(!outputs.insert({linkId, outputRef}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "output must be unique";
        return false;
    }

    return true;
}

bool ComponentType::AddObservationLink(int observationRef, int linkId)
{
    if(!observations.insert({linkId, observationRef}).second)
    {
        LOG_INTERN(LogLevel::Warning) << "observation must be unique";
        return false;
    }

    return true;
}

} // namespace SimulationSlave
