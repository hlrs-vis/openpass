/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  VehicleModelsInterface.h
//! @brief This file provides the interface of the vehicle model container.
//-----------------------------------------------------------------------------

#pragma once

#include <unordered_map>

#include "Common/globalDefinitions.h"

using VehicleModelMap = std::unordered_map<std::string, VehicleModelParameters>;

class VehicleModelsInterface
{
public:
    VehicleModelsInterface() = default;
    ~VehicleModelsInterface() = default;

    virtual VehicleModelMap& GetVehicleModelMap() = 0;
    virtual VehicleModelParameters GetVehicleModel(std::string vehicleModelType) = 0;
};
