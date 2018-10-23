/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* Copyright (c) 2018 in-tech GmbH.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <memory>
#include <qglobal.h>
#include "action_brakelight_basic_implementation.h"
#include "brakingSignal.h"

void Action_BrakeLight_Basic_Implementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " (component " << GetComponentId() << ", agent " << GetAgent()->GetAgentId() << ", input data for local link " << localLinkId << ": ";
    LOG(CbkLogLevel::Debug, log.str());

    if (localLinkId == 0)
    {
        const std::shared_ptr<DoubleSignal const> signal = std::dynamic_pointer_cast<DoubleSignal const>(data);
        if(!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_aVehicle = signal->value;
    }
    else if (localLinkId == 1)
    {
        const std::shared_ptr<DoubleSignal const> signal = std::dynamic_pointer_cast<DoubleSignal const>(data);
        if(!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_aCoasting = signal->value;
    }
    else if (localLinkId == 2)
    {
        const std::shared_ptr<BrakingSignal const> signal = std::dynamic_pointer_cast<BrakingSignal const>(data);
        if(!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_aVehicle = signal->acceleration;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void Action_BrakeLight_Basic_Implementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);

    std::runtime_error("invalid link");
}

void Action_BrakeLight_Basic_Implementation::Trigger(int time) 
{
    Q_UNUSED(time);

    GetAgent()->SetBrakeLight( in_aVehicle < -in_aCoasting );
}
