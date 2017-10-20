/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  wordLibrary.h
//! @brief This file contains the internal representation of the library of a
//!        worldInterface.
//-----------------------------------------------------------------------------

#ifndef WORLDLIBRARY_H
#define WORLDLIBRARY_H

#include <QLibrary>
#include "worldBinding.h"
#include "callbackInterface.h"

namespace SimulationSlave
{

class WorldLibrary
{
public:
    typedef const std::string &(*WorldInterface_GetVersion)();
    typedef WorldInterface *(*WorldInterface_CreateInstanceType)(
            const CallbackInterface *callbacks);
    typedef void (*WorldInterface_DestroyInstanceType)(WorldInterface *implementation);


    WorldLibrary(const std::string &worldLibraryPath,
                 const std::string &libraryName,
                 CallbackInterface *callbacks) :
           worldLibraryPath(worldLibraryPath),
           libraryName(libraryName),
           callbacks(callbacks)
    {}

    WorldLibrary(const WorldLibrary&) = delete;
    WorldLibrary(WorldLibrary&&) = delete;
    WorldLibrary& operator=(const WorldLibrary&) = delete;
    WorldLibrary& operator=(WorldLibrary&&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor, deletes the stored library (unloads it if necessary)
    //-----------------------------------------------------------------------------
    virtual ~WorldLibrary();

    //-----------------------------------------------------------------------------
    //! Creates a QLibrary based on the path from the constructor and stores function
    //! pointer for getting the library version, creating and destroying instances
    //! and setting the stochasticsInterface item
    //! (see typedefs for corresponding signatures).
    //!
    //! @return                 Null pointer
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    //! Delete the worldInterface and the library
    //!
    //! @return                         Flag if the release was successful
    //-----------------------------------------------------------------------------
    bool ReleaseWorld();

    //-----------------------------------------------------------------------------
    //! Make sure that the library exists and is loaded, then call the "create instance"
    //! function pointer using the parameters from the stochastics instance to get
    //! a stochastics interface, which is then used to instantiate a stochasticsInterface
    //! which is stored.
    //!
    //! @return                         stochasticsInterface created
    //-----------------------------------------------------------------------------
    WorldInterface *CreateWorld();

private:
    const std::string DllGetVersionId = "OpenPASS_GetVersion";
    const std::string DllCreateInstanceId = "OpenPASS_CreateInstance";
    const std::string DllDestroyInstanceId = "OpenPASS_DestroyInstance";

    std::string worldLibraryPath;
    std::string libraryName;
    WorldInterface *worldInterface = nullptr;
    QLibrary *library = nullptr;
    CallbackInterface *callbacks;
    WorldInterface_GetVersion getVersionFunc;
    WorldInterface_CreateInstanceType createInstanceFunc;
    WorldInterface_DestroyInstanceType destroyInstanceFunc;
};

} // namespace SimulationSlave

#endif // WORLDLIBRARY_H
