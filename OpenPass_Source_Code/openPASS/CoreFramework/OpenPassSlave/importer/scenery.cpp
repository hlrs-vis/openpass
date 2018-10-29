/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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

