/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <memory>
#include <qglobal.h>
#include "dynamics_copyTrajectory_implementation.h"

Dynamics_CopyTrajectory_Implementation::Dynamics_CopyTrajectory_Implementation(int componentId,
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
    counter = 0;
    timeVec = GetAgent()->GetTrajectoryTime();
    xPosVec = GetAgent()->GetTrajectoryXPos();
    yPosVec = GetAgent()->GetTrajectoryYPos();
    velVec = GetAgent()->GetTrajectoryVelocity();
    psiVec = GetAgent()->GetTrajectoryAngle();
}

void Dynamics_CopyTrajectory_Implementation::UpdateInput(int localLinkId,
                                                         const std::shared_ptr<SignalInterface const> &data, int time)
{
    // no inputs of the module
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void Dynamics_CopyTrajectory_Implementation::UpdateOutput(int localLinkId,
                                                          std::shared_ptr<SignalInterface const> &data, int time)
{
    // no outputs of the module
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void Dynamics_CopyTrajectory_Implementation::Trigger(int time)
{
    Q_UNUSED(time);
    AgentInterface *ownAgent = GetAgent();

    ownAgent->SetPositionX(xPosVec->at(counter));
    ownAgent->SetPositionY(yPosVec->at(counter));
    ownAgent->SetVelocityX(velVec->at(counter));
    ownAgent->SetYawAngle(psiVec->at(counter));

    std::stringstream log;
    log << GetComponentId() << " (agent " << ownAgent->GetAgentId() << "): newX = " << xPosVec->at(
            counter) << ", newY = " << yPosVec->at(counter);
    LOG(CbkLogLevel::Debug, log.str());

    counter++;

}

