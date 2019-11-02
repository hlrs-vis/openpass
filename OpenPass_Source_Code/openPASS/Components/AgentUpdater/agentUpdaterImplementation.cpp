/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  agentUpdaterImplementation.cpp
//! @brief This file contains the implementation of the header file
//-----------------------------------------------------------------------------

#include "agentUpdaterImplementation.h"
#include <qglobal.h>

void AgentUpdaterImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
        {
            // from DynamicsPrioritizer
            const std::shared_ptr<DynamicsSignal const> signal = std::dynamic_pointer_cast<DynamicsSignal const>(data);
            if (!signal)
            {
                const std::string msg = COMPONENTNAME + " invalid signaltype";
                LOG(CbkLogLevel::Debug, msg);
                throw std::runtime_error(msg);
            }

            acceleration = signal->acceleration;
            velocity = signal->velocity;
            positionX = signal->positionX;
            positionY = signal->positionY;
            yaw = signal->yaw;
            yawRate = signal->yawRate;
            steeringWheelAngle = signal->steeringWheelAngle;
            travelDistance = signal->travelDistance;
        }
        else
        {
            const std::string msg = COMPONENTNAME + " invalid link";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
}

void AgentUpdaterImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void AgentUpdaterImplementation::Trigger(int time)
{
    Q_UNUSED(time);
    AgentInterface *agent = GetAgent();

    agent->SetAcceleration(acceleration);
    agent->SetVelocity(velocity);
    agent->SetPositionX(positionX);
    agent->SetPositionY(positionY);
    agent->SetYaw(yaw);
    agent->SetYawRate(yawRate);
    agent->SetSteeringWheelAngle(steeringWheelAngle);
    agent->SetDistanceTraveled(travelDistance + agent->GetDistanceTraveled());
}
