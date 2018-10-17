/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  OpenDriveTypeMapper.cpp
//! @brief This file provides conversions from OpenDRIVE constants to OSI
//!        constants
//-----------------------------------------------------------------------------


#include "OpenDriveTypeMapper.h"

#include "roadInterface/roadElementTypes.h"

#include "osi/osi_road.pb.h"

osi3::world::Road_Type OpenDriveTypeMapper::RoadType(const RoadTypeInformation odRoadType)
{
    osi3::world::Road_Type osiType;

    switch (odRoadType)
    {
        case RoadTypeInformation::Bicycle:
            osiType = osi3::world::Road_Type_TYPE_BIKEWAY;
            break;

        case RoadTypeInformation::LowSpeed:
            osiType = osi3::world::Road_Type_TYPE_LOWSPEED;
            break;

        case RoadTypeInformation::Motorway:
            osiType = osi3::world::Road_Type_TYPE_HIGHWAY;
            break;

        case RoadTypeInformation::Pedestrian:
            osiType = osi3::world::Road_Type_TYPE_PEDESTRIAN;
            break;

        case RoadTypeInformation::Rural:
            osiType = osi3::world::Road_Type_TYPE_RURAL;
            break;

        case RoadTypeInformation::Town:
            osiType = osi3::world::Road_Type_TYPE_TOWN;
            break;

        default:
            osiType = osi3::world::Road_Type_TYPE_HIGHWAY;
            break;
    }

    return osiType;
}
