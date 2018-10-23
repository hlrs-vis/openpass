/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <cassert>
#include <map>
#include <limits>
#include "scheduleItem.h"
#include "schedulePolicy.h"
#include "component.h"
#include "agent.h"
#include "channel.h"
#include "log.h"
#include "spawnPoint.h"
#include "runConfig.h"
#include "agentFactory.h"
#include "spawnPointInterface.h"

namespace SimulationSlave
{

ScheduleItemType ScheduleItem::GetType() const
{
    return type;
}

void ScheduleItem::Invalidate()
{
    valid = false;
}

bool ScheduleItem::GetValid() const
{
    return valid;
}

int ScheduleSpawnItem::GetContentId() const
{
    return spawnPoint->GetId();
}

int ScheduleSpawnItem::CompareToPriority(ScheduleItem *other) const
{
    Q_UNUSED(other);
    return 1; // spawn points always have the hightest priority (order among spawn points is not relevant)
}

bool ScheduleSpawnItem::GetInit() const
{
    // spawn tasks are not init tasks
    return false;
}

bool ScheduleSpawnItem::Execute(SchedulePolicy *schedule)
{
    if(spawnPoint->GetAgentSpawnItems().empty())
    {
        return false;
    }

    std::vector<const AgentSpawnItem*> &agentSpawnItems = spawnPoint->GetAgentSpawnItems();

    // retrieve implementation specific index
    int max = static_cast<int>(agentSpawnItems.size()) - 1;

    assert(nullptr != spawnPoint->GetImplementation());

    try
    {
        // initialize spawn item configuration and select agent type within range [0, max]
        if(!spawnPoint->GetLibrary()->SetSpawnItem(spawnPoint->GetImplementation(), spawnItemParameter, max))
        {
            LOG_INTERN(LogLevel::Error) << "spawn point " << spawnPoint->GetId() << " could not set spawn item";
            return false;
        }
    }
    catch(std::runtime_error const &ex)
    {
        LOG_INTERN(LogLevel::Error) << "spawn point " << spawnPoint->GetId() << " could not set spawn item: " << ex.what();
        return false;
    }
    catch(...)
    {
        LOG_INTERN(LogLevel::Error) << "spawn point " << spawnPoint->GetId() << " could not set spawn item";
        return false;
    }

    int index = spawnItemParameter.GetIndex();
    if(0 > index || max < index)
    {
        LOG_INTERN(LogLevel::Warning) << "generated invalid random number by spawn point";
        index = 0;
    }

    LOG_INTERN(LogLevel::DebugCore) << "spawn point " << spawnPoint->GetId() << " creates agent instance ref " << (agentSpawnItems[index])->GetId();

    Agent *agent = spawnPoint->GetAgentFactory()->AddAgent(agentSpawnItems[index], spawnItemParameter, schedule->GetCurrentTime());
    if(!agent)
    {
        return false;
    }

    if(!schedule->SpawnAgent(agent))
    {
        return false;
    }

    return true;
}

int ScheduleSpawnItem::GetNextTime()
{
    int nextTime = spawnItemParameter.GetNextTimeOffset();
    LOG_INTERN(LogLevel::DebugCore) << "respawn after " << nextTime << "ms";
    if(0 > nextTime)
    {
        // invalidate single shot spawn tasks
        Invalidate();
        nextTime = 0;
    }
    return nextTime;
}

int ScheduleTaskItem::GetContentId() const
{
    return agent->GetId();
}

int ScheduleTaskItem::CompareToPriority(ScheduleItem *other) const
{
    ScheduleItemType otherType = other->GetType();

    if(ScheduleItemType::ScheduleSpawnItemType == otherType)
    {
        return -1;
    }
    // at this point otherType is also a ScheduleTaskItem

    if(!GetInit() && other->GetInit())
    {
        return -1;
    }

    if(GetInit() && !other->GetInit())
    {
        return 1;
    }
    // at this point both are init tasks or both are non-init tasks

    ScheduleTaskItem *otherTask = static_cast<ScheduleTaskItem*>(other);
    int componentPriority = component->GetPriority();
    int otherComponentPriority = otherTask->component->GetPriority();

    if(componentPriority < otherComponentPriority)
    {
        return -1;
    }

    if(componentPriority > otherComponentPriority)
    {
        return 1;
    }

    int agentPriority = agent->GetAgentPriority();
    int otherAgentPriority = otherTask->agent->GetAgentPriority();

    if(agentPriority < otherAgentPriority)
    {
        return -1;
    }

    if(agentPriority > otherAgentPriority)
    {
        return 1;
    }

    if(ScheduleItemType::ScheduleUpdateItemType == type && ScheduleItemType::ScheduleTriggerItemType == otherType)
    {
        return -1;
    }

    if(ScheduleItemType::ScheduleTriggerItemType == type && ScheduleItemType::ScheduleUpdateItemType == otherType)
    {
        return 1;
    }

    return 0; // type == otherType
}

int ScheduleTaskItem::GetNextTime()
{
    return component->GetCycleTime();
}

Component *ScheduleTaskItem::GetComponent() const
{
    return component;
}

bool ScheduleTaskItem::GetInit() const
{
    return component->GetInit();
}

const Agent *ScheduleTaskItem::GetAgent() const
{
    return agent;
}

bool ScheduleTriggerItem::Execute(SchedulePolicy *schedule)
{
    if(!agent->IsValid()) // marked for removal by component model via RemoveAgent()
    {
        // immediately remove all schedule tasks of this agent
        schedule->RemoveFromSchedule(agent);

        // postpone removal of agent from global data to next time step to keep global
        // data of current time step consistent (other agents might still access agent
        // data during this time step)
        world->QueueAgentRemove(agent->GetAgentAdapter());

        return true;
    }

    if(!component->TriggerCycle(schedule->GetCurrentTime()))
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred during trigger agent: " << agent->GetId();
        return false;
    }

    if(GetInit())
    {
        Invalidate();
    }

    return true;
}

bool ScheduleUpdateItem::Execute(SchedulePolicy *schedule)
{
    if(!agent->IsValid()) // marked for removal by component model via RemoveAgent()
    {
        // immediately remove all schedule tasks of this agent
        schedule->RemoveFromSchedule(agent);

        // postpone removal of agent from global data to next time step to keep global
        // data of current time step consistent (other agents might still access agent
        // data during this time step)
        world->QueueAgentRemove(agent->GetAgentAdapter());

        return true;
    }

    for(auto &itChannel : component->GetOutputLinks())
    {
        Channel *channel = itChannel.second;
        int outputLinkId = itChannel.first;

        // trigger update of output data
        if(!component->AcquireOutputData(outputLinkId, schedule->GetCurrentTime()))
        {
            LOG_INTERN(LogLevel::Error) << "an error occurred during the time loop";
            return false;
        }

        for(const std::tuple<int, Component*> &item : channel->GetTargets())
        {
            int targetLinkId = std::get<static_cast<size_t>(Channel::TargetLinkType::LinkId)>(item);
            Component *targetComponent = std::get<static_cast<size_t>(Channel::TargetLinkType::Component)>(item);

            // trigger update of input data
            if(!targetComponent->UpdateInputData(targetLinkId, schedule->GetCurrentTime()))
            {
                LOG_INTERN(LogLevel::Error) << "an error occurred during the time loop";
                return false;
            }
        }

        if(!component->ReleaseOutputData(outputLinkId)) // release buffer
        {
            LOG_INTERN(LogLevel::Error) << "an error occurred during the time loop";
            return false;
        }
    }

    if(GetInit())
    {
        Invalidate();
    }

    return true;
}

} // namespace SimulationSlave
