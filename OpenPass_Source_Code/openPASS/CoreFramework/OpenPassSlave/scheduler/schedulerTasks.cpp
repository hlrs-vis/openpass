/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <algorithm>
#include <utility>

#include "schedulerTasks.h"

//-----------------------------------------------------------------------------
/** \file  SchedulerTasks.cpp */
//-----------------------------------------------------------------------------

namespace SimulationSlave {
namespace Scheduling {

SchedulerTasks::SchedulerTasks(std::list<TaskItem> bootstrapTasks,
                               std::list<TaskItem> commonTasks,
                               std::list<TaskItem> finalizeRecurringTasks,
                               std::list<TaskItem> finalizeTasks,
                               int scheduledTimestampsInterval)
{
    this->bootstrapTasks.tasks = std::multiset<TaskItem>(bootstrapTasks.begin(), bootstrapTasks.end());
    this->commonTasks.tasks = std::multiset<TaskItem>(commonTasks.begin(), commonTasks.end());
    this->finalizeRecurringTasks.tasks = std::multiset<TaskItem>(finalizeRecurringTasks.begin(),
                                         finalizeRecurringTasks.end());
    this->finalizeTasks.tasks = std::multiset<TaskItem>(finalizeTasks.begin(), finalizeTasks.end());

    this->scheduledTimestampsInterval = scheduledTimestampsInterval;
    lowerBoundOfScheduledTimestamps = 0;
    upperBoundOfScheduledTimestamps = scheduledTimestampsInterval;
    CreateNewScheduledTimestamps();
}

void SchedulerTasks::ScheduleNewRecurringTasks(std::list<TaskItem> newTasks)
{
    ScheduleNewTasks(recurringTasks, std::move(newTasks));
}

void SchedulerTasks::ScheduleNewNonRecurringTasks(std::list<TaskItem> newTasks)
{
    ScheduleNewTasks(nonRecurringTasks, newTasks);
}

void SchedulerTasks::ScheduleNewTasks(Tasks& tasks, std::list<TaskItem> newTasks)
{
    for (const auto& newTask : newTasks)
    {
        tasks.AddTask(newTask);
        UpdateScheduledTimestamps(newTask.cycletime, newTask.delay);
    }
}

void SchedulerTasks::UpdateScheduledTimestamps(int cycleTime, int delay)
{
    if (delay > lowerBoundOfScheduledTimestamps && delay < upperBoundOfScheduledTimestamps)
    {
        scheduledTimestamps.insert(delay);
    }

    if (cycleTime == 0)
    {
        // next timestamp is already in scheduledTimesteps for execution of init task
        return;
    }

    int currentScheduleTime = cycleTime + delay;
    int numberOfSkippedTimestamps = std::max(0,
                                    (lowerBoundOfScheduledTimestamps - currentScheduleTime + cycleTime - 1) /
                                    cycleTime); //We add cycletime -1 because we want to round up
    currentScheduleTime += cycleTime * numberOfSkippedTimestamps;

    if (currentScheduleTime <= upperBoundOfScheduledTimestamps)
    {
        while (currentScheduleTime <= upperBoundOfScheduledTimestamps)
        {
            scheduledTimestamps.insert(currentScheduleTime);
            currentScheduleTime += cycleTime;
        }
    }
    // scheduledTimesteps higher than horizon will be considered on next timestamp
}

void SchedulerTasks::UpdateScheduledTimestamps(std::multiset<TaskItem>& tasks)
{
    for (const auto& task : tasks)
    {
        UpdateScheduledTimestamps(task.cycletime, task.delay);
    }
}

void SchedulerTasks::CreateNewScheduledTimestamps()
{
    scheduledTimestamps.clear();
    scheduledTimestamps.insert(lowerBoundOfScheduledTimestamps);
    scheduledTimestamps.insert(upperBoundOfScheduledTimestamps);

    UpdateScheduledTimestamps(commonTasks.tasks);
    UpdateScheduledTimestamps(recurringTasks.tasks);
    UpdateScheduledTimestamps(nonRecurringTasks.tasks);
}

int SchedulerTasks::GetNextTimestamp(int timestamp)
{
    ExpandUpperBoundary(timestamp);

    for (const auto& scheduledTimestamp : scheduledTimestamps)
    {
        if (scheduledTimestamp > timestamp)
        {
            return scheduledTimestamp;
        }
    }

    throw std::runtime_error("Can not handle given timestamp. Please check validity.");
}

void SchedulerTasks::ExpandUpperBoundary(int timestamp)
{
    while (timestamp >= upperBoundOfScheduledTimestamps)
    {
        upperBoundOfScheduledTimestamps += scheduledTimestampsInterval;
        lowerBoundOfScheduledTimestamps += scheduledTimestampsInterval;
        CreateNewScheduledTimestamps();
    }
    // we are not time travelling.. backwards is illogical
}

void SchedulerTasks::GetTasks(int timestamp, std::multiset<TaskItem>& tasks, std::list<TaskItem>& currentTasks)
{
    for (const auto& task : tasks)
    {
        if (task.cycletime == 0)
        {
            currentTasks.push_back(task);
            continue;
        }

        if ((timestamp - task.delay) % task.cycletime == 0)
        {
            currentTasks.push_back(task);
        }
    }
}

std::list<TaskItem> SchedulerTasks::GetTasks(int timestamp)
{
    std::list<TaskItem> currentTasks{};

    ExpandUpperBoundary(timestamp);
    if (scheduledTimestamps.find(timestamp) == scheduledTimestamps.end())
    {
        return currentTasks;
    }

    GetTasks(timestamp, commonTasks.tasks, currentTasks);
    PullNonRecurringTasks(timestamp, currentTasks);
    GetTasks(timestamp, recurringTasks.tasks, currentTasks);
    GetTasks(timestamp, finalizeRecurringTasks.tasks, currentTasks);

    return currentTasks;
}

std::list<TaskItem> SchedulerTasks::GetCommonTasks(int timestamp)
{
    std::list<TaskItem> currentTasks;
    GetTasks(timestamp, commonTasks.tasks, currentTasks);
    return currentTasks;
}

std::list<TaskItem> SchedulerTasks::ConsumeNonRecurringTasks(int timestamp)
{
    std::list<TaskItem> currentTasks;
    PullNonRecurringTasks(timestamp, currentTasks);
    return currentTasks;
}

std::list<TaskItem> SchedulerTasks::GetRecurringTasks(int timestamp)
{
    std::list<TaskItem> currentTasks;
    GetTasks(timestamp, recurringTasks.tasks, currentTasks);
    GetTasks(timestamp, finalizeRecurringTasks.tasks, currentTasks);
    return currentTasks;
}

void SchedulerTasks::PullNonRecurringTasks(int timestamp, std::list<TaskItem>& currentTasks)
{
    GetTasks(timestamp, nonRecurringTasks.tasks, currentTasks);
    ClearNonrecurringTasks();
}

std::multiset<TaskItem> SchedulerTasks::GetBootstrapTasks()
{
    return bootstrapTasks.tasks;
}

std::multiset<TaskItem> SchedulerTasks::GetFinalizeTasks()
{
    return finalizeTasks.tasks;
}

void SchedulerTasks::ClearNonrecurringTasks()
{
    nonRecurringTasks.tasks.clear();
}

void SchedulerTasks::DeleteAgentTasks(int agentId)
{
    std::list<int> agentIds{agentId};
    DeleteAgentTasks(agentIds);
}

void SchedulerTasks::DeleteAgentTasks(std::list<int>& agentIds)
{
    for (const auto& agentId : agentIds)
    {
        recurringTasks.DeleteTasks(agentId);
        nonRecurringTasks.DeleteTasks(agentId); //if agent immediately will be removed after spawning
    }

    if (!agentIds.empty())
    {
        CreateNewScheduledTimestamps();
    }
}

} // namespace Scheduling
} // namespace SimulationSlave
