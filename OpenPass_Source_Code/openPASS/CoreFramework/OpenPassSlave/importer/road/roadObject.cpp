/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
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

