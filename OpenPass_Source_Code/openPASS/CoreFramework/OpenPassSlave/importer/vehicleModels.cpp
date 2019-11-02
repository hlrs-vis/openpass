/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "vehicleModels.h"

namespace Configuration
{

VehicleModels::VehicleModels()
{
}

VehicleModels::~VehicleModels()
{
}

VehicleModelMap &VehicleModels::GetVehicleModelMap()
{
    return vehicleModelMap;
}

VehicleModelParameters VehicleModels::GetVehicleModel(std::string vehicleModelType)
{
    return vehicleModelMap.at(vehicleModelType);
}

} //namespace Configuration
