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
//! @file  CollisionDetection/collisionDetection.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "collisionDetection.h"
#include "collisionDetection_implementation.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;


extern "C" COLLISIONDETECTIONSHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" COLLISIONDETECTIONSHARED_EXPORT CollisionDetectionInterface *OpenPASS_CreateInstance(
        CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    try
    {
        return (CollisionDetectionInterface*)(new (std::nothrow) CollisionDetection_Implementation(callbacks));
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

extern "C" COLLISIONDETECTIONSHARED_EXPORT void OpenPASS_DestroyInstance(CollisionDetectionInterface *implementation)
{
    delete implementation;
}
