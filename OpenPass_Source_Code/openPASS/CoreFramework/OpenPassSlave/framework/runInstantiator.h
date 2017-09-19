/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  runInstantiator.h
//! @brief This file contains the component which triggers the simulation runs.
//-----------------------------------------------------------------------------

#ifndef RUNINSTANTIATOR_H
#define RUNINSTANTIATOR_H

#include <string>
#include <map>
#include <QMutex>
#include "collisionDetection.h"
#include "spawnPointNetwork.h"
#include "world.h"

namespace SimulationSlave
{

class FrameworkConfig;
class AgentFactory;
class Stochastics;
class ObservationNetwork;

class RunInstantiator
{
public:    
    RunInstantiator(FrameworkConfig *frameworkConfig,
                    ObservationNetwork *observationNetwork,
                    AgentFactory *agentFactory,
                    World* world,
                    SpawnPointNetwork *spawnPointNetwork,
                    Stochastics *stochastics,
                    CollisionDetection *collisionDetection) :
        frameworkConfig(frameworkConfig),
        observationNetwork(observationNetwork),
        agentFactory(agentFactory),
        world(world),
        spawnPointNetwork(spawnPointNetwork),
        stochastics(stochastics),
        collisionDetection(collisionDetection)
    {}
    RunInstantiator(const RunInstantiator&) = delete;
    RunInstantiator(RunInstantiator&&) = delete;
    RunInstantiator& operator=(const RunInstantiator&) = delete;
    RunInstantiator& operator=(RunInstantiator&&) = delete;
    virtual ~RunInstantiator() = default;

    //-----------------------------------------------------------------------------
    //! @brief Executes the run by preparing the stochastics, world and observation
    //!     network instances, then scheduling for each run invocation an update
    //!     on the observation network, finalizing the run invocation and, after all
    //!     invocations have terminated, the observation network itself.
    //!
    //! Executes the run by initializing the stochastics generator with the random
    //! seed from the run configuration, instantiating and initializing the observation
    //! network with the observation instances from the run configuration and the
    //! observation result path from the framework configuration. For each run
    //! invocation:
    //! - configure the world paremeters from the run configuration
    //! - import the scenery file and the world global objects
    //! - init the observation network run
    //! - instantiate the world's spawn point network
    //! - start the scheduling with the observation networks's UpdateTimeStep as
    //!     update callback
    //! - finalize the run invocation using the result on the observation network
    //!
    //! Finally, finalize the observation network and clear teh world.
    //!
    //! @return                             Flag if the update was successful
    //-----------------------------------------------------------------------------
    bool ExecuteRun();

    //-----------------------------------------------------------------------------
    //! Stops the current run.
    //-----------------------------------------------------------------------------
    void StopRun();

private:
    QMutex stopMutex;
    bool stopped = true;

    FrameworkConfig *frameworkConfig;
    ObservationNetwork *observationNetwork;
    AgentFactory *agentFactory;
    World* world;
    SpawnPointNetwork *spawnPointNetwork;
    Stochastics *stochastics;
    CollisionDetection *collisionDetection;
};

} // namespace SimulationSlave

#endif // RUNINSTANTIATOR_H
