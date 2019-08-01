/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "roadSignal.h"
#include <exception>

RoadSignalType RoadSignal::GetType() const
{
    return signal.type;
}

std::string RoadSignal::GetId() const
{
    return signal.id;
}

double RoadSignal::GetValue() const
{
    if(signal.unit == RoadSignalUnit::Undefined ||
       signal.unit == RoadSignalUnit::Meter ||
       signal.unit == RoadSignalUnit::MetersPerSecond ||
       signal.unit == RoadSignalUnit::Kilogram ||
       signal.unit == RoadSignalUnit::Percent)
    {
        return signal.value;
    }
    if(signal.unit == RoadSignalUnit::Kilometer)
    {
        return signal.value * convert_km;
    }
    if(signal.unit == RoadSignalUnit::Feet)
    {
        return signal.value * convert_ft;
    }
    if(signal.unit == RoadSignalUnit::LandMile)
    {
        return signal.value * convert_mile;
    }
    if(signal.unit == RoadSignalUnit::MilesPerHour)
    {
        return signal.value * convert_mph;
    }
    if(signal.unit == RoadSignalUnit::KilometersPerHour)
    {
        return signal.value * convert_km_h;
    }
    if(signal.unit == RoadSignalUnit::MetricTons)
    {
        return signal.value * convert_t;
    }

    throw std::runtime_error("RoadSignal::GetValue with unrecognized unit");
}


double RoadSignal::GetS() const
{
    return signal.s;
}

double RoadSignal::GetT() const
{
    return signal.t;
}

bool RoadSignal::IsValidForLane(int laneId) const
{
    return signal.validity.all ||
      ( std::find( signal.validity.lanes.begin(), signal.validity.lanes.end(), laneId) != signal.validity.lanes.end() );
}

double RoadSignal::GetHeight() const
{
    return signal.height;
}

double RoadSignal::GetWidth() const
{
    return signal.width;
}

double RoadSignal::GetLength() const
{
    return signal.length;
}

double RoadSignal::GetPitch() const
{
   return signal.pitch;
}

double RoadSignal::GetRoll() const
{
    return signal.roll;
}

double RoadSignal::GetYaw() const
{
    return signal.yaw;
}

bool RoadSignal::GetIsDynamic() const
{
    return signal.dynamic == "yes";
}

