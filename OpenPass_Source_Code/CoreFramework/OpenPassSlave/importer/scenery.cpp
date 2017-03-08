/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <cassert>
#include <cmath>
#include <algorithm>
#include "scenery.h"
#include "vector2d.h"
#include "log.h"

namespace SimulationSlave
{

Scenery::~Scenery()
{
    Clear();
}

void Scenery::Clear()
{
    for(std::pair<const std::string, RoadInterface*> &item : roads)
    {
        delete item.second;
    }

    roads.clear();
}

RoadInterface *Scenery::AddRoad(const std::string &id)
{
    Road *road = new (std::nothrow) Road(id);

    if(!road)
    {
        return nullptr;
    }

    if(!roads.insert({id, road}).second)
    {
        LOG_INTERN(LogLevel::Error) << "roads must be unique";
        delete road;
        return nullptr;
    }

    return road;
}

} // namespace SimulationSlave

