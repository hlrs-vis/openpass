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

//-----------------------------------------------------------------------------
//! @file  Scenery.h
//! @brief This file contains the representation of the static objects of the
//!        scenery.
//-----------------------------------------------------------------------------

#pragma once

#include <cassert>
#include <map>
#include "Interfaces/worldInterface.h"
#include "Interfaces/sceneryInterface.h"
#include "road.h"
#include "Interfaces/roadInterface/junctionInterface.h"
#include "junction.h"

namespace Configuration
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
    //! Adds a new junction to a scenery by creating a new Junction object and adding it
    //! via its ID to a stored mapping of junctions.
    //!
    //! @param[in]  id                  ID of the junction to add
    //! @return                         created junction
    //-----------------------------------------------------------------------------
    JunctionInterface *AddJunction(const std::string &id);


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
    //! Returns the stored list of junctions.
    //!
    //! @return                         list of junctions
    //-----------------------------------------------------------------------------
    std::map<std::string, JunctionInterface*> &GetJunctions()
    {
        return junctions;
    }

    //-----------------------------------------------------------------------------
    //! Returns the junction with the provided ID from the scenery.
    //!
    //! @param[in]  id                  ID of the junction
    //! @return                         junction with the provided ID
    //-----------------------------------------------------------------------------
    virtual JunctionInterface *GetJunction(const std::string& id)
    {
        JunctionInterface *junction;
        try
        {
            junction = junctions.at(id);
        }
        catch(const std::out_of_range&)
        {
            junction = nullptr;
        }

        return junction;
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

    std::map<std::string, JunctionInterface*> junctions;
};

} // namespace SimulationSlave


