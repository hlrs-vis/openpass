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
//! @file  OpenDriveTypeMapper.cpp
//! @brief This file provides conversions from OpenDRIVE constants to OSI
//!        constants
//-----------------------------------------------------------------------------


#include "OpenDriveTypeMapper.h"

#include "roadInterface/roadElementTypes.h"

#include "osi/osi_road.pb.h"
#include "osi/osi_lane.pb.h"
#include "OWL/DataTypes.h"

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

osi3::Lane_Classification_Type OpenDriveTypeMapper::LaneType(const RoadLaneType odLaneType)
{
    osi3::Lane_Classification_Type osiType;

    switch (odLaneType)
    {
        case RoadLaneType::Undefined:
            osiType = osi3::Lane_Classification_Type_TYPE_UNKNOWN;
            break;
        case RoadLaneType::Driving:
            osiType = osi3::Lane_Classification_Type_TYPE_DRIVING;
            break;
        default:
            osiType = osi3::Lane_Classification_Type_TYPE_OTHER;
            break;
    }
    return osiType;
}

osi3::TrafficSign_MainSign_Classification_Type OpenDriveTypeMapper::TrafficSignType(const RoadSignalType odSignalType)
{
    osi3::TrafficSign_MainSign_Classification_Type osiType;

    switch (odSignalType)
    {
    case RoadSignalType::Undefined:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OTHER;
        break;
    case RoadSignalType::MaximumSpeedLimit:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_BEGIN;
        break;
    case RoadSignalType::MinimumSpeedLimit:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_MINIMUM_SPEED_BEGIN;
        break;
    case RoadSignalType::EndOfMaximumSpeedLimit:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_END;
        break;
    case RoadSignalType::EndOfMinimumSpeedLimit:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_MINIMUM_SPEED_END;
        break;
    case RoadSignalType::EndOffAllSpeedLimitsAndOvertakingRestrictions:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_ALL_RESTRICTIONS_END;
        break;
    case RoadSignalType::TownBegin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_TOWN_BEGIN;
        break;
    case RoadSignalType::TownEnd:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_TOWN_END;
        break;
    case RoadSignalType::Zone30Begin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_ZONE_BEGIN;
        break;
    case RoadSignalType::Zone30End:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_ZONE_END;
        break;
    case RoadSignalType::TrafficCalmedDistrictBegin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_LIVING_STREET_BEGIN;
        break;
    case RoadSignalType::TrafficCalmedDistrictEnd:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_LIVING_STREET_END;
        break;
    case RoadSignalType::EnvironmentalZoneBegin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_ENVIRONMENTAL_ZONE_BEGIN;
        break;
    case RoadSignalType::EnvironmentalZoneEnd:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_ENVIRONMENTAL_ZONE_END;
        break;
    case RoadSignalType::OvertakingBanBegin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_BEGIN;
        break;
    case RoadSignalType::OvertakingBanEnd:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_END;
        break;
    case RoadSignalType::OvertakingBanForTrucksBegin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_FOR_TRUCKS_BEGIN;
        break;
    case RoadSignalType::OvertakingBanForTrucksEnd:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_FOR_TRUCKS_END;
        break;
    case RoadSignalType::RightOfWayBegin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_RIGHT_OF_WAY_BEGIN;
        break;
    case RoadSignalType::RightOfWayEnd:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_RIGHT_OF_WAY_END;
        break;
    case RoadSignalType::RightOfWayNextIntersection:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_RIGHT_OF_WAY_NEXT_INTERSECTION;
        break;
    case RoadSignalType::Stop:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_STOP;
        break;
    case RoadSignalType::DoNotEnter:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_DO_NOT_ENTER;
        break;
    case RoadSignalType::HighWayBegin:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_HIGHWAY_BEGIN;
        break;
    case RoadSignalType::HighWayEnd:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_HIGHWAY_END;
        break;
    case RoadSignalType::HighWayExit:
        osiType = osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_HIGHWAY_EXIT;
        break;
    }

    return osiType;
}


OWL::LaneType OpenDriveTypeMapper::OdToOwlLaneType(const RoadLaneType laneType)
{
    switch (laneType)
    {
        case RoadLaneType::None:
            return OWL::LaneType::None;
        case RoadLaneType::Driving:
            return OWL::LaneType::Driving;
        case RoadLaneType::Stop:
            return OWL::LaneType::Stop;
        case RoadLaneType::Shoulder:
            return OWL::LaneType::Shoulder;
        case RoadLaneType::Biking:
            return OWL::LaneType::Biking;
        case RoadLaneType::Sidewalk:
            return OWL::LaneType::Sidewalk;
        case RoadLaneType::Border:
            return OWL::LaneType::Border;
        case RoadLaneType::Restricted:
            return OWL::LaneType::Restricted;
        case RoadLaneType::Parking:
            return OWL::LaneType::Parking;
        case RoadLaneType::Bidirectional:
            return OWL::LaneType::Bidirectional;
        case RoadLaneType::Median:
            return OWL::LaneType::Median;
        case RoadLaneType::Special1:
            return OWL::LaneType::Special1;
        case RoadLaneType::Special2:
            return OWL::LaneType::Special2;
        case RoadLaneType::Special3:
            return OWL::LaneType::Special3;
        case RoadLaneType::Roadworks:
            return OWL::LaneType::Roadworks;
        case RoadLaneType::Tram:
            return OWL::LaneType::Tram;
        case RoadLaneType::Rail:
            return OWL::LaneType::Rail;
        case RoadLaneType::Entry:
            return OWL::LaneType::Entry;
        case RoadLaneType::Exit:
            return OWL::LaneType::Exit;
        case RoadLaneType::OffRamp:
            return OWL::LaneType::OffRamp;
        case RoadLaneType::OnRamp:
            return OWL::LaneType::OnRamp;
        default:
            return OWL::LaneType::Undefined;
    }
}

CommonTrafficSign::Type OpenDriveTypeMapper::OsiToOdTrafficSignType(const osi3::TrafficSign_MainSign_Classification_Type trafficSignType)
{
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OTHER)
    {
        return CommonTrafficSign::Type::Undefined;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_BEGIN)
    {
        return CommonTrafficSign::Type::MaximumSpeedLimit;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_END)
    {
        return CommonTrafficSign::Type::EndOfMaximumSpeedLimit;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_MINIMUM_SPEED_BEGIN)
    {
        return CommonTrafficSign::Type::MinimumSpeedLimit;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_MINIMUM_SPEED_END)
    {
        return CommonTrafficSign::Type::EndOfMinimumSpeedLimit;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_ALL_RESTRICTIONS_END)
    {
        return CommonTrafficSign::Type::EndOffAllSpeedLimitsAndOvertakingRestrictions;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_TOWN_BEGIN)
    {
        return CommonTrafficSign::Type::TownBegin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_TOWN_END)
    {
        return CommonTrafficSign::Type::TownEnd;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_ZONE_BEGIN)
    {
        return CommonTrafficSign::Type::Zone30Begin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_SPEED_LIMIT_ZONE_END)
    {
        return CommonTrafficSign::Type::Zone30End;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_LIVING_STREET_BEGIN)
    {
        return CommonTrafficSign::Type::TrafficCalmedDistrictBegin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_LIVING_STREET_END)
    {
        return CommonTrafficSign::Type::TrafficCalmedDistrictEnd;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_ENVIRONMENTAL_ZONE_BEGIN)
    {
        return CommonTrafficSign::Type::EnvironmentalZoneBegin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_ENVIRONMENTAL_ZONE_END)
    {
        return CommonTrafficSign::Type::EnvironmentalZoneEnd;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_BEGIN)
    {
        return CommonTrafficSign::Type::OvertakingBanBegin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_END)
    {
        return CommonTrafficSign::Type::OvertakingBanEnd;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_FOR_TRUCKS_BEGIN)
    {
        return CommonTrafficSign::Type::OvertakingBanForTrucksBegin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_OVERTAKING_BAN_FOR_TRUCKS_END)
    {
        return CommonTrafficSign::Type::OvertakingBanForTrucksEnd;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_RIGHT_OF_WAY_BEGIN)
    {
        return CommonTrafficSign::Type::RightOfWayBegin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_RIGHT_OF_WAY_END)
    {
        return CommonTrafficSign::Type::RightOfWayEnd;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_RIGHT_OF_WAY_NEXT_INTERSECTION)
    {
        return CommonTrafficSign::Type::RightOfWayNextIntersection;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_STOP)
    {
        return CommonTrafficSign::Type::Stop;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_DO_NOT_ENTER)
    {
        return CommonTrafficSign::Type::DoNotEnter;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_HIGHWAY_BEGIN)
    {
        return CommonTrafficSign::Type::HighWayBegin;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_HIGHWAY_END)
    {
        return CommonTrafficSign::Type::HighWayEnd;
    }
    if(trafficSignType == osi3::TrafficSign_MainSign_Classification_Type::TrafficSign_MainSign_Classification_Type_TYPE_HIGHWAY_EXIT)
    {
        return CommonTrafficSign::Type::HighWayExit;
    }

    throw std::logic_error("Type of traffic sign not supported.");
}
