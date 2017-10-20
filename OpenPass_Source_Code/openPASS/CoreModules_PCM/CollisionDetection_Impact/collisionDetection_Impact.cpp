/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "collisionDetection_Impact.h"
#include "collisionDetection_Impact_implementation.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;


extern "C" COLLISIONDETECTIONIMPACTSHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" COLLISIONDETECTIONIMPACTSHARED_EXPORT CollisionDetectionInterface
*OpenPASS_CreateInstance(
    CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    try {
        return (CollisionDetectionInterface *)(new (std::nothrow) CollisionDetection_Impact_Implementation(
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

extern "C" COLLISIONDETECTIONIMPACTSHARED_EXPORT void OpenPASS_DestroyInstance(
    CollisionDetectionInterface *implementation)
{
    delete implementation;
}
