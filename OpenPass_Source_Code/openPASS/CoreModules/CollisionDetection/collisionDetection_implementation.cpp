/******************************************************************************
* Copyright (c) 2016 ITK Engineering AG.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <cassert>
#include <limits>
#include <cmath>
#include <array>

#include "collisionDetection_implementation.h"

namespace
{

typedef enum
{
    UpperLeft = 0,
    UpperRight,
    LowerRight,
    LowerLeft,
    NumberCorners
} CornerType;

typedef enum
{
    UpperEdge = 0,
    RightEdge,
    LowerEdge,
    LeftEdge,
    NumberEdges
} EdgeType;

typedef enum
{
    Right = 0,
    Up,
    NumberNormals
} NormalType;

typedef enum
{
    ProjectedFirst = 0,
    ProjectedSecond,
    NumberProjectedOwnAxis,
    ProjectedThird = 2,
    ProjectedFourth,
    NumberProjectedOpponentAxis
} ProjectedType;

static const double ROTATION_EPS = 0.0001;

} // namespace

void CollisionDetection_Implementation::SetAgents(const std::map<int, const AgentInterface*> &agents)
{
    this->agents = &agents;
}

// calculate agent geometry for a specific position
void CollisionDetection_Implementation::CalculateAgentGeometry(const AgentInterface *agent,
                                                const Common::Vector2d &position,
                                                std::array<Common::Vector2d, 4> &resultCorners,
                                                std::array<Common::Vector2d, 2> &resultNormals)
{


    double agentLength = agent->GetLength();
    double agentWidthHalf = agent->GetWidth() / 2;
    double agentDistanceCenter = agent->GetDistanceCOGtoLeadingEdge();
    double agentAngle = agent->GetYawAngle();

    // upper left corner if angle == 0
    resultCorners[UpperLeft].x = agentDistanceCenter - agentLength;
    resultCorners[UpperLeft].y = agentWidthHalf;

    // upper right corner if angle == 0
    resultCorners[UpperRight].x = agentDistanceCenter;
    resultCorners[UpperRight].y = agentWidthHalf;

    // lower right corner if angle == 0
    resultCorners[LowerRight].x = agentDistanceCenter;
    resultCorners[LowerRight].y = -agentWidthHalf;

    // lower left corner if angle == 0
    resultCorners[LowerLeft].x = agentDistanceCenter - agentLength;
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

void CollisionDetection_Implementation::GetAgentGeometry(const AgentInterface *agent,
                                          std::array<Common::Vector2d, 4> &resultCorners,
                                          std::array<Common::Vector2d, 2> &resultNormals)
{
    Common::Vector2d agentPosition(agent->GetPositionX(), agent->GetPositionY());
    CalculateAgentGeometry(agent, agentPosition, resultCorners, resultNormals);
}

void CollisionDetection_Implementation::GetMinMax4(std::array<double, 4> &input,
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

void CollisionDetection_Implementation::GetMinMax2(std::array<double, 2> &input,
                                    double &maxValue,
                                    double &minValue)
{
    if(input[0] > input[1])
    {
        maxValue = input[0];
    }
    else
    {
        maxValue = input[1];
    }

    if(input[1] < input[0])
    {
        minValue = input[1];
    }
    else
    {
        minValue = input[0];
    }
}

bool CollisionDetection_Implementation::CalculateDistOnBorder(const AgentInterface *agent,
                                               int corner,
                                               double cornerDistance,
                                               double &result)
{
    double distance = 0;

    if(UpperLeft < corner)
    {
        distance += agent->GetLength();
    }
    else
    {
        distance += cornerDistance * agent->GetLength();
        result = distance;
        return true;
    }

    if(UpperRight < corner)
    {
        distance += agent->GetWidth();
    }
    else
    {
        distance += cornerDistance * agent->GetWidth();
        result = distance;
        return true;
    }

    if(LowerRight < corner)
    {
        distance += agent->GetLength();
    }
    else
    {
        distance += cornerDistance * agent->GetLength();
        result = distance;
        return true;
    }

    assert(LowerLeft >= corner);

    distance += cornerDistance * agent->GetWidth();
    result = distance;
    return true;
}

// intersectionPoints tuple elements: intersection occurred on edge, scaled distance from previous corner, edge direction, distance from initial position to intersection
bool CollisionDetection_Implementation::CalculateIntersectionPoints(const AgentInterface *agent,
                                                     const AgentInterface *other,
                                                     bool &intersected,
                                                     Common::Vector2d &intersectionPoint,
                                                     double &otherDistanceFromCorner,
                                                     int &otherFromCorner,
                                                     double &agentDistanceFromInitial,
                                                     Common::Vector2d &agentCOG,
                                                     Common::Vector2d &otherCOG,
                                                     int &agentPenetratingCorner)
{
    // reduce problem of two moving agents into problem of one moving agent:
    // add translation of other agent relative to translation of first agent
    // and assume no movement of other agent
    Common::Vector2d otherVelocityX(other->GetVelocityX(), 0);
    Common::Vector2d otherVelocityY(0, other->GetVelocityY());
    double otherYawAngle = other->GetYawAngle();
    otherVelocityX.Rotate(otherYawAngle);
    otherVelocityY.Rotate(otherYawAngle);
    Common::Vector2d otherVelocity = otherVelocityX + otherVelocityY;

    Common::Vector2d agentVelocityX(agent->GetVelocityX(), 0);
    Common::Vector2d agentVelocityY(0, agent->GetVelocityY());
    double agentYawAngle = agent->GetYawAngle();
    agentVelocityX.Rotate(agentYawAngle);
    agentVelocityY.Rotate(agentYawAngle);
    Common::Vector2d agentVelocity = agentVelocityX + agentVelocityY;
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
    CalculateAgentGeometry(other, otherInitialPosition, otherInitialCorners, otherInitialNormals);

    std::array<Common::Vector2d, NumberCorners> agentInitialCorners;
    std::array<Common::Vector2d, NumberNormals> agentInitialNormals;
    CalculateAgentGeometry(agent, agentInitialPosition, agentInitialCorners, agentInitialNormals);

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
                    agentCOG = agentPosition - agentVelocity * distanceTime * fraction;
                    otherCOG = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateAgentGeometry(other, otherCOG, otherIntersectionCorners, otherIntersectionNormals);
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
                    agentCOG = agentPosition - agentVelocity * distanceTime * fraction;
                    otherCOG = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateAgentGeometry(other, otherCOG, otherIntersectionCorners, otherIntersectionNormals);
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
                    agentCOG = agentPosition - agentVelocity * distanceTime * fraction;
                    otherCOG = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateAgentGeometry(other, otherCOG, otherIntersectionCorners, otherIntersectionNormals);
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
                    agentCOG = agentPosition - agentVelocity * distanceTime * fraction;
                    otherCOG = otherInitialPosition - otherVelocity * distanceTime * fraction;

                    // calculate intersection point
                    std::array<Common::Vector2d, NumberCorners> otherIntersectionCorners;
                    std::array<Common::Vector2d, NumberNormals> otherIntersectionNormals;
                    CalculateAgentGeometry(other, otherCOG, otherIntersectionCorners, otherIntersectionNormals);
                    intersectionPoint = otherIntersectionCorners[LowerLeft] + otherEdgeDirection * t; // yaw angle is kept constant
                }
            }
        }
    } // for each corner

    return true;
}

bool CollisionDetection_Implementation::CalculatePointOfContact(const AgentInterface *agent,
                                                 const AgentInterface *other,
                                                 double &resultAgentDistOnBorder,
                                                 double &resultOtherDistOnBorder,
                                                 Common::Vector2d &resultAgentCOG,
                                                 Common::Vector2d &resultOtherCOG)
{
    bool agentIntersected; // is agent intersected at all
    Common::Vector2d agentIntersectionPoint; // coordinates of intersection point
    double agentDistanceFromCorner; // distance from previous corner of intersected edge
    int agentFromCorner; // starting corner for measuring distance on edge
    double otherDistanceFromInitial; // distance of other moving agent from it's non-colliding start position
    int otherPenetratingCorner; // corner of other agent which penetrated first
    Common::Vector2d movingOtherCOG; // COG of agent at point of collision
    Common::Vector2d penetratedAgentCOG; // COG of other at point of collision

    if(!CalculateIntersectionPoints(other,
                                    agent,
                                    agentIntersected,
                                    agentIntersectionPoint,
                                    agentDistanceFromCorner,
                                    agentFromCorner,
                                    otherDistanceFromInitial,
                                    movingOtherCOG,
                                    penetratedAgentCOG,
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
    Common::Vector2d movingAgentCOG;
    Common::Vector2d penetratedOtherCOG;

    if(!CalculateIntersectionPoints(agent,
                                    other,
                                    otherIntersected,
                                    otherIntersectionPoint,
                                    otherDistanceFromCorner,
                                    otherFromCorner,
                                    agentDistanceFromInitial,
                                    movingAgentCOG,
                                    penetratedOtherCOG,
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

        resultAgentCOG = penetratedAgentCOG;
        resultOtherCOG = movingOtherCOG;
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

        resultAgentCOG = movingAgentCOG;
        resultOtherCOG = penetratedOtherCOG;
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

            resultAgentCOG = movingAgentCOG;
            resultOtherCOG = penetratedOtherCOG;
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

            resultAgentCOG = penetratedAgentCOG;
            resultOtherCOG = movingOtherCOG;
        }
    }

    return true;
}

bool CollisionDetection_Implementation::CreateResult(const AgentInterface *agent,
                                      const AgentInterface *other,
                                      RunResultInterface &runResult)
{
    double resultAgentDistOnBorder;
    double resultOtherDistOnBorder;
    Common::Vector2d resultAgentCOG;
    Common::Vector2d resultOtherCOG;

    runResult.SetCollision();

    if(!CalculatePointOfContact(agent,
                                other,
                                resultAgentDistOnBorder,
                                resultOtherDistOnBorder,
                                resultAgentCOG,
                                resultOtherCOG))
    {
        LOG(CbkLogLevel::Warning, "could not calculate contact point");
        return false;
    }

    if(!runResult.AddCollision(agent,
                               resultAgentCOG,
                               resultAgentDistOnBorder,
                               other,
                               resultOtherCOG,
                               resultOtherDistOnBorder))
    {
        LOG(CbkLogLevel::Error, "could not store collision result");
        return false;
    }

    return true;
}

bool CollisionDetection_Implementation::HandleCollisionsInAgents(RunResultInterface &runResult,
                                     bool &isCollision)
{
    if(!agents)
    {
        LOG(CbkLogLevel::Warning, "collision detection not initialized");
        return false;
    }

    // reset collision flags
    isCollision = false;


    // accumulate collisions
    for(auto it = agents->cbegin(); it != agents->cend(); ++it)
    {
        bool calculatedNormal0 = false;
        bool calculatedNormal1 = false;

        const AgentInterface *agent = it->second;
        assert(agent != nullptr);

        // calculate agent corners and normals
        std::array<Common::Vector2d, NumberCorners> agentCorners;
        std::array<Common::Vector2d, NumberNormals> agentNormals;
        GetAgentGeometry(agent, agentCorners, agentNormals);

        // postpone calculation of projected values until they are really needed
        std::array<double, NumberProjectedOwnAxis> agentNormal0_agentCornerProjected; // only two points need to be projected on own normals since they are aligned
        double agentNormal0_agentCornerProjectedMaxValue;
        double agentNormal0_agentCornerProjectedMinValue;

        std::array<double, NumberProjectedOwnAxis> agentNormal1_agentCornerProjected; // only two points need to be projected on own normals since they are aligned
        double agentNormal1_agentCornerProjectedMaxValue;
        double agentNormal1_agentCornerProjectedMinValue;

        for(auto otherIt = std::next(it); otherIt != agents->cend(); ++otherIt)
        {
            const AgentInterface *other = otherIt->second;
            if(!other)
            {
                LOG(CbkLogLevel::Warning, "collision detection aborted");
                return false;
            }

            // calculate other corners and normals
            std::array<Common::Vector2d, NumberCorners> otherCorners;
            std::array<Common::Vector2d, NumberNormals> otherNormals;
            GetAgentGeometry(other, otherCorners, otherNormals);

            // quick check
            double quickDistance = agent->GetLength() + agent->GetWidth() + other->GetLength() + other->GetWidth();
            if(fabs(agentCorners[UpperLeft].x - otherCorners[UpperLeft].x) > quickDistance ||
                    fabs(agentCorners[UpperLeft].y - otherCorners[UpperLeft].y) > quickDistance)
            {
                continue;
            }

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
                continue;
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
                continue;
            }

            // skip check for other normals if agents are approximately aligned to same axes
            if(fabs(fmod(fabs(agent->GetYawAngle()), 90.0) - fmod(fabs(other->GetYawAngle()), 90.0)) > ROTATION_EPS)
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
                    continue;
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
                    continue;
                }

                // no separations given on any axis -> collision
                CreateResult(agent, other, runResult);
                isCollision = true;
                const_cast<AgentInterface*>(agent)->UpdateCollision(other->GetAgentId());
                const_cast<AgentInterface*>(other)->UpdateCollision(agent->GetAgentId());
            }
            else
            {
                // no separations given on any axis -> collision
                CreateResult(agent, other, runResult);
                isCollision = true;
                const_cast<AgentInterface*>(agent)->UpdateCollision(other->GetAgentId());
                const_cast<AgentInterface*>(other)->UpdateCollision(agent->GetAgentId());
            }
        }
    }

    return true;
}
