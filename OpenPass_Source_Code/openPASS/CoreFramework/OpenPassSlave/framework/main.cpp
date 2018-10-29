/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QThreadPool>
#include <QElapsedTimer>
#include "frameworkConfigImporter.h"
#include "frameworkConfig.h"
#include "runInstantiator.h"
#include "agentFactory.h"
#include "modelBinding.h"
#include "callbacks.h"
#include "stochastics.h"
#include "stochasticsBinding.h"
#include "collisionDetection.h"
#include "log.h"
#include "spawnPointBinding.h"
#include "spawnPointNetwork.h"
#include "observationBinding.h"
#include "observationNetwork.h"
#include "callbacks.h"
#include "world.h"

using namespace SimulationSlave;

//-----------------------------------------------------------------------------
//! Entry point of program.
//!
//! Setup of program parameters and framework infrastructure.
//! Startup of servers.
//!
//! @param[in]     argc    number of program arguments
//! @param[in]     argv    program arguments
//! @return                program error code / result
//-----------------------------------------------------------------------------


#ifndef OPENPASSSLAVELIBRARY
int run(int argc, char *argv[])
#else
extern "C" Q_DECL_EXPORT int run(int argc, char *argv[])
#endif //OPENPASSSLAVELIBRARY

{
    QElapsedTimer timer;
    timer.start();

    //import frameworkConfiguration either via XML or application arguments,
    //in this case via application arguments given by starting the executable

    FrameworkConfig *frameworkConfig = FrameworkConfigImporter::Import(argc, argv);

    if (!frameworkConfig)
    {
        LOG_INTERN(LogLevel::Error) << "could not import framework configuration";
        exit(EXIT_FAILURE);
    }

    LogOutputPolicy::SetFile(frameworkConfig->GetLogFile());
    LogFile::ReportingLevel() = static_cast<LogLevel>(frameworkConfig->GetLogLevel());
    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### slave start ##";
    LOG_INTERN(LogLevel::DebugCore) << "library path: " << frameworkConfig->GetLibraryPath();
    LOG_INTERN(LogLevel::DebugCore) << "agent configuration: " << frameworkConfig->GetAgentConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "observation result path: " <<
                                    frameworkConfig->GetObservationResultPath();
    LOG_INTERN(LogLevel::DebugCore) << "run configuration: " << frameworkConfig->GetRunConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "scenery configuration: " <<
                                    frameworkConfig->GetSceneryConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "openScenario configuration: " <<
                                    frameworkConfig->GetOpenScenarioConfigFile();

    // create folders for files which are generated during runtime
    if (!QDir(QString::fromStdString(frameworkConfig->GetLibraryPath())).exists())
    {
        LOG_INTERN(LogLevel::Error) << "Library path does not exist. ";
        return -1;
    }

    if (!QFileInfo(QString::fromStdString(
                       frameworkConfig->GetAgentConfigFile())).exists())
    {
        LOG_INTERN(LogLevel::Error) << "Agent Config File does not exist. ";
        return -1;
    }

    if (!QDir(QString::fromStdString(frameworkConfig->GetObservationResultPath())).exists())
    {
        LOG_INTERN(LogLevel::DebugCore) << "created folder";
        QDir().mkpath(QString::fromStdString(frameworkConfig->GetObservationResultPath()));
    }

    if (!QFileInfo(QString::fromStdString(
                       frameworkConfig->GetRunConfigFile())).exists())
    {
        LOG_INTERN(LogLevel::Error) << "Run Config File does not exist. ";
        return -1;
    }

    // setup framework infrastructure
    SimulationCommon::Callbacks callbacks;
    SpawnPointBinding spawnPointBinding(frameworkConfig, &callbacks);
    WorldBinding worldBinding(frameworkConfig, &callbacks);
    World world(&worldBinding);
    SpawnPointNetwork spawnPointNetwork(&spawnPointBinding, &world);
    StochasticsBinding stochasticsBinding(frameworkConfig, &callbacks);
    Stochastics stochastics(&stochasticsBinding);

    CollisionDetectionBinding collisionDetectionBinding(frameworkConfig, &callbacks);
    CollisionDetection collisionDetection(&collisionDetectionBinding);

    ObservationBinding observationBinding(frameworkConfig, &callbacks);
    ObservationNetwork observationNetwork(&observationBinding);

    // ensure that library container is never released/deleted before component network
    // (C++ guarantees reverse order of destructors at end of block)
    ModelBinding modelBinding(frameworkConfig,
                              &callbacks);
    AgentFactory agentFactory(frameworkConfig,
                              &modelBinding,
                              &world,
                              &stochastics,
                              &observationNetwork);
    RunInstantiator runInstantiator(frameworkConfig,
                                    &observationNetwork,
                                    &agentFactory,
                                    &world,
                                    &spawnPointNetwork,
                                    &stochastics,
                                    &collisionDetection);

    // process commands
    observationNetwork.Clear();
    spawnPointNetwork.Clear();
    agentFactory.Clear();

    int exitCode = 0;
    if (runInstantiator.ExecuteRun())
    {
        LOG_INTERN(LogLevel::DebugCore) << "simulation finished successfully";
    }
    else
    {
        LOG_INTERN(LogLevel::Error) << "simulation failed";
        exitCode = -1;
    }

    delete frameworkConfig;

    qDebug() << "Simulation time elapsed: " << timer.elapsed() << " ms";

    LOG_INTERN(LogLevel::DebugCore) << "Simulation time elapsed: " << timer.elapsed() << " ms";
    LOG_INTERN(LogLevel::DebugCore) << "end slave";

    return exitCode;
}

#ifndef OPENPASSSLAVELIBRARY
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    return run(argc, argv);
}
#endif //OPENPASSSLAVELIBRARY
