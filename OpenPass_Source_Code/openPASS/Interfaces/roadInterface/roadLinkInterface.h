/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadLinkInterface.h
//! @brief This file contains a interface representing a link to a road.
//-----------------------------------------------------------------------------

#ifndef ROADLINKINTERFACE
#define ROADLINKINTERFACE

#include "roadInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a link to a road.
//-----------------------------------------------------------------------------
class RoadLinkInterface
{
public:
    RoadLinkInterface() = default;
    RoadLinkInterface(const RoadLinkInterface&) = delete;
    RoadLinkInterface(RoadLinkInterface&&) = delete;
    RoadLinkInterface& operator=(const RoadLinkInterface&) = delete;
    RoadLinkInterface& operator=(RoadLinkInterface&&) = delete;
    virtual ~RoadLinkInterface() = default;

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkType of the RoadLink.
    //!
    //! @return                         RoadLinkType of the RoadLink
    //-----------------------------------------------------------------------------
    virtual RoadLinkType GetType() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkElementType of the RoadLink.
    //!
    //! @return                         RoadLinkElementType of the RoadLink
    //-----------------------------------------------------------------------------
    virtual RoadLinkElementType GetElementType() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the ID of the RoadLink.
    //!
    //! @return                         ID of the RoadLink
    //-----------------------------------------------------------------------------
    virtual const std::string &GetElementId() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the ContactPointType of the RoadLink.
    //!
    //! @return                         ContactPointType of the RoadLink
    //-----------------------------------------------------------------------------
    virtual ContactPointType GetContactPoint() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkDirectionType of the RoadLink.
    //!
    //! @return                         RoadLinkDirectionType of the RoadLink
    //-----------------------------------------------------------------------------
    virtual RoadLinkDirectionType GetDirection() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the RoadLinkSideType of the RoadLink.
    //!
    //! @return                         RoadLinkSideType of the RoadLink
    //-----------------------------------------------------------------------------
    virtual RoadLinkSideType GetSide() const = 0;
};

#endif // ROADLINKINTERFACE

