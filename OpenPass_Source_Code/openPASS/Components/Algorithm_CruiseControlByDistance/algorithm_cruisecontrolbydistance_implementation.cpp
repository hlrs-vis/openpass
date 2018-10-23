/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <qglobal.h>
#include "algorithm_cruisecontrolbydistance_implementation.h"
#include "primitiveSignals.h"

void Algorithm_CruiseControlByDistance_Implementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId==0)
    {
        const std::shared_ptr<DoubleSignal const> signal = std::dynamic_pointer_cast<DoubleSignal const>(data);
        if(!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
        in_distance2NextAgent = signal->value;
    }
    else if (localLinkId==1)
    {
        const std::shared_ptr<DoubleSignal const> signal = std::dynamic_pointer_cast<DoubleSignal const>(data);
        if(!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
        in_velocity = signal->value;
    }
    else
    {
        const std::string msgDefault = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msgDefault);
        throw std::runtime_error(msgDefault);
    }

}

void Algorithm_CruiseControlByDistance_Implementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try {
            data = std::make_shared<DoubleSignal const>(out_desiredAcceleration);
        }
        catch(const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }else if(localLinkId == 1)
    {
        try {
            data = std::make_shared<DoubleSignal const>(out_aCoasting);
        }catch(const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void Algorithm_CruiseControlByDistance_Implementation::Trigger(int time)
{


    out_desiredAcceleration = 0;
    if(time > 0)
    {
        out_aCoasting = -_airDragCoefficent * pow(in_velocity,2); // negative acceleration caused by the air

        double gapMin = in_velocity * 3.6 / 2; // The desired gap shall be half the velocity in km/h

        if((in_distance2NextAgent > gapMin))
        { //nobody is in front or Sensor is mistaken OR distance to next agent is bigger than the desired gap
            if(in_velocity > Par_desiredVelocity)
            {// Check the desired velocity
                out_desiredAcceleration = out_aCoasting; // Don't brake, just roll out.
            }
            else
            {
                out_desiredAcceleration = (Par_desiredVelocity - in_velocity) / _standardCycleTimeSecond ; // Accelerate maximally
            }
        }
        else
        {
            double ttc = _standardCycleTimeSecond;
            out_desiredAcceleration = 2*(in_distance2NextAgent - gapMin) / ((ttc + Par_reactionTime) * _standardCycleTimeSecond); //Decelerate maximally
        }
    }

    out_desiredAcceleration = PlausibleTestAcceleration(out_desiredAcceleration);

    previousDistance = in_distance2NextAgent;
}

double Algorithm_CruiseControlByDistance_Implementation::PlausibleTestAcceleration(double desiredAcceleration)
{
    if(desiredAcceleration > 0)
    {
        return std::min(desiredAcceleration, _accelerationMax);
    }
    else
    {
        return std::max(desiredAcceleration, accelerationMin);
    }
}
