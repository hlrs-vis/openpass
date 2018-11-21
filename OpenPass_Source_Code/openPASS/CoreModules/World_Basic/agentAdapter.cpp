/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/


#include <cassert>
#include <algorithm>
#include "agentAdapter.h"

AgentAdapter::AgentAdapter(WorldInterface *world, const CallbackInterface *callbacks) :
    world(world),
    callbacks(callbacks)
{}

bool AgentAdapter::InitAgentParameter(int id,
                                      int agentTypeId,
                                      int spawnTime,
                                      const AgentSpawnItem *agentSpawnItem,
                                      const SpawnItemParameterInterface &spawnItemParameter)
{
    Q_UNUSED(spawnTime);

    this->vehicleType = agentSpawnItem->GetVehicleType();
    this->id = id;
    this->agentTypeId = agentTypeId;

    UpdatePositionX(spawnItemParameter.GetPositionX());
    UpdatePositionY(spawnItemParameter.GetPositionY());
    UpdateWidth(agentSpawnItem->GetWidth());
    UpdateLength(agentSpawnItem->GetLength());
    UpdateVelocityX(spawnItemParameter.GetVelocityX());
    UpdateVelocityY(spawnItemParameter.GetVelocityY());
    UpdateDistanceCOGtoFrontAxle(agentSpawnItem->GetDistanceCOGtoFrontAxle());
    UpdateWeight(agentSpawnItem->GetWeight());
    UpdateHeightCOG(agentSpawnItem->GetHeightCOG());
    UpdateWheelbase(agentSpawnItem->GetWheelbase());
    UpdateMomentInertiaRoll(agentSpawnItem->GetMomentInertiaRoll());
    UpdateMomentInertiaPitch(agentSpawnItem->GetMomentInertiaPitch());
    UpdateMomentInertiaYaw(agentSpawnItem->GetMomentInertiaYaw());
    UpdateFrictionCoeff(agentSpawnItem->GetFrictionCoeff());
    UpdateTrackWidth(agentSpawnItem->GetTrackWidth());
    UpdateDistanceCOGtoLeadingEdge(agentSpawnItem->GetDistanceCOGtoLeadingEdge());
    UpdateAccelerationX(spawnItemParameter.GetAccelerationX());
    UpdateAccelerationY(spawnItemParameter.GetAccelerationY());
    UpdateYawAngle(spawnItemParameter.GetYawAngle());

    if(!Locate())
    {
        LOG(CbkLogLevel::Info, "agent starts outside road");
    }

    return true;
}

int AgentAdapter::GetAgentTypeId() const
{
    return agentTypeId;
}

double AgentAdapter::GetDistanceToFrontAgent(int laneId)
{
    Q_UNUSED(laneId);
    double posX = GetPositionX();
    double distance = INFINITY;

    for (const auto &it: world->GetAgents()){
        const AgentInterface *otherAgent = it.second;
        double posXother = otherAgent->GetPositionX();
        if ((otherAgent->GetAgentId() != id) && (posX < posXother))
        {
           distance = std::min(distance, posXother - posX);
        }
    }

    return distance;
}
