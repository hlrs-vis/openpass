/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <memory>
#include <qglobal.h>
#include "action_brakelight_basic_implementation.h"

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
