/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  scheduler.h
//! @brief This file contains the generic schedule managing component which
//!        refers to concrete scheduling policies.
//-----------------------------------------------------------------------------

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <functional>
#include "collisionDetection.h"
#include "worldInterface.h"

namespace SimulationSlave
{

class RunResult;
class SchedulePolicy;
class SpawnpointNetwork;

class Scheduler
{
public:
    Scheduler(WorldInterface *world, SpawnPointNetwork *spawnPointNetwork, CollisionDetection *collisionDetection);
    Scheduler(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;
    virtual ~Scheduler() = default;

    //-----------------------------------------------------------------------------
    //! Init schedule policy and create world global driving view. Then, as long as
    //! run has not ended, sync the global data of the agent network in world, recreate
    //! the world global driving view. call the update function pointer and check,
    //! if the result is due to an end condition.
    //!
    //! @param[in]  updateCallback      Function pointer to the update method
    //! @param[in]  endCallback         Function pointer to the method to check if
    //!                                 the run has stopped
    //! @param[in]  start               Start time
    //! @param[in]  end                 End time
    //! @param[out] runResult           Reference to result storage location
    //-----------------------------------------------------------------------------
    void Run(const std::function<bool(int, RunResult&)> &updateCallback,
             const std::function<bool()> &endCallback,
             int start,
             int end,
             RunResult &runResult);

private:    
    SchedulePolicy schedulePolicy;
    WorldInterface *world;
    CollisionDetection *collisionDetection;
};

} // namespace SimulationSlave

#endif // SCHEDULER_H
