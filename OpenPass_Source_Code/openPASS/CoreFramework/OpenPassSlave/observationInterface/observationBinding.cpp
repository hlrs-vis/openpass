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

#include "observationBinding.h"
#include "Interfaces/observationInterface.h"
#include "observationLibrary.h"

namespace SimulationSlave {

ObservationBinding::ObservationBinding(CallbackInterface* callbacks) :
    callbacks(callbacks)
{}

ObservationBinding::~ObservationBinding()
{
    Unload();
}

ObservationModule* ObservationBinding::Instantiate(const std::string libraryPath,
        ParameterInterface* parameters,
        StochasticsInterface* stochastics,
        WorldInterface* world,
        EventNetworkInterface* eventNetwork)
{
    if (!library)
    {
        library = new (std::nothrow) ObservationLibrary(libraryPath,
                callbacks);
        if (!library)
        {
            return nullptr;
        }

        if (!library->Init())
        {
            delete library;
            return nullptr;
        }
    }

    return library->CreateObservationModule(parameters,
                                            stochastics,
                                            world,
                                            eventNetwork);
}

void ObservationBinding::Unload()
{
    if (library != nullptr)
    {
        delete library;
        library = nullptr;
    }
}

} // namespace SimulationSlave
