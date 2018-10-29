/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
