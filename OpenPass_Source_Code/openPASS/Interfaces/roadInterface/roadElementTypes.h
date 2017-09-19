/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadElementTypes.h
//! @brief This file contains enumerations for different types of roads.
//-----------------------------------------------------------------------------

#ifndef ROADTYPES
#define ROADTYPES

//-----------------------------------------------------------------------------
//! Road link connection orientation
//-----------------------------------------------------------------------------
enum class RoadLinkType
{
    Undefined = 0,
    Predecessor,
    Successor,
    Neighbor
};

//-----------------------------------------------------------------------------
//! Road link connection type
//-----------------------------------------------------------------------------
enum class RoadLinkElementType
{
    Undefined = 0,
    Road
};

//-----------------------------------------------------------------------------
//! Contact point of connections
//-----------------------------------------------------------------------------
enum class ContactPointType
{
    Undefined = 0,
    Start,
    End
};

//-----------------------------------------------------------------------------
//! Orientation between roads
//-----------------------------------------------------------------------------
enum class RoadLinkDirectionType
{
    Undefined = 0,
    Same,
    Opposite
};

//-----------------------------------------------------------------------------
//! Connection between roads
//-----------------------------------------------------------------------------
enum class RoadLinkSideType
{
    Undefined = 0,
    Left,
    Right
};

//-----------------------------------------------------------------------------
//! Type of lane
//-----------------------------------------------------------------------------
enum class RoadLaneTypeType
{
    Undefined = 0,
    None,
    Driving,
    Stop,
    Shoulder,
    Biking,
    Sidewalk,
    Border,
    Restricted,
    Parking,
    MwyEntry,
    MwyExit,
    Special1,
    Special2,
    Special3,
    Tram
};

#endif // ROADTYPES

