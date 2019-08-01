/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

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
    double viewAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegmentOfMarks(markType, &agentPoint,
                                                                        viewAngle, range);

    double minDistance = minLineSegment.CalcDistanceFromPoint(&agentPoint, viewAngle, range);

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
    double viewAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegmentOfMarks(markType, &agentPoint,
                                                                        viewAngle, range);

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
    double viewAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_Point nearestPoint = data->GetNearestPointOfMarks(markType, &agentPoint, viewAngle, range);

    double pointAngle = PCM_Helper::CalcAngleBetweenPoints(&agentPoint, &nearestPoint);

    double agentAngle = GetYawAngle();
    double viewDirection = pointAngle - agentAngle;

    viewDirection = CommonHelper::ConvertAngleToPi(viewDirection);

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
    double viewAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_LineSegment minLineSegment = data->GetNearestLineSegmentOfObject(objectType, &agentPoint,
                                                                         viewAngle, range);

    double minDistance = minLineSegment.CalcDistanceFromPoint(&agentPoint, viewAngle, range);

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
    double viewAngle = GetAbsoluteViewAngle(mainViewDirection);

    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    const PCM_Data *data = worldPCM->GetPCM_Data();
    PCM_Point nearestPoint = data->GetNearestPointOfObject(objectType, &agentPoint, viewAngle, range);

    double pointAngle = PCM_Helper::CalcAngleBetweenPoints(&agentPoint, &nearestPoint);

    double agentAngle = GetYawAngle();
    double viewDirection = pointAngle - agentAngle;

    viewDirection = CommonHelper::ConvertAngleToPi(viewDirection);

    return viewDirection;
}

int AgentAdapter::GetIdOfNearestAgent(AgentViewDirection agentViewDirection, double range) const
{
    double mainViewDirection = CommonHelper::ConvertagentViewDirectionToRadian(agentViewDirection);
    return GetIdOfNearestAgent(mainViewDirection, range);
}

int AgentAdapter::GetIdOfNearestAgent(double mainViewDirection, double range) const
{
    AgentDetection agentDet = GetNearestAgent(mainViewDirection, range);
    return agentDet.oppId;
}

double AgentAdapter::GetViewDirectionToPoint(const PCM_Point &point) const
{
    if(PCM_Helper::CheckPointValid(&point))
    {
        const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
        double pointAngle = PCM_Helper::CalcAngleBetweenPoints(&agentPoint, &point);

        double agentAngle = GetYawAngle();
        double viewDirection = pointAngle - agentAngle;

        return CommonHelper::ConvertAngleToPi(viewDirection);
    }
    return INFINITY;
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
    AgentDetection agentDet = GetNearestAgent(mainViewDirection, range);

//    LOGINFO(QString().sprintf("%.2f m from ego center to nearest agent", distance).toStdString());

//    // calculate the ego edge towards the nearest opponent
//    const PCM_Data *data = worldPCM->GetPCM_Data();
//    int egoId = GetAgentId();
//    double viewDirection = GetViewDirectionToPoint(&nearestPoint);
//    const PCM_Point egoCrossPoint = data->GetNearestPointOfAgents(egoId, -1, &agentPoint, viewDirection);

//    // calculate the distance from ego center to the ego edge (towards the nearest opponent)
//    double distanceInner = PCM_Helper::CalcDistanceBetweenPoints(&agentPoint, &egoCrossPoint);

//    if(distanceInner > 0)
//        LOGINFO(QString().sprintf("%.2f m from ego center to ego edge", distanceInner).toStdString());
//    else{
//        LOGINFO(QString().sprintf("No distance between ego (%.2f, %.2f) and ego crossing point (%.2f, %.2f)",
//                                  GetPositionX(), GetPositionY(), egoCrossPoint.GetX(), egoCrossPoint.GetY()).toStdString());
//    }

//    // deduct the line segment within ego from the distance
//    distance = distance - distanceInner;

    return agentDet.distance;
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
    AgentDetection agentDet = GetNearestAgent(mainViewDirection, range);
    return GetViewDirectionToPoint(agentDet.oppPoint);
}

// private method
AgentDetection AgentAdapter::GetNearestAgent(double mainViewDirection,double range) const
{
    int egoId = GetAgentId();
    const PCM_Data *data = worldPCM->GetPCM_Data();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    double viewAngle = GetAbsoluteViewAngle(mainViewDirection); // convert the view direction from the ego coordinate to the world coordinate

    AgentDetection agentDet = data->GetNearestAgent(egoId, &agentPoint, viewAngle, range);
//    LOGINFO(QString().sprintf("AgentAdapter: ego%d 's nearest agent at (%.2f, %.2f) with viewAngle %.2f and range %.2f",
//                                      agentDet.egoId, agentDet.oppPoint.GetX(), agentDet.oppPoint.GetY(), viewAngle, range).toStdString());

    return agentDet;
}

bool AgentAdapter::GetObstacleViewRanges(double viewAngle, double range,
                                         double distanceMax,
                                         const std::map<int, PCM_Line *> *lineMap,
                                         std::vector<std::pair<double, double>> &obstacleViewRanges) const
{
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    PCM_Point *prevPoint = nullptr;

    for (std::pair<int, PCM_Line *> pcmLinePair : *lineMap)
    {
        PCM_Line *line = pcmLinePair.second;
        const std::map<int, const PCM_Point *> *pointMap = line->GetPointMap();
        for (std::pair<int, const PCM_Point *> pcmPointPair : *pointMap)
        {
            const PCM_Point *point = pcmPointPair.second;
            if(prevPoint != nullptr)
            {
                PCM_LineSegment objLineSeg = PCM_LineSegment(*prevPoint, *point);
                double objDistance = objLineSeg.CalcDistanceFromPoint(&agentPoint, viewAngle, range);

                if ((objDistance > 0) && (objDistance < distanceMax))
                {
                    // restrict the object line segment within ego' view range
                    PCM_LineSegment objLineSegSub = objLineSeg.CalcSubLineSegmentInViewRange(&agentPoint, viewAngle, range);

                    double viewDirObjMin = GetViewDirectionToPoint(objLineSegSub.GetFirstPoint());
                    double viewDirObjMax = GetViewDirectionToPoint(objLineSegSub.GetSecondPoint());
                    if(viewDirObjMin > viewDirObjMax)
                        std::swap(viewDirObjMin, viewDirObjMax);

//                    LOGINFO(QString().sprintf("AgentAdapter: obstacle line from (%f, %f) to (%f, %f) (%f m restricted from %f m) with range %f (%f - %f)",
//                                              objLineSegSub.GetFirstPoint().GetX(), objLineSegSub.GetFirstPoint().GetY(),
//                                              objLineSegSub.GetSecondPoint().GetX(), objLineSegSub.GetSecondPoint().GetY(),
//                                              objLineSegSub.CalculateLength(), objLineSeg.CalculateLength(),
//                                (viewDirObjMax - viewDirObjMin), viewDirObjMin, viewDirObjMax).toStdString());

                    if((!std::isinf(viewDirObjMin)) && (!std::isinf(viewDirObjMax)))
                    {
                        bool rangeUpdated = false;
                        for (auto& viewRange : obstacleViewRanges)
                        {
                            if((viewDirObjMin >= viewRange.first) && (viewDirObjMax <= viewRange.second))
                            {   // overlapped in the middle
                                rangeUpdated = true;
                                break;
                            }
                            else
                            {
                                if((viewDirObjMin < viewRange.first) && (viewDirObjMax >= viewRange.first))
                                {   // overlapped on the right
//                                    LOGINFO(QString().sprintf("AgentAdapter: update obstacleViewRange min from %f (%f - %f) to %f (%f - %f)",
//                                                (viewRange.second - viewRange.first), viewRange.first, viewRange.second,
//                                                (viewRange.second - viewDirObjMin), viewDirObjMin, viewRange.second).toStdString());
                                    viewRange.first = viewDirObjMin;
                                    rangeUpdated = true;
                                }

                                if((viewDirObjMin <= viewRange.second) && (viewDirObjMax > viewRange.second))
                                {   // overlapped on the left
//                                    LOGINFO(QString().sprintf("AgentAdapter: update obstacleViewRange max from %f (%f - %f) to %f (%f - %f)",
//                                                (viewRange.second - viewRange.first), viewRange.first, viewRange.second,
//                                                (viewDirObjMax - viewRange.first), viewRange.first, viewDirObjMax).toStdString());
                                    viewRange.second = viewDirObjMax;
                                    rangeUpdated = true;
                                }

                                if(rangeUpdated)
                                    break;
                            }
                        }
                        if(!rangeUpdated)
                        {
//                            LOGINFO(QString().sprintf("AgentAdapter: add new obstacleViewRange %f (%.10f - %.10f)",
//                                        (viewDirObjMax - viewDirObjMin), viewDirObjMin, viewDirObjMax).toStdString());
                            obstacleViewRanges.push_back(std::pair<double, double>(viewDirObjMin, viewDirObjMax));
                        }

                    }
                }
            }
            prevPoint = (PCM_Point *)point;
        }
    }
    return true;
}

double AgentAdapter::GetVisibilityToNearestAgentInViewRange(double mainViewDirection, double range) const
{
    int egoId = GetAgentId();
    const PCM_Data *data = worldPCM->GetPCM_Data();
    const PCM_Point agentPoint(-1, GetPositionX(), GetPositionY(), 0);
    double viewAngle = GetAbsoluteViewAngle(mainViewDirection);

    /** @addtogroup sim_step_ac_2
     * Find the nearest agent.
    */
    AgentDetection agentDet = data->GetNearestAgent(egoId, &agentPoint, viewAngle, range);

    if(agentDet.oppId<0) // opponent is not found
        return 0;

    /** @addtogroup sim_step_ac_2
     * Calculate the full view range of opponent from ego's perspective, regardless of ego's view range.
    */
    double viewDirOppFullMin = GetViewDirectionToPoint(agentDet.fullOppLineSegment.GetFirstPoint());
    double viewDirOppFullMax = GetViewDirectionToPoint(agentDet.fullOppLineSegment.GetSecondPoint());
    if(viewDirOppFullMin > viewDirOppFullMax)
        std::swap(viewDirOppFullMin, viewDirOppFullMax);

    /** @addtogroup sim_step_ac_2
     * Calculate the actual view range of opponent from ego's perspective, which might be limited by ego's view range.
    */
    double viewDirOppSubMin = GetViewDirectionToPoint(agentDet.subOppLineSegment.GetFirstPoint());
    double viewDirOppSubMax = GetViewDirectionToPoint(agentDet.subOppLineSegment.GetSecondPoint());
    if(viewDirOppSubMin > viewDirOppSubMax)
        std::swap(viewDirOppSubMin, viewDirOppSubMax);

    double viewDirMin = viewDirOppSubMin, viewDirMax = viewDirOppSubMax;

    /** @addtogroup sim_step_ac_2
     * Calculate the view ranges of obstacles (if any) between ego and the opponent, including both objects and view objects.
    */
    double viewDirOpp = (viewDirMin + viewDirMax)/2.0;
    double viewRange = viewDirMax - viewDirMin;
    std::vector<std::pair<double, double>> obstacleViewRanges;
    GetObstacleViewRanges(viewDirOpp, viewRange, agentDet.distance, data->GetObject()->GetLineMap(),     obstacleViewRanges);
    GetObstacleViewRanges(viewDirOpp, viewRange, agentDet.distance, data->GetViewObject()->GetLineMap(), obstacleViewRanges);

    LOGINFO(QString().sprintf("AgentAdapter: agent %d sees agent %d with view-range %f (%.2f - %.2f) out of %f (%.2f - %.2f) and %d obstacleViewRanges",
                              egoId, agentDet.oppId, viewRange, viewDirMin, viewDirMax, (viewDirOppFullMax - viewDirOppFullMin),
                              viewDirOppFullMin, viewDirOppFullMax, obstacleViewRanges.size()).toStdString());

    /** @addtogroup sim_step_ac_2
     * Reduce the actual view range of opponent with the ranges of obstacles between ego and opponent. This may have 4 situations:
     *  - opponent is fully covered by the object.
     *  - object is at the right edge of ego's view range to opponent.
     *  - object is at the left edge of ego's view range to opponent.
     *  - object is inside ego's view range to opponent
    */
    double viewDirObjMin, viewDirObjMax;
    for (auto& objViewRange : obstacleViewRanges)
    {
        viewDirObjMin = objViewRange.first;
        viewDirObjMax = objViewRange.second;

        LOGINFO(QString().sprintf("AgentAdapter: agent %d sees agent %d with viewRange %f (%.15f - %.15f) and an obstacleViewRange at (%.15f - %.15f)",
                                  egoId, agentDet.oppId, viewRange, viewDirMin, viewDirMax, viewDirObjMin, viewDirObjMax).toStdString());

        if((viewDirObjMin <= viewDirMin) && (viewDirObjMax >= viewDirMax))
        {
            // opponent is fully covered by the object
            viewRange = 0.0;
            break;
        }
        else if((viewDirObjMin <= viewDirMin) && (viewDirObjMax > viewDirMin))
        {   // object is at the right edge of ego's view range to opponent
            LOGINFO(QString().sprintf("AgentAdapter: agent %d sees agent %d with view range (%f - %f) and an obstacle on the right edge at (%f - %f)",
                                          egoId, agentDet.oppId, viewDirMin, viewDirMax, viewDirObjMin, viewDirObjMax).toStdString());
            viewRange -= (viewDirObjMax - viewDirMin);
            viewDirMin = viewDirObjMax; // reduce the right (min) edge of view range
        }
        else if((viewDirObjMax >= viewDirMax) && (viewDirObjMin < viewDirMax))
        {   // object is at the left edge of ego's view range to opponent
            LOGINFO(QString().sprintf("AgentAdapter: agent %d sees agent %d with view range (%.2f - %.2f) and an obstacle on the left edge at (%f - %f)",
                                          egoId, agentDet.oppId, viewDirMin, viewDirMax, viewDirObjMin, viewDirObjMax).toStdString());
            viewRange -= (viewDirMax - viewDirObjMin);
            viewDirMax = viewDirObjMin; // reduce the left (max) edge of view range
        }
        else if((viewDirObjMin > viewDirMin) && (viewDirObjMax < viewDirMax))
        {   // object is inside ego's view range to opponent
            LOGINFO(QString().sprintf("AgentAdapter: agent %d sees agent %d with view range (%f - %f) and an obstacle in the middle at (%f - %f)",
                                          egoId, agentDet.oppId, viewDirMin, viewDirMax, viewDirObjMin, viewDirObjMax).toStdString());
            viewRange -= (viewDirObjMax - viewDirObjMin);
        }
    }

    /** @addtogroup sim_step_ac_2
     * Finally, the visibility is calculated as (the reduced view range / full view range of opponent )
    */
    double visibility = viewRange / (viewDirOppFullMax - viewDirOppFullMin);

//    if(agentDet.oppId >0)
//        LOGINFO(QString().sprintf("AgentAdapter: agent %d sees agent %d in %.2fm with visibility of %.2f (%.2f - %.2f) at direction of (%.2f # %.2f)",
//                                egoId, agentDet.oppId, agentDet.distance, visibility, viewDirMin, viewDirMax, mainViewDirection, range).toStdString());
    return visibility;
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
    return worldPCM->GetTrajectory(GetAgentId())->GetUVelVec();
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
    double viewAngle = INFINITY;
    if (!std::isinf(mainViewDirection))
    {
        double agentAngle = GetYawAngle();
        viewAngle = agentAngle + mainViewDirection;

        viewAngle = CommonHelper::ConvertAngleToPi(viewAngle);
    }

    return viewAngle;
}
