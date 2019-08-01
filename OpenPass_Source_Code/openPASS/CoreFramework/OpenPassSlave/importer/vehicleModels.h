/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "CoreFramework/CoreShare/log.h"
#include "Interfaces/vehicleModelsInterface.h"

namespace Configuration
{

class VehicleModels : public VehicleModelsInterface
{
public:
    VehicleModels();
    ~VehicleModels();

    VehicleModelMap& GetVehicleModelMap();
    VehicleModelParameters GetVehicleModel(std::string vehicleModelType);

private:
    VehicleModelMap vehicleModelMap;
};


} //namespace Configuration
