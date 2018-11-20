/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include <qglobal.h>

#include <cmath>
#include <limits>
#include <vector>

#include "CommonSceneryHelper.h"
#include "OWL/DataTypes.h"
#include "WorldData.h"

using namespace OWL;

namespace CommonSceneryHelper
{
// would be cool to move laneIds to its own domain type
int SceneryHelper::GetPotentialLaneIdLeft(int laneId)
{
    if(laneId == 0) return (int)INFINITY;
    return laneId + (laneId < 0 ? 1 : -1);
}

// would be cool to move laneIds to its own domain type
int SceneryHelper::GetPotentialLaneIdRight(int laneId)
{
    if(laneId == 0) return (int)INFINITY;
    return laneId - (laneId < 0 ? 1 : -1);
}

double SceneryHelper::GetDistanceToEndOfLaneHelper(CLane* lane, double distanceOfInterest, double currentPosition)
{
    if (distanceOfInterest < lane->GetLength() - currentPosition)
    {
        return INFINITY;
    }

    const CLanes& nextLanes = lane->GetNextLanes();
    if (nextLanes.size() == 0)
    {
        return lane->GetLength() - currentPosition;
    }

    // if there is an intersection, the maximal possible length is returned (maxDist not minDist)
    double maxDist = 0;

    for (auto nextLane : nextLanes)
    {
        double currDist = GetDistanceToEndOfLaneHelper(nextLane,
                                                       distanceOfInterest - lane->GetLength() + currentPosition,
                                                       -(lane->GetLength() - currentPosition));
        if (currDist == INFINITY)
        {
            return currDist;
        }

        maxDist = std::max(maxDist, currDist);
    }

    return maxDist;
}

void SceneryHelper::AssignCarInfo(AgentInterface* agentInterface, double accSensDist)
{
    // TODO compare with BMW World
    CarInfo* carInfo = new CarInfo();
    agentInterface->SetCarInfo(carInfo);
}

double SceneryHelper::GetVelocityLatP(AgentInterface* agentInterface)
{
    CarInfo* objectP = agentInterface->GetCarInfo();
    return objectP->vw;
}

double SceneryHelper::GetVelocityAbsP(AgentInterface* agentInterface)
{
    CarInfo* objectP = agentInterface->GetCarInfo();
    return objectP->v;
}

double SceneryHelper::GetPositionLatP(const AgentInterface *agentInterface)
{
    CarInfo* objectP = agentInterface->GetCarInfo();
    return objectP->w;
}

const AgentInterface *SceneryHelper::GetLastAgent(std::vector<const AgentInterface *> *agents)
{
    const AgentInterface* lastAgent = nullptr;

    if((int) agents->size() == 0)
    {
        return lastAgent;
    }

    double minimumDistance = std::numeric_limits<double>::max();
    for(const AgentInterface* agent : *agents)
    {
        double agentDistanceToStart = agent->GetDistanceToStartOfRoad();
        if(agentDistanceToStart < minimumDistance)
        {
            minimumDistance = agentDistanceToStart;
            lastAgent = agent;
        }
    }

    return lastAgent;
}

const AgentInterface *SceneryHelper::GetFirstAgent(std::vector<const AgentInterface *> *agents)
{
    const AgentInterface* firstAgent = nullptr;

    if((int) agents->size() == 0)
    {
        return firstAgent;
    }

    double maximumDistance = 0;
    for(const AgentInterface* agent : *agents)
    {
        double agentDistanceToStart = agent->GetDistanceToStartOfRoad();
        if(agentDistanceToStart > maximumDistance)
        {
            maximumDistance = agentDistanceToStart;
            firstAgent = agent;
        }
    }

    return firstAgent;
}

} // CommonSceneryHelper
