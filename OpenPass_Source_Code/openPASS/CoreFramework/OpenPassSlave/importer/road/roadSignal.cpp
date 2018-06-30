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

bool RoadSignal::IsValidForLane(int laneId) const
{
    return signal.validity.all ||
      ( std::find( signal.validity.lanes.begin(), signal.validity.lanes.end(), laneId) != signal.validity.lanes.end() );
}

