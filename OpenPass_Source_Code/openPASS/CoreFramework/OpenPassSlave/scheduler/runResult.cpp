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
#include <utility>

#include "CoreFramework/CoreShare/log.h"
#include "runResult.h"
//-----------------------------------------------------------------------------
/** \file  RunResult.cpp */
//-----------------------------------------------------------------------------

namespace SimulationSlave
{

void RunResult::AddCollisionId(const int agentId)
{
    //SetCollision
    result |= maskCollision;

    //Insert new collision ids
    if(std::find(collisionIds.begin(), collisionIds.end(), agentId) == collisionIds.end())
    {
        collisionIds.push_back(agentId);
    }
}

} // namespace SimulationSlave
