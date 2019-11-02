/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  dynamics_collisionImplementation.cpp
//! @brief This file contains the implementation header file
//-----------------------------------------------------------------------------

#include "dynamics_collisionImplementation.h"
#include <qglobal.h>

DynamicsCollisionImplementation::DynamicsCollisionImplementation(std::string componentName,
                                                                 bool isInit,
                                                                 int priority,
                                                                 int offsetTime,
                                                                 int responseTime,
                                                                 int cycleTime,
                                                                 StochasticsInterface *stochastics,
                                                                 WorldInterface *world,
                                                                 const ParameterInterface *parameters,
                                                                 const std::map<int, ObservationInterface*> *observations,
                                                                 const CallbackInterface *callbacks,
                                                                 AgentInterface *agent)
    : UnrestrictedModelInterface { componentName,
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
                                   agent },
      dynamicsSignal{}
{
    dynamicsSignal.componentState = ComponentState::Disabled;
}

void DynamicsCollisionImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void DynamicsCollisionImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try
        {
            data = std::make_shared<DynamicsSignal const>(dynamicsSignal);
        }
        catch(const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void DynamicsCollisionImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    if (GetAgent()->GetCollisionPartners().size() > numberOfCollisionPartners)
    {
        numberOfCollisionPartners = GetAgent()->GetCollisionPartners().size();
        isActive = true;
        dynamicsSignal.componentState = ComponentState::Acting;
        bool collisionWithFixedObject = false;

        //Calculate new velocity by inelastic collision
        double sumOfWeights = GetAgent()->GetWeight();
        double sumOfImpulsesX = GetAgent()->GetVelocity() * GetAgent()->GetWeight() * std::cos(GetAgent()->GetYaw());
        double sumOfImpulsesY = GetAgent()->GetVelocity() * GetAgent()->GetWeight() * std::sin(GetAgent()->GetYaw());

        auto collisionPartners = GetAgent()->GetCollisionPartners();
        for (const auto &partner : collisionPartners)
        {
            if (partner.first == ObjectTypeOSI::Object)
            {
                collisionWithFixedObject = true;
                break;
            }
            const AgentInterface* partnerAgent = GetWorld()->GetAgent(partner.second);
            if (partnerAgent != nullptr)
            {
                sumOfWeights += partnerAgent->GetWeight();
                sumOfImpulsesX += partnerAgent->GetVelocity() * partnerAgent->GetWeight() * std::cos(partnerAgent->GetYaw());
                sumOfImpulsesY += partnerAgent->GetVelocity() * partnerAgent->GetWeight() * std::sin(partnerAgent->GetYaw());
            }
        }

        if (collisionWithFixedObject)
        {
            velocity = 0.0;
        }
        else
        {
            double velocityX = sumOfImpulsesX / sumOfWeights;
            double velocityY = sumOfImpulsesY / sumOfWeights;
            velocity = std::hypot(velocityX, velocityY);
            if (velocityY > 0.0)
            {
                movingDirection = std::acos(velocityX / velocity);
            }
            else if (velocity != 0.0)
            {
                movingDirection = -std::acos(velocityX / velocity);
            }
            else
            {
                movingDirection = 0.0;
            }
        }
        dynamicsSignal.yaw = GetAgent()->GetYaw();
        dynamicsSignal.yawRate = GetAgent()->GetYawRate();
    }
    if (isActive)
    {
        const double deceleration = 10.0;
        velocity -= deceleration * GetCycleTime() * 0.001;
        velocity = std::max(0.0, velocity);
        // change of path coordinate since last time step [m]
        double ds = velocity * static_cast<double>(GetCycleTime()) * 0.001;
        // change of inertial x-position due to ds and yaw [m]
        double dx = ds * std::cos(movingDirection);
        // change of inertial y-position due to ds and yaw [m]
        double dy = ds * std::sin(movingDirection);
        // new inertial x-position [m]
        double x = GetAgent()->GetPositionX() + dx;
        // new inertial y-position [m]
        double y = GetAgent()->GetPositionY() + dy;

        dynamicsSignal.velocity = velocity;
        dynamicsSignal.acceleration = 0.0;
        dynamicsSignal.positionX = x;
        dynamicsSignal.positionY = y;
        dynamicsSignal.travelDistance = ds;
    }
}
