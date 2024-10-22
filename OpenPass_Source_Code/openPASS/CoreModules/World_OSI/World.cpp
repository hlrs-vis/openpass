/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  world.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "World.h"
#include "CoreModules/World_OSI/WorldImplementation.h"
#include "Interfaces/worldInterface.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;

extern "C" WORLD_SHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" WORLD_SHARED_EXPORT WorldInterface *OpenPASS_CreateInstance(
        CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    try
    {
        return (WorldInterface*)(new (std::nothrow) WorldImplementation(callbacks));
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

extern "C" WORLD_SHARED_EXPORT void OpenPASS_DestroyInstance(WorldInterface *implementation)
{
    delete implementation;
}
