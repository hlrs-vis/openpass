/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH.
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "worldBinding.h"
#include "frameworkConfig.h"
#include "worldLibrary.h"

namespace SimulationSlave
{

WorldBinding::WorldBinding(const FrameworkConfig *frameworkConfig,
                           SimulationCommon::Callbacks *callbacks) :
        frameworkConfig(frameworkConfig),
        callbacks(callbacks)
{}

WorldBinding::~WorldBinding()
{
    Unload();
}

WorldInterface *WorldBinding::Instantiate(WorldParameter *worldParameter)
{
    if(!library)
    {
        library = new (std::nothrow) WorldLibrary(frameworkConfig->GetLibraryPath(),
                                                       worldParameter->GetLibraryName(),
                                                       callbacks);
        if(!library)
        {
            return nullptr;
        }

        if(!library->Init())
        {
            delete library;
            library = nullptr;
            return nullptr;
        }
    }

    return library->CreateWorld();
}

void WorldBinding::Unload()
{
    if(library){
        library->ReleaseWorld();
        delete library;
        library = nullptr;
    }
}

} // namespace SimulationSlave
