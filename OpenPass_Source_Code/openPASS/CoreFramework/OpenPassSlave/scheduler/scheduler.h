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
//! @file  Scheduler.h
//! @brief This file contains the generic schedule managing component
//-----------------------------------------------------------------------------

#pragma once

#include <functional>
#include <memory>
#include "worldInterface.h"
#include "taskBuilder.h"
#include "schedulerTasks.h"
#include "spawnControlInterface.h"
#include "spawnControl.h"

namespace SimulationSlave
{

class RunResult;
class EventNetworkInterface;
class SchedulePolicy;
class SpawnPointNetworkInterface;

enum class SchedulerReturnState
{
    NoError,
    AbortInvocation,
    AbortSimulation
};

enum class ExecuteReturnState
{
    NoError,
    Potential,
    FatalError
};

//-----------------------------------------------------------------------------
/** \brief execute all tasks for an simulation run
* 	\details The scheduler triggers TaskBuilder to build up common tasks and
*           SchedulerTasks to manage sorting of all tasks. Each timestep all
*           given tasks are executed.
*
* 	\ingroup OpenPassSlave
*/
//-----------------------------------------------------------------------------
class Scheduler
{
public:
    Scheduler(WorldInterface *world,
              SpawnPointNetworkInterface *spawnPointNetwork,
              EventDetectorNetworkInterface *eventDetectorNetwork,
              ManipulatorNetworkInterface *manipulatorNetwork,
              ObservationNetworkInterface *observationNetwork);
    Scheduler(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;

    /*!
    * \brief Run
    *
    * \details execute all tasks for one simulation run
    *
    *
    * @param[in]     startTime              simulation start
    * @param[in]     endTime                simulation end
    * @param[in]     EventNetworkInterface  EventNetwork
    * @param[out]    runResult              RunResult
    * @returns SchedulerReturnState for invocation control
    */
    SchedulerReturnState Run(int startTime,
             int endTime,
             RunResult &runResult,
             EventNetworkInterface *eventNetwork);

    /*!
    * \brief ScheduleAgentTasks
    *
    * \details schedule all tasks of an new agent
    *           e.g. for respawning
    *
    * @param[in]     Agent    new agent
    */
    void ScheduleAgentTasks(const Agent &agent);

private:    
    WorldInterface *world;
    SpawnPointNetworkInterface *spawnPointNetwork = nullptr;
    EventDetectorNetworkInterface *eventDetectorNetwork = nullptr;
    ManipulatorNetworkInterface *manipulatorNetwork = nullptr;
    ObservationNetworkInterface *observationNetwork = nullptr;

    const TaskItem* failedTaskItem;

    int currentTime;

    constexpr static int frameworkUpdateRate = 100;

    /*!
    * \brief UpdateAgents
    *
    * \details schedule new agents and remove deleted ones
    *
    *
    * @param[in]     SchedulerTasks         logic for scheduling tasks
    * @param[out]    SpawnControl           controlling of spawning
    * @param[out]    WorldInterface         world
    */
    void UpdateAgents(SpawnControlInterface &spawnControl, WorldInterface *world);

    /*!
    * \brief ExecuteTasks
    *
    * \details execute function of given task
    *
    *
    * @param[in]     tasks     execute function of given tasks
    * @return                  false, if a task reports error
    */
    template<typename T>
    bool ExecuteTasks(T tasks);

    SchedulerReturnState ParseAbortReason(const SpawnControl& spawnControl, int currentTime);

    std::unique_ptr<SchedulerTasks> taskList;

    friend class Scheduler_UpdateAgents_ScheduleNewTasks_Test;
};

} // namespace SimulationSlave


