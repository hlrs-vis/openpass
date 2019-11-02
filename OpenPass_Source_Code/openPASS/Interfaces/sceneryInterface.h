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
//! @file  sceneryInterface.h
//! @brief This file contains the interface to translate the scenery between
//!        framework and world.
//-----------------------------------------------------------------------------

#ifndef SCENERYINTERFACE
#define SCENERYINTERFACE

#include <map>
#include "roadInterface/roadInterface.h"
#include "roadInterface/junctionInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a scenery as a list of roads.
//-----------------------------------------------------------------------------
class SceneryInterface
{
public:
    SceneryInterface() = default;
    //SceneryInterface(const SceneryInterface&) = delete;
    //SceneryInterface(SceneryInterface&&) = delete;
    //SceneryInterface& operator=(const SceneryInterface&) = delete;
    //SceneryInterface& operator=(SceneryInterface&&) = delete;
    virtual ~SceneryInterface() = default;

    //-----------------------------------------------------------------------------
    //! Clears the scenery by deleting all stored roads.
    //-----------------------------------------------------------------------------
    virtual void Clear() = 0;

    //-----------------------------------------------------------------------------
    //! Adds a new road to a scenery by creating a new Road object and adding it
    //! via its ID to a stored mapping of roads.
    //!
    //! @param[in]  id                  ID of the road to add
    //! @return                         created road
    //-----------------------------------------------------------------------------
    virtual RoadInterface *AddRoad(const std::string &id) = 0;

    //-----------------------------------------------------------------------------
    //! Adds a new junction to a scenery by creating a new Junction object and adding it
    //! via its ID to a stored mapping of junctions.
    //!
    //! @param[in]  id                  ID of the junction to add
    //! @return                         created junction
    //-----------------------------------------------------------------------------
    virtual JunctionInterface *AddJunction(const std::string &id) = 0;


    //-----------------------------------------------------------------------------
    //! Returns the stored list of roads.
    //!
    //! @return                         list of roads
    //-----------------------------------------------------------------------------
    virtual std::map<std::string, RoadInterface*> &GetRoads() = 0;


    //-----------------------------------------------------------------------------
    //! Returns the road with the provided ID from the scenery.
    //!
    //! @param[in]  id                  ID of the road
    //! @return                         road with the provided ID
    //-----------------------------------------------------------------------------
    virtual RoadInterface *GetRoad(const std::string& id) = 0;


    //-----------------------------------------------------------------------------
    //! Returns the stored list of junctions.
    //!
    //! @return                         list of roads
    //-----------------------------------------------------------------------------
    virtual std::map<std::string, JunctionInterface*> &GetJunctions() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the junction with the provided ID from the scenery.
    //!
    //! @param[in]  id                  ID of the junction
    //! @return                         junction with the provided ID
    //-----------------------------------------------------------------------------
    virtual JunctionInterface *GetJunction(const std::string& id) = 0;
};

#endif // SCENERYINTERFACE
