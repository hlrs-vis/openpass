/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  schedulePolicy.h
//! @brief This file implements the schedule policy which can be exchanged
//!        in the scheduler.
//-----------------------------------------------------------------------------

#ifndef SCHEDULEPOLICY_H
#define SCHEDULEPOLICY_H

#include <list>
#include <map>

namespace SimulationSlave
{

class SpawnPointNetwork;
class ScheduleItem;
class Agent;
class Component;
class RunResult;

//-----------------------------------------------------------------------------
//! Represents a basic priority list for schedule items
//-----------------------------------------------------------------------------
class PriorityList
{
public:
    PriorityList(int relativeOffset);
    PriorityList(const PriorityList&) = delete;
    PriorityList(PriorityList&&) = delete;
    PriorityList& operator=(const PriorityList&) = delete;
    PriorityList& operator=(PriorityList&&) = delete;
    virtual ~PriorityList() = default;

    //-----------------------------------------------------------------------------
    //! Decreases the stored relative offset by the provided value.
    //!
    //! @param[in]  subtractOffset      Value by with the stored relative offset
    //!                                 is decreased
    //! @return                         Flag if subtraction could be performed
    //-----------------------------------------------------------------------------
    bool SubtractRelativeOffset(int subtractOffset);

    //-----------------------------------------------------------------------------
    //! Returns the stored relative offset.
    //!
    //! @return                         Stored relative offset
    //-----------------------------------------------------------------------------
    int GetRelativeOffset() const;

    //-----------------------------------------------------------------------------
    //! Inserts schedule item in priority list depending on its priority.
    //!
    //! @param[in]  scheduleItem        Schedule item to add to the priority list
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool InsertScheduleItem(ScheduleItem *scheduleItem);

    //-----------------------------------------------------------------------------
    //! Returns if the priority list is empty.
    //!
    //! @return                         True if empty, falso otherwise
    //-----------------------------------------------------------------------------
    bool IsEmpty();

    //-----------------------------------------------------------------------------
    //! Removes the task with the highest priority and returns it.
    //!
    //! @param[out]  task               The task with the highest priority
    //! @return                         False if the priority list is empty, true
    //!                                 otherwise
    //-----------------------------------------------------------------------------
    bool RemoveNextTask(ScheduleItem **task);

    //-----------------------------------------------------------------------------
    //! Prints a textual representation of the priority list.
    //-----------------------------------------------------------------------------
    void PrintList();

private:
    int relativeOffset;
    std::list<ScheduleItem*> tasks;
};

//-----------------------------------------------------------------------------
//! Scheduler for items, represented as list of time frames realised as priority
//! lists.
//-----------------------------------------------------------------------------
class SchedulePolicy
{
public:
    SchedulePolicy(SpawnPointNetwork *spawnPointNetwork);
    SchedulePolicy(const SchedulePolicy&) = delete;
    SchedulePolicy(SchedulePolicy&&) = delete;
    SchedulePolicy& operator=(const SchedulePolicy&) = delete;
    SchedulePolicy& operator=(SchedulePolicy&&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor, clears the tasks.
    //-----------------------------------------------------------------------------
    virtual ~SchedulePolicy();

    //-----------------------------------------------------------------------------
    //! Inits the scheduler run by inserting all spawn point tasks from the world's
    //! spawn point network.
    //!
    //! @param[in]   startTime          Start time of the run
    //! @param[in]   endTime            End time of the run
    //! @return                         False if a spawn point task could not be
    //!                                 added, true otherwise
    //-----------------------------------------------------------------------------
    bool InitSchedule(int startTime, int endTime);

    //-----------------------------------------------------------------------------
    //! Returns the next task in the schedule by removing it from the priority list
    //! in the first time slot.
    //!
    //! @param[out]   task              Next task in the schedule
    //! @param[out]   runResult         Reference to result storage location
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool GetTask(ScheduleItem **task,
                 RunResult &runResult) const;

    //-----------------------------------------------------------------------------
    //! Schedule the given task.
    //!
    //! @param[in]   task               Task that is scheduled
    //! @return                         Result of inserting the task
    //-----------------------------------------------------------------------------
    bool ScheduleTask(ScheduleItem *task);

    //-----------------------------------------------------------------------------
    //! Performes a time step by deleting the first priority list for the first
    //! time slot, if it is empty, then advance the time by the relative offset of
    //! the new first time slot and return the realtive offset.
    //!
    //! @return                         Relative offset of the new first time slot,
    //!                                 if the previous one was empty and deleted,
    //!                                 0 otherwise
    //-----------------------------------------------------------------------------
    int TimeStep();
    int GetTimeCurrent()
    {
        return currentTime;
    }

    //-----------------------------------------------------------------------------
    //! Delete all stored schedule items in the scheduler.
    //-----------------------------------------------------------------------------
    void ClearTasks();

    //-----------------------------------------------------------------------------
    //! @brief Spawns an agent by inserting the trigger tasks for each component
    //!         after calculating their offset time as well as inserting the update
    //!         tasks for each component after calculating their response time.
    //!
    //! The current offset is calculated w.r.t. the cycle time of the component before
    //! the components offset time is added. For the update tasks, also the component's
    //! response time is added.
    //!
    //! @param[in]   agent              Agent that should be spawned
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool SpawnAgent(const Agent *agent);

    //-----------------------------------------------------------------------------
    //! Removes an agent from the schedule by invalidating the trigger and update
    //! tasks of its component. Also removes agent during runtime.
    //!
    //! @param[in]   agent              Agent that should be removed from schedule
    //-----------------------------------------------------------------------------
    void RemoveFromSchedule(const Agent *agent);

    //-----------------------------------------------------------------------------
    //! Prints a string representation of the schedule.
    //-----------------------------------------------------------------------------
    void PrintSchedule();

private:

    //-----------------------------------------------------------------------------
    //! Inserts an item in the schedule by adding it in the priority list of the
    //! matching time frame, eventually adding this new time frame if non-existent.
    //!
    //! @param[in]    scheduleItem      Item to add to scheduler
    //! @param[in]    relativeOffset    Relative offset of the item to schedule
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    bool InsertScheduleItem(ScheduleItem *scheduleItem, int relativeOffset);

    //-----------------------------------------------------------------------------
    //! List of time frames, each time frame represented as priority list
    //-----------------------------------------------------------------------------
    std::list<PriorityList*> tasks;

    int startTime = 0;
    int endTime = 0;
    int currentTime = 0;

    SpawnPointNetwork *spawnPointNetwork = nullptr;
};

} // namespace SimulationSlave

#endif // SCHEDULEPOLICY_H
