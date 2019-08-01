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

#include <qglobal.h>
#include "dynamics_collision_implementation.h"

Dynamics_Collision_Implementation::Dynamics_Collision_Implementation(int componentId,
                                                                     bool isInit,
                                                                     int priority,
                                                                     int offsetTime,
                                                                     int responseTime,
                                                                     int cycleTime,
                                                                     StochasticsInterface *stochastics,
                                                                     WorldInterface *world,
                                                                     const ParameterInterface *parameters,
                                                                     const std::map<int, ObservationInterface *> *evaluations,
                                                                     const CallbackInterface *callbacks,
                                                                     AgentInterface *agent) :
    DynamicsInterface(componentId,
                      isInit,
                      priority,
                      offsetTime,
                      responseTime,
                      cycleTime,
                      stochastics,
                      world,
                      parameters,
                      evaluations,
                      callbacks,
                      agent)
{
}

void Dynamics_Collision_Implementation::UpdateInput(int localLinkId,
                                                    const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateInput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = inputPorts.at(localLinkId)->SetSignalValue(data);

    if (success) {
        log << COMPONENTNAME << " UpdateInput successful";
        LOG(CbkLogLevel::Debug, log.str());
    } else {
        log << COMPONENTNAME << " UpdateInput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

void Dynamics_Collision_Implementation::UpdateOutput(int localLinkId,
                                                     std::shared_ptr<SignalInterface const> &data, int time)
{
    // no outputs of the module
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);

    std::runtime_error("invalid link");
}

void Dynamics_Collision_Implementation::Trigger(int time)
{
    Q_UNUSED(time);

    CollisionState nextState = CollisionState::NOCOLLISION;
    bool in_collisionOccured = collisionOccured.GetValue();
    switch (collisionState) {
    case CollisionState::NOCOLLISION:
        if (in_collisionOccured) {
            nextState = CollisionState::COLLISIONIMPULS;
        } else {
            nextState = CollisionState::NOCOLLISION;
        }
        break;
    case CollisionState::COLLISIONIMPULS:
        if (in_collisionOccured) {
            nextState = CollisionState::COLLISION;
        } else {
            nextState = CollisionState::NOCOLLISION;
        }
        break;
    case CollisionState::COLLISION:
        if (in_collisionOccured) {
            nextState = CollisionState::COLLISION;
        } else {
            nextState = CollisionState::NOCOLLISION;
        }
        break;
    }

    collisionState = nextState; // immediate transition

    if (collisionState == CollisionState::COLLISIONIMPULS) {
        std::vector<int> collisionPartners = GetAgent()->GetCollisionPartners();
        if (collisionPartners.size() > 0) {
            int collisionPartnerId = collisionPartners.at(0);
            std::vector<void *> collisionData =
                GetAgent()->GetCollisionData(collisionPartnerId, POSTCRASHDYNAMICID);
            if (collisionData.size() > 0) {
                postCrashDynamic = static_cast<PostCrashDynamic *>(collisionData.back());
            }
        }
        if (postCrashDynamic != nullptr) { // check if postCrashDynamic is valid
            double cycleTime = (double)GetCycleTime() / 1000;

            double yawAngle = GetAgent()->GetYawAngle();
            Common::Vector2d velocityPrevious(GetAgent()->GetVelocityX(), GetAgent()->GetVelocityY()); // car CS
            velocityPrevious.Rotate(yawAngle); // global CS
            GetAgent()->SetPositionX( GetAgent()->GetPositionX() + velocityPrevious.x *
                                      cycleTime ); // global CS
            GetAgent()->SetPositionY( GetAgent()->GetPositionY() + velocityPrevious.y *
                                      cycleTime ); // global CS
            GetAgent()->SetYawAngle( yawAngle + GetAgent()->GetYawVelocity() * cycleTime);

            double velocity = postCrashDynamic->GetVelocity();
            double velocityDirection = postCrashDynamic->GetVelocityDirection();
            Common::Vector2d velocityVector(velocity * std::cos(velocityDirection),
                                            velocity * std::sin(velocityDirection));
            velocityVector.Rotate(-yawAngle);

            GetAgent()->SetVelocityX(velocityVector.x);
            GetAgent()->SetVelocityY(velocityVector.y);

            double yawVelocity = postCrashDynamic->GetYawVelocity();
            GetAgent()->SetYawVelocity(yawVelocity);
        }
        GetAgent()->SetAccelerationX(0.0);
        GetAgent()->SetAccelerationY(0.0);
        GetAgent()->SetYawAcceleration(0.0);

        postCrashDynamic = nullptr;
    }
}
