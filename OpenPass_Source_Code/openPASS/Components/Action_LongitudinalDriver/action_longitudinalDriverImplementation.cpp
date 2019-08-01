/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  action_longitudinalDriverImplementation.cpp */
//-----------------------------------------------------------------------------

#include <memory>
#include <qglobal.h>
#include "action_longitudinalDriverImplementation.h"
#include "Common/longitudinalSignal.h"

void ActionLongitudinalDriverImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " (component " << GetComponentName() << ", agent " << GetAgent()->GetId() << ", input data for local link " << localLinkId << ": ";
    LOG(CbkLogLevel::Debug, log.str());

    if(localLinkId == 0)
    {
        const std::shared_ptr<LongitudinalSignal const> signal = std::dynamic_pointer_cast<LongitudinalSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_accPedalPos = signal->accPedalPos; // percent value [0-1]
        in_brakePedalPos = signal->brakePedalPos;
        in_gear = signal->gear;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void ActionLongitudinalDriverImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);

    std::runtime_error("invalid link");
}

void ActionLongitudinalDriverImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    //! Set position of the accecaleration pedal position in percent.
    GetAgent()->SetEffAccelPedal(in_accPedalPos);
    //! Set position of the brake pedal position in percent.
    GetAgent()->SetEffBrakePedal(in_brakePedalPos);
    //! Set gear
    GetAgent()->SetGear(in_gear);
}
