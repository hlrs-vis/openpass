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

#include <functional>
#include <sstream>

#include "agentFactory.h"
#include "Interfaces/agentBlueprintProviderInterface.h"
#include "agentType.h"
#include "channel.h"
#include "component.h"
#include "CoreFramework/CoreShare/log.h"
#include "Interfaces/observationNetworkInterface.h"
#include "observationModule.h"
#include "respawner.h"
#include "runInstantiator.h"
#include "runResult.h"
#include "scheduler.h"
#include "spawnPointNetwork.h"
#include "stochastics.h"
#include "invocationControl.h"
#include "parameters.h"


#define CHECKFALSE(element) \
    do { \
    if(!(element)) \
    { \
    LOG_INTERN(LogLevel::Warning) << "an error occurred during run generation"; \
    if(world->isInstantiated()) \
    { \
        world->Reset(); \
    }\
    return false;\
    } \
    } \
    while(0);

namespace SimulationSlave {

void RunInstantiator::ClearRun()
{
    world->Reset();
    agentFactory->Clear();
    spawnPointNetwork->Clear();
    eventNetwork->Clear();
}

bool RunInstantiator::InitializeFrameworkModules(ExperimentConfig& experimentConfig, ScenarioInterface* scenario)
{
    CHECKFALSE(stochastics->Instantiate(frameworkModules.stochasticsLibrary));
    stochastics->InitGenerator(experimentConfig.randomSeed);

    CHECKFALSE(world->Instantiate());

    CHECKFALSE(eventDetectorNetwork->Instantiate(frameworkModules.eventDetectorLibrary,
               scenario,
               eventNetwork,
               stochastics));
    CHECKFALSE(manipulatorNetwork->Instantiate(frameworkModules.manipulatorLibrary,
               scenario,
               eventNetwork));

    SimulationCommon::ObservationParameters observationParameters;
    observationParameters.AddParameterStringVector("LoggingGroups", experimentConfig.loggingGroups);
    observationParameters.AddParameterString("SceneryFile", scenario->GetSceneryPath());

    // TODO: This is a workaround, as the OSI use case only imports a single observation library -> implement new observation concept
    std::map<int, ObservationInstance> observationInstances
    {
        { 0, { frameworkModules.observationLibrary, &observationParameters } }
    };

    CHECKFALSE(observationNetwork->Instantiate(observationInstances,
               stochastics,
               world,
               eventNetwork));
    CHECKFALSE(observationNetwork->InitAll(outputDir));

    return true;
    }

bool RunInstantiator::ExecuteRun()
    {
    LOG_INTERN(LogLevel::DebugCore) << std::endl << "### execute run ###";

        stopMutex.lock();
    stopped = false;
        stopMutex.unlock();

    SlaveConfigInterface* slaveConfig = configurationContainer.GetSlaveConfig();
    ExperimentConfig experimentConfig = slaveConfig->GetExperimentConfig();
    ScenarioInterface* scenario = configurationContainer.GetScenario();
    SceneryInterface* scenery = configurationContainer.GetScenery();

    if (!InitializeFrameworkModules(experimentConfig, scenario))
    {
        return false;
    }
    world->CreateScenery(scenery);

    InvocationControl invocationControl(experimentConfig.numberOfInvocations);
    while (invocationControl.Progress())
    {
        LOG_INTERN(LogLevel::DebugCore) << std::endl << "### run number: " << invocationControl.CurrentInvocation() << " ###";

        agentFactory->ResetIds();

        ClearRun();
        int maxInvoc = runConfig->GetNumberInvocations();
        SimulationCommon::WorldParameters worldParameters;
        sampler.SampleWorldParameters(slaveConfig->GetEnvironmentConfig(), &worldParameters);

        world->ExtractParameter(&worldParameters);

        observationNetwork->InitRun();

        SimulationCommon::SpawnPointParameters spawnPointParameters;
        sampler.SampleSpawnPointParameters(slaveConfig->GetTrafficConfig(), &spawnPointParameters);

        CHECKFALSE(spawnPointNetwork->Instantiate(frameworkModules.spawnPointLibrary,
                                                  agentFactory,
                   agentBlueprintProvider,
                   &spawnPointParameters,
                   sampler,
                   scenario));

        LOG_INTERN(LogLevel::DebugCore) << std::endl << "### start scheduling ###";

        // instantiate Scheduler last step since destructors are called in the inverse order of instantiation
        // otherwise dangling references might exists in Schedule
        Scheduler scheduler(world, spawnPointNetwork, eventDetectorNetwork, manipulatorNetwork, observationNetwork);

        Respawner respawner(scheduler, spawnPointNetwork->GetSpawnPoint());

        // prepare result storage
        RunResult runResult;

        // TODO: This is a workaround, as the OSI use case only imports a single observation library -> implement new observation concept
        auto& observationModule = *(observationNetwork->GetObservationModules().begin()->second);

        eventNetwork->Initialize(&respawner,
                                 &runResult,
                                 observationModule.GetImplementation());

        auto schedulerReturnState = scheduler.Run(
                                        0,
                                        scenario->GetEndTime(),
                                        runResult,
                                        eventNetwork);

        if (schedulerReturnState == SchedulerReturnState::NoError)
        {
        observationNetwork->FinalizeRun(runResult);
        }

        if (schedulerReturnState == SchedulerReturnState::AbortInvocation)
        {
            invocationControl.Retry();
        }

        if (schedulerReturnState == SchedulerReturnState::AbortSimulation)
        {
            invocationControl.Abort();
        }

        stochastics->ReInit();

        // Reset EventDetectors
        eventDetectorNetwork->ResetAll();
    }

    LOG_INTERN(LogLevel::DebugCore) << std::endl << "### end scheduling ###";

    bool successfullyFinalized = observationNetwork->FinalizeAll();

    ClearRun();
    world->Clear();

    if (!successfullyFinalized || invocationControl.GetAbortFlag())
    {
        return false;
    }

    return true;
}

} // namespace SimulationSlave
