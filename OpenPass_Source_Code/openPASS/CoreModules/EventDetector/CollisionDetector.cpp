/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  CollisionDetector.cpp */
//-----------------------------------------------------------------------------

#include "CollisionDetector.h"


CollisionDetector::CollisionDetector(WorldInterface *world,
                                     ParameterInterface *parameters,
                                     SimulationSlave::EventNetworkInterface *eventNetwork,
                                     const CallbackInterface *callbacks,
                                     StochasticsInterface *stochastics):
    EventDetectorCommonBase(
        world,
        parameters,
        eventNetwork,
        callbacks,
        stochastics)
{
    cycleTime = 100;
    eventType = EventDefinitions::EventType::Collision;

    trafficObjects = &(world->GetTrafficObjects());
    agents = &(world->GetAgents());
}

// calculate agent geometry for a specific position
void CollisionDetector::CalculateWorldObjectGeometry(const WorldObjectInterface *worldObject,
                                                     const Common::Vector2d &position,
                                                     std::array<Common::Vector2d, 4> &resultCorners,
                                                     std::array<Common::Vector2d, 2> &resultNormals)
{
    double agentLength = worldObject->GetLength();
    double agentWidthHalf = worldObject->GetWidth() / 2;
    double agentDistanceLeadingEdge = worldObject->GetDistanceReferencePointToLeadingEdge();
    double agentAngle = worldObject->GetYaw();

    // upper left corner if angle == 0
    resultCorners[UpperLeft].x = agentDistanceLeadingEdge - agentLength;
    resultCorners[UpperLeft].y = agentWidthHalf;

    // upper right corner if angle == 0
    resultCorners[UpperRight].x = agentDistanceLeadingEdge;
    resultCorners[UpperRight].y = agentWidthHalf;

    // lower right corner if angle == 0
    resultCorners[LowerRight].x = agentDistanceLeadingEdge;
    resultCorners[LowerRight].y = -agentWidthHalf;

    // lower left corner if angle == 0
    resultCorners[LowerLeft].x = agentDistanceLeadingEdge - agentLength;
    resultCorners[LowerLeft].y = -agentWidthHalf;

    // transform corners
    for(int i = 0; i < NumberCorners; ++i)
    {
        resultCorners[i].Rotate(agentAngle);
        resultCorners[i].Translate(position.x, position.y);
    }

    resultNormals[Right] = resultCorners[LowerRight] - resultCorners[LowerLeft]; // pointing right if angle == 0
    resultNormals[Up] = resultCorners[UpperLeft] - resultCorners[LowerLeft]; // pointing up if angle == 0
}

void CollisionDetector::GetWorldObjectGeometry(const WorldObjectInterface *worldObject,
                                               std::array<Common::Vector2d, 4> &resultCorners,
                                               std::array<Common::Vector2d, 2> &resultNormals)
{
    Common::Vector2d agentPosition(worldObject->GetPositionX(), worldObject->GetPositionY());
    CalculateWorldObjectGeometry(worldObject, agentPosition, resultCorners, resultNormals);
}

void CollisionDetector::GetMinMax4(std::array<double, 4> &input,
                                   double &maxValue,
                                   double &minValue)
{
    maxValue = input[0];
    minValue = input[0];

    for(int index = 1; index < 4; ++index)
    {
        if(input[index] > maxValue)
        {
            maxValue = input[index];
        }

        if(input[index] < minValue)
        {
            minValue = input[index];
        }
    }
}

void CollisionDetector::GetMinMax2(std::array<double, 2> &input,
                                   double &maxValue,
                                   double &minValue)
{
    maxValue = std::max(input[0], input[1]);
    minValue = std::min(input[0], input[1]);
}

bool CollisionDetector::CalculateDistOnBorder(const WorldObjectInterface* worldObject,
                                              int corner,
                                              double cornerDistance,
                                              double &result)
{
    double distance = 0;

    if(UpperLeft < corner)
    {
        distance += worldObject->GetLength();
    }
    else
    {
        distance += cornerDistance * worldObject->GetLength();
        result = distance;
        return true;
    }

    if(UpperRight < corner)
    {
        distance += worldObject->GetWidth();
    }
    else
    {
        distance += cornerDistance * worldObject->GetWidth();
        result = distance;
        return true;
    }

    if(LowerRight < corner)
    {
        distance += worldObject->GetLength();
    }
    else
    {
        distance += cornerDistance * worldObject->GetLength();
        result = distance;
        return true;
    }

    assert(LowerLeft >= corner);

    distance += cornerDistance * worldObject->GetWidth();
    result = distance;
    return true;
}

// intersectionPoints tuple elements: intersection occurred on edge, scaled distance from previous corner, edge direction, distance from initial position to intersection
bool CollisionDetector::CalculateIntersectionPoints(const WorldObjectInterface *agent,
                                                    const WorldObjectInterface* other,
                                                    bool &intersected,
                                                    Common::Vector2d &intersectionPoint,
                                                    double &otherDistanceFromCorner,
                                                    int &otherFromCorner,
                                                    double &agentDistanceFromInitial,
                                                    Common::Vector2d &agentReferencePoint,
                                                    Common::Vector2d &otherReferencePoint,
                                                    int &agentPenetratingCorner)
{
    // reduce problem of two moving agents into problem of one moving agent:
    // add translation of other agent relative to translation of first agent
    // and assume no movement of other agent
    Common::Vector2d otherVelocityLongitudinal(other->GetVelocity(), 0);
    Common::Vector2d otherVelocityLateral(0, 0);
    double otherYawAngle = other->GetYaw();
    otherVelocityLongitudinal.Rotate(otherYawAngle);
    otherVelocityLateral.Rotate(otherYawAngle);
    Common::Vector2d otherVelocity = otherVelocityLongitudinal + otherVelocityLateral;

    Common::Vector2d agentVelocityLongitudinal(agent->GetVelocity(), 0);
    Common::Vector2d agentVelocityLateral(0, 0);
    double agentYawAngle = agent->GetYaw();
    agentVelocityLongitudinal.Rotate(agentYawAngle);
    agentVelocityLateral.Rotate(agentYawAngle);
    Common::Vector2d agentVelocity = agentVelocityLongitudinal + agentVelocityLateral;
    Common::Vector2d agentRelativeVelocity = agentVelocity;

    if(0 == otherVelocity.x && 0 == otherVelocity.y && 0 == agentRelativeVelocity.x && 0 == agentRelativeVelocity.y)
    {
        LOG(CbkLogLevel::Warning, + "calculation of point of contact not possible since velocities of both agents are 0"
                                    " (this could happen for example if agents are spawned with overlaps)");
        return false;
    }

    agentRelativeVelocity.Sub(otherVelocity);

    if(agentRelativeVelocity.Length() == 0){
        LOG(CbkLogLevel::Warning, + "calculation of point of contact not possible since velocities of both agents are the same"
                                    " (this could happen for example if agents are spawned with overlaps)");
        return false;
    }

    // agentRelativeVelocity now represents the normed moving direction of the first
    // agent to relative the second agent.

    // calculate points of intersection (moving first agent only)

    // calculate (arbitrary) initial position where still no collision occurs
    double otherLength = other->GetLength();
    double otherWidth = other->GetWidth();
    double agentLength = agent->GetLength();
    double agentWidth = agent->GetWidth();
    double distanceLength = otherLength + otherWidth + agentLength + agentWidth;
    double distanceTime = distanceLength / agentRelativeVelocity.Length();

    Common::Vector2d otherInitialPosition(other->GetPositionX(), other->GetPositionY());
    Common::Vector2d agentPosition(agent->GetPositionX(), agent->GetPositionY());
    Common::Vector2d agentInitialPosition = agentPosition - agentRelativeVelocity * distanceTime;
    // otherInitialPosition and agentInitialPosition now represent the initial starting
    // position prior to collision
    // agentPosition now represents the position at the time of collision detection

    std::array<Common::Vector2d, NumberCorners> otherInitialCorners;
    std::array<Common::Vector2d, NumberNormals> otherInitialNormals;
    CalculateWorldObjectGeometry(other, otherInitialPosition, otherInitialCorners, otherInitialNormals);

    std::array<Common::Vector2d, NumberCorners> agentInitialCorners;
    std::array<Common::Vector2d, NumberNormals> agentInitialNormals;
    CalculateWorldObjectGeometry(agent, agentInitialPosition, agentInitialCorners, agentInitialNormals);

    /*
     * calculation of intersection point
     * trajectory line of first agent = (ax, ay) * s + (bx, by)
     * edge of second other agent = (cx, cy) * t + (dx, dy)
     *
     * |ax   -cx| * |s| = |dx - bx|
     * |ay   -cy| * |t| = |dy - by|
     * -> rule of cramer:
     * det = cx*ay - cy*ax
     * s = ((dy-by)*cx - (dx-bx)*cy) / det
     * t = ((dy-by)*ax - (dx-bx)*ay) / det
     *
     * ax, ay: agentDirection.x/.y
     * bx, by: agentInitialCorners[].x/.y
     * cx, cy: otherEdgeDirection.x/.y
     * dx, dy: otherInitialCorners[].x/.y
     */
    Common::Vector2d agentDirection = agentPosition - agentInitialPosition;
    Common::Vector2d otherEdgeDirection;
    double det;

    intersected = false;
    agentDistanceFromInitial = std::numeric_limits<double>::max();

    for(int corner = 0; corner < NumberCorners; ++corner) // for each corner in agentInitialCorners[]
    {
        // intersection point with upper edge
        otherEdgeDirection = otherInitialCorners[UpperRight] - otherInitialCorners[UpperLeft];
        det = otherEdgeDirection.x * agentDirection.y - otherEdgeDirection.y * agentDirection.x;
        if(0 != det)
        {
            double s = ((otherInitialCorners[UpperLeft].y - agentInitialCorners[corner].y) * otherEdgeDirection.x -
                        (otherInitialCorners[UpperLeft].x - agentInitialCorners[corner].x) * otherEdgeDirection.y) / det;
            double t = ((otherInitialCorners[UpperLeft].y - agentInitialCorners[corner].y) * agentDirection.x -
                        (otherInitialCorners[UpperLeft].x - agentInitialCorners[corner].x) * agentDirection.y) / det;
            if(0.0 <= s && 1.0 >= s && 0.0 <= t && 1.0 >= t) // within bounds of edge
            {
                double tmpDistance = (agentDirection * s).Length();
                if(agentDistanceFromInitial > tmpDistance)
                {
                    intersected = true;
                    agentDistanceFromInitial = tmpDistance;
                    agentPenetratingCorner = corner;
                    otherDistanceFromCorner = t;
                    otherFromCorner = UpperLeft;

                    // calculate untransformed point of impact
                    double fraction = (agentDirection.Length() - tmpDistance) / agentDirection.Length();
                    agentReferencePoint = agentPosition - agentVelocity * distanceTime * fraction;
                    otherReferencePoint = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateWorldObjectGeometry(other, otherReferencePoint, otherIntersectionCorners, otherIntersectionNormals);
                    intersectionPoint = otherIntersectionCorners[UpperLeft] + otherEdgeDirection * t; // yaw angle is kept constant
                }
            }
        }

        // intersection point with right edge
        otherEdgeDirection = otherInitialCorners[LowerRight] - otherInitialCorners[UpperRight];
        det = otherEdgeDirection.x * agentDirection.y - otherEdgeDirection.y * agentDirection.x;
        if(0 != det)
        {
            double s = ((otherInitialCorners[UpperRight].y - agentInitialCorners[corner].y) * otherEdgeDirection.x -
                        (otherInitialCorners[UpperRight].x - agentInitialCorners[corner].x) * otherEdgeDirection.y) / det;
            double t = ((otherInitialCorners[UpperRight].y - agentInitialCorners[corner].y) * agentDirection.x -
                        (otherInitialCorners[UpperRight].x - agentInitialCorners[corner].x) * agentDirection.y) / det;
            if(0.0 <= s && 1.0 >= s && 0.0 <= t && 1.0 >= t) // within bounds of edge
            {
                double tmpDistance = (agentDirection * s).Length();
                if(agentDistanceFromInitial > tmpDistance)
                {
                    intersected = true;
                    agentDistanceFromInitial = tmpDistance;
                    agentPenetratingCorner = corner;
                    otherDistanceFromCorner = t;
                    otherFromCorner = UpperRight;

                    // calculate untransformed point of impact
                    double fraction = (agentDirection.Length() - tmpDistance) / agentDirection.Length();
                    agentReferencePoint = agentPosition - agentVelocity * distanceTime * fraction;
                    otherReferencePoint = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateWorldObjectGeometry(other, otherReferencePoint, otherIntersectionCorners, otherIntersectionNormals);
                    intersectionPoint = otherIntersectionCorners[UpperRight] + otherEdgeDirection * t; // yaw angle is kept constant
                }
            }
        }

        // intersection point with lower edge
        otherEdgeDirection = otherInitialCorners[LowerLeft] - otherInitialCorners[LowerRight];
        det = otherEdgeDirection.x * agentDirection.y - otherEdgeDirection.y * agentDirection.x;
        if(0 != det)
        {
            double s = ((otherInitialCorners[LowerRight].y - agentInitialCorners[corner].y) * otherEdgeDirection.x -
                        (otherInitialCorners[LowerRight].x - agentInitialCorners[corner].x) * otherEdgeDirection.y) / det;
            double t = ((otherInitialCorners[LowerRight].y - agentInitialCorners[corner].y) * agentDirection.x -
                        (otherInitialCorners[LowerRight].x - agentInitialCorners[corner].x) * agentDirection.y) / det;
            if(0.0 <= s && 1.0 >= s && 0.0 <= t && 1.0 >= t) // within bounds of edge
            {
                double tmpDistance = (agentDirection * s).Length();
                if(agentDistanceFromInitial > tmpDistance)
                {
                    intersected = true;
                    agentDistanceFromInitial = tmpDistance;
                    agentPenetratingCorner = corner;
                    otherDistanceFromCorner = t;
                    otherFromCorner = LowerRight;

                    // calculate untransformed point of impact
                    double fraction = (agentDirection.Length() - tmpDistance) / agentDirection.Length();
                    agentReferencePoint = agentPosition - agentVelocity * distanceTime * fraction;
                    otherReferencePoint = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateWorldObjectGeometry(other, otherReferencePoint, otherIntersectionCorners, otherIntersectionNormals);
                    intersectionPoint = otherIntersectionCorners[LowerRight] + otherEdgeDirection * t; // yaw angle is kept constant
                }
            }
        }

        // intersection point with left edge
        otherEdgeDirection = otherInitialCorners[UpperLeft] - otherInitialCorners[LowerLeft];
        det = otherEdgeDirection.x * agentDirection.y - otherEdgeDirection.y * agentDirection.x;
        if(0 != det)
        {
            double s = ((otherInitialCorners[LowerLeft].y - agentInitialCorners[corner].y) * otherEdgeDirection.x -
                        (otherInitialCorners[LowerLeft].x - agentInitialCorners[corner].x) * otherEdgeDirection.y) / det;
            double t = ((otherInitialCorners[LowerLeft].y - agentInitialCorners[corner].y) * agentDirection.x -
                        (otherInitialCorners[LowerLeft].x - agentInitialCorners[corner].x) * agentDirection.y) / det;
            if(0.0 <= s && 1.0 >= s && 0.0 <= t && 1.0 >= t) // within bounds of edge
            {
                double tmpDistance = (agentDirection * s).Length();
                if(agentDistanceFromInitial > tmpDistance)
                {
                    intersected = true;
                    agentDistanceFromInitial = tmpDistance;
                    agentPenetratingCorner = corner;
                    otherDistanceFromCorner = t;
                    otherFromCorner = LowerLeft;

                    // calculate untransformed point of impact
                    double fraction = (agentDirection.Length() - tmpDistance) / agentDirection.Length();
                    agentReferencePoint = agentPosition - agentVelocity * distanceTime * fraction;
                    otherReferencePoint = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateWorldObjectGeometry(other, otherReferencePoint, otherIntersectionCorners, otherIntersectionNormals);
                    intersectionPoint = otherIntersectionCorners[LowerLeft] + otherEdgeDirection * t; // yaw angle is kept constant
                }
            }
        }
    } // for each corner

    return true;
}

bool CollisionDetector::CalculatePointOfContact(AgentInterface *agent,
                                                const WorldObjectInterface* other,
                                                double &resultAgentDistOnBorder,
                                                double &resultOtherDistOnBorder,
                                                Common::Vector2d &resultAgentReferencePoint,
                                                Common::Vector2d &resultOtherReferencePoint)
{
    bool agentIntersected; // is agent intersected at all
    Common::Vector2d agentIntersectionPoint; // coordinates of intersection point
    double agentDistanceFromCorner; // distance from previous corner of intersected edge
    int agentFromCorner; // starting corner for measuring distance on edge
    double otherDistanceFromInitial; // distance of other moving agent from it's non-colliding start position
    int otherPenetratingCorner; // corner of other agent which penetrated first
    Common::Vector2d movingOtherReferencePoint; // ReferencePoint of agent at point of collision
    Common::Vector2d penetratedAgentReferencePoint; // ReferencePoint of other at point of collision

    if(!CalculateIntersectionPoints(other,
                                    agent,
                                    agentIntersected,
                                    agentIntersectionPoint,
                                    agentDistanceFromCorner,
                                    agentFromCorner,
                                    otherDistanceFromInitial,
                                    movingOtherReferencePoint,
                                    penetratedAgentReferencePoint,
                                    otherPenetratingCorner))
    {
        return false;
    }

    bool otherIntersected;
    Common::Vector2d otherIntersectionPoint;
    double otherDistanceFromCorner;
    int otherFromCorner;
    double agentDistanceFromInitial;
    int agentPenetratingCorner;
    Common::Vector2d movingAgentReferencePoint;
    Common::Vector2d penetratedOtherReferencePoint;

    if(!CalculateIntersectionPoints(agent,
                                    other,
                                    otherIntersected,
                                    otherIntersectionPoint,
                                    otherDistanceFromCorner,
                                    otherFromCorner,
                                    agentDistanceFromInitial,
                                    movingAgentReferencePoint,
                                    penetratedOtherReferencePoint,
                                    agentPenetratingCorner))
    {
        return false;
    }

    if(agentIntersected==0 && otherIntersected==0){
        assert(agentIntersected || otherIntersected);
    }

    if(agentIntersected && !otherIntersected)
    {
        if(!CalculateDistOnBorder(agent, agentFromCorner, agentDistanceFromCorner, resultAgentDistOnBorder))
        {
            return false;
        }

        if(!CalculateDistOnBorder(other, otherPenetratingCorner, 0, resultOtherDistOnBorder))
        {
            return false;
        }

        resultAgentReferencePoint = penetratedAgentReferencePoint;
        resultOtherReferencePoint = movingOtherReferencePoint;
    }
    else if(!agentIntersected && otherIntersected)
    {
        if(!CalculateDistOnBorder(agent, agentPenetratingCorner, 0, resultAgentDistOnBorder))
        {
            return false;
        }

        if(!CalculateDistOnBorder(other, otherFromCorner, otherDistanceFromCorner, resultOtherDistOnBorder))
        {
            return false;
        }

        resultAgentReferencePoint = movingAgentReferencePoint;
        resultOtherReferencePoint = penetratedOtherReferencePoint;
    }
    else
    {
        // decide which agent entered first into body of other agent
        if(agentDistanceFromInitial < otherDistanceFromInitial)
        {
            // agent corner penetrated first into body of other
            if(!CalculateDistOnBorder(agent, agentPenetratingCorner, 0, resultAgentDistOnBorder))
            {
                return false;
            }

            if(!CalculateDistOnBorder(other, otherFromCorner, otherDistanceFromCorner, resultOtherDistOnBorder))
            {
                return false;
            }

            resultAgentReferencePoint = movingAgentReferencePoint;
            resultOtherReferencePoint = penetratedOtherReferencePoint;
        }
        else
        {
            // other corner penetrated first into body of agent
            if(!CalculateDistOnBorder(agent, agentFromCorner, agentDistanceFromCorner, resultAgentDistOnBorder))
            {
                return false;
            }

            if(!CalculateDistOnBorder(other, otherPenetratingCorner, 0, resultOtherDistOnBorder))
            {
                return false;
            }

            resultAgentReferencePoint = penetratedAgentReferencePoint;
            resultOtherReferencePoint = movingOtherReferencePoint;
        }
    }

    return true;
}

void CollisionDetector::Trigger(int time)
{
    // accumulate collisions
    for(auto it = agents->cbegin(); it != agents->cend(); ++it)
    {
        AgentInterface *agent = it->second;
        assert(agent != nullptr);

        // calculate agent corners and normals
        std::array<Common::Vector2d, NumberCorners> agentCorners;
        std::array<Common::Vector2d, NumberNormals> agentNormals;
        GetWorldObjectGeometry(agent, agentCorners, agentNormals);

        for(auto otherIt = std::next(it); otherIt != agents->cend(); ++otherIt)
        {
            AgentInterface *other = otherIt->second;
            if(!DetectCollision(other, agent, agentCorners, agentNormals))
            {
                continue;
            }
            else
            {
                // no separations given on any axis -> collision
                DetectedCollisionWithAgent(time, agent, other);
            }
        }

        // second loop to avoid comparing traffic objects with traffic objects
        for(auto trafficObjectIt = trafficObjects->begin(); trafficObjectIt != trafficObjects->end(); ++trafficObjectIt)
        {
            const TrafficObjectInterface* otherObject = *trafficObjectIt;
            if(!otherObject)
            {
                LOG(CbkLogLevel::Warning, "collision detection aborted");
                throw std::runtime_error("Invalid other worldObject. Collision detection cancled.");
            }
            if(!DetectCollision(otherObject, agent, agentCorners, agentNormals))
            {
                continue;
            }
            else
            {
                DetectedCollisionWithObject(time, agent, otherObject);
            }
        }
    }
}

template <typename T>
bool IsInVector(const std::vector<T>& v, T element)
{
    return std::find(v.begin(), v.end(), element) != v.end();
}

bool CollisionDetector::DetectCollision(const WorldObjectInterface *other,
                                           AgentInterface *agent,
                                           std::array<Common::Vector2d,
                                           NumberCorners> agentCorners,
                                           std::array<Common::Vector2d,
                                           NumberNormals> agentNormals)
{
    if( IsInVector(agent->GetCollisionPartners(), std::make_pair(other->GetType(), other->GetId())) )
    {
        return false;
    }

    // calculate other corners and normals
    std::array<Common::Vector2d, NumberCorners> otherCorners;
    std::array<Common::Vector2d, NumberNormals> otherNormals;
    GetWorldObjectGeometry(other, otherCorners, otherNormals);

    // quick check
    double quickDistance = agent->GetLength() + agent->GetWidth() + other->GetLength() + other->GetWidth();
    if(fabs(agentCorners[UpperLeft].x - otherCorners[UpperLeft].x) > quickDistance ||
            fabs(agentCorners[UpperLeft].y - otherCorners[UpperLeft].y) > quickDistance)
    {
        return false;
    }

    return DetectIntersectionOfTwoWorldObjects(agent, agentNormals, agentCorners, other, otherNormals, otherCorners);
}

bool CollisionDetector::DetectIntersectionOfTwoWorldObjects(const WorldObjectInterface *agent,
                                                            std::array<Common::Vector2d, NumberNormals> agentNormals,
                                                            std::array<Common::Vector2d, NumberCorners> agentCorners,
                                                            const WorldObjectInterface *other,
                                                            std::array<Common::Vector2d, NumberNormals> otherNormals,
                                                            std::array<Common::Vector2d, NumberCorners> otherCorners)
{
    double agentNormal1_agentCornerProjectedMinValue;
    double agentNormal1_agentCornerProjectedMaxValue;

    double agentNormal0_agentCornerProjectedMinValue;
    double agentNormal0_agentCornerProjectedMaxValue;

    // only two points need to be projected on own normals since they are aligned
    std::array<double, NumberProjectedOwnAxis> agentNormal0_agentCornerProjected;
    std::array<double, NumberProjectedOwnAxis> agentNormal1_agentCornerProjected;

    bool calculatedNormal0 = false;
    bool calculatedNormal1 = false;

    // project agent corners on agent normal0 (take origin as reference, unnormalized since only comparision result is significant)
    if(!calculatedNormal0)
    {
        agentNormal0_agentCornerProjected[ProjectedFirst] = agentCorners[LowerLeft].Dot(agentNormals[Right]);
        agentNormal0_agentCornerProjected[ProjectedSecond] = agentCorners[LowerRight].Dot(agentNormals[Right]); // right normal
        GetMinMax2(agentNormal0_agentCornerProjected,
                   agentNormal0_agentCornerProjectedMaxValue,
                   agentNormal0_agentCornerProjectedMinValue);
        calculatedNormal0 = true;
    }

    // project other corners on agent normal0 (take origin as reference, unnormalized since only comparision result is significant)
    std::array<double, NumberProjectedOpponentAxis> agentNormal0_otherCornerProjected;
    for(int i = 0; i < NumberProjectedOpponentAxis; ++i)
    {
        agentNormal0_otherCornerProjected[i] = otherCorners[i].Dot(agentNormals[Right]);
    }

    double agentNormal0_otherCornerProjectedMaxValue;
    double agentNormal0_otherCornerProjectedMinValue;
    GetMinMax4(agentNormal0_otherCornerProjected,
               agentNormal0_otherCornerProjectedMaxValue,
               agentNormal0_otherCornerProjectedMinValue);

    // check for collision on agent normal 0
    if(agentNormal0_agentCornerProjectedMaxValue < agentNormal0_otherCornerProjectedMinValue ||
            agentNormal0_otherCornerProjectedMaxValue < agentNormal0_agentCornerProjectedMinValue)
    {
        // agents separated on agent normal 0 axis
        return false;
    }

    // project agent corners on agent normal1 (take origin as reference, unnormalized since only comparision result is significant)
    if(!calculatedNormal1)
    {
        agentNormal1_agentCornerProjected[ProjectedFirst] = agentCorners[LowerLeft].Dot(agentNormals[Up]);
        agentNormal1_agentCornerProjected[ProjectedSecond] = agentCorners[UpperLeft].Dot(agentNormals[Up]); // up normal
        GetMinMax2(agentNormal1_agentCornerProjected,
                   agentNormal1_agentCornerProjectedMaxValue,
                   agentNormal1_agentCornerProjectedMinValue);
        calculatedNormal1 = true;
    }

    // project other corners  on agent normal1 (take origin as reference, unnormalized since only comparision result is significant)
    std::array<double, NumberProjectedOpponentAxis> agentNormal1_otherCornerProjected;
    for(int i = 0; i < NumberProjectedOpponentAxis; ++i)
    {
        agentNormal1_otherCornerProjected[i] = otherCorners[i].Dot(agentNormals[Up]);
    }

    double agentNormal1_otherCornerProjectedMaxValue;
    double agentNormal1_otherCornerProjectedMinValue;
    GetMinMax4(agentNormal1_otherCornerProjected,
               agentNormal1_otherCornerProjectedMaxValue,
               agentNormal1_otherCornerProjectedMinValue);

    // check for collision on agent normal 1
    if(agentNormal1_agentCornerProjectedMaxValue < agentNormal1_otherCornerProjectedMinValue ||
            agentNormal1_otherCornerProjectedMaxValue < agentNormal1_agentCornerProjectedMinValue)
    {
        // agents separated on agent normal 1 axis
        return false;
    }

    // skip check for other normals if agents are approximately aligned to same axes
    if(fabs(fmod(fabs(agent->GetYaw()), 90.0) - fmod(fabs(other->GetYaw()), 90.0)) > ROTATION_EPS)
    {
        // project agent corners on other normal0 (take origin as reference, unnormalized since only comparision result is significant)
        std::array<double, NumberProjectedOpponentAxis> otherNormal0_agentCornerProjected;
        for(int i = 0; i < NumberProjectedOpponentAxis; ++i)
        {
            otherNormal0_agentCornerProjected[i] = agentCorners[i].Dot(otherNormals[Right]);
        }

        double otherNormal0_agentCornerProjectedMaxValue;
        double otherNormal0_agentCornerProjectedMinValue;
        GetMinMax4(otherNormal0_agentCornerProjected,
                   otherNormal0_agentCornerProjectedMaxValue,
                   otherNormal0_agentCornerProjectedMinValue);

        // project other corners on other normal0 (take origin as reference, unnormalized since only comparision result is significant)
        std::array<double, NumberProjectedOwnAxis> otherNormal0_otherCornerProjected; // only two points need to be projected on own normals since they are aligned
        otherNormal0_otherCornerProjected[ProjectedFirst] = otherCorners[LowerLeft].Dot(otherNormals[Right]); // right normal
        otherNormal0_otherCornerProjected[ProjectedSecond] = otherCorners[LowerRight].Dot(otherNormals[Right]);

        double otherNormal0_otherCornerProjectedMaxValue;
        double otherNormal0_otherCornerProjectedMinValue;
        GetMinMax2(otherNormal0_otherCornerProjected,
                   otherNormal0_otherCornerProjectedMaxValue,
                   otherNormal0_otherCornerProjectedMinValue);

        // check for collision on other normal 0
        if(otherNormal0_agentCornerProjectedMaxValue < otherNormal0_otherCornerProjectedMinValue ||
                otherNormal0_otherCornerProjectedMaxValue < otherNormal0_agentCornerProjectedMinValue)
        {
            // agents separated on other normal 0 axis
            return false;
        }

        // project agent corners on other normal1 (take origin as reference, unnormalized since only comparision result is significant)
        std::array<double, NumberProjectedOpponentAxis> otherNormal1_agentCornerProjected;
        for(int i = 0; i < NumberProjectedOpponentAxis; ++i)
        {
            otherNormal1_agentCornerProjected[i] = agentCorners[i].Dot(otherNormals[Up]);
        }

        double otherNormal1_agentCornerProjectedMaxValue;
        double otherNormal1_agentCornerProjectedMinValue;
        GetMinMax4(otherNormal1_agentCornerProjected,
                   otherNormal1_agentCornerProjectedMaxValue,
                   otherNormal1_agentCornerProjectedMinValue);

        // project other corners on other normal1 (take origin as reference, unnormalized since only comparision result is significant)
        std::array<double, NumberProjectedOwnAxis> otherNormal1_otherCornerProjected; // only two points need to be projected on own normals since they are aligned
        otherNormal1_otherCornerProjected[ProjectedFirst] = otherCorners[LowerLeft].Dot(otherNormals[Up]); // up normal
        otherNormal1_otherCornerProjected[ProjectedSecond] = otherCorners[UpperLeft].Dot(otherNormals[Up]);

        double otherNormal1_otherCornerProjectedMaxValue;
        double otherNormal1_otherCornerProjectedMinValue;
        GetMinMax2(otherNormal1_otherCornerProjected,
                   otherNormal1_otherCornerProjectedMaxValue,
                   otherNormal1_otherCornerProjectedMinValue);

        // check for collision on other normal 1
        if(otherNormal1_agentCornerProjectedMaxValue < otherNormal1_otherCornerProjectedMinValue ||
                otherNormal1_otherCornerProjectedMaxValue < otherNormal1_agentCornerProjectedMinValue)
        {
            // agents separated on other normal 1 axis
            return false;
        }

        return true;
    }
    else
    {
        return true;
    }
}

void CollisionDetector::DetectedCollisionWithObject(int time, AgentInterface *agent, const WorldObjectInterface *other)
{
    std::shared_ptr<CollisionEvent> event = std::make_shared<CollisionEvent>(time,
                                                                             COMPONENTNAME,
                                                                             sequenceName,
                                                                             eventType,
                                                                             false,
                                                                             agent->GetId(),
                                                                             other->GetId());

    eventNetwork->InsertEvent(event);
}

void CollisionDetector::DetectedCollisionWithAgent(int time, AgentInterface *agent, AgentInterface *other)
{
    std::shared_ptr<CollisionEvent> event = std::make_shared<CollisionEvent>(time,
                                                                             COMPONENTNAME,
                                                                             sequenceName,
                                                                             eventType,
                                                                             true,
                                                                             agent->GetId(),
                                                                             other->GetId());

    eventNetwork->InsertEvent(event);
}

