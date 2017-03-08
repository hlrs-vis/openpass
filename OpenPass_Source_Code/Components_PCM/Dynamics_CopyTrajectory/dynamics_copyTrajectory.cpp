/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  dynamics_copyTrajectory.cpp
//! @brief This file contains the implementation of of the interface for model
//!        instance construction and destruction.
//-----------------------------------------------------------------------------

#include "dynamics_copyTrajectory.h"
#include "dynamics_copyTrajectory_implementation.h"

const std::string Version = "1.1.0";    //!< version of the current module - has to be incremented manually
static const CallbackInterface *Callbacks = nullptr;

//-----------------------------------------------------------------------------
//! dll-function to obtain the version of the current module
//!
//! @return                       Version of the current module
//-----------------------------------------------------------------------------
extern "C" DYNAMICS_COPYTRAJECTORYSHARED_EXPORT const std::string &OpenPASS_GetVersion() {
    return Version;
}

//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
extern "C" DYNAMICS_COPYTRAJECTORYSHARED_EXPORT SensorInterface *OpenPASS_CreateInstance(int componentId,
                                                                               bool isInit,
                                                                               int priority,
                                                                               int offsetTime,
                                                                               int responseTime,
                                                                               int cycleTime,
                                                                               StochasticsInterface *stochastics,
                                                                               WorldInterface *world,
                                                                               const ParameterInterface *parameters,
                                                                               const std::map<int, ObservationInterface*> *evaluations,
                                                                               AgentInterface *agent,
                                                                               const CallbackInterface *callbacks) {
    Callbacks = callbacks;

    return (SensorInterface*)(new (std::nothrow) Dynamics_CopyTrajectory_Implementation(componentId,
                                                                                       isInit,
                                                                                       priority,
                                                                                       offsetTime,
                                                                                       responseTime,
                                                                                       cycleTime,
                                                                                       stochastics,
                                                                                       world,
                                                                                       parameters,
                                                                                       evaluations,
                                                                                       callbacks,
                                                                                       agent));
}

//-----------------------------------------------------------------------------
//! dll-function to destroy/delete an instance of the module.
//!
//! @param[in]     implementation    The instance that should be freed
//-----------------------------------------------------------------------------
extern "C" DYNAMICS_COPYTRAJECTORYSHARED_EXPORT void OpenPASS_DestroyInstance(SensorInterface *implementation) {
    delete (Dynamics_CopyTrajectory_Implementation*)implementation;
}

extern "C" DYNAMICS_COPYTRAJECTORYSHARED_EXPORT bool OpenPASS_UpdateInput(ModelInterface *implementation,
                                                                       int localLinkId,
                                                                       const std::shared_ptr<SignalInterface const> &data,
                                                                       int time)
{
    try
    {
        implementation->UpdateInput(localLinkId, data, time);
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

extern "C" DYNAMICS_COPYTRAJECTORYSHARED_EXPORT bool OpenPASS_UpdateOutput(ModelInterface *implementation,
                                                                        int localLinkId,
                                                                        std::shared_ptr<SignalInterface const> &data,
                                                                        int time)
{
    try
    {
        implementation->UpdateOutput(localLinkId, data, time);
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

extern "C" DYNAMICS_COPYTRAJECTORYSHARED_EXPORT bool OpenPASS_Trigger(ModelInterface *implementation,
                                                                   int time)
{
    try
    {
        implementation->Trigger(time);
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
