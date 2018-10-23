/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  world.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------


#include "world_pcm.h"
#include "world_pcm_implementation.h"
#include "worldInterface.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;

extern "C" WORLD_PCMSHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" WORLD_PCMSHARED_EXPORT WorldInterface *OpenPASS_CreateInstance(
    CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    try {
        return (WorldInterface *)(new (std::nothrow) World_PCM_Implementation(callbacks));
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

extern "C" WORLD_PCMSHARED_EXPORT void OpenPASS_DestroyInstance(WorldInterface *implementation)
{
    delete implementation;
}
