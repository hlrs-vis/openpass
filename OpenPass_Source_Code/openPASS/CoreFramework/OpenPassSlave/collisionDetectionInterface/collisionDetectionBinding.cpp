/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
