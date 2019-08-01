/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  AgentBlueprintProviderInterface.h
//! @brief This file contains the interface of the agentBlueprintProvider to interact
//!        with the framework.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/agentBlueprintInterface.h"

class AgentBlueprintProviderInterface
{
public:
    virtual ~AgentBlueprintProviderInterface() = default;

    /*!
    * \brief Samples an entire agent
    *
    * \details Samples an entired agent from a given SystemConfig or from a dynamically built agent
    *
    *
    * @param[in/out]    agentBlueprint          All results get stored in the agent blueprint
    * @param[in]        laneCategory            Category of the lane the agent will be spawned in only relevant for common agents
    * @param[in]        scenarioAgentIterator     Iterator for scenario agent arrays.
    *
    * @return           true if Agent was succesfully sampled
    */
    virtual bool SampleAgent(AgentBlueprintInterface& agentBlueprint,
                             LaneCategory laneCategory,
                             unsigned int scenarioAgentIterator) = 0;

};

