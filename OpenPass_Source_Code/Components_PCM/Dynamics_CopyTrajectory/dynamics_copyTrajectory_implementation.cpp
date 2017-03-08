/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
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
    SensorInterface(componentId,
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
                   agent),
    _timeVec(*GetParameters()->GetParametersIntVector().at(0)),
    _xPosVec(*GetParameters()->GetParametersDoubleVector().at(1)),
    _yPosVec(*GetParameters()->GetParametersDoubleVector().at(2)),
    _velVec(*GetParameters()->GetParametersDoubleVector().at(3)),
    _psiVec(*GetParameters()->GetParametersDoubleVector().at(4))
{
    _counter = 0;
}

void Dynamics_CopyTrajectory_Implementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    // no inputs of the module
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void Dynamics_CopyTrajectory_Implementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    // no outputs of the module
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void Dynamics_CopyTrajectory_Implementation::Trigger(int time)
{
    Q_UNUSED(time);
    AgentInterface* ownAgent = GetAgent();

    ownAgent->SetPositionX(_xPosVec[_counter]);
    ownAgent->SetPositionY(_yPosVec[_counter]);
    ownAgent->SetVelocityX(_velVec[_counter]);
    ownAgent->SetYawAngle(_psiVec[_counter]);

    std::stringstream log;
    log << GetComponentId() << " (agent " << ownAgent->GetAgentId() << "): newX = " << _xPosVec[_counter] << ", newY = " << _yPosVec[_counter];
    LOG(CbkLogLevel::Debug, log.str());

    _counter++;

}
