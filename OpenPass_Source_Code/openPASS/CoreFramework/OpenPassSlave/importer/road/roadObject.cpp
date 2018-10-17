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

double RoadObject::GetHeight() const
{
    return object.height;
}

double RoadObject::GetPitch() const
{
    return object.pitch;
}

double RoadObject::GetRoll() const
{
    return object.roll;
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

