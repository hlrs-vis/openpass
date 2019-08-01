/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  TaskBuilder.cpp */
//-----------------------------------------------------------------------------


#include <functional>
#include "taskBuilder.h"
#include "eventDetector.h"
#include "manipulator.h"
#include <algorithm>

using namespace SimulationSlave;
using namespace SimulationSlave::Scheduling;

TaskBuilder::TaskBuilder(const int& currentTime,
                         RunResult& runResult,
                         const int frameworkUpdateRate,
                         WorldInterface* const world,
                         SpawnControlInterface* const spawnControl,
                         ObservationNetworkInterface* const observationNetwork,
                         EventDetectorNetworkInterface* const eventDetectorNetwork,
                         ManipulatorNetworkInterface* const manipulatorNetwork) :
    currentTime{currentTime},
    runResult{runResult},
    frameworkUpdateRate{frameworkUpdateRate},
    world{world},
    spawnControl{spawnControl},
    observationNetwork{observationNetwork},
    eventDetectorNetwork{eventDetectorNetwork},
    manipulatorNetwork{manipulatorNetwork}
{
    BuildEventDetectorTasks();
    BuildManipulatorTasks();
}

std::list<TaskItem> TaskBuilder::CreateBootstrapTasks()
{
    return std::list<TaskItem>
    {
        ObservationTaskItem(frameworkUpdateRate,
                            std::bind(&ObservationNetworkInterface::UpdateTimeStep,
                                      observationNetwork, std::ref(currentTime), std::ref(runResult)))
    };
}

std::list<TaskItem> TaskBuilder::CreateCommonTasks()
{
    std::list<TaskItem> items {};
    items.emplace_back(SpawningTaskItem(frameworkUpdateRate, std::bind(&SpawnControlInterface::Execute, spawnControl,
                                        std::ref(currentTime))));

    std::copy(std::begin(eventDetectorTasks), std::end(eventDetectorTasks), std::back_inserter(items));
    std::copy(std::begin(manipulatorTasks), std::end(manipulatorTasks), std::back_inserter(items));

    items.emplace_back(ObservationTaskItem(frameworkUpdateRate, std::bind(
            &ObservationNetworkInterface::UpdateTimeStep,
            observationNetwork,
            std::ref(currentTime), std::ref(runResult))));

    return items;
}

std::list<TaskItem> TaskBuilder::CreateFinalizeRecurringTasks()
{
    return std::list<TaskItem>
    {
        SyncWorldTaskItem(ScheduleAtEachCycle, std::bind(&WorldInterface::SyncGlobalData, world))
    };
}

std::list<TaskItem> TaskBuilder::CreateFinalizeTasks()
{
    std::list<TaskItem> items {};

    std::copy(std::begin(eventDetectorTasks), std::end(eventDetectorTasks), std::back_inserter(items));
    std::copy(std::begin(manipulatorTasks), std::end(manipulatorTasks), std::back_inserter(items));

    items.emplace_back(ObservationTaskItem(frameworkUpdateRate, std::bind(
            &ObservationNetworkInterface::UpdateTimeStep,
            observationNetwork,
            std::ref(currentTime),
            std::ref(runResult))));

    return items;
}

void TaskBuilder::BuildEventDetectorTasks()
{
    const auto& eventDetectors = eventDetectorNetwork->GetEventDetectors();
    std::transform(std::begin(eventDetectors), std::end(eventDetectors),
                   std::back_inserter(eventDetectorTasks),
                   [&](auto eventDetector)
    {
        return EventDetectorTaskItem(frameworkUpdateRate, std::bind(
                                         &EventDetectorInterface::Trigger,
                                         eventDetector->GetImplementation(),
                                         std::ref(currentTime)));
    });
}

void TaskBuilder::BuildManipulatorTasks()
{
    const auto& manipulators = manipulatorNetwork->GetManipulators();
    std::transform(std::begin(manipulators), std::end(manipulators),
                   std::back_inserter(manipulatorTasks),
                   [&](auto manipulator)
    {
        return EventDetectorTaskItem(frameworkUpdateRate, std::bind(
                                         &ManipulatorInterface::Trigger,
                                         manipulator->GetImplementation(),
                                         std::ref(currentTime)));
    });
}
