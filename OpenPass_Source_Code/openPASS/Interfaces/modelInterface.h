/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  modelInterface.h
//! @brief This file contains the interface of the component models to interact
//!        with the framework.
//!
//! contains classes SignalInterface, ModelInterface, RestrictedModelInterface,
//! UnrestrictedModelInterface, AlgorithmInterface, ActionInterface,
//! DynamicsInterface, SensorInterface, InitInterface
//-----------------------------------------------------------------------------

#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include <string>
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "parameterInterface.h"
#include "stochasticsInterface.h"
#include "worldInterface.h"
#include "observationInterface.h"
#include "callbackInterface.h"

//-----------------------------------------------------------------------------
//! Provides functionality to print information of signals
//!
//! @param[in]     stream  Output stream for printing information
//! @param[in]     signal  Signal to be printed
//! @return                Output stream for concatenation
//-----------------------------------------------------------------------------
inline std::ostream &operator<<(std::ostream &stream,
                                const SignalInterface &signal)
{
    return stream << static_cast<std::string>(signal);
}

//-----------------------------------------------------------------------------
//! Interface of functionality which has to be provided by component models
//-----------------------------------------------------------------------------
class ModelInterface
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     isInit         Corresponds to "init" of "Component"
    //! @param[in]     priority       Corresponds to "priority" of "Component"
    //! @param[in]     offsetTime     Corresponds to "offsetTime" of "Component"
    //! @param[in]     responseTime   Corresponds to "responseTime" of "Component"
    //! @param[in]     cycleTime      Corresponds to "cycleTime" of "Component"
    //-----------------------------------------------------------------------------
    ModelInterface(bool isInit,
                   int priority,
                   int offsetTime,
                   int responseTime,
                   int cycleTime) :
        isInit(isInit),
        priority(priority),
        offsetTime(offsetTime),
        responseTime(responseTime),
        cycleTime(cycleTime)
    {}
    ModelInterface(const ModelInterface &) = delete;
    ModelInterface(ModelInterface &&) = delete;
    ModelInterface &operator=(const ModelInterface &) = delete;
    ModelInterface &operator=(ModelInterface &&) = delete;
    virtual ~ModelInterface() = default;

    //-----------------------------------------------------------------------------
    //! Function is called by framework when another component delivers a signal over
    //! a channel to this component (scheduler calls update taks of other component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    //! @param[in]     data           Referenced signal (copied by sending component)
    //! @param[in]     time           Current scheduling time
    //-----------------------------------------------------------------------------
    virtual void UpdateInput(int localLinkId,
                             const std::shared_ptr<SignalInterface const> &data,
                             int time) = 0;

    //-----------------------------------------------------------------------------
    //! Function is called by framework when this component has to deliver a signal over
    //! a channel to another component (scheduler calls update task of this component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    //! @param[out]    data           Referenced signal (copied by this component)
    //! @param[in]     time           Current scheduling time
    //-----------------------------------------------------------------------------
    virtual void UpdateOutput(int localLinkId,
                              std::shared_ptr<SignalInterface const> &data,
                              int time) = 0;

    //-----------------------------------------------------------------------------
    //! Function is called by framework when the scheduler calls the trigger task
    //! of this component
    //!
    //! @param[in]     time           Current scheduling time
    //-----------------------------------------------------------------------------
    virtual void Trigger(int time) = 0;

    //-----------------------------------------------------------------------------
    //! Checks if this component is configured as init module
    //!
    //! @return                       True if this component is an init module
    //-----------------------------------------------------------------------------
    bool GetInit()
    {
        return isInit;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves priority of this component
    //!
    //! @return                       Component priority
    //-----------------------------------------------------------------------------
    int GetPriority()
    {
        return priority;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves offset time of the trigger task of this component
    //!
    //! @return                       Trigger task offset time
    //-----------------------------------------------------------------------------
    int GetOffsetTime()
    {
        return offsetTime;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves response time of the update task of this component (time between
    //! trigger task and update task)
    //!
    //! @return                       Update task response time
    //-----------------------------------------------------------------------------
    int GetResponseTime()
    {
        return responseTime;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves cycle time of the trigger task of this component
    //!
    //! @return                       Trigger task cycle time
    //-----------------------------------------------------------------------------
    int GetCycleTime()
    {
        return cycleTime;
    }

private:
    bool isInit;                          //!< Indicates if component is an init module
    int priority;                         //!< Priority of this component
    int offsetTime;                       //!< Offset time of this components trigger task
    int responseTime;                     //!< Response time of this components update task
    int cycleTime;                        //!< Cycle time of this components trigger task
};

//-----------------------------------------------------------------------------
//! Interface of restricted functionality
//-----------------------------------------------------------------------------
class RestrictedModelInterface : public ModelInterface
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     isInit         Corresponds to "init" of "Component"
    //! @param[in]     priority       Corresponds to "priority" of "Component"
    //! @param[in]     offsetTime     Corresponds to "offsetTime" of "Component"
    //! @param[in]     responseTime   Corresponds to "responseTime" of "Component"
    //! @param[in]     cycleTime      Corresponds to "cycleTime" of "Component"
    //-----------------------------------------------------------------------------
    RestrictedModelInterface(int componentId,
                             bool isInit,
                             int priority,
                             int offsetTime,
                             int responseTime,
                             int cycleTime,
                             StochasticsInterface *stochastics,
                             const ParameterInterface *parameters,
                             const std::map<int, ObservationInterface *> *observations,
                             const CallbackInterface *callbacks) :
        ModelInterface(isInit, priority, offsetTime, responseTime, cycleTime),
        callbacks(callbacks),
        componentId(componentId),
        stochastics(stochastics),
        parameters(parameters),
        observations(observations)
    {}
    RestrictedModelInterface(const RestrictedModelInterface &) = delete;
    RestrictedModelInterface(RestrictedModelInterface &&) = delete;
    RestrictedModelInterface &operator=(const RestrictedModelInterface &) = delete;
    RestrictedModelInterface &operator=(RestrictedModelInterface &&) = delete;
    virtual ~RestrictedModelInterface() = default;

    //-----------------------------------------------------------------------------
    //! Retrieves id of this component.
    //!
    //! @return                       Component id of this model
    //-----------------------------------------------------------------------------
    int GetComponentId()
    {
        return componentId;
    }

protected:
    //-----------------------------------------------------------------------------
    //! Retrieves the stochastics functionality of the framework
    //!
    //! @return                       Stochastics functionality
    //-----------------------------------------------------------------------------
    StochasticsInterface *GetStochastics() const
    {
        return stochastics;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the configuration parameters of this component
    //!
    //! @return                       Reference to configuration parameters
    //-----------------------------------------------------------------------------
    const ParameterInterface *GetParameters() const
    {
        return parameters;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the references to the observation modules used to track and
    //! observe information
    //!
    //! @return                       Mapping of observation modules.
    //-----------------------------------------------------------------------------
    const std::map<int, ObservationInterface *> *GetObservations() const
    {
        return observations;
    }

    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message)
    {
        if (callbacks) {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    // Access to the following members is provided by the corresponding member
    // functions.
    const CallbackInterface *callbacks;   //!< Reference to framework callbacks
    int componentId;                      //!< Id of this component
    StochasticsInterface
    *stochastics;    //!< Reference to the stochastics functionality of the framework
    const ParameterInterface *parameters; //!< Reference to the configuration parameters
    const std::map<int, ObservationInterface *> *observations; //!< Mapping of observation modules
};

//-----------------------------------------------------------------------------
//! Interface of unrestricted functionality
//-----------------------------------------------------------------------------
class UnrestrictedModelInterface : public RestrictedModelInterface
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
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
    //! @param[in]     observations   Pointer to the observations of the module
    //! @param[in]     callbacks      Pointer to the callbacks
    //! @param[in]     agent          Pointer to the agent in which the module is situated
    //-----------------------------------------------------------------------------
    UnrestrictedModelInterface(int componentId,
                               bool isInit,
                               int priority,
                               int offsetTime,
                               int responseTime,
                               int cycleTime,
                               StochasticsInterface *stochastics,
                               WorldInterface *world,
                               const ParameterInterface *parameters,
                               const std::map<int, ObservationInterface *> *observations,
                               const CallbackInterface *callbacks,
                               AgentInterface *agent) :
        RestrictedModelInterface(componentId,
                                 isInit,
                                 priority,
                                 offsetTime,
                                 responseTime,
                                 cycleTime,
                                 stochastics,
                                 parameters,
                                 observations,
                                 callbacks),
        agent(agent),
        world(world)
    {}
    UnrestrictedModelInterface(const UnrestrictedModelInterface &) = delete;
    UnrestrictedModelInterface(UnrestrictedModelInterface &&) = delete;
    UnrestrictedModelInterface &operator=(const UnrestrictedModelInterface &) = delete;
    UnrestrictedModelInterface &operator=(UnrestrictedModelInterface &&) = delete;
    virtual ~UnrestrictedModelInterface() = default;

protected:
    //-----------------------------------------------------------------------------
    //! Retrieves the world representation from the framework
    //!
    //! @return                       World (scenery and agents)
    //-----------------------------------------------------------------------------
    WorldInterface *GetWorld() const
    {
        return world;
    }

    //-----------------------------------------------------------------------------
    //! Retrieves the agent which contains this component
    //!
    //! @return                       Agent embedding this component
    //-----------------------------------------------------------------------------
    AgentInterface *GetAgent() const
    {
        return agent;
    }

    //-----------------------------------------------------------------------------
    //! Requests removal of the agent containing this component during the
    //! simulation
    //-----------------------------------------------------------------------------
    void RemoveAgent()
    {
        agent->RemoveAgent();
    }

private:
    AgentInterface *agent;                //!< Reference to agent containing this component
    WorldInterface *world;                //!< Reference to the world representation of the framework
};

class AlgorithmInterface : public RestrictedModelInterface
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     componentId    Corresponds to "id" of "Component"
    //! @param[in]     isInit         Corresponds to "init" of "Component"
    //! @param[in]     priority       Corresponds to "priority" of "Component"
    //! @param[in]     offsetTime     Corresponds to "offsetTime" of "Component"
    //! @param[in]     responseTime   Corresponds to "responseTime" of "Component"
    //! @param[in]     cycleTime      Corresponds to "cycleTime" of "Component"
    //! @param[in]     stochastics    Pointer to the stochastics class loaded by the framework
    //! @param[in]     parameters     Pointer to the parameters of the module
    //! @param[in]     observations   Pointer to the observations of the module
    //! @param[in]     callbacks      Pointer to the callbacks
    //! @param[in]     agent          Pointer to the agent in which the module is situated
    //-----------------------------------------------------------------------------
    AlgorithmInterface(int componentId,
                       bool isInit,
                       int priority,
                       int offsetTime,
                       int responseTime,
                       int cycleTime,
                       StochasticsInterface *stochastics,
                       const ParameterInterface *parameters,
                       const std::map<int, ObservationInterface *> *observations,
                       const CallbackInterface *callbacks,
                       int agentId) :
        RestrictedModelInterface(componentId,
                                 isInit,
                                 priority,
                                 offsetTime,
                                 responseTime,
                                 cycleTime,
                                 stochastics,
                                 parameters,
                                 observations,
                                 callbacks),
        agentId(agentId)
    {}
    AlgorithmInterface(const AlgorithmInterface &) = delete;
    AlgorithmInterface(AlgorithmInterface &&) = delete;
    AlgorithmInterface &operator=(const AlgorithmInterface &) = delete;
    AlgorithmInterface &operator=(AlgorithmInterface &&) = delete;
    virtual ~AlgorithmInterface() = default;

protected:
    //-----------------------------------------------------------------------------
    //! Retrieves the agent id
    //!
    //! @return                       agent id
    //-----------------------------------------------------------------------------
    int GetAgentId() const
    {
        return agentId;
    }

private:
    int agentId;                //!< Agent id
};

class ActionInterface : public UnrestrictedModelInterface
{
    using UnrestrictedModelInterface::UnrestrictedModelInterface;
};

class DynamicsInterface : public UnrestrictedModelInterface
{
    using UnrestrictedModelInterface::UnrestrictedModelInterface;
};

class SensorInterface : public UnrestrictedModelInterface
{
    using UnrestrictedModelInterface::UnrestrictedModelInterface;
};

class InitInterface : public UnrestrictedModelInterface
{
    using UnrestrictedModelInterface::UnrestrictedModelInterface;
};

#endif // MODELINTERFACE_H
