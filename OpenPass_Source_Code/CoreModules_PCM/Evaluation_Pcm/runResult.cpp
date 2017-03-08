/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <algorithm>
#include <utility>
#include "runResult.h"

//-----------------------------------------------------------------------------
//! Function adding another agent to the run result that is involved in a
//! collision
//!
//! @param[in]  agent       The agent that should be added (containing all static information)
//! @param[in]  position    The position of that agent
//! @param[in]  yawAngle    The yaw angle of that agent
//! @param[in]  velocityX   The lateral velocity of that agent
//! @param[in]  velocityy   The vertical velocity of that agent
//! @param[in]  distance    The 1d distance of the collision point on the boundary of the agent
//-----------------------------------------------------------------------------
bool RunResult::AddCollisionAgent(const AgentInterface* agent,
                                  Common::Vector2d &position,
                                  double yawAngle,
                                  double velocityX,
                                  double velocityY,
                                  double distance)
{
    agents.push_back(agent);

    if(!positions.insert({agent, std::make_tuple(position.x, position.y)}).second)
    {
        return false;
    }

    if(!velocities.insert({agent, std::make_tuple(velocityX, velocityY)}).second)
    {
        return false;
    }

    if(!yawAngles.insert({agent, yawAngle}).second)
    {
        return false;
    }

    if(!distances.insert({agent, distance}).second)
    {
        return false;
    }

    return true;
}
