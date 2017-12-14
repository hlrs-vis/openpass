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

    if (!Locate())
    {
        LOG(CbkLogLevel::Info, "agent starts outside road");
    }

    return true;
}

void AgentAdapter::UpdateCollision(int collisionPartnerId, int collisionDataId, void *collisionData)
{
    UpdateCollision(collisionPartnerId);

    std::map<int, std::map<int, std::vector<void *>>>::iterator collisionCrashDataIterator
        = collisionCrashData.find(collisionPartnerId);
    if (collisionCrashDataIterator != collisionCrashData.end())
    {
        std::map<int, std::vector<void *>> *crashData
                                        = &collisionCrashDataIterator->second;
        std::map<int, std::vector<void *>>::iterator crashDataIterator
                                        = crashData->find(collisionDataId);
        if (crashDataIterator != crashData->end())
        {
            std::vector<void *> *crashDataVector = &crashDataIterator->second;
            crashDataVector->push_back(collisionData);
        }
        else
        {
            std::vector<void *> crashDataVector;
            crashDataVector.push_back(collisionData);
            crashData->emplace(std::make_pair(collisionDataId, crashDataVector));
        }
    }
    else
    {
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
    if (collisionCrashDataIterator != collisionCrashData.end())
    {
        std::map<int, std::vector<void *>> crashData
                                        = collisionCrashDataIterator->second;
        std::map<int, std::vector<void *>>::const_iterator crashDataIterator
                                        = crashData.find(collisionDataId);
        if (crashDataIterator != crashData.end())
        {
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

    for (const auto &it : world->GetAgents())
    {
        const AgentInterface *otherAgent = it.second;
        double posXother = otherAgent->GetPositionX();
        if ((otherAgent->GetAgentId() != id) && (posX < posXother))
        {
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

MarkType AgentAdapter::GetTypeOfNearestMark() const
{
    const PCM_Data *data = worldPCM->GetPCM_Data();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    MarkType minMarkType = data->GetMarkTypeOfNearestLineSegment(&agentPoint);
    return minMarkType;
}

std::string AgentAdapter::GetTypeOfNearestMarkString() const
{
    return PCM_Helper::ConvertMarkTypeToDBString(GetTypeOfNearestMark());
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
    return CommonHelper::ConvertRadianToAgentViewDirection(viewDirection);
}

double AgentAdapter::GetOrientationOfNearestMark(MarkType markType) const
{
    return GetOrientationOfNearestMarkInViewRange(markType, INFINITY, INFINITY);
}

double AgentAdapter::GetDistanceToNearestMarkInViewDirection(MarkType markType,
                                                             AgentViewDirection agentViewDirection) const
{
    double viewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
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
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
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
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetDistanceToNearestMarkInViewRange(markType, mainViewDirection, range);
}

double AgentAdapter::GetDistanceToNearestMarkInViewRange(MarkType markType,
                                                         double mainViewDirection, double range) const
{
    double resultingAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegmentOfMarks(markType, &agentPoint,
                                                                        resultingAngle, range);

    double minDistance = minLineSegment.CalcDistanceFromPoint(&agentPoint, resultingAngle, range);

    return minDistance;
}

double AgentAdapter::GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                            AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetOrientationOfNearestMarkInViewRange(markType, mainViewDirection, range);
}

double AgentAdapter::GetOrientationOfNearestMarkInViewRange(MarkType markType,
                                                            double mainViewDirection, double range) const
{
    double resultingAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegmentOfMarks(markType, &agentPoint,
                                                                        resultingAngle,
                                                                        range);

    double lineAngle = minLineSegment.CalculateAngle();
    double agentAngle = GetYawAngle();
    double angle = fabs(agentAngle - lineAngle);
    double alternateAngle;
    if (lineAngle > 0)
    {
        alternateAngle = fabs(agentAngle - (lineAngle - M_PI));
    }
    else
    {
        alternateAngle = fabs(agentAngle - (lineAngle + M_PI));
    }

    if (angle < alternateAngle)
    {
        return angle;
    }
    else
    {
        return alternateAngle;
    }
}

double AgentAdapter::GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                              AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetViewDirectionToNearestMarkInViewRange(markType, mainViewDirection, range);
}

double AgentAdapter::GetViewDirectionToNearestMarkInViewRange(MarkType markType,
                                                              double mainViewDirection, double range) const
{
    double resultingAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_Point nearestPoint = data->GetNearestPointOfMarks(markType, &agentPoint, resultingAngle, range);

    double pointAngle = PCM_Helper::CalcAngleBetweenPoints(&agentPoint, &nearestPoint);

    double agentAngle = GetYawAngle();
    double viewDirection = pointAngle - agentAngle;

    viewDirection = ConvertAngleToPi(viewDirection);

    return viewDirection;
}

std::string AgentAdapter::GetTypeOfNearestObject(AgentViewDirection agentViewDirection,
                                                 double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetTypeOfNearestObject(mainViewDirection, range);
}

std::string AgentAdapter::GetTypeOfNearestObject(double mainViewDirection, double range) const
{
    const PCM_Data *data = worldPCM->GetPCM_Data();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    ObjectType minObjectType = data->GetObjectTypeOfNearestLineSegment(&agentPoint, mainViewDirection,
                                                                       range);

    return PCM_Helper::ConvertObjectTypeToDBString(minObjectType);
}

double AgentAdapter::GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                           AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetDistanceToNearestObjectInViewRange(objectType, mainViewDirection, range);
}

double AgentAdapter::GetDistanceToNearestObjectInViewRange(ObjectType objectType,
                                                           double mainViewDirection, double range) const
{
    double resultingAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegmentOfObject(objectType, &agentPoint,
                                                                         resultingAngle, range);

    double minDistance = minLineSegment.CalcDistanceFromPoint(&agentPoint, resultingAngle, range);

    return minDistance;
}

double AgentAdapter::GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                                AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetViewDirectionToNearestObjectInViewRange(objectType, mainViewDirection, range);
}

double AgentAdapter::GetViewDirectionToNearestObjectInViewRange(ObjectType objectType,
                                                                double mainViewDirection, double range) const
{
    double resultingAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_Point nearestPoint = data->GetNearestPointOfObject(objectType, &agentPoint, resultingAngle,
                                                           range);

    double pointAngle = PCM_Helper::CalcAngleBetweenPoints(&agentPoint, &nearestPoint);

    double agentAngle = GetYawAngle();
    double viewDirection = pointAngle - agentAngle;

    viewDirection = ConvertAngleToPi(viewDirection);

    return viewDirection;
}

int AgentAdapter::GetIdOfNearestAgent(AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetIdOfNearestAgent(mainViewDirection, range);
}

int AgentAdapter::GetIdOfNearestAgent(double mainViewDirection, double range) const
{
    int egoId = GetAgentId();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    int id = data->GetIdOfNearestAgent(egoId, &agentPoint, mainViewDirection, range);

    return id;
}

double AgentAdapter::GetDistanceToNearestAgentInViewRange(AgentViewDirection agentViewDirection,
                                                          double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetDistanceToNearestAgentInViewRange(mainViewDirection, range);
}

double AgentAdapter::GetDistanceToNearestAgentInViewRange(double mainViewDirection,
                                                          double range) const
{
    double resultingAngle = GetAbsoluteViewAngle(mainViewDirection);

    int egoId = GetAgentId();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegmentOfAgents(egoId, &agentPoint,
                                                                         resultingAngle, range);

    double minDistance = minLineSegment.CalcDistanceFromPoint(&agentPoint, resultingAngle, range);

    return minDistance;
}

double AgentAdapter::GetViewDirectionToNearestAgentInViewRange(AgentViewDirection
                                                               agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetViewDirectionToNearestAgentInViewRange(mainViewDirection, range);
}

double AgentAdapter::GetViewDirectionToNearestAgentInViewRange(double mainViewDirection,
                                                               double range) const
{
    double resultingAngle = GetAbsoluteViewAngle(mainViewDirection);

    int egoId = GetAgentId();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_Point nearestPoint = data->GetNearestPointOfAgents(egoId, &agentPoint, resultingAngle,
                                                           range);

    double pointAngle = PCM_Helper::CalcAngleBetweenPoints(&agentPoint, &nearestPoint);

    double agentAngle = GetYawAngle();
    double viewDirection = pointAngle - agentAngle;

    viewDirection = ConvertAngleToPi(viewDirection);

    return viewDirection;
}

double AgentAdapter::GetYawVelocity()
{
    return yawVelocity;
}

void AgentAdapter::SetYawVelocity(double yawVelocity)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateYawVelocity(arg);
    },
    yawVelocity);
}

double AgentAdapter::GetYawAcceleration()
{
    return yawAcceleration;
}

void AgentAdapter::SetYawAcceleration(double yawAcceleration)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateYawAcceleration(arg);
    },
    yawAcceleration);
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

void AgentAdapter::SetAccelerationIntention(double accelerationIntention)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateAccelerationIntention(arg);
    },
    accelerationIntention);
}

double AgentAdapter::GetAccelerationIntention() const
{
    return accelerationIntention;
}

void AgentAdapter::SetDecelerationIntention(double decelerationIntention)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateDecelerationIntention(arg);
    },
    decelerationIntention);
}

double AgentAdapter::GetDecelerationIntention() const
{
    return decelerationIntention;
}

void AgentAdapter::SetAngleIntention(double angleIntention)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateAngleIntention(arg);
    },
    angleIntention);
}

double AgentAdapter::GetAngleIntention() const
{
    return angleIntention;
}

void AgentAdapter::SetCollisionState(bool collisionState)
{
    world->QueueAgentUpdate([this](double arg)
    {
        UpdateCollisionState(arg);
    },
    collisionState);
}

bool AgentAdapter::GetCollisionState() const
{
    return collisionState;
}

double AgentAdapter::GetAccelerationAbsolute() const
{
    Common::Vector2d accelerationVec(accelerationX, accelerationY);
    return accelerationVec.Length();
}

double AgentAdapter::GetAbsoluteViewAngle(double mainViewDirection) const
{
    double resultingAngle = INFINITY;
    if (!std::isinf(mainViewDirection))
    {
        double agentAngle = GetYawAngle();
        resultingAngle = agentAngle + mainViewDirection;

        resultingAngle = ConvertAngleToPi(resultingAngle);
    }

    return resultingAngle;
}

double AgentAdapter::ConvertAngleToPi(double angle) const
{
    double out_angle = angle;
    if (fabs(out_angle) > M_PI)
    {
        if (out_angle > 0)
        {
            out_angle -= 2 * M_PI;
        }
        else
        {
            out_angle += 2 * M_PI;
        }
    }

    return out_angle;
}
