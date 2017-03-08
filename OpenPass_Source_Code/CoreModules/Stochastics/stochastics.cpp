/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  stochastics.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "stochastics.h"
#include "stochastics_implementation.h"
#include "stochasticsInterface.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;

extern "C" STOCHASTICSSHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" STOCHASTICSSHARED_EXPORT StochasticsInterface *OpenPASS_CreateInstance(
        CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    try
    {
        return (StochasticsInterface*)(new (std::nothrow) Stochastics_Implementation(callbacks));
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

extern "C" STOCHASTICSSHARED_EXPORT void OpenPASS_DestroyInstance(StochasticsInterface *implementation)
{
    delete implementation;
}
