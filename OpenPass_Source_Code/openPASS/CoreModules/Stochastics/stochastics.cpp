/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  Stochastics.cpp */
//-----------------------------------------------------------------------------

#include "stochastics.h"
#include "stochastics_implementation.h"
#include "Interfaces/stochasticsInterface.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;

extern "C" STOCHASTICS_SHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" STOCHASTICS_SHARED_EXPORT StochasticsInterface *OpenPASS_CreateInstance(
        CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    try
    {
        return (StochasticsInterface*)(new (std::nothrow) StochasticsImplementation(callbacks));
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

extern "C" STOCHASTICS_SHARED_EXPORT void OpenPASS_DestroyInstance(StochasticsInterface *implementation)
{
    delete implementation;
}
