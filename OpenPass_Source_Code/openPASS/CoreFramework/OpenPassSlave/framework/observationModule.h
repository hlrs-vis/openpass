/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  ObservationModule.h
//! @brief This file contains the internal representation of an observation
//!        module.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <map>

#include "Interfaces/observationInterface.h"
#include "observationLibrary.h"
#include "CoreFramework/CoreShare/log.h"

namespace SimulationSlave
{

class ObservationModule
{
public:
    ObservationModule(ObservationInterface *implementation,
                      ObservationLibrary *library);
    ObservationModule(const ObservationModule&) = delete;
    ObservationModule(ObservationModule&&) = delete;
    ObservationModule& operator=(const ObservationModule&) = delete;
    ObservationModule& operator=(ObservationModule&&) = delete;
    virtual ~ObservationModule();

    int GetId() const
    {
        return id;
    }

    ObservationInterface *GetImplementation()
    {
        return implementation;
    }

    ObservationLibrary *GetLibrary()
    {
        return library;
    }

private:
    ObservationInterface *implementation;
    ObservationLibrary *library;
    int id = 0;
};

} // namespace SimulationSlave


