/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
