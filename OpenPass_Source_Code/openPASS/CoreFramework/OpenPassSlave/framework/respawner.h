/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include "Interfaces/respawnInterface.h"
#include "spawnPoint.h"
#include "scheduler.h"

class Respawner : public RespawnInterface
{
public:
    Respawner(SimulationSlave::Scheduler &scheduler,
              SimulationSlave::SpawnPoint *spawnPoint);

    virtual void RespawnAgent(int time);

private:
    SimulationSlave::Scheduler &scheduler;
    SimulationSlave::SpawnPoint *spawnPoint{nullptr};
};
