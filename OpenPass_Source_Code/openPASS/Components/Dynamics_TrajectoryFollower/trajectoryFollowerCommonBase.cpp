/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  trajectoryFollowerCommonBase.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>

#include "trajectoryFollowerCommonBase.h"

TrajectoryFollowerCommonBase::TrajectoryFollowerCommonBase(std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface* stochastics,
        WorldInterface* world,
        const ParameterInterface* parameters,
        const std::map<int, ObservationInterface*>* observations,
        const CallbackInterface* callbacks,
        AgentInterface* agent) :
    UnrestrictedModelInterface(
        componentName,
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
    currentWorldPosition.xPos = 0;
    currentWorldPosition.yPos = 0;
    currentWorldPosition.yawAngle = 0;

    cycleTimeInSeconds = static_cast<double>(cycleTime) / 1000.0;

    ParseParameters(parameters);
}

void TrajectoryFollowerCommonBase::ParseParameters(const ParameterInterface* parameters)
{
    try
    {
        auto boolParameters = parameters->GetParametersBool();
        enforceTrajectory = boolParameters.at("EnforceTrajectory");
        automaticDeactivation = boolParameters.at("AutomaticDeactivation");
    }
    catch (const std::out_of_range& error)
    {
        LOG(CbkLogLevel::Error, error.what());
        throw std::runtime_error(error.what());
    }
}

void TrajectoryFollowerCommonBase::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const>& data,
        int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        if (!enforceTrajectory)
        {
            const std::shared_ptr<ComponentStateSignalInterface const> stateSignal =
                std::dynamic_pointer_cast<ComponentStateSignalInterface const>(data);
            if (stateSignal != nullptr && stateSignal->componentState == ComponentState::Acting)
            {
                const std::shared_ptr<LateralSignal const> signal = std::dynamic_pointer_cast<LateralSignal const>(data);
                if (!signal)
                {
                    ThrowInvalidSignalTypeError();
                }

                const bool isLateralSignalActing = signal->componentState == ComponentState::Acting;
                if (isLateralSignalActing)
                {
                    UpdateState(ComponentState::Disabled);
                }
            }
        }
    }
    else
        if (localLinkId == 1)
        {
            if (!enforceTrajectory)
            {
                const std::shared_ptr<ComponentStateSignalInterface const> stateSignal =
                    std::dynamic_pointer_cast<ComponentStateSignalInterface const>(data);
                if (stateSignal != nullptr && stateSignal->componentState == ComponentState::Acting)
                {
                    const std::shared_ptr<AccelerationSignal const> signal = std::dynamic_pointer_cast<AccelerationSignal const>(data);
                    if (!signal)
                    {
                        ThrowInvalidSignalTypeError();
                    }

                    inputAccelerationActive = true;
                    inputAcceleration = signal->acceleration;
                }
                else
                {
                    inputAcceleration = 0;
                }
            }
        }
        else if (localLinkId == 83)
        {
            HandleCompCtrlSignalInput(data);
        }
        else
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Error, msg);
            throw std::runtime_error(msg);
        }
}

void TrajectoryFollowerCommonBase::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const>& data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        try
        {
            data = std::make_shared<DynamicsSignal const>(componentState,
                    currentAcceleration,
                    currentVelocity,
                    currentWorldPosition.xPos,
                    currentWorldPosition.yPos,
                    currentWorldPosition.yawAngle,
                    currentYawRate,
                    0.0,
                    distance);
        }
        catch (const std::bad_alloc&)
        {
            ThrowCouldNotInstantiateSignalError();
        }
    }
    else if (localLinkId == 83)
    {
        HandleCompCtrlSignalOutput(data);
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

Position TrajectoryFollowerCommonBase::GetLastWorldPosition()
{
    return currentWorldPosition;
}

double TrajectoryFollowerCommonBase::GetLastVelocity()
{
    return currentVelocity;
}

double TrajectoryFollowerCommonBase::GetLastAcceleration()
{
    return currentAcceleration;
}

void TrajectoryFollowerCommonBase::HandleCompCtrlSignalInput(const std::shared_ptr<SignalInterface const>& data)
{
    const std::shared_ptr<CompCtrlToAgentCompSignal const> ccSignal =
        std::dynamic_pointer_cast<CompCtrlToAgentCompSignal const>(data);
    if (!ccSignal)
    {
        ThrowInvalidSignalTypeError();
    }
    else
    {
        if (ccSignal->GetMaxReachableState() != ComponentState::Undefined)
        {
            UpdateState(ccSignal->GetMaxReachableState());
        }
    }
}

void TrajectoryFollowerCommonBase::HandleCompCtrlSignalOutput(std::shared_ptr<SignalInterface const>& data)
{
    try
    {
        data = std::make_shared<AgentCompToCompCtrlSignal>(ComponentType::TrajectoryFollower,
                                                           GetComponentName(),
                                                           componentState);
    }
    catch (const std::bad_alloc&)
    {
        ThrowCouldNotInstantiateSignalError();
    }
}

ComponentState TrajectoryFollowerCommonBase::GetState() const
{
    return componentState;
}

void TrajectoryFollowerCommonBase::UpdateState(const ComponentState newState)
{
    // only update state if the newstate differs from the current state
    if (newState != componentState)
    {
        if (newState == ComponentState::Acting)
        {
            if (canBeActivated)
            {
                componentState = newState;
                std::shared_ptr<AgentBasedEvent> event = std::make_shared<AgentBasedEvent>(currentTime,
                        GetComponentName(),
                        "TEST_ACTIVATE",
                        EventDefinitions::EventType::TrajectoryFollowerActivated,
                        GetAgent()->GetId());

                GetObservations()->at(0)->InsertEvent(event);
            }
        }
        else if (newState == ComponentState::Disabled)
        {
            canBeActivated = false;
            componentState = newState;
            std::shared_ptr<AgentBasedEvent> event = std::make_shared<AgentBasedEvent>(currentTime,
                    GetComponentName(),
                    "TEST_DEACTIVATE",
                    EventDefinitions::EventType::TrajectoryFollowerDeactivated,
                    GetAgent()->GetId());

            GetObservations()->at(0)->InsertEvent(event);
        }
        else
        {
            throw std::runtime_error("TrajectoryFollower cannot handle new component state");
        }
    }
}

void TrajectoryFollowerCommonBase::ThrowCouldNotInstantiateSignalError()
{
    const std::string msg = COMPONENTNAME + " could not instantiate signal";
    LOG(CbkLogLevel::Debug, msg);
    throw std::runtime_error(msg);
}

void TrajectoryFollowerCommonBase::ThrowInvalidSignalTypeError()
{
    const std::string msg = COMPONENTNAME + " invalid signaltype";
    LOG(CbkLogLevel::Debug, msg);
    throw std::runtime_error(msg);
}
