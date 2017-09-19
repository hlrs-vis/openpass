/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "collisionDetectionBinding.h"

namespace SimulationSlave
{

CollisionDetectionBinding::CollisionDetectionBinding(const FrameworkConfig *frameworkConfig,
                                                     SimulationCommon::Callbacks *callbacks) :
    frameworkConfig(frameworkConfig),
    callbacks(callbacks)
{}

CollisionDetectionBinding::~CollisionDetectionBinding()
{
     Unload();
}

CollisionDetectionInterface *CollisionDetectionBinding::Instantiate(
        SimulationCommon::RunConfig::CollisionDetectionInstance *collisionDetectionInstance)
{
    if(!library)
    {
        library = new (std::nothrow) CollisionDetectionLibrary(frameworkConfig->GetLibraryPath(),
                                                       collisionDetectionInstance->GetLibraryName(),
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

    return library->CreateCollisionDetection();
}

void CollisionDetectionBinding::Unload()
{
    if(library){
        library->ReleaseCollisionDetection();
        delete library;
        library = nullptr;
    }
}

} // namespace SimulationSlave
