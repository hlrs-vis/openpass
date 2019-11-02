/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  OpenDriveTypeMapper.h
//! @brief This file provides conversions from OpenDRIVE constants to OSI
//!        constants
//-----------------------------------------------------------------------------

#pragma once

#include "roadInterface/roadElementTypes.h"
#include "osi/osi_road.pb.h"
#include "osi/osi_lane.pb.h"
#include "osi/osi_trafficsign.pb.h"
#include "Common/globalDefinitions.h"

namespace OWL
{
enum class LaneType;
}

class OpenDriveTypeMapper
{
public:
    static osi3::world::Road_Type RoadType(const RoadTypeInformation odRoadType);
    static osi3::Lane_Classification_Type LaneType(const RoadLaneType odLaneType);
    static osi3::TrafficSign_MainSign_Classification_Type TrafficSignType(const RoadSignalType odSignalType);

    static OWL::LaneType OdToOwlLaneType(const RoadLaneType laneType);
    static CommonTrafficSign::Type OsiToOdTrafficSignType(const osi3::TrafficSign_MainSign_Classification_Type trafficSignType);
};
