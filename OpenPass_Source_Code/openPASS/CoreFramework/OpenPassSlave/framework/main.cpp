/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    QElapsedTimer timer;
    timer.start();

    //import frameworkConfigutation either via XML or application arguments,
    //in this case via application arguments given by starting the executable
    FrameworkConfig *frameworkConfig = FrameworkConfigImporter::Import(application);

    if(!frameworkConfig)
    {
        LOG_INTERN(LogLevel::Error) << "could not import framework configuration";
        exit(EXIT_FAILURE);
    }

    LogOutputPolicy::SetFile(frameworkConfig->GetLogFile().c_str());
    LogFile::ReportingLevel() = static_cast<LogLevel>(frameworkConfig->GetLogLevel());
    LOG_INTERN(LogLevel::DebugCore) << std::endl << std::endl << "### slave start ##";
    LOG_INTERN(LogLevel::DebugCore) << "library path: " << frameworkConfig->GetLibraryPath();
    LOG_INTERN(LogLevel::DebugCore) << "agent configuration: " << frameworkConfig->GetAgentConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "observation result path: " << frameworkConfig->GetObservationResultPath();
    LOG_INTERN(LogLevel::DebugCore) << "run configuration: " << frameworkConfig->GetRunConfigFile();
    LOG_INTERN(LogLevel::DebugCore) << "scenery configuration: " << frameworkConfig->GetSceneryConfigFile();

    // create folders for files which are generated during runtime
    if(!QDir(QString::fromStdString(frameworkConfig->GetLibraryPath())).exists())
    {
        LOG_INTERN(LogLevel::DebugCore) << "created folder";
        QDir().mkpath(QString::fromStdString(frameworkConfig->GetLibraryPath()));
    }

    if(!QDir(QFileInfo(QString::fromStdString(frameworkConfig->GetAgentConfigFile())).absolutePath()).exists())
    {
        LOG_INTERN(LogLevel::DebugCore) << "created folder";
        QDir().mkpath(QFileInfo(QString::fromStdString(frameworkConfig->GetAgentConfigFile())).absolutePath());
    }

    if(!QDir(QString::fromStdString(frameworkConfig->GetObservationResultPath())).exists())
    {
        LOG_INTERN(LogLevel::DebugCore) << "created folder";
        QDir().mkpath(QString::fromStdString(frameworkConfig->GetObservationResultPath()));
    }

    if(!QDir(QFileInfo(QString::fromStdString(frameworkConfig->GetRunConfigFile())).absolutePath()).exists())
    {
        LOG_INTERN(LogLevel::DebugCore) << "created folder";
        QDir().mkpath(QFileInfo(QString::fromStdString(frameworkConfig->GetRunConfigFile())).absolutePath());
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

    if(runInstantiator.ExecuteRun())
    {
        LOG_INTERN(LogLevel::DebugCore) << "simulation finished successfully";
    }
    else
    {
        LOG_INTERN(LogLevel::Error) << "simulation failed";
    }

    delete frameworkConfig;

    qDebug() <<"Simulation time elapsed: "<< timer.elapsed() << " ms";

    LOG_INTERN(LogLevel::DebugCore) << "Simulation time elapsed: "<< timer.elapsed() << " ms";
    LOG_INTERN(LogLevel::DebugCore) << "end slave";

    return 0;
}
