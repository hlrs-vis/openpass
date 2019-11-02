/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <cassert>
#include <sstream>
#include "observation_collision_implementation.h"

Observation_Collision_Implementation::Observation_Collision_Implementation(
    StochasticsInterface *stochastics,
    WorldInterface *world,
    const ParameterInterface *parameters,
    const CallbackInterface *callbacks) :
    ObservationInterface(stochastics, world, parameters, callbacks)
{
    std::stringstream log;
    log << "Constructing " << COMPONENTNAME;
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    std::map<int, double> parameterMapDoubleExternal = GetParameters()->GetParametersDouble();
    foreach (auto &iterator, parameterMapDouble)
    {
        int id = iterator.first;
        parameterMapDouble.at(id)->SetValue(parameterMapDoubleExternal.at(id));
    }

    log << "Construction of " << COMPONENTNAME << " successful";
    LOG(CbkLogLevel::Debug, log.str());
}

void Observation_Collision_Implementation::SlavePreHook(const std::string &path)
{
    Q_UNUSED(path);
}

void Observation_Collision_Implementation::SlavePreRunHook()
{

}

void Observation_Collision_Implementation::SlaveUpdateHook(int time, RunResultInterface &runResult)
{
    Q_UNUSED(time);

    if (runResult.GetCollisions()->size() > 0 )
    {
        collisionOccured = true;
    }

    if (collisionOccured)
    {
        bool stop = true;
        for (auto &agentIterator : GetWorld()->GetAgents())
        {
            if (agentIterator.second->GetVehicleType() == AgentVehicleType::Car &&
                    agentIterator.second->GetVelocityAbsolute() > endVelocity.GetValue())
            {
                stop = false;
                break;
            }
        }
        if (stop)
        {
            runResult.SetEndCondition();
        }
    }
}

void Observation_Collision_Implementation::SlavePostRunHook(const RunResultInterface &runResult)
{
    Q_UNUSED(runResult);
}

void Observation_Collision_Implementation::SlavePostHook()
{

}
