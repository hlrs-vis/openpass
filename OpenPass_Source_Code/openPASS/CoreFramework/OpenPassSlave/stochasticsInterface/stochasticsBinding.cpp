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

#include "stochasticsBinding.h"
#include "Interfaces/stochasticsInterface.h"
#include "stochasticsLibrary.h"

namespace SimulationSlave {

StochasticsBinding::StochasticsBinding(CallbackInterface* callbacks) :
    callbacks(callbacks)
{}

StochasticsBinding::~StochasticsBinding()
{
    Unload();
}

StochasticsInterface* StochasticsBinding::Instantiate(std::string libraryPath)
{
    if (library == nullptr)
    {
        try
        {
            library = std::make_shared<StochasticsLibrary>(libraryPath,
                      callbacks);
        }
        catch (const std::bad_alloc&)
        {
            library = nullptr;
            return nullptr;
        }

        if (!library->Init())
        {
            return nullptr;
        }
    }

    return library->CreateStochastics();
}

void StochasticsBinding::Unload()
{
    if (library != nullptr)
    {
        library->ReleaseStochastics();
        library = nullptr;
    }
}

} // namespace SimulationSlave
