/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  collisionDetectionInterface.h
//! @brief This file contains the interface to communicate between framework
//!        and the collision detection library.
//-----------------------------------------------------------------------------

#ifndef COLLISIONDETECTIONINTERFACE
#define COLLISIONDETECTIONINTERFACE

#include "runResultInterface.h"
//-----------------------------------------------------------------------------
//! Interface for collision detection.
//! Implement algorithm for detecting a collision between two agents
//-----------------------------------------------------------------------------
class CollisionDetectionInterface
{
public:
    CollisionDetectionInterface() = default;
    CollisionDetectionInterface(const CollisionDetectionInterface&) = delete;
    CollisionDetectionInterface(CollisionDetectionInterface&&) = delete;
    CollisionDetectionInterface& operator=(const CollisionDetectionInterface&) = delete;
    CollisionDetectionInterface& operator=(CollisionDetectionInterface&&) = delete;
    virtual ~CollisionDetectionInterface() = default;

    /*!
     * \brief calculates the collisions between agents
     * calculates the collisions between agents, returns true if calculation was successful
     *
     * \param[in] result                 pointer where the result is saved
     * \param[in] isCollision            pointer where isCollision is saved
     * \return                           true, if calculation was successful
     */
    virtual bool HandleCollisionsInAgents(RunResultInterface &result, bool &isCollision) = 0;

    /*!
     * \brief sets a map of agents for collision detection
     *
     * \param agents                     map of agents
     */
    virtual void SetAgents(const std::map<int, const AgentInterface*> &agents) = 0;
};

#endif // COLLISIONDETECTIONINTERFACE

