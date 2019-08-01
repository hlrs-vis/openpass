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
//! @file  SpawnPoint.h
//! @brief This file contains the internal representation of the a spawn point.
//-----------------------------------------------------------------------------

#pragma once

#include <list>

#include "CoreFramework/CoreShare/parameters.h"
#include "Interfaces/spawnPointInterface.h"
#include "spawnPointLibrary.h"
#include "Interfaces/worldInterface.h"
#include "agentBlueprint.h"
#include "CoreFramework/CoreShare/log.h"

namespace SimulationSlave
{

class SpawnPoint
{
public:    
    SpawnPoint(AgentFactoryInterface *agentFactory,
               SpawnPointInterface *implementation,
               SpawnPointLibrary *library) :
        library(library),
        implementation(implementation),
        agentFactory(agentFactory)
    {
        LOG_INTERN(LogLevel::DebugCore) << "created spawn point " << id;
    }
    SpawnPoint(const SpawnPoint&) = delete;
    SpawnPoint(SpawnPoint&&) = delete;
    SpawnPoint& operator=(const SpawnPoint&) = delete;
    SpawnPoint& operator=(SpawnPoint&&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor, releases the spawn point from the stored library.
    //-----------------------------------------------------------------------------
    virtual ~SpawnPoint()
    {
        library->ReleaseSpawnPoint(this);
        LOG_INTERN(LogLevel::DebugCore) << "destroyed spawn point " << id;
    }

    //-----------------------------------------------------------------------------
    //! Return the ID of the respective spawn point instance.
    //!
    //! @return                         ID of the respective spawn point instance
    //-----------------------------------------------------------------------------
    int GetId() const
    {
        return id;
    }

    //-----------------------------------------------------------------------------
    //! Returns the agent factory.
    //!
    //! @return                         Agent factory
    //-----------------------------------------------------------------------------
    AgentFactoryInterface *GetAgentFactory()
    {
        return agentFactory;
    }

    //-----------------------------------------------------------------------------
    //! Returns the spawn point interface.
    //!
    //! @return                         Spawn point interface
    //-----------------------------------------------------------------------------
    SpawnPointInterface *GetImplementation()
    {
        return implementation;
    }

    //-----------------------------------------------------------------------------
    //! Returns the spawn point library.
    //!
    //! @return                         Spawn point library
    //-----------------------------------------------------------------------------
    SpawnPointLibrary *GetLibrary()
    {
        return library;
    }

    Agent* respawnAgent(int time)
    {
        AgentBlueprint agentBlueprint;

        try
        {
            GetLibrary()->GenerateAgent(GetImplementation(), &agentBlueprint);
        }
        catch(...)
        {
            return nullptr;
        }

        Agent *agent = GetAgentFactory()->AddAgent(&agentBlueprint, time);

        return agent;
    }

private:
    SpawnPointLibrary *library;
    SpawnPointInterface *implementation;
    AgentFactoryInterface *agentFactory;
    int id = 0;
};

} // namespace SimulationSlave


