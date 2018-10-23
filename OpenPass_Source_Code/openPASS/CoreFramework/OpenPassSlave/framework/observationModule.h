/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  observationModule.h
//! @brief This file contains the internal representation of an observation
//!        module.
//-----------------------------------------------------------------------------

#ifndef OBSERVATIONMODULE_H
#define OBSERVATIONMODULE_H

#include <string>
#include <vector>
#include <map>
#include "runConfig.h"
#include "observationInterface.h"
#include "observationLibrary.h"
#include "log.h"

namespace SimulationSlave
{

class ObservationModule
{
public:
    ObservationModule(SimulationCommon::RunConfig::ObservationInstance *observationInstance,
                     ObservationInterface *implementation,
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
    int id;
};

} // namespace SimulationSlave

#endif // OBSERVATIONMODULE_H
