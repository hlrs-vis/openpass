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

#include <memory>
#include <qglobal.h>
#include "sensor_collision_implementation.h"

Sensor_Collision_Implementation::Sensor_Collision_Implementation(int componentId,
                                                                 bool isInit,
                                                                 int priority,
                                                                 int offsetTime,
                                                                 int responseTime,
                                                                 int cycleTime,
                                                                 StochasticsInterface *stochastics,
                                                                 WorldInterface *world,
                                                                 const ParameterInterface *parameters, const std::map<int, ObservationInterface *> *observations,
                                                                 const CallbackInterface *callbacks,
                                                                 AgentInterface *agent) :
    SensorInterface(componentId,
                    isInit,
                    priority,
                    offsetTime,
                    responseTime,
                    cycleTime,
                    stochastics,
                    world,
                    parameters,
                    observations,
                    callbacks,
                    agent)
{
    try
    {
        std::map<int, int> parameterMapIntExternal = GetParameters()->GetParametersInt();
        foreach (auto &iterator, parameterMapInt)
        {
            int id = iterator.first;
            parameterMapInt.at(id)->SetValue(parameterMapIntExternal.at(id));
        }
    }
    catch (...)
    {
        const std::string msg = COMPONENTNAME + " could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}

Sensor_Collision_Implementation::~Sensor_Collision_Implementation()
{
}

void Sensor_Collision_Implementation::UpdateInput(int localLinkId,
                                                  const std::shared_ptr<SignalInterface const> &data, int time)
{
    // no input necessary
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);

    std::runtime_error("invalid link");
}

void Sensor_Collision_Implementation::UpdateOutput(int localLinkId,
                                                   std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateOutput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = outputPorts.at(localLinkId)->GetSignalValue(data);

    if (success)
    {
        log << COMPONENTNAME << " UpdateOutput successful";
        LOG(CbkLogLevel::Debug, log.str());
    }
    else
    {
        log << COMPONENTNAME << " UpdateOutput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

void Sensor_Collision_Implementation::Trigger(int time)
{
    std::vector<int> collisionPartners = GetAgent()->GetCollisionPartners();
    int numberOfCollisionData = 0;
    for (size_t i = 0; i < collisionPartners.size(); i++)
    {
        std::vector<void *> collisionData = GetAgent()->GetCollisionData(collisionPartners.at(i),
                                                                         POSTCRASHDYNAMICID);
        numberOfCollisionData += collisionData.size();
    }

    CollisionState nextState = CollisionState::NOCOLLISION;

    switch (collisionState)
    {
    case CollisionState::NOCOLLISION:
        if (previousNumberOfCollisionData == numberOfCollisionData)
        {
            nextState = CollisionState::NOCOLLISION;
        }
        else
        {
            if (penetrationTime.GetValue() < GetCycleTime())
            {
                nextState = CollisionState::COLLISION;
            }
            else
            {
                timeOfFirstCollision = time;
                nextState = CollisionState::COLLISIONPENETRATING;
            }
        }
        break;
    case CollisionState::COLLISIONPENETRATING:
        if (((time - timeOfFirstCollision) < (penetrationTime.GetValue() -
                                              GetCycleTime())) // collision penetration time is not over
                && (previousNumberOfCollisionData != numberOfCollisionData))   // collision is still happening
        {
            nextState = CollisionState::COLLISIONPENETRATING;
        }
        else
        {
            nextState = CollisionState::COLLISION;
        }
        break;
    case CollisionState::COLLISION:
        if (previousNumberOfCollisionData == numberOfCollisionData)
        {
            nextState = CollisionState::NOCOLLISION;
        }
        else
        {
            nextState = CollisionState::COLLISION;
        }
        break;
    }

    switch (nextState)
    {
    case CollisionState::NOCOLLISION:
    case CollisionState::COLLISIONPENETRATING:
        collisionOccured.SetValue(false);
        GetAgent()->SetCollisionState(false);
        break;
    case CollisionState::COLLISION:
        collisionOccured.SetValue(true);
        GetAgent()->SetCollisionState(true);
        break;
    }

    collisionState = nextState;

    previousNumberOfCollisionData = numberOfCollisionData;
}
