/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "agent.h"
#include "component.h"
#include "scheduleItem.h"
#include "observationNetwork.h"
#include "runResult.h"
#include "collisionDetection.h"
#include "schedulePolicy.h"
#include "scheduler.h"
#include "log.h"

namespace SimulationSlave
{

Scheduler::Scheduler(WorldInterface *world, SpawnPointNetwork *spawnPointNetwork, CollisionDetection *collisionDetection) :
    schedulePolicy(spawnPointNetwork),
    world(world),
    collisionDetection(collisionDetection)
{}

void Scheduler::Run(const std::function<bool(int, RunResult&)> &updateCallback,
                    const std::function<bool()> &endCallback,
                    int start,
                    int end,
                    RunResult &runResult)
{
    if(start > end)
    {
        LOG_INTERN(LogLevel::Error) << "start time greater than end time";
        return;
    }

    if(!schedulePolicy.InitSchedule(start, end))
    {
        LOG_INTERN(LogLevel::Error) << "could not initialize schedule";
        return;
    }

    if(!world->CreateGlobalDrivingView())
    {
        LOG_INTERN(LogLevel::Error) << "could not create global driving view";
        return;
    }

    ScheduleItem *task;
    int deltaTimeStep = 0;
    int lastTime = schedulePolicy.GetCurrentTime();

    // update observation modules (observe initial time step)
    if(!updateCallback(lastTime, runResult))
    {
        LOG_INTERN(LogLevel::Error) << "an error occurred when processing observation modules";
        return;
    }

    while(!endCallback())
    {
        if(!schedulePolicy.GetTask(&task, runResult))
        {
            return;
        }

        if(task->GetValid())
        {
            LOG_INTERN(LogLevel::DebugAPI) << "*** currentTime: " << schedulePolicy.GetCurrentTime() << "ms";

            if(ScheduleItemType::ScheduleTriggerItemType == task->GetType())
            {
                LOG_INTERN(LogLevel::DebugAPI) << "    "
                                               << (task->GetInit() ? "init " : "")
                                               << "trigger task (agent instance "
                                               << task->GetContentId()
                                               << ", agent type "
                                               << static_cast<ScheduleTaskItem*>(task)->GetAgent()->GetAgentType()->GetId()
                                               << ", component " << static_cast<ScheduleTaskItem*>(task)->GetComponent()->GetId() << ")";
            }
            else if(ScheduleItemType::ScheduleUpdateItemType == task->GetType())
            {
                LOG_INTERN(LogLevel::DebugAPI) << "    "
                                               << (task->GetInit() ? "init " : "")
                                               << "update task (agent instance "
                                               << task->GetContentId()
                                               << ", agent type "
                                               << static_cast<ScheduleTaskItem*>(task)->GetAgent()->GetAgentType()->GetId()
                                               << ", component " << static_cast<ScheduleTaskItem*>(task)->GetComponent()->GetId() << ")";
            }
            else if(ScheduleItemType::ScheduleSpawnItemType == task->GetType())
            {
                LOG_INTERN(LogLevel::DebugAPI) << "    spawn task (spawn id "
                                               << task->GetContentId() << ")";
            }
            else
            {
                LOG_INTERN(LogLevel::Error) << "    unknown task type";
                return;
            }

            if(!task->Execute(&schedulePolicy))
            {
                LOG_INTERN(LogLevel::Error) << "an error occurred during the time loop";
                return;
            }

            // task could have been invalidated during execution (e.g. single spawn items or removal of agent)
            if(task->GetValid() && !schedulePolicy.ScheduleTask(task))
            {
                LOG_INTERN(LogLevel::Error) << "could not reschedule task";
                return;
            }
        }

        lastTime = schedulePolicy.GetCurrentTime(); // remember time before potential time step
        deltaTimeStep = schedulePolicy.TimeStep();
        if(0 < deltaTimeStep) // if time step occurred
        {
            runResult.Clear();

            // update global data synchronously
            world->SyncGlobalData();

            // calculate collision detection at each time step
            bool isCollision;
            if(!collisionDetection->HandleCollisionsInAgents(runResult, isCollision))
            {
                LOG_INTERN(LogLevel::Error) << "an error occurred during the collision detection";
                return;
            }

            if(!world->CreateGlobalDrivingView())
            {
                LOG_INTERN(LogLevel::Error) << "could not create global driving view";
                return;
            }

            // update observation modules
            if(!updateCallback(lastTime, runResult))
            {
                LOG_INTERN(LogLevel::Error) << "an error occurred when processing observation modules";
                return;
            }

            if(runResult.IsEndCondition())
            {
                LOG_INTERN(LogLevel::DebugCore) << "observation module indicated end of simulation run";
                return;
            }
        }
    }
}

} // namespace SimulationSlave
