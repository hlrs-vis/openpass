/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
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
