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

#include "worldBinding.h"
#include "worldLibrary.h"

namespace SimulationSlave
{

WorldBinding::WorldBinding(std::string libraryPath,
                           CallbackInterface *callbacks) :
        libraryPath(libraryPath),
        callbacks(callbacks)
{}

WorldBinding::~WorldBinding()
{
    Unload();
}

WorldInterface *WorldBinding::Instantiate()
{
    if(!library)
    {
        library = new (std::nothrow) WorldLibrary(libraryPath,
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
