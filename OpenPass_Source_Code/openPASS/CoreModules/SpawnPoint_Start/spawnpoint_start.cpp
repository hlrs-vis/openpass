/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


//-----------------------------------------------------------------------------
//! @file  spawnpoint_start.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "spawnpoint_start.h"
#include "spawnpoint_start_implementation.h"
#include "spawnPointInterface.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;

extern "C" SPAWNPOINT_STARTSHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" SPAWNPOINT_STARTSHARED_EXPORT SpawnPointInterface *OpenPASS_CreateInstance(
        StochasticsInterface *stochastics,
        WorldInterface *world,
        const ParameterInterface *parameters,
        const CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    try
    {
        return (SpawnPointInterface*)(new (std::nothrow) SpawnPoint_Start_Implementation(
                                          stochastics,
                                          world,
                                          parameters,
                                          callbacks));
    }
    catch(const std::runtime_error &ex)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return nullptr;
    }
    catch(...)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return nullptr;
    }
}

extern "C" SPAWNPOINT_STARTSHARED_EXPORT void OpenPASS_DestroyInstance(SpawnPointInterface *implementation)
{
    delete implementation;
}

extern "C" SPAWNPOINT_STARTSHARED_EXPORT bool OpenPASS_SetSpawnItem(SpawnPointInterface *implementation,
                                                                        SpawnItemParameterInterface &spawnItem,
                                                                        int maxIndex)
{
    try
    {
        implementation->SetSpawnItem(spawnItem, maxIndex);
    }
    catch(const std::runtime_error &ex)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return false;
    }
    catch(...)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return false;
    }

    return true;
}

