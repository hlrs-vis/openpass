/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnPointLibrary.h
//! @brief This file contains the internal representation of the library of a
//!        spawn point.
//-----------------------------------------------------------------------------

#ifndef SPAWNPOINTLIBRARY_H
#define SPAWNPOINTLIBRARY_H

#include <list>
#include <string>
#include <QLibrary>
#include "spawnPointBinding.h"
#include "spawnPointInterface.h"
#include "runConfig.h"
#include "callbackInterface.h"

namespace SimulationSlave
{

class AgentFactory;

class SpawnPointLibrary
{
public:    
    typedef const std::string &(*SpawnPointInterface_GetVersion)();
    typedef SpawnPointInterface *(*SpawnPointInterface_CreateInstanceType)(StochasticsInterface *stochastics,
                                                                           WorldInterface *world,
                                                                           const ParameterInterface *parameters,
                                                                           const CallbackInterface *callbacks);
    typedef void (*SpawnPointInterface_DestroyInstanceType)(SpawnPointInterface *implementation);
    typedef bool (*SpawnPointInterface_SetSpawnItemType)(SpawnPointInterface *implementation,
                                                         SpawnItemParameterInterface &spawnItem,
                                                         int maxIndex);

    SpawnPointLibrary(const std::string &spawnPointLibraryPath,
                      const std::string &libraryName,
                      CallbackInterface *callbacks) :
        spawnPointLibraryPath(spawnPointLibraryPath),
        libraryName(libraryName),
        callbacks(callbacks)
    {}
    SpawnPointLibrary(const SpawnPointLibrary&) = delete;
    SpawnPointLibrary(SpawnPointLibrary&&) = delete;
    SpawnPointLibrary& operator=(const SpawnPointLibrary&) = delete;
    SpawnPointLibrary& operator=(SpawnPointLibrary&&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor, deletes the stored library (unloads it if necessary), if the list
    //! of stored spawn points is empty.
    //-----------------------------------------------------------------------------
    virtual ~SpawnPointLibrary();

    //-----------------------------------------------------------------------------
    //! Creates a QLibrary based on the path from the constructor and stores function
    //! pointer for getting the library version, creating and destroying instances
    //! and setting the spawn item (see typedefs for corresponding signatures).
    //!
    //! @return                 Null pointer
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    //! Find spawn point in the sored list of spawn points, then call the "destroy
    //! instance" function pointer with its implementation and remove it from the list
    //! of stored spawn points.
    //!
    //! @param[in]  spawnPoint          Spawn point to release
    //! @return                         Flag if the release was successful
    //-----------------------------------------------------------------------------
    bool ReleaseSpawnPoint(SpawnPoint *spawnPoint);

    //-----------------------------------------------------------------------------
    //! Make sure that the library exists and is loaded, then call the "create instance"
    //! function pointer using the parameters from the spawn point instance to get
    //! a spawn point interface, which is then used to instantiate a spawn point
    //! which is also stored in the list of spawn points.
    //!
    //! @param[in]  spawnPointInstance  Spawn point instance that is instantiated
    //! @param[in]  agentFactory        Factory for the agents
    //! @param[in]  stochastics         Stochastics instance
    //! @param[in]  world               World instance
    //! @return                         Spawn point created
    //-----------------------------------------------------------------------------
    SpawnPoint *CreateSpawnPoint(SimulationCommon::RunConfig::SpawnPointInstance *spawnPointInstance,
                                 AgentFactory *agentFactory,
                                 StochasticsInterface *stochastics,
                                 WorldInterface *world);

    //-----------------------------------------------------------------------------
    //! Calls the "set spawn item" function pointer with the provided parameters and
    //! returns the result of this call.
    //!
    //! @param[in]  spawnPointInstance  Spawn point instance that is instantiated
    //! @param[in]  spawnItem           Parameter iterface for the spawned item
    //! @param[in]  maxIndex            Maximum index of the agent spawn item to be
    //!                                 created
    //! @return                         Flag if setting the spawn item was successful
    //-----------------------------------------------------------------------------
    bool SetSpawnItem(SpawnPointInterface *implementation,
                      SpawnItemParameterInterface &spawnItem,
                      int maxIndex)
    {
        return setSpawnItemFunc(implementation, spawnItem, maxIndex);
    }

private:    
    const std::string DllGetVersionId = "OpenPASS_GetVersion";
    const std::string DllCreateInstanceId = "OpenPASS_CreateInstance";
    const std::string DllDestroyInstanceId = "OpenPASS_DestroyInstance";
    const std::string DllSetSpawnItemId = "OpenPASS_SetSpawnItem";

    std::string spawnPointLibraryPath;
    std::string libraryName;
    std::list<SpawnPoint*> spawnPoints;
    QLibrary *library = nullptr;
    CallbackInterface *callbacks;
    SpawnPointInterface_GetVersion getVersionFunc;
    SpawnPointInterface_CreateInstanceType createInstanceFunc;
    SpawnPointInterface_DestroyInstanceType destroyInstanceFunc;
    SpawnPointInterface_SetSpawnItemType setSpawnItemFunc;
};

} // namespace SimulationSlave

#endif // SPAWNPOINTLIBRARY_H
