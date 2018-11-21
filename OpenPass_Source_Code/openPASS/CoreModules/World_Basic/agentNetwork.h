/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


//-----------------------------------------------------------------------------
//! @file  agentNetwork.h
//! @brief This file contains the representation of the agent network during a
//!        simulation run.
//-----------------------------------------------------------------------------


#ifndef AGENTNETWORK_H
#define AGENTNETWORK_H

#include <functional>
#include <tuple>
#include <algorithm>
#include <utility>
#include <map>
#include "agentInterface.h"
#include "agentAdapter.h"
#include "worldInterface.h"


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
    bool AddAgent(int id, const AgentInterface *agent);

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
    void QueueAgentUpdate(std::function<void(double)> func,
                          double val);

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
    virtual const AgentInterface *GetAgent(int id) const;

    /*!
     * \brief GetAgents
     * Retrieves all agents
     *
     * \return              Mapping of ids to agents
     */
    virtual const std::map<int, const AgentInterface*> &GetAgents() const;

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
    std::map<int, const AgentInterface*> agents;
    std::list<const AgentInterface*> removedAgents;
    std::list<std::tuple<std::function<void(double)>, double>> updateQueue;
    std::list<const AgentInterface*> removeQueue;

    const CallbackInterface *callbacks;
};

#endif // AGENTNETWORK_H
