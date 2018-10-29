/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "observationBinding.h"
#include "observationLibrary.h"
#include "observationInterface.h"
#include "runConfig.h"
#include "frameworkConfig.h"

namespace SimulationSlave
{

ObservationBinding::ObservationBinding(const FrameworkConfig *frameworkConfig,
                                       SimulationCommon::Callbacks *callbacks) :
    frameworkConfig(frameworkConfig),
    callbacks(callbacks)
{}

ObservationBinding::~ObservationBinding()
{
    Unload();
}

ObservationModule *ObservationBinding::Instantiate(SimulationCommon::RunConfig::ObservationInstance *observationInstance,
                                                   StochasticsInterface *stochastics,
                                                   WorldInterface *world)
{
    ObservationLibrary *library;

    try
    {
        library = libraries.at(observationInstance->GetLibraryName());
    }
    catch(const std::out_of_range&)
    {
        library = nullptr;
    }

    if(!library)
    {
        library = new (std::nothrow) ObservationLibrary(frameworkConfig->GetLibraryPath(),
                                                        observationInstance->GetLibraryName(),
                                                        callbacks);
        if(!library)
        {
            return nullptr;
        }

        if(!library->Init())
        {
            delete library;
            return nullptr;
        }

        if(!libraries.insert({observationInstance->GetLibraryName(), library}).second)
        {
            delete library;
            return nullptr;
        }
    }

    return library->CreateObservationModule(observationInstance,
                                            stochastics,
                                            world);
}

void ObservationBinding::Unload()
{
    for(std::pair<const std::string, ObservationLibrary*> &item : libraries)
    {
        delete item.second;
    }

    libraries.clear();
}

} // namespace SimulationSlave
