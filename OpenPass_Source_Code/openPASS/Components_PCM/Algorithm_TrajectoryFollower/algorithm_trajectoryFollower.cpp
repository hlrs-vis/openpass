/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//! @file  algorithm_trajectoryFollower.cpp
//! @brief This file contains the implementation of of the interface for model
//!        instance construction and destruction.

#include "algorithm_trajectoryFollower.h"
#include "algorithm_trajectoryFollower_implementation.h"
#include "defaultPrio_PCM.h"

const std::string Version =
    "1.1.0"; //!< version of the current module - has to be incremented manually
static const CallbackInterface *Callbacks = nullptr;

//! dll-function to obtain the version of the current module
//!
//! @return                       Version of the current module

extern "C" ALGORITHM_TRAJECTORYFOLLOWER_SHARED_EXPORT const std::string &OpenPASS_GetVersion()
{
    return Version;
}

//! dll-function to create an instance of the module.
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
//! @return                       A pointer to the created module instance.

extern "C" ALGORITHM_TRAJECTORYFOLLOWER_SHARED_EXPORT
ModelInterface *OpenPASS_CreateInstance(int componentId,
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
        priority = (int)PCMdefaultPrio::Algorithm_Trajectory;
    }
    try
    {
        return (ModelInterface *)(new (std::nothrow) Algorithm_TrajectoryFollower_Implementation(
                                      componentId,
                                      isInit,
                                      priority,
                                      offsetTime,
                                      responseTime,
                                      cycleTime,
                                      stochastics,
                                      parameters,
                                      observations,
                                      callbacks,
                                      agent->GetAgentId()));
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


//! dll-function to destroy/delete an instance of the module.
//!
//! @param[in]     implementation    The instance that should be freed
extern "C" ALGORITHM_TRAJECTORYFOLLOWER_SHARED_EXPORT
void OpenPASS_DestroyInstance(ModelInterface *implementation)
{
    delete (Algorithm_TrajectoryFollower_Implementation *)implementation;
}

extern "C" ALGORITHM_TRAJECTORYFOLLOWER_SHARED_EXPORT bool OpenPASS_UpdateInput(
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

extern "C" ALGORITHM_TRAJECTORYFOLLOWER_SHARED_EXPORT bool OpenPASS_UpdateOutput(
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

extern "C" ALGORITHM_TRAJECTORYFOLLOWER_SHARED_EXPORT bool OpenPASS_Trigger(
    ModelInterface *implementation,
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
