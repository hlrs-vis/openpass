/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <cassert>
#include <cmath>
#include <algorithm>

#include "CoreFramework/CoreShare/log.h"
#include "scenery.h"

namespace Configuration
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

JunctionInterface *Scenery::AddJunction(const std::string &id)
{
    Junction *junction = new (std::nothrow) Junction(id);

    if(!junction)
    {
        return nullptr;
    }

    if(!junctions.insert({id, junction}).second)
    {
        LOG_INTERN(LogLevel::Error) << "junctions must be unique";
        delete junction;
        return nullptr;
    }

    return junction;
}

} // namespace SimulationSlave

