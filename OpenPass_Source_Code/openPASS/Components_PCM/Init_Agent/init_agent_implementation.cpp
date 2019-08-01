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
#include "init_agent_implementation.h"

Init_Agent_Implementation::Init_Agent_Implementation(
    int componentId,
    bool isInit,
    int priority,
    int offsetTime,
    int responseTime,
    int cycleTime,
    StochasticsInterface *stochastics,
    WorldInterface *world,
    const ParameterInterface *parameters,
    const std::map<int, ObservationInterface *> *observations,
    const CallbackInterface *callbacks,
    AgentInterface *agent) :
    InitInterface(
        componentId,
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
    // read parameters
    try
    {
    }
    catch (...)
    {
        const std::string msg = COMPONENTNAME + " could not init parameters";
        LOG(CbkLogLevel::Error, msg);
        throw std::runtime_error(msg);
    }
}

void Init_Agent_Implementation::UpdateInput(int localLinkId,
                                            const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);
}

void Init_Agent_Implementation::UpdateOutput(int localLinkId,
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

void Init_Agent_Implementation::Trigger(int time)
{
    Q_UNUSED(time);

    weight.SetValue(GetAgent()->GetWeight());
    wheelbase.SetValue(GetAgent()->GetWheelbase());
    distanceToCOG.SetValue(GetAgent()->GetDistanceCOGtoFrontAxle());
    trajectory.SetValue(PCM_Trajectory(GetAgent()->GetTrajectoryTime(),
                                       GetAgent()->GetTrajectoryXPos(),
                                       GetAgent()->GetTrajectoryYPos(),
                                       GetAgent()->GetTrajectoryVelocity(),
                                       nullptr,
                                       GetAgent()->GetTrajectoryAngle()));
}
