/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  collisionDetectionLibrary.h
//! @brief This file contains the internal representation of the library of a
//!        collisionDetectionInterface.
//-----------------------------------------------------------------------------

#ifndef COLLISIONDETECTIONLIBRARY_H
#define COLLISIONDETECTIONLIBRARY_H

#include <QLibrary>
#include "collisionDetectionInterface.h"
#include "callbackInterface.h"
#include "log.h"

namespace SimulationSlave
{

class CollisionDetectionLibrary
{
public:
    typedef const std::string &(*CollisionDetectionInterface_GetVersion)();
    typedef CollisionDetectionInterface *(*CollisionDetectionInterface_CreateInstanceType)(
            const CallbackInterface *callbacks);
    typedef void (*CollisionDetectionInterface_DestroyInstanceType)(CollisionDetectionInterface *implementation);

    CollisionDetectionLibrary(const std::string &collisionDetectionLibraryPath,
                      const std::string &libraryName,
                      CallbackInterface *callbacks) :
        collisionDetectionLibraryPath(collisionDetectionLibraryPath),
        libraryName(libraryName),
        callbacks(callbacks)
    {}
    CollisionDetectionLibrary(const CollisionDetectionLibrary&) = delete;
    CollisionDetectionLibrary(CollisionDetectionLibrary&&) = delete;
    CollisionDetectionLibrary& operator=(const CollisionDetectionLibrary&) = delete;
    CollisionDetectionLibrary& operator=(CollisionDetectionLibrary&&) = delete;
    virtual ~CollisionDetectionLibrary();

    //-----------------------------------------------------------------------------
    //! Creates a QLibrary based on the path from the constructor and stores function
    //! pointer for getting the library version, creating and destroying instances
    //! and setting the collisionDetectionInterface item
    //! (see typedefs for corresponding signatures).
    //!
    //! @return                 Null pointer
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    //! Delete the collisionDetectionInterface and the library
    //!
    //! @return                         Flag if the release was successful
    //-----------------------------------------------------------------------------
    bool ReleaseCollisionDetection();

    //-----------------------------------------------------------------------------
    //! Make sure that the library exists and is loaded, then call the "create instance"
    //! function pointer using the parameters from the collision detection instance to get
    //! a collision detection interface, which is then used to instantiate a collisionDetectionInterface
    //! which is stored.
    //!
    //! @return                         collisionDetectionInterface created
    //-----------------------------------------------------------------------------
    CollisionDetectionInterface *CreateCollisionDetection();

private:
    const std::string DllGetVersionId = "OpenPASS_GetVersion";
    const std::string DllCreateInstanceId = "OpenPASS_CreateInstance";
    const std::string DllDestroyInstanceId = "OpenPASS_DestroyInstance";

    std::string collisionDetectionLibraryPath;
    std::string libraryName;
    CollisionDetectionInterface *collisionDetectionInterface = nullptr;
    QLibrary *library = nullptr;
    CallbackInterface *callbacks;
    CollisionDetectionInterface_GetVersion getVersionFunc;
    CollisionDetectionInterface_CreateInstanceType createInstanceFunc;
    CollisionDetectionInterface_DestroyInstanceType destroyInstanceFunc;
};

} // namespace SimulationSlave

#endif // COLLISIONDETECTIONLIBRARY_H
