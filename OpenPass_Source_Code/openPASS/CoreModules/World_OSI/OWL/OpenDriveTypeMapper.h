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
//! @file  OpenDriveTypeMapper.h
//! @brief This file provides conversions from OpenDRIVE constants to OSI
//!        constants
//-----------------------------------------------------------------------------

#pragma once

#include "roadInterface/roadElementTypes.h"

#include "osi/osi_road.pb.h"

class OpenDriveTypeMapper
{
public:
    static osi3::world::Road_Type RoadType(const RoadTypeInformation odRoadType);
};
