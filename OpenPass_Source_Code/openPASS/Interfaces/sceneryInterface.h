/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  sceneryInterface.h
//! @brief This file contains the interface to translate the scenery between
//!        framework and world.
//-----------------------------------------------------------------------------

#ifndef SCENERYINTERFACE
#define SCENERYINTERFACE

#include <map>
#include "roadInterface/roadInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a scenery as a list of roads.
//-----------------------------------------------------------------------------
class SceneryInterface
{
public:
    SceneryInterface() = default;
    SceneryInterface(const SceneryInterface &) = delete;
    SceneryInterface(SceneryInterface &&) = delete;
    SceneryInterface &operator=(const SceneryInterface &) = delete;
    SceneryInterface &operator=(SceneryInterface &&) = delete;
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
    //! Returns the stored list of roads.
    //!
    //! @return                         list of roads
    //-----------------------------------------------------------------------------
    virtual std::map<std::string, RoadInterface *> &GetRoads() = 0;


    //-----------------------------------------------------------------------------
    //! Returns the road with the provided ID from the scenery.
    //!
    //! @param[in]  id                  ID of the road
    //! @return                         road with the provided ID
    //-----------------------------------------------------------------------------
    virtual RoadInterface *GetRoad(const std::string &id) = 0;
};

#endif // SCENERYINTERFACE
