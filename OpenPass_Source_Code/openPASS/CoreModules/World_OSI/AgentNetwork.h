/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  AgentNetwork.h
//! @brief This file contains the representation of the agent network during a
//!        simulation run.
//-----------------------------------------------------------------------------


#pragma once

#include <functional>
#include <tuple>
#include <algorithm>
#include <utility>
#include <map>
#include "Interfaces/agentInterface.h"
#include "AgentAdapter.h"
#include "Interfaces/worldInterface.h"


/*!
* \brief network of agents
*
* This class stores all agents in a network. It is used to synchronize the update of all
* values of all agents.
*/
class AgentNetwork
{
public:    
    AgentNetwork(WorldInterface *world, const CallbackInterface *callbacks);
    AgentNetwork(const AgentNetwork&) = delete;
    AgentNetwork(AgentNetwork&&) = delete;
    AgentNetwork& operator=(const AgentNetwork&) = delete;
    AgentNetwork& operator=(AgentNetwork&&) = delete;
    virtual ~AgentNetwork();

    /*!
     * \brief AddAgent
     * Add agent to agent network:
     *
     * \param[in] id        id of agent
     * \param[in] agent     agent reference
     * \return              true for success
     */
    bool AddAgent(int id, AgentInterface *agent);

    /*!
     * \brief Clear
     * Clear map of agent network
     */
    void Clear();

    /*!
     * \brief QueueAgentUpdate
     * This function is used to store operations on the agents in a list.
     * At the end of each time step all queued operations will be executed.
     *
     * \param[in] func      function which is to stored to be executed later
     * \param[in] val       value for the function
     */
    void QueueAgentUpdate(std::function<void()> func);

    /*!
     * \brief QueueAgentRemove
     *
     * This function queues agents in a list that will be removed after a time step.
     * \param agent agent which shall be removed
     */
    void QueueAgentRemove(const AgentInterface *agent);

    /*!
     * \brief SyncGlobalData
     * This function is called after each timestep and executes all update function
     * and removes all agents in the remove list.
     */
    void SyncGlobalData();

    /*!
     * \brief GetAgent
     *
     * Retrieves specific agent by id
     * \param[in] id        Agent id
     * \return              Agent reference
     */
    AgentInterface *GetAgent(int id) const;

    /*!
     * \brief GetAgents
     * Retrieves all agents that currently exist
     *
     * \return              Mapping of ids to agents
     */
    virtual const std::map<int, AgentInterface *> &GetAgents() const;

    /*!
     * \brief GetRemovedAgents
     * Retrieves agents that were removed during the simulation run
     *
     * \return              List of agent references
     */
    virtual const std::list<const AgentInterface*> &GetRemovedAgents() const;

protected:
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
        if(callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:    
    WorldInterface *world;
    std::map<int, AgentInterface*> agents;
    std::list<const AgentInterface*> removedAgents;
    std::list<std::function<void()>> updateQueue;
    std::list<const AgentInterface*> removeQueue;

    const CallbackInterface *callbacks;
};


