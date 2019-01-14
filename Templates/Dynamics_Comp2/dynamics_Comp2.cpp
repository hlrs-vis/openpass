/*********************************************************************
* This Example Content is intended to demonstrate usage of
* Eclipse technology. It is provided to you under the terms
* and conditions of the Eclipse Distribution License v1.0
* which is available at http://www.eclipse.org/org/documents/edl-v10.php
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  dynamics_Comp2.cpp
//! @brief This file contains the DLL wrapper.
//-----------------------------------------------------------------------------

#include "dynamics_Comp2.h"
#include "modelInterface.h"
#include "dynamics_Comp2_implementation.h"
#include "defaultPrio_PCM.h"


const std::string Version = "0.0.1";
static const CallbackInterface *Callbacks = nullptr;

extern "C" DYNAMICS_COMP2_SHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

extern "C" DYNAMICS_COMP2_SHARED_EXPORT ModelInterface *OpenPASS_CreateInstance(
    int componentId,
    bool isInit,
    int priority,
    int offsetTime,
    int responseTime,
    int cycleTime,
    StochasticsInterface *stochastics,
    WorldInterface *world,
    const ParameterInterface *parameters,
    const std::map<int, ObservationInterface *> *observations,
    AgentInterface *agent,
    const CallbackInterface *callbacks)
{
    Q_UNUSED(world);
    Callbacks = callbacks;

    if (priority == 0)
    {
		priority = (int)PCMdefaultPrio::Dynamics_Comp2;
    }
    try
    {
        return (DynamicsInterface *)(new (std::nothrow) Dynamics_Comp2_Implementation(componentId,
                                                                                          isInit,
                                                                                          priority,
                                                                                          offsetTime,
                                                                                          responseTime,
                                                                                          cycleTime,
                                                                                          stochastics,
                                                                                          world,
                                                                                          parameters,
                                                                                          observations,
                                                                                          callbacks,
                                                                                          agent));
    }
    catch (const std::runtime_error &ex)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return nullptr;
    }
    catch (...)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return nullptr;
    }
}

extern "C" DYNAMICS_COMP2_SHARED_EXPORT void OpenPASS_DestroyInstance(
    ModelInterface *implementation)
{
    delete (Dynamics_Comp2_Implementation *)implementation;
}

extern "C" DYNAMICS_COMP2_SHARED_EXPORT bool OpenPASS_UpdateInput(
    ModelInterface *implementation,
    int localLinkId,
    const std::shared_ptr<SignalInterface const> &data,
    int time)
{
    try
    {
        implementation->UpdateInput(localLinkId, data, time);
    }
    catch (const std::runtime_error &ex)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return false;
    }
    catch (...)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return false;
    }

    return true;
}

extern "C" DYNAMICS_COMP2_SHARED_EXPORT bool OpenPASS_UpdateOutput(
    ModelInterface *implementation,
    int localLinkId,
    std::shared_ptr<SignalInterface const> &data,
    int time)
{
    try
    {
        implementation->UpdateOutput(localLinkId, data, time);
    }
    catch (const std::runtime_error &ex)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return false;
    }
    catch (...)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return false;
    }

    return true;
}

extern "C" DYNAMICS_COMP2_SHARED_EXPORT bool OpenPASS_Trigger(ModelInterface *implementation,
                                                                     int time)
{
    try
    {
        implementation->Trigger(time);
    }
    catch (const std::runtime_error &ex)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, ex.what());
        }

        return false;
    }
    catch (...)
    {
        if (Callbacks != nullptr)
        {
            Callbacks->Log(CbkLogLevel::Error, __FILE__, __LINE__, "unexpected exception");
        }

        return false;
    }

    return true;
}
