#include "roadObject.h"
#include <exception>

RoadObjectType RoadObject::GetType() const
{
    return object.type;
}

double RoadObject::GetLength() const
{
    return object.length;
}

double RoadObject::GetWidth() const
{
    return object.width;
}

double RoadObject::GetHdg() const
{
    return object.hdg;
}

std::string RoadObject::GetId() const
{
    return object.id;
}

double RoadObject::GetS() const
{
    return object.s;
}

double RoadObject::GetT() const
{
    return object.t;
}

bool RoadObject::IsValidForLane(int laneId) const
{
    return object.validity.all ||
      ( std::find( object.validity.lanes.begin(), object.validity.lanes.end(), laneId) != object.validity.lanes.end() );
}

