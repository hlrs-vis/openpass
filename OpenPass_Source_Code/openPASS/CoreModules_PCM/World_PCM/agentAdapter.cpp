/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <cassert>
#include <algorithm>
#include "agentAdapter.h"
#include "world_pcm_implementation.h"

AgentAdapter::AgentAdapter(WorldInterface *world, const CallbackInterface *callbacks) :
    world(world),
    callbacks(callbacks)
{
    worldPCM = static_cast<World_PCM_Implementation *>(world);
}

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

    if (!Locate()) {
        LOG(CbkLogLevel::Info, "agent starts outside road");
    }

    return true;
}

void AgentAdapter::UpdateCollision(int collisionPartnerId, int collisionDataId, void *collisionData)
{
    UpdateCollision(collisionPartnerId);

    std::map<int, std::map<int, std::vector<void *>>>::iterator collisionCrashDataIterator
        = collisionCrashData.find(collisionPartnerId);
    if (collisionCrashDataIterator != collisionCrashData.end()) {
        std::map<int, std::vector<void *>> *crashData
                                        = &collisionCrashDataIterator->second;
        std::map<int, std::vector<void *>>::iterator crashDataIterator
                                        = crashData->find(collisionDataId);
        if (crashDataIterator != crashData->end()) {
            std::vector<void *> *crashDataVector = &crashDataIterator->second;
            crashDataVector->push_back(collisionData);
        } else {
            std::vector<void *> crashDataVector;
            crashDataVector.push_back(collisionData);
            crashData->emplace(std::make_pair(collisionDataId, crashDataVector));
        }
    } else {
        std::vector<void *> crashDataVector;
        crashDataVector.push_back(collisionData);
        std::map<int, std::vector<void *>> crashData;
        crashData.emplace(std::make_pair(collisionDataId, crashDataVector));
        collisionCrashData.emplace(std::make_pair(collisionPartnerId, crashData));
    }
}

std::vector<void *> AgentAdapter::GetCollisionData(int collisionPartnerId,
                                                   int collisionDataId) const
{
    std::vector<void *> collisionData;
    std::map<int, std::map<int, std::vector<void *>>>::const_iterator collisionCrashDataIterator
        = collisionCrashData.find(collisionPartnerId);
    if (collisionCrashDataIterator != collisionCrashData.end()) {
        std::map<int, std::vector<void *>> crashData
                                        = collisionCrashDataIterator->second;
        std::map<int, std::vector<void *>>::const_iterator crashDataIterator
                                        = crashData.find(collisionDataId);
        if (crashDataIterator != crashData.end()) {
            collisionData = crashDataIterator->second;
        }
    }
    return collisionData;
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

    for (const auto &it : world->GetAgents()) {
        const AgentInterface *otherAgent = it.second;
        double posXother = otherAgent->GetPositionX();
        if ((otherAgent->GetAgentId() != id) && (posX < posXother)) {
            distance = std::min(distance, posXother - posX);
        }
    }

    return distance;
}

double AgentAdapter::GetVelocityAbsolute() const
{
    Common::Vector2d velocityVec(velocityX, velocityY);
    return velocityVec.Length();
}

std::string AgentAdapter::GetTypeOfNearestMark() const
{
    const PCM_Data *data = worldPCM->GetPCM_Data();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    MarkType minMarkType = data->GetTypeOfNearestLineSegment(&agentPoint);

    return PCM_Helper::ConvertMarkTypeToDBString(minMarkType);
}

double AgentAdapter::GetDistanceToNearestMark(MarkType markType) const
{
    double minDistance = GetDistanceToNearestMarkInViewRange(markType, INFINITY, INFINITY);
    return minDistance;
}

double AgentAdapter::GetViewDirectionToNearestMark(MarkType markType) const
{
    return GetViewDirectionToNearestMarkInViewRange(markType, INFINITY, INFINITY);
}

AgentViewDirection AgentAdapter::GetAgentViewDirectionToNearestMark(MarkType markType) const
{
    double viewDirection = GetViewDirectionToNearestMark(markType);
    return CommonHelper::ConvertRadiantToAgentViewDirection(viewDirection);
}

double AgentAdapter::GetOrientationOfNearestMark(MarkType markType) const
{
    return GetOrientationOfNearestMarkInViewRange(markType, INFINITY, INFINITY);
}

double AgentAdapter::GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                             AgentViewDirection agentViewDirection) const
{
    double viewDirection = CommonHelper::ConvertagentViewDirectionToRadiant(agentViewDirection);
    return GetDistanceToNearestMarkInViewDirection(markType, viewDirection);
}

double AgentAdapter::GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                             double mainViewDirection) const
{
    return GetDistanceToNearestMarkInViewRange(markType, mainViewDirection, INFINITY);
}

double AgentAdapter::GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                                AgentViewDirection agentViewDirection) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadiant(agentViewDirection);
    return GetOrientationOfNearestMarkInViewDirection(markType, mainViewDirection);
}

double AgentAdapter::GetOrientationOfNearestMarkInViewDirection(MarkType markType,
                                                                double mainViewDirection) const
{
    return GetOrientationOfNearestMarkInViewRange(markType, mainViewDirection, INFINITY);
}

double AgentAdapter::GetDistanceToNearestMarkInViewRange(MarkType markType,
                                                         AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadiant(agentViewDirection);
    return GetDistanceToNearestMarkInViewRange(markType, mainViewDirection, range);
}

double AgentAdapter::GetDistanceToNearestMarkInViewRange(MarkType markType,
                                                         double mainViewDirection, double range) const
{
    double resultingAngle = INFINITY;
    if (!std::isinf(mainViewDirection)) {
        double agentAngle = GetYawAngle();
        resultingAngle = agentAngle + mainViewDirection;
        if (fabs(resultingAngle) > M_PI) {
            if (resultingAngle > 0) {
                resultingAngle -= 2 * M_PI;
            } else {
                resultingAngle += 2 * M_PI;
            }
        }
    }

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegment(markType, &agentPoint,
                                                                 resultingAngle, range);

    double minDistance = minLineSegment.CalcDistanceFromPoint(&agentPoint, resultingAngle, range);

    return minDistance;
}

double AgentAdapter::GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                            AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadiant(agentViewDirection);
    return GetOrientationOfNearestMarkInViewRange(markType, mainViewDirection, range);
}

double AgentAdapter::GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                            double mainViewDirection, double range) const
{
    double agentAngle = GetYawAngle();
    double resultingAngle = INFINITY;
    if (!std::isinf(mainViewDirection)) {
        resultingAngle = agentAngle + mainViewDirection;
        if (fabs(resultingAngle) > M_PI) {
            if (resultingAngle > 0) {
                resultingAngle -= 2 * M_PI;
            } else {
                resultingAngle += 2 * M_PI;
            }
        }
    }

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegment(markType, &agentPoint, resultingAngle,
                                                                 range);

    double lineAngle = minLineSegment.CalculateAngle();
    double angle = fabs(agentAngle - lineAngle);
    double alternateAngle;
    if (lineAngle > 0) {
        alternateAngle = fabs(agentAngle - (lineAngle - M_PI));
    } else {
        alternateAngle = fabs(agentAngle - (lineAngle + M_PI));
    }

    if (angle < alternateAngle) {
        return angle;
    } else {
        return alternateAngle;
    }
}

double AgentAdapter::GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                              AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadiant(agentViewDirection);
    return GetViewDirectionToNearestMarkInViewRange(markType, mainViewDirection, range);
}

double AgentAdapter::GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                              double mainViewDirection, double range) const
{
    double agentAngle = GetYawAngle();
    double resultingAngle = INFINITY;
    if (!std::isinf(mainViewDirection)) {
        resultingAngle = agentAngle + mainViewDirection;
        if (fabs(resultingAngle) > M_PI) {
            if (resultingAngle > 0) {
                resultingAngle -= 2 * M_PI;
            } else {
                resultingAngle += 2 * M_PI;
            }
        }
    }

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_Point nearestPoint = data->GetNearestPoint(markType, &agentPoint, resultingAngle, range);

    double pointAngle = PCM_Helper::CalcAngleBetweenPoints(&agentPoint, &nearestPoint);

    double viewDirection = pointAngle - agentAngle;

    if (fabs(viewDirection) > M_PI) {
        if (viewDirection > 0) {
            viewDirection -= 2 * M_PI;
        } else {
            viewDirection += 2 * M_PI;
        }
    }

    return viewDirection;
}

double AgentAdapter::GetYawVelocity()
{
    return yawVelocity;
}

void AgentAdapter::SetYawVelocity(double yawVelocity)
{
    this->yawVelocity = yawVelocity;
}

double AgentAdapter::GetYawAcceleration()
{
    return yawAcceleration;
}

void AgentAdapter::SetYawAcceleration(double yawAcceleration)
{
    this->yawAcceleration = yawAcceleration;
}

const std::vector<int> *AgentAdapter::GetTrajectoryTime() const
{
    return worldPCM->GetTrajectory(GetAgentId())->GetTimeVec();
}

const std::vector<double> *AgentAdapter::GetTrajectoryXPos() const
{
    return worldPCM->GetTrajectory(GetAgentId())->GetXPosVec();
}

const std::vector<double> *AgentAdapter::GetTrajectoryYPos() const
{
    return worldPCM->GetTrajectory(GetAgentId())->GetYPosVec();
}

const std::vector<double> *AgentAdapter::GetTrajectoryVelocity() const
{
    return worldPCM->GetTrajectory(GetAgentId())->GetVelVec();
}

const std::vector<double> *AgentAdapter::GetTrajectoryAngle() const
{
    return worldPCM->GetTrajectory(GetAgentId())->GetPsiVec();
}
