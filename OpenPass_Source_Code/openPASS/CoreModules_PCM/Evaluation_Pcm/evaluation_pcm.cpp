/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "evaluation_pcm.h"
#include "observationInterface.h"
#include "evaluation_pcm_implementation.h"

const std::string Version = "1.1.0";    //!< version of the current module - has to be incremented manually
static const CallbackInterface *Callbacks = nullptr;

//-----------------------------------------------------------------------------
//! dll-function to obtain the version of the current module
//!
//! @return                       Version of the current module
//-----------------------------------------------------------------------------
extern "C" EVALUATION_PCM_SHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

//-----------------------------------------------------------------------------
//! dll-function to create an instance of the module
//!
//! @param[in]     componentId    Corresponds to "id" of "Component"
//! @param[in]     isInit         Corresponds to "init" of "Component"
//! @param[in]     priority       Corresponds to "priority" of "Component"
//! @param[in]     offsetTime     Corresponds to "offsetTime" of "Component"
//! @param[in]     responseTime   Corresponds to "responseTime" of "Component"
//! @param[in]     cycleTime      Corresponds to "cycleTime" of "Component"
//! @param[in]     stochastics    Pointer to the stochastics class loaded by the framework
//! @param[in]     world          Pointer to the world
//! @param[in]     parameters     Pointer to the parameters of the module
//! @param[in]     evaluations    Pointer to the evaluations of the module
//! @param[in]     agent          Pointer to the agent in which the module is situated
//! @param[in]     callbacks      Pointer to the callbacks
//! @return                       A pointer to the created module instance
//-----------------------------------------------------------------------------
extern "C" EVALUATION_PCM_SHARED_EXPORT ObservationInterface *OpenPASS_CreateInstance(StochasticsInterface *stochastics,
                                                                            WorldInterface *world,
                                                                            const ParameterInterface *parameters,
                                                                            const CallbackInterface *callbacks)
{
    Callbacks = callbacks;

    return (ObservationInterface*)(new (std::nothrow) Evaluation_Pcm_Implementation(stochastics,
                                                                                   world,
                                                                                   parameters,
                                                                                   callbacks));
}

//-----------------------------------------------------------------------------
//! dll-function to destroy/delete an instance of the module
//!
//! @param[in]     implementation    The instance that should be freed
//-----------------------------------------------------------------------------
extern "C" EVALUATION_PCM_SHARED_EXPORT void OpenPASS_DestroyInstance(ObservationInterface *implementation)
{
    delete implementation;
}

extern "C" EVALUATION_PCM_SHARED_EXPORT bool OpenPASS_MasterPreHook(ObservationInterface *implementation)
{
    try
    {
        implementation->MasterPreHook();
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

extern "C" EVALUATION_PCM_SHARED_EXPORT bool OpenPASS_MasterPostHook(ObservationInterface *implementation,
                                                                    const std::string &filename)
{
    try
    {
        implementation->MasterPostHook(filename);
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

extern "C" EVALUATION_PCM_SHARED_EXPORT bool OpenPASS_SlavePreHook(ObservationInterface *implementation,
                                                                  const std::string &path)
{
    try
    {
        implementation->SlavePreHook(path);
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

extern "C" EVALUATION_PCM_SHARED_EXPORT bool OpenPASS_SlavePreRunHook(ObservationInterface *implementation)
{
    try
    {
        implementation->SlavePreRunHook();
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

extern "C" EVALUATION_PCM_SHARED_EXPORT bool OpenPASS_SlaveUpdateHook(ObservationInterface *implementation,
                                                                     int time,
                                                                     RunResultInterface &runResult)
{
    try
    {
        implementation->SlaveUpdateHook(time, runResult);
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

extern "C" EVALUATION_PCM_SHARED_EXPORT bool OpenPASS_SlavePostRunHook(ObservationInterface *implementation,
                                                                      const RunResultInterface &runResult)
{
    try
    {
        implementation->SlavePostRunHook(runResult);
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

extern "C" EVALUATION_PCM_SHARED_EXPORT bool OpenPASS_SlavePostHook(ObservationInterface *implementation)
{
    try
    {
        implementation->SlavePostHook();
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

extern "C" EVALUATION_PCM_SHARED_EXPORT const std::string OpenPASS_SlaveResultFile(ObservationInterface *implementation)
{
    try
    {
        return implementation->SlaveResultFile();
    }
    catch(const std::runtime_error &ex)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return "";
    }
    catch(...)
    {
        if(Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return "";
    }
}
