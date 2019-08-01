/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  SpawnPointExport.cpp */
//-----------------------------------------------------------------------------

#include "SpawnPointExport.h"
#include "SpawnPoint.h"

const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;

extern "C" SPAWNPOINT_SHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" SPAWNPOINT_SHARED_EXPORT SpawnPointInterface *OpenPASS_CreateInstance(
        WorldInterface *world,
        const ParameterInterface *parameters,
        const CallbackInterface *callbacks,
        AgentBlueprintProviderInterface* agentBlueprintProvider,
        SamplerInterface *sampler,
        ScenarioInterface *scenario)
{
    Callbacks = callbacks;

    try
    {
        return (SpawnPointInterface*)(new (std::nothrow) SpawnPoint(
                                          world,
                                          parameters,
                                          callbacks,
                                          agentBlueprintProvider,
                                          sampler,
                                          scenario));
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

    throw std::runtime_error("Neither try nor catch was called in SpawnPoint");
}

extern "C" SPAWNPOINT_SHARED_EXPORT void OpenPASS_DestroyInstance(SpawnPointInterface *implementation)
{
    delete implementation;
}

extern "C" SPAWNPOINT_SHARED_EXPORT bool OpenPASS_GenerateAgent(SpawnPointInterface *implementation, AgentBlueprintInterface* agentBlueprint)
{
    try
    {
        return implementation->GenerateAgent(agentBlueprint);
    }
    catch(const std::runtime_error &error)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, error.what());
        }

        return false;
    }
    catch(const std::logic_error &error)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, error.what());
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
}
