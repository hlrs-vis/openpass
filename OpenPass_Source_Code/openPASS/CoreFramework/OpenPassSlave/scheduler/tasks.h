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

//-----------------------------------------------------------------------------
/** \file  Tasks.h
*   \brief This file contains TaskItem and Tasks
*   \details TaskItems are the data types to store in task set.
*           Each task has a function for execution.
*/
//-----------------------------------------------------------------------------

#include <exception>
#include <set>
#include <functional>

namespace SimulationSlave {
namespace Scheduling {

enum TaskType
{
    Trigger,
    Update,
    Spawning,
    EventDetector,
    Manipulator,
    Observation,
    UpdateGlobalDrivingView,
    SyncGlobalData
};

//-----------------------------------------------------------------------------
/** \brief handles data to store scheduler tasks
*
*   \ingroup OpenPassSlave
*/
//-----------------------------------------------------------------------------

class TaskItem
{
public:
    int agentId;
    int priority;
    int cycletime;
    int delay;
    TaskType taskType;
    std::function<bool()> func;

    TaskItem(int agentId, int priority, int cycleTime, int delay, TaskType taskType, std::function<bool()> func) :
        agentId(agentId),
        priority(priority),
        cycletime(cycleTime),
        delay(delay),
        taskType(taskType),
        func(func)
    {}
    virtual ~TaskItem() = default;

    static constexpr int VALID_FOR_ALL_AGENTS = -1;
    static constexpr int NO_DELAY = 0;

    static constexpr int PRIORITY_SPAWNING = 5;
    static constexpr int PRIORITY_EVENTDETECTOR = 4;
    static constexpr int PRIORITY_MANIPULATOR = 3;
    static constexpr int PRIORITY_SYNCGLOBALDATA = 2;
    static constexpr int PRIORITY_UPDATEGLOBALDRIVINGVIEW = 1;
    static constexpr int PRIORITY_OBSERVATION = 0;

    bool operator<(const TaskItem& rhs) const;
    bool operator==(const TaskItem& rhs) const;
};

/*!
* \brief voidFunctionWrapper
*
* \details function of taskItem usually returns bool, so for void this function wraps this
*
*
* @param[in]     function   void function
* @return        bool       true if func was executed
*/
inline bool voidFunctionWrapper(std::function<void()> func)
{
    func();
    return true;
}

//-----------------------------------------------------------------------------
/** \brief taskItem for triggering task */
//-----------------------------------------------------------------------------

class TriggerTaskItem : public TaskItem
{
public:
    TriggerTaskItem(int agentId, int priority, int cycleTime, int delay, std::function<bool()> func) :
        TaskItem(agentId, priority, cycleTime, delay, TaskType::Trigger, func) {}
};

//-----------------------------------------------------------------------------
/** \brief taskItem for update task */
//-----------------------------------------------------------------------------

class UpdateTaskItem : public TaskItem
{
public:
    UpdateTaskItem(int agentId, int priority, int cycleTime, int delay, std::function<bool()> func) :
        TaskItem(agentId, priority, cycleTime, delay, TaskType::Update, func)
    {
    }
};

//-----------------------------------------------------------------------------
/** \brief taskItem for spawning task */
//-----------------------------------------------------------------------------

class SpawningTaskItem : public TaskItem
{
public:
    SpawningTaskItem(int cycleTime, std::function<bool()> func) :
        TaskItem(VALID_FOR_ALL_AGENTS, PRIORITY_SPAWNING, cycleTime, NO_DELAY,
                 TaskType::Spawning,
                 func) { }
};

//-----------------------------------------------------------------------------
/** \brief taskItem for eventDetector task */
//-----------------------------------------------------------------------------

class EventDetectorTaskItem : public TaskItem
{
public:
    EventDetectorTaskItem(int cycleTime, std::function<void()> func) :
        TaskItem(VALID_FOR_ALL_AGENTS, PRIORITY_EVENTDETECTOR, cycleTime, NO_DELAY,
                 TaskType::EventDetector,
                 std::bind(&voidFunctionWrapper, func)) { }
};

//-----------------------------------------------------------------------------
/** \brief taskItem for manipulator task */
//-----------------------------------------------------------------------------

class ManipulatorTaskItem : public TaskItem
{
public:
    ManipulatorTaskItem(int cycleTime, std::function<void()> func) :
        TaskItem(VALID_FOR_ALL_AGENTS, PRIORITY_MANIPULATOR, cycleTime, NO_DELAY,
                 TaskType::Manipulator,
                 std::bind(&voidFunctionWrapper, func)) { }
};

//-----------------------------------------------------------------------------
/** \brief taskItem for observation task */
//-----------------------------------------------------------------------------

class ObservationTaskItem : public TaskItem
{
public:
    ObservationTaskItem(int cycleTime, std::function<bool()> task) :
        TaskItem(VALID_FOR_ALL_AGENTS, PRIORITY_OBSERVATION, cycleTime, NO_DELAY,
                 TaskType::Observation, task)
    {
    }
};

//-----------------------------------------------------------------------------
/** \brief taskItem for synchronize world task */
//-----------------------------------------------------------------------------

class SyncWorldTaskItem : public TaskItem
{
public:
    SyncWorldTaskItem(int cycleTime, std::function<void()> func) :
        TaskItem(VALID_FOR_ALL_AGENTS, PRIORITY_SYNCGLOBALDATA, cycleTime, NO_DELAY,
                 TaskType::SyncGlobalData,
                 std::bind(&voidFunctionWrapper, func)) { }
};

//-----------------------------------------------------------------------------
/** \brief taskItem for update global driving view task */
//-----------------------------------------------------------------------------

class UpdateGlobalDrivingViewTaskItem : public TaskItem
{
public:
    UpdateGlobalDrivingViewTaskItem(int cycleTime, std::function<bool()> func) :
        TaskItem(VALID_FOR_ALL_AGENTS, PRIORITY_UPDATEGLOBALDRIVINGVIEW, cycleTime, NO_DELAY,
                 TaskType::UpdateGlobalDrivingView, func)
    {
    }
};

//-----------------------------------------------------------------------------
/** \brief stores taskItems in multiset
*
*   \ingroup OpenPassSlave
*/
//-----------------------------------------------------------------------------

class Tasks
{

public:
    /*!
    * \brief AddTask
    *
    * \details add given taskItem to intern multiset tasks
    *
    *
    * @param[in]     TaskItem    subclass of taskItem
    */
    void AddTask(const TaskItem& newTask);

    /*!
    * \brief DeleteTasks
    *
    * \details add given taskItem to intern multiset tasks
    *
    *
    * @param[in]     int    id of removed agent to filter it out of tasks
    */
    void DeleteTasks(int agentId);

    std::multiset<TaskItem> tasks;
};


} // namespace Scheduling
} // namespace SimulationSlave
