/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <math.h>
#include <list>
#include <cassert>
#include <sstream>
#include <algorithm>
#include "schedulePolicy.h"
#include "agent.h"
#include "spawnPointNetwork.h"
#include "spawnPoint.h"
#include "component.h"
#include "log.h"
#include "runConfig.h"
#include "scheduleItem.h"
#include "spawnPointInterface.h"
#include "runResult.h"

namespace SimulationSlave
{

PriorityList::PriorityList(int relativeOffset) :
    relativeOffset(relativeOffset)
{}

bool PriorityList::SubtractRelativeOffset(int subtractOffset)
{
    if(relativeOffset < subtractOffset)
    {
        LOG_INTERN(LogLevel::Warning) << "trying to set negative time offset";
        return false;
    }

    relativeOffset -= subtractOffset;

    return true;
}

int PriorityList::GetRelativeOffset() const
{
    return relativeOffset;
}

bool PriorityList::InsertScheduleItem(ScheduleItem *scheduleItem)
{
    if(!scheduleItem)
    {
        LOG_INTERN(LogLevel::Warning) << "could not add schedule item";
        return false;
    }

    for(auto itTasks = tasks.begin(); itTasks != tasks.end(); ++itTasks)
    {
        ScheduleItem *itItem = *itTasks;

        // priority ordering
        // 1. spawnTasks
        // 2. initTriggerTasks (before initUpdateTasks due to zero response time)
        // 3. initUpdateTasks
        // 4. componentPrio
        // 5. agentPrio
        // 6. triggerTasks (before updateTasks due to zero response time)
        // 7. updateTasks
        if(0 > itItem->CompareToPriority(scheduleItem))
        {
            // insert before itItem if new scheduleItem has higher priority
            tasks.insert(itTasks, scheduleItem);
            return true;
        }
    }

    tasks.push_back(scheduleItem);

    return true;
}

bool PriorityList::IsEmpty()
{
    return tasks.empty();
}

bool PriorityList::RemoveNextTask(ScheduleItem **task)
{
    if(tasks.empty())
    {
        return false;
    }

    *task = tasks.front();
    tasks.remove(*task);

    return true;
}

void PriorityList::PrintList()
{
    std::stringstream line;
    line.str(std::string());
    ScheduleTaskItem *item;

    for(ScheduleItem *&itItem : tasks)
    {
        if(!itItem)
        {
            LOG_INTERN(LogLevel::Warning) << "could not print task list";
            return;
        }

        switch(itItem->GetType())
        {
        case ScheduleItemType::ScheduleSpawnItemType:
            line << std::endl << "   spawn point " << itItem->GetContentId() << ", type spawn";
            break;
        case ScheduleItemType::ScheduleTriggerItemType:
            item = static_cast<ScheduleTaskItem*>(itItem);
            line << std::endl << "   component " << item->GetComponent()->GetId() << ", agent " << itItem->GetContentId() << ", init " << (itItem->GetInit() ? "1" : "0") << ", type trigger";
            break;
        case ScheduleItemType::ScheduleUpdateItemType:
            item = static_cast<ScheduleTaskItem*>(itItem);
            line << std::endl << "   component " << item->GetComponent()->GetId() << ", agent " << itItem->GetContentId() << ", init " << (itItem->GetInit() ? "1" : "0") << ", type update";
            break;
        default:
            line << std::endl << "   undefined schedule item";
        }
    }

    LOG_INTERN(LogLevel::DebugCore) << line.str();
}

SchedulePolicy::SchedulePolicy(SpawnPointNetwork *spawnPointNetwork) : spawnPointNetwork(spawnPointNetwork)
{}

SchedulePolicy::~SchedulePolicy()
{
    ClearTasks();
}

void SchedulePolicy::ClearTasks()
{
    for(PriorityList *&item : tasks)
    {
        delete item;
    }

    tasks.clear();
}

bool SchedulePolicy::InitSchedule(int startTime, int endTime)
{
    this->startTime = currentTime = startTime;
    this->endTime = endTime;

    // setup spawn tasks
    for(const std::pair<const int, SpawnPoint*> &item : spawnPointNetwork->GetSpawnPoints())
    {
        SpawnPoint *spawnPoint = item.second;

        // insert spawn point into schedule
        if(!InsertScheduleItem(spawnPoint->GetSpawnTask(), 0))
        {
            LOG_INTERN(LogLevel::Warning) << "error during creation of schedule table";
            ClearTasks();
            return false;
        }

        LOG_INTERN(LogLevel::DebugCore) << "added spawn task " << spawnPoint->GetId() << " to schedule table";
    }

    PrintSchedule();

    return true;
}

bool SchedulePolicy::SpawnAgent(const Agent *agent)
{
    for(const std::pair<const int, Component*> &itComponent : agent->GetComponents())
    {
        Component *component = itComponent.second;

        int cycleTime = component->GetCycleTime();
        int offsetTime = 0;
        if(!component->GetInit()) // skip init tasks (set to spawn time)
        {
            // importer has to assure cycleTime > 0
            offsetTime = (currentTime - startTime) % cycleTime; // consider adding agent during runtime
            offsetTime += component->GetOffsetTime();
        }

        if(!InsertScheduleItem(component->GetTriggerTask(), offsetTime))
        {
            LOG_INTERN(LogLevel::Error) << "error during processing of schedule table";
            return false;
        }
        LOG_INTERN(LogLevel::DebugCore) << "added trigger task to schedule table for agent "
                                        << component->GetAgent()->GetId() << ", component "
                                        << component->GetId() << " at time " << offsetTime << "ms";

        int responseTime = offsetTime;
        responseTime += component->GetResponseTime();

        if(!InsertScheduleItem(component->GetUpdateTask(), responseTime))
        {
            LOG_INTERN(LogLevel::Error) << "error during processing of schedule table";
            return false;
        }
        LOG_INTERN(LogLevel::DebugCore) << "added update task to schedule table for agent "
                                        << component->GetAgent()->GetId() << ", component "
                                        << component->GetId() << " at time " << responseTime << "ms";
    }

    return true;
}

void SchedulePolicy::RemoveFromSchedule(const Agent *agent)
{
    for(const std::pair<const int, Component*> &itComponent : agent->GetComponents())
    {
        Component *component = itComponent.second;
        if(component)
        {
            component->GetTriggerTask()->Invalidate();
            component->GetUpdateTask()->Invalidate();
        }
    }
}

void SchedulePolicy::PrintSchedule()
{
    int currentOffset = 0;
    LOG_INTERN(LogLevel::DebugCore) << "print schedule:";

    for(PriorityList *&list : tasks)
    {
        if(!list)
        {
            LOG_INTERN(LogLevel::Warning) << "could not print task schedule";
            return;
        }

        currentOffset += list->GetRelativeOffset();
        LOG_INTERN(LogLevel::DebugCore) << "task list at " << currentOffset << "ms:";
        list->PrintList();
    }
}

int SchedulePolicy::TimeStep()
{
    if(tasks.empty())
    {
        LOG_INTERN(LogLevel::DebugCore) << "empty scheduling list";
        return 0;
    }

    PriorityList *list = tasks.front();
    if(!list)
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve task list";
        return 0;
    }

    int deltaTime = 0;
    if(list->IsEmpty()) // time step
    {
        tasks.remove(list);
        delete list;

        if(tasks.empty())
        {
            LOG_INTERN(LogLevel::DebugCore) << "empty scheduling list";
            return 0;
        }

        // retrieve offset time to add to current time
        list = tasks.front();
        if(!list || list->IsEmpty())
        {
            LOG_INTERN(LogLevel::Warning) << "invalid task list";
            return 0;
        }

        deltaTime = list->GetRelativeOffset();
        currentTime += deltaTime; // advance time
        LOG_INTERN(LogLevel::DebugAPI) << "next list at offset " << deltaTime;
    }

    return deltaTime;
}

bool SchedulePolicy::ScheduleTask(ScheduleItem *task)
{
    return InsertScheduleItem(task, task->GetNextTime());
}

bool SchedulePolicy::GetTask(ScheduleItem **task,
                             RunResult &runResult) const
{
    if(currentTime > endTime)
    {
        LOG_INTERN(LogLevel::DebugCore) << "end time reached";
        runResult.SetTimeOver();
        return false;
    }

    if(tasks.empty())
    {
        LOG_INTERN(LogLevel::DebugCore) << "empty scheduling list";
        return false;
    }

    PriorityList *list = tasks.front();
    if(!list || list->IsEmpty())
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve next task";
        return false;
    }

    if(!list->RemoveNextTask(task) || !(*task))
    {
        LOG_INTERN(LogLevel::Warning) << "could not retrieve next task";
        return false;
    }

    return true;
}

bool SchedulePolicy::InsertScheduleItem(ScheduleItem *scheduleItem, int relativeOffset)
{
    if(0 > relativeOffset)
    {
        LOG_INTERN(LogLevel::Error) << "scheduling item with relative offset: " << relativeOffset;
        return false;
    }

    int currentRelativeOffset = relativeOffset;

    PriorityList *list;
    PriorityList *nextList;
    std::list<PriorityList*>::iterator nextIt;
    std::list<PriorityList*>::iterator it = tasks.begin();
    while(it != tasks.end())
    {
        list = *it;

        if(0 == currentRelativeOffset) // relative offset matches exactly one existing schedule item
        {
            list->InsertScheduleItem(scheduleItem);
            return true;
        }

        nextIt = std::next(it);
        if(nextIt == tasks.end()) // relative offset already beyond last schedule item
        {
            PriorityList *newList = new (std::nothrow) PriorityList(currentRelativeOffset);
            newList->InsertScheduleItem(scheduleItem);
            tasks.push_back(newList);
            return true;
        }

        nextList = *nextIt;
        if(currentRelativeOffset < nextList->GetRelativeOffset()) // relative offset before next schedule item
        {
            // insert new list before next schedule item and update offset of following list
            if(!nextList->SubtractRelativeOffset(currentRelativeOffset))
            {
                LOG_INTERN(LogLevel::Warning) << "could not add schedule item";
                return false;
            }

            PriorityList *newList = new (std::nothrow) PriorityList(currentRelativeOffset);
            newList->InsertScheduleItem(scheduleItem);
            tasks.insert(nextIt, newList);
            return true;
        }

        // advance relative offset
        currentRelativeOffset -= nextList->GetRelativeOffset();
        it = nextIt;
    }

    // if list is empty
    PriorityList *newList = new (std::nothrow) PriorityList(currentRelativeOffset);
    newList->InsertScheduleItem(scheduleItem);
    tasks.push_back(newList);

    return true;
}

} // namespace SimulationSlave
