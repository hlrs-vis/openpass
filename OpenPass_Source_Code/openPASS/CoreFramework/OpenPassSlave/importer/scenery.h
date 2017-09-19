/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  scenery.h
//! @brief This file contains the representation of the static objects of the
//!        scenery.
//-----------------------------------------------------------------------------

#ifndef SCENERY_H
#define SCENERY_H

#include <cassert>
#include <map>
#include "worldInterface.h"
#include "sceneryInterface.h"
#include "vector2d.h"
#include "road.h"

namespace SimulationSlave
{

//-----------------------------------------------------------------------------
//! Class representing a scenery as a list of roads.
//-----------------------------------------------------------------------------
class Scenery :public SceneryInterface
{
public:
    Scenery() = default;
    Scenery(const Scenery&) = delete;
    Scenery(Scenery&&) = delete;
    Scenery& operator=(const Scenery&) = delete;
    Scenery& operator=(Scenery&&) = delete;
    virtual ~Scenery();

    //-----------------------------------------------------------------------------
    //! Clears the scenery by deleting all stored roads.
    //-----------------------------------------------------------------------------
    void Clear();

    //-----------------------------------------------------------------------------
    //! Adds a new road to a scenery by creating a new Road object and adding it
    //! via its ID to a stored mapping of roads.
    //!
    //! @param[in]  id                  ID of the road to add
    //! @return                         created road
    //-----------------------------------------------------------------------------
    RoadInterface *AddRoad(const std::string &id);


    //-----------------------------------------------------------------------------
    //! Returns the stored list of roads.
    //!
    //! @return                         list of roads
    //-----------------------------------------------------------------------------
    std::map<std::string, RoadInterface*> &GetRoads()
    {
        return roads;
    }

    //-----------------------------------------------------------------------------
    //! Returns the road with the provided ID from the scenery.
    //!
    //! @param[in]  id                  ID of the road
    //! @return                         road with the provided ID
    //-----------------------------------------------------------------------------
    RoadInterface *GetRoad(const std::string& id)
    {
        RoadInterface *road;
        try
        {
            road = roads.at(id);
        }
        catch(const std::out_of_range&)
        {
            road = nullptr;
        }

        return road;
    }

private:
    std::map<std::string, RoadInterface*> roads;
};

} // namespace SimulationSlave

#endif // SCENERY_H
