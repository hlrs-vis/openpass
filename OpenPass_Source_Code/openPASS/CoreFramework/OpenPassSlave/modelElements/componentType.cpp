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

#include <algorithm>
#include <iostream>

#include "componentType.h"
#include "CoreFramework/CoreShare/log.h"
#include "modelLibrary.h"
#include "CoreFramework/CoreShare/parameters.h"

namespace SimulationSlave
{

ComponentType::ComponentType(std::string name,
                             bool isInit,
                             int priority,
                             int offsetTime,
                             int responseTime,
                             int cycleTime,
                             const std::string &modelLibrary) :
    name(name),
    isInit(isInit),
    priority(priority),
    offsetTime(offsetTime),
    responseTime(responseTime),
    cycleTime(cycleTime),
    modelLibrary(modelLibrary)
{}

bool ComponentType::AddInputLink(int localLinkId, int channelId)
{
    if(!inputs.insert({localLinkId, channelId}).second)
    {
        LOG_INTERN(LogLevel::Error) << "input must be unique";
        return false;
    }

    return true;
}

bool ComponentType::AddOutputLink(int localLinkId, int channelId)
{
    if(!outputs.insert({localLinkId, channelId}).second)
    {
        LOG_INTERN(LogLevel::Error) << "output must be unique";
        return false;
    }

    return true;
}

std::string ComponentType::GetName() const
{
    return name;
}

} // namespace SimulationSlave
