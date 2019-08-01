/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "respawner.h"
#include "agent.h"

Respawner::Respawner(Scheduler &scheduler, SimulationSlave::SpawnPoint* spawnPoint) :
    scheduler(scheduler),
    spawnPoint(spawnPoint)
{

}

void Respawner::RespawnAgent(int time)
{
    if(spawnPoint != nullptr)
    {
        const SimulationSlave::Agent* agent = spawnPoint->respawnAgent(time);

        if (agent->IsValid()) {
            scheduler.ScheduleAgentTasks(*agent);
    	}
    }
}
