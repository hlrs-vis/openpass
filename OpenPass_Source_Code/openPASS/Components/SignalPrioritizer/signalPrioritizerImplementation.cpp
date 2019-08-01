/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  SignalPrioritizerImplementation.cpp */
//-----------------------------------------------------------------------------

#include <exception>
#include <qglobal.h>
#include "signalPrioritizerImplementation.h"
#include "Common/primitiveSignals.h"

SignalPrioritizerImplementation::SignalPrioritizerImplementation(
        std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface *> *observations,
        const CallbackInterface *callbacks,
        AgentInterface *agent) :
    AlgorithmInterface (
        componentName,
        isInit,
        priority,
        offsetTime,
        responseTime,
        cycleTime,
        stochastics,
        parameters,
        observations,
        callbacks,
        agent)
{
    ParseSenderPriorities(parameters->GetParametersInt());
}

void SignalPrioritizerImplementation::ParseSenderPriorities(const std::map<std::string, int> &priorities)
{
    for (auto priority : priorities)
    {
        senderPriorities.insert({std::stoi(priority.first), priority.second});
    }
}

void SignalPrioritizerImplementation::UpdateInput(int localLinkId, const Signal &signal, int time)
{
    Q_UNUSED(time);

    const std::shared_ptr<ComponentStateSignalInterface const> componentSignal =
          std::dynamic_pointer_cast<ComponentStateSignalInterface const>(signal);

    if (componentSignal == nullptr)
    {
        // no input signal available
        return;
    }

    if (senderPriorities.find(localLinkId) == senderPriorities.end())
    {
        Error("sender with id " + std::to_string(localLinkId) + " not registered in priority list");
    }

    holdSignals[localLinkId] = componentSignal;
}

bool SignalPrioritizerImplementation::IsActive(const std::shared_ptr<ComponentStateSignalInterface const> &signal) const
{
    return signal && signal->componentState == ComponentState::Acting;
}

bool SignalPrioritizerImplementation::IsPrior(int firstSignalLokalLinkId,int secondSignalLokalLinkId) const
{
    if (secondSignalLokalLinkId == -1)
    {
        return true;
    }

    if (senderPriorities.find(firstSignalLokalLinkId) != senderPriorities.end()
            && senderPriorities.find(secondSignalLokalLinkId) != senderPriorities.end())
    {
        return senderPriorities.at(firstSignalLokalLinkId) > senderPriorities.at(secondSignalLokalLinkId);
    }

    Error("sender with id " + std::to_string(firstSignalLokalLinkId) + " not registered in priority list");
}

void SignalPrioritizerImplementation::UpdateOutput(int localLinkId, Signal &signal, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(time);

    std::shared_ptr<SignalInterface const> outputSignal = GetSignalWithHighestPriority();

    if (outputSignal != nullptr)
    {
        signal = outputSignal;
    }
    else
    {
        signal = std::make_shared<ComponentStateSignal>(ComponentState::Undefined);
    }
}

std::shared_ptr<SignalInterface const> SignalPrioritizerImplementation::GetSignalWithHighestPriority() const
{
    Signal signalWithHighestPriority = nullptr;
    int signalWithHighestPriorityLocalLinkId = -1;

    for (auto holdSignal : holdSignals)
    {
        if (IsActive(holdSignal.second)
                && IsPrior(holdSignal.first, signalWithHighestPriorityLocalLinkId))
        {
            signalWithHighestPriority = holdSignal.second;
            signalWithHighestPriorityLocalLinkId = holdSignal.first;
        }
    }

    return signalWithHighestPriority;
}

void SignalPrioritizerImplementation::Error(std::string message) const
{
    const std::string errorMsg = COMPONENTNAME + " " + message;
    LOG(CbkLogLevel::Debug, errorMsg);
    throw std::runtime_error(errorMsg);
}
