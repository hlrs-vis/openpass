/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <map>
#include <string>

namespace EventDefinitions
{

enum struct EventCategory
{
    Undefined = 0,
    Basic,
    AgentBased,
    Collision,
    ComponentStateChange
};

enum struct EventType
{
    Undefined = 0,
    Collision,
    Conditional,
    AEBActive,
    AEBInactive,
    ComponentStateChange,
    TrajectoryFollowerActivated,
    TrajectoryFollowerDeactivated
};

const std::string EventTypeStrings[] =
{
    "Undefined",
    "Collision",
    "Conditional",
    "AEBActive",
    "AEBInactive",
    "ComponentStateChange",
    "TrajectoryFollowerActivated",
    "TrajectoryFollowerDeactivated"
};

const std::map<std::string, EventType> eventTypesMap = {{"Collision", EventType::Collision},
                                                        {"Conditional", EventType::Conditional}};
}//namespace EventDefinitions
