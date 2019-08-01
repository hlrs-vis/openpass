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
//! @file  SensorFusionQuery.h
//! @brief This file provides methods to retrieve structured basic information
//!        from sensorData provided by SensorFusionImplementation::Output().
//-----------------------------------------------------------------------------

#pragma once
#include "osi/osi_sensordata.pb.h"
#include <map>
#include <vector>

class SensorFusionHelperFunctions
{
public:
    static std::vector<osi3::DetectedMovingObject> RetrieveMovingObjectsBySensorId(std::vector<int> sensorIds, const osi3::SensorData &sensorData)
    {
        std::vector<osi3::DetectedMovingObject> result;
        auto detectedMovingObjects = sensorData.moving_object();
        for (const auto& object : detectedMovingObjects)
        {
            if(std::count(sensorIds.cbegin(), sensorIds.cend(), object.header().sensor_id(0).value()) > 0)
            {
                result.push_back(object);
            }
        }
        return result;
    }


    static std::vector<osi3::DetectedStationaryObject> RetrieveStationaryObjectsBySensorId(std::vector<int> sensorIds, const osi3::SensorData &sensorData)
    {
        std::vector<osi3::DetectedStationaryObject> result;
        auto detectedStationaryObjects = sensorData.stationary_object();
        for (const auto& object : detectedStationaryObjects)
        {
            if(std::count(sensorIds.cbegin(), sensorIds.cend(), object.header().sensor_id(0).value()) > 0)
            {
                result.push_back(object);
            }
        }
        return result;
    }
};
