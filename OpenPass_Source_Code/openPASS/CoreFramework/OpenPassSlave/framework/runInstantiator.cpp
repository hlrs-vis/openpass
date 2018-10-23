/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <functional>
#include <sstream>
#include "agentTypeImporter.h"
#include "agentType.h"
#include "runConfig.h"
#include "runConfigImporter.h"
#include "frameworkConfig.h"
#include "component.h"
#include "channel.h"
#include "agentFactory.h"
#include "scenery.h"
#include "sceneryImporter.h"
#include "scenarioImporter.h"
#include "spawnPointNetwork.h"
#include "observationNetwork.h"
#include "stochastics.h"
#include "runResult.h"
#include "log.h"
#include "runInstantiator.h"
#include "schedulePolicy.h"
#include "scheduler.h"

#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
    { \
    LOG_INTERN(LogLevel::Warning) << "an error occurred during run generation"; \
    if(world->isInstantiated()) \
    { \
        world->Clear(); \
    }\
    return false;\
    } \
    } \
    while(0);

namespace SimulationSlave {

void RunInstantiator::StopRun()
{
    stopMutex.lock();
    stopped = true;
    stopMutex.unlock();
}

bool RunInstantiator::ExecuteRun()
{
    LOG_INTERN(LogLevel::DebugCore) << std::endl << "### import run configuration ###";

    stopMutex.lock();
    stopped = false;
    stopMutex.unlock();

    std::unique_ptr<SimulationCommon::RunConfig> runConfig(SimulationCommon::RunConfigImporter::Import(
                                                               frameworkConfig->GetRunConfigFile()));
    CHECKFALSE(runConfig);

    CHECKFALSE(stochastics->Instantiate(runConfig->GetStochasticsInstance()));
    stochastics->InitGenerator(runConfig->GetRandomSeed());

    CHECKFALSE(world->Instantiate(runConfig->GetWorldParameter()));

    observationNetwork->Instantiate(runConfig->GetObservationInstances(), stochastics, world);
    observationNetwork->InitAll(frameworkConfig->GetObservationResultPath());

    if (!agentFactory->ReloadAgentTypes())
    {
        LOG_INTERN(LogLevel::Error) << "could not load agent configuration file";
        exit(-1);
    }

    std::function<bool(int, RunResult &)> updateTimeStep =
        [this](int time, RunResult & runResult)
    {
        return observationNetwork->UpdateTimeStep(time, runResult);
    };

    std::function<bool()> simulationEnd =
        [this]()
    {
        bool result;
        stopMutex.lock();
        result = stopped;
        stopMutex.unlock();
        return result;
    };

    for (int invocationCount = 0; invocationCount < runConfig->GetNumberInvocations();
            ++invocationCount)
    {
        LOG_INTERN(LogLevel::DebugCore) << std::endl << "### run number: "
                                        << invocationCount << " ##################################################";

        agentFactory->ResetIds();

        world->Clear();
        agentFactory->Clear();
        spawnPointNetwork->Clear();
        world->SetParameter(runConfig->GetWorldParameter());

        CHECKFALSE(SceneryImporter::Import(frameworkConfig->GetSceneryConfigFile(),
                                           world));

        if (!ScenarioImporter::Import(frameworkConfig->GetOpenScenarioConfigFile(), world))
        {
            LOG_INTERN(LogLevel::Warning) << "No openScenario file found." ;
        }

        observationNetwork->InitRun();

        CHECKFALSE(spawnPointNetwork->Instantiate(runConfig->GetSpawnPointInstances(),
                                                  runConfig->GetAgentSpawnItems(),
                                                  agentFactory,
                                                  stochastics));

        if (collisionDetection->Instantiate(runConfig->GetCollisionDetectionInstance()))
        {
            collisionDetection->SetAgents(world->GetAgents());
        }

        LOG_INTERN(LogLevel::DebugCore) << std::endl << "### start scheduling ###";
        // instantiate Scheduler last step since destructors are called in the inverse order of instantiation
        // otherwise dangling references might exists in Schedule
        Scheduler scheduler(world, spawnPointNetwork, collisionDetection);

        // prepare result storage
        RunResult runResult;
        scheduler.Run(updateTimeStep,
                      simulationEnd,
                      runConfig->GetStartTime(),
                      runConfig->GetEndTime(),
                      runResult);

        observationNetwork->FinalizeRun(runResult);

        stochastics->ReInit();
    }

    LOG_INTERN(LogLevel::DebugCore) << std::endl << "### end scheduling ###";

    observationNetwork->FinalizeAll();

    world->Clear();
    agentFactory->Clear();
    spawnPointNetwork->Clear();

    return true;
}

} // namespace SimulationSlave
