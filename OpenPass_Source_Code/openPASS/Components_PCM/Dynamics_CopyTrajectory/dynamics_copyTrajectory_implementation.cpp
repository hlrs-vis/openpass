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
    timeStep = static_cast<double> (GetCycleTime()) / 1000.0;
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

    if (counter < timeVec->size())
    {
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
    else
    {
        AgentInterface *ownAgent = GetAgent();

        ownAgent->SetPositionX(GetAgent()->GetPositionX() + timeStep * velVec->at(counter - 1)
                               * cos(psiVec->at(counter - 1)));
        ownAgent->SetPositionY(GetAgent()->GetPositionY() + timeStep * velVec->at(counter - 1)
                               * sin(psiVec->at(counter - 1)));
        ownAgent->SetVelocityX(velVec->at(counter - 1));
        ownAgent->SetYawAngle(psiVec->at(counter - 1));

        std::stringstream log;
        log << GetComponentId() << " (agent " << ownAgent->GetAgentId() << "): newX = " << xPosVec->at(
                counter - 1) << ", newY = " << yPosVec->at(counter - 1);
        LOG(CbkLogLevel::Debug, log.str());
    }

}

