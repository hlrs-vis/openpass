/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  action_secondaryDriverTasksImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include "action_secondaryDriverTasksImplementation.h"
#include "Common/secondaryDriverTasksSignal.h"

void ActionSecondaryDriverTasksImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " (component " << GetComponentName() << ", agent " << GetAgent()->GetId() << ", input data for local link " << localLinkId << ": ";
    LOG(CbkLogLevel::Debug, log.str());

    if(localLinkId == 0)
    {
        const std::shared_ptr<SecondaryDriverTasksSignal const> signal = std::dynamic_pointer_cast<SecondaryDriverTasksSignal const>(data);

        if(!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
        in_IndicatorState = (IndicatorState)signal->indicatorState;
        in_hornSwitch = signal->hornSwitch;
        in_headLightSwitch = signal->headLightSwitch;
        in_highBeamLightSwitch = signal->highBeamLightSwitch;
        in_flasherSwitch = signal->flasherSwitch;
    }

    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void ActionSecondaryDriverTasksImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);

    std::runtime_error("invalid link");
}

void ActionSecondaryDriverTasksImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    if (GetAgent()->GetEffBrakePedal()>0)
    {
        GetAgent()->SetBrakeLight(true);
    }else
    {
        GetAgent()->SetBrakeLight(false);
    }

    GetAgent()->SetIndicatorState(in_IndicatorState);
    GetAgent()->SetHeadLight(in_headLightSwitch);
    GetAgent()->SetHighBeamLight(in_highBeamLightSwitch);
    GetAgent()->SetHorn(in_hornSwitch);
    GetAgent()->SetFlasher(in_flasherSwitch);
}
