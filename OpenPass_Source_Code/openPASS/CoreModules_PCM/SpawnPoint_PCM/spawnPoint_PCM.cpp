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
//! @file  spawnPoint_PCM.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "spawnPoint_PCM.h"
#include "spawnPoint_PCM_implementation.h"
#include "spawnPointInterface.h"

const std::string Version = "1.1.0";
static const CallbackInterface *Callbacks = nullptr;

extern "C" SPAWNPOINT_PCMSHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" SPAWNPOINT_PCMSHARED_EXPORT SpawnPointInterface *OpenPASS_CreateInstance(
    StochasticsInterface *stochastics,
    WorldInterface *world,
    const ParameterInterface *parameters,
    const CallbackInterface *callbacks)
{
    Callbacks = callbacks;
    try {
        return (SpawnPointInterface *)(new (std::nothrow) SpawnPoint_PCM_Implementation(stochastics,
                                                                                            world,
                                                                                            parameters,
                                                                                            callbacks));
    } catch (const std::runtime_error &ex) {
        if (Callbacks != nullptr) {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return nullptr;
    } catch (...) {
        if (Callbacks != nullptr) {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return nullptr;
    }
}

extern "C" SPAWNPOINT_PCMSHARED_EXPORT void OpenPASS_DestroyInstance(
    SpawnPointInterface *implementation)
{
    delete implementation;
}

extern "C" SPAWNPOINT_PCMSHARED_EXPORT bool OpenPASS_SetSpawnItem(
    SpawnPointInterface *implementation,
    SpawnItemParameterInterface &spawnItem,
    int maxIndex)
{
    try {
        implementation->SetSpawnItem(spawnItem, maxIndex);
    } catch (const std::runtime_error &ex) {
        if (Callbacks != nullptr) {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return false;
    } catch (...) {
        if (Callbacks != nullptr) {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return false;
    }

    return true;
}
