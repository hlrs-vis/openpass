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
//! @file  RunResultInterface.h
//! @brief This file contains the interface to gain access to the results of a
//!        run.
//!
//! Important for the collision detection library.
//-----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include "Interfaces/agentInterface.h"


//! Provides access to run results
class RunResultInterface
{
public:
    RunResultInterface() = default;
    RunResultInterface(const RunResultInterface&) = delete;
    RunResultInterface(RunResultInterface&&) = delete;
    RunResultInterface& operator=(const RunResultInterface&) = delete;
    RunResultInterface& operator=(RunResultInterface&&) = delete;
    virtual ~RunResultInterface() = default;

    //-----------------------------------------------------------------------------
    //! Indicate end of simulation run by observation module
    //-----------------------------------------------------------------------------
    virtual void SetEndCondition()
    {}

    //-----------------------------------------------------------------------------
    //! Provides the agents which collided in the simulation run
    //!
    //! @return                List of agents
    //-----------------------------------------------------------------------------
    virtual const std::list<int> *GetCollisionIds() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the position of the agent reference point at the moment of contact
    //!
    //! @return                Mapping of agents to x-/y-coordinates
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, std::tuple<double, double>> *GetPositions() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the velocities of the agents at the moment of contact
    //!
    //! @return                Mapping of agents to forward-/sideward-velocities
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, std::tuple<double, double>> *GetVelocities() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the yaw angles of the agents at the moment of contact
    //!
    //! @return                Mapping of agents to yaw angles
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, double> *GetYaws() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Provides the distances on the border of the agents at the moment of
    //! contact. The are counted clock-wise starting from the upper left corner of
    //! the boundary box of an agent and ending at the point of contact.
    //!
    //! @return                Mapping of agents to
    //-----------------------------------------------------------------------------
    virtual const std::map<const AgentInterface*, double> *GetDistances() const
    {
        return nullptr;
    }

    //-----------------------------------------------------------------------------
    //! Indicates if the simulation ended by a collision
    //!
    //! @return                True if the simulation ended by collision
    //-----------------------------------------------------------------------------
    virtual bool IsCollision() const
    {
        return false;
    }

    //-----------------------------------------------------------------------------
    //! Indicates if the simulation ended by exceeding the end time
    //!
    //! @return                True if the simulation ended by time over
    //-----------------------------------------------------------------------------
    virtual bool IsTimeOver() const
    {
        return false;
    }

    //-----------------------------------------------------------------------------
    //! Indicates if the simulation ended by an observation module
    //!
    //! @return                True if the simulation ended by the end condition of
    //!                        an observation module
    //-----------------------------------------------------------------------------
    virtual bool IsEndCondition() const
    {
        return false;
    }

    //-----------------------------------------------------------------------------
    //! Adds a collision to a list of collisions with information about the collided
    //! agents.
    //!
    //! @return                True if successful
    //-----------------------------------------------------------------------------
    virtual void AddCollisionId(const int agentId) = 0;
};



