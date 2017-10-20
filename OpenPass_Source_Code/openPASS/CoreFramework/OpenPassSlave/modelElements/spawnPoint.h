/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnPoint.h
//! @brief This file contains the internal representation of the a spawn point.
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include <list>
#include "runConfig.h"
#include "parameters.h"
#include "spawnPointInterface.h"
#include "spawnPointLibrary.h"
#include "scheduleItem.h"
#include "worldInterface.h"
#include "log.h"

namespace SimulationSlave
{

class SpawnPoint
{
public:    
    SpawnPoint(SimulationCommon::RunConfig::SpawnPointInstance *spawnPointInstance,
               AgentFactory *agentFactory,
               SpawnPointInterface *implementation,
               SpawnPointLibrary *library,
               WorldInterface *world) :
        library(library),
        implementation(implementation),
        agentFactory(agentFactory),
        id(spawnPointInstance->GetId()),
        spawnTask(this, world)
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
    //! Returns the stored list of agent spawn items.
    //!
    //! @return                         List of agent spawn items
    //-----------------------------------------------------------------------------
    std::vector<const AgentSpawnItem*> &GetAgentSpawnItems()
    {
        return agentSpawnItems;
    }

    //-----------------------------------------------------------------------------
    //! Returns the agent factory.
    //!
    //! @return                         Agent factory
    //-----------------------------------------------------------------------------
    AgentFactory *GetAgentFactory()
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
    //! Adds a new agent spawn item to the stored list.
    //!
    //! @param[in]  agentSpawnItem      Agent spawn item to store
    //-----------------------------------------------------------------------------
    void AddAgentSpawnItem(const AgentSpawnItem *agentSpawnItem)
    {
        agentSpawnItems.push_back(agentSpawnItem);
    }

    //-----------------------------------------------------------------------------
    //! Returns the task, i.e. the scheduled spawn item
    //!
    //! @return                         Scheduled spawn item
    //-----------------------------------------------------------------------------
    ScheduleItem *GetSpawnTask()
    {
        return &spawnTask;
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

private:
    SpawnPointLibrary *library;
    SpawnPointInterface *implementation;
    AgentFactory *agentFactory;
    int id;
    std::vector<const AgentSpawnItem*> agentSpawnItems;

    ScheduleSpawnItem spawnTask;
};

} // namespace SimulationSlave

#endif // SPAWNPOINT_H
