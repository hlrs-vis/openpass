/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "observationModule.h"
#include "observationLibrary.h"
#include "log.h"

namespace SimulationSlave
{

ObservationModule::ObservationModule(SimulationCommon::RunConfig::ObservationInstance *observationInstance,
                                   ObservationInterface *implementation,
                                   ObservationLibrary *library) :
    implementation(implementation),
    library(library),
    id(observationInstance->GetId())
{}

ObservationModule::~ObservationModule()
{
    library->ReleaseObservationModule(this);
}

} // namespace SimulationSlave
