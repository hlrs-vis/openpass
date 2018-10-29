/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef COLLISIONDETECTIONBINDING_H
#define COLLISIONDETECTIONBINDING_H

#include "runConfig.h"
#include "frameworkConfig.h"
#include "collisionDetectionInterface.h"
#include "collisionDetectionLibrary.h"
#include "callbacks.h"

namespace SimulationSlave
{

class StochasticsLibrary;

class CollisionDetectionBinding
{
public:
    CollisionDetectionBinding(const FrameworkConfig *frameworkConfig,
                              SimulationCommon::Callbacks *callbacks);
    CollisionDetectionBinding(const CollisionDetectionBinding&) = delete;
    CollisionDetectionBinding(CollisionDetectionBinding&&) = delete;
    CollisionDetectionBinding& operator=(const CollisionDetectionBinding&) = delete;
    CollisionDetectionBinding& operator=(CollisionDetectionBinding&&) = delete;
    virtual ~CollisionDetectionBinding();

    //-----------------------------------------------------------------------------
    //! Gets the collision detection instance library and stores it,
    //! then creates a new collisionDetectionInterface of the library.
    //!
    //! @param[in]  collisionDetectionInstance  collision detection instance that is instantiated
    //! @return                                 CollisionDetectionInterface created from the library
    //-----------------------------------------------------------------------------
    CollisionDetectionInterface *Instantiate(SimulationCommon::RunConfig::CollisionDetectionInstance *collisionDetectionInstance);

    //-----------------------------------------------------------------------------
    //! Unloads the CollisionDetectionInterface binding by deleting the library.
    //-----------------------------------------------------------------------------
    void Unload();

private:
    const FrameworkConfig *frameworkConfig;
    CollisionDetectionLibrary *library = nullptr;
    SimulationCommon::Callbacks *callbacks;
};

} // namespace SimulationSlave

#endif // COLLISIONDETECTIONBINDING_H
