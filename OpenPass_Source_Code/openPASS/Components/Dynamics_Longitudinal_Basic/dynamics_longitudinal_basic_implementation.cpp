/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#include <memory>
#include <qglobal.h>
#include <cassert>
#include "dynamics_longitudinal_basic_implementation.h"

void Dynamics_Longitudinal_Basic_Implementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        // from Algorithm_MergeDriverIBrake
        const std::shared_ptr<DoubleSignal const> signal = std::dynamic_pointer_cast<DoubleSignal const>(data);
        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_aVehicle = signal->value;
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void Dynamics_Longitudinal_Basic_Implementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);
    Q_UNUSED(time);

    std::runtime_error("invalid link");

}

void Dynamics_Longitudinal_Basic_Implementation::Trigger(int time)
{
    Q_UNUSED(time);

    double v = GetAgent()->GetVelocityX() + in_aVehicle*GetCycleTime()/1000.;

    if (v >= _VLowerLimit)
    {
        GetAgent()->SetVelocityX(v);
        GetAgent()->SetAccelerationX(in_aVehicle);
        GetAgent()->SetPositionX(GetAgent()->GetPositionX() + v * GetCycleTime() / 1000.0);
    }
    else
    {
        GetAgent()->SetVelocityX(_VLowerLimit);
        GetAgent()->SetAccelerationX(0);
        GetAgent()->SetPositionX(GetAgent()->GetPositionX() + _VLowerLimit * GetCycleTime() / 1000.0);
    }
}

