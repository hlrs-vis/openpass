/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <algorithm>
#include <utility>
#include "runResult.h"
#include "vector2d.h"
#include "log.h"

namespace SimulationSlave
{

void RunResult::Clear()
{
    result = 0;
    collisions.clear();
}

bool RunResult::AddCollision(const AgentInterface* agent,
                             Common::Vector2d &positionAgent,
                             double distanceOnBorderAgent,
                             const AgentInterface* other,
                             Common::Vector2d &positionOther,
                             double distanceOnBorderOther)
{
    collisions.push_back(Collision(OpponentItem(agent, positionAgent, distanceOnBorderAgent),
                                   OpponentItem(other, positionOther, distanceOnBorderOther)));
    return true;
}

} // namespace SimulationSlave
