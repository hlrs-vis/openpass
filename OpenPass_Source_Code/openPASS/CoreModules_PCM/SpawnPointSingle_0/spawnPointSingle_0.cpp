/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnPointSingle_0.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "spawnPointSingle_0.h"
#include "spawnPointSingle_0_implementation.h"
#include "spawnPointInterface.h"

const std::string Version = "1.1.0";
static const CallbackInterface *Callbacks = nullptr;

extern "C" SPAWNPOINTSINGLE_0SHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" SPAWNPOINTSINGLE_0SHARED_EXPORT SpawnPointInterface *OpenPASS_CreateInstance(StochasticsInterface *stochastics,
                                                                               WorldInterface *world,
                                                                               const ParameterInterface *parameters,
                                                                               const CallbackInterface *callbacks)
{
    Callbacks = callbacks;
    try
    {
        return (SpawnPointInterface*)(new (std::nothrow) SpawnPointSingle_0_Implementation(stochastics,
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

extern "C" SPAWNPOINTSINGLE_0SHARED_EXPORT void OpenPASS_DestroyInstance(SpawnPointInterface *implementation)
{
    delete implementation;
}

extern "C" SPAWNPOINTSINGLE_0SHARED_EXPORT bool OpenPASS_SetSpawnItem(SpawnPointInterface *implementation,
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
