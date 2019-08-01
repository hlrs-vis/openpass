/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
/** \file  absoluteWorldCoordinateTrajectoryFollower.cpp */
//-----------------------------------------------------------------------------

#include "absoluteWorldCoordinateTrajectoryFollower.h"

AbsoluteWorldCoordinateTrajectoryFollower::AbsoluteWorldCoordinateTrajectoryFollower(std::string componentName,
        bool isInit,
        int priority,
        int offsetTime,
        int responseTime,
        int cycleTime,
        StochasticsInterface *stochastics,
        WorldInterface *world,
        const ParameterInterface *parameters,
        const std::map<int, ObservationInterface*> *observations,
        const CallbackInterface *callbacks,
        AgentInterface *agent,
        TrajectoryInterface *trajectory) :
    TrajectoryFollowerCommonBase(
        componentName,
        isInit,
        priority,
        offsetTime,
        responseTime,
        cycleTime,
        stochastics,
        world,
        parameters,
        observations,
        callbacks,
        agent)
{
    absoluteWorldTrajectory = *(trajectory->GetWorldCoordinates());
    previousTrajectoryIterator = absoluteWorldTrajectory.begin();
    nextTrajectoryIterator = ++(absoluteWorldTrajectory.begin());
}

void AbsoluteWorldCoordinateTrajectoryFollower::TriggerWithActiveAccelerationInput()
{
    Position previousPosition = lastWorldPosition;
    Position nextPosition = (*nextTrajectoryIterator).second;

    const double velocity = lastVelocity + inputAcceleration * cycleTimeInSeconds;

    if(velocity <= 0.0)
    {
        currentVelocity = 0;
        currentAcceleration = 0;
        currentYawRate = 0;
        distance = 0;

        if(automaticDeactivation)
        {
            UpdateState(ComponentState::Disabled);
        }

        return;
    }

    double remainingDistance = velocity * cycleTimeInSeconds;
    distance = remainingDistance;
    double percentageTraveledBetweenCoordinates = 0.0;

    while (remainingDistance > 0.0)
    {
        double distanceBetweenPoints = CalculateDistanceBetweenWorldCoordinates(previousPosition, nextPosition);
        if (distanceBetweenPoints < remainingDistance)
        {
            previousTrajectoryIterator++;
            nextTrajectoryIterator++;

            previousPosition = (*previousTrajectoryIterator).second;

            if(nextTrajectoryIterator != absoluteWorldTrajectory.end())
            {
                nextPosition = (*nextTrajectoryIterator).second;
            }
        }
        else
        {
            percentageTraveledBetweenCoordinates = remainingDistance / distanceBetweenPoints;

        }
        remainingDistance -= distanceBetweenPoints;
    }

    Common::Vector2d direction = CalculateScaledVector(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);
    const double deltaYawAngle = CalculateScaledDeltaYawAngle(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);

    UpdateDynamics(previousPosition, direction, deltaYawAngle, velocity, inputAcceleration);
}

void AbsoluteWorldCoordinateTrajectoryFollower::TriggerWithInactiveAccelerationInput()
{
    std::pair<int, Position> previousCoordinate = {lastCoordinateTimestamp, lastWorldPosition};
    std::pair<int, Position> nextCoordinate = *nextTrajectoryIterator;

    double remainingTime = GetCycleTime();
    double timeBetweenCoordinates = nextCoordinate.first - previousCoordinate.first;
    double deltaS {0};

    while (timeBetweenCoordinates <= remainingTime &&
           (nextTrajectoryIterator) != absoluteWorldTrajectory.end())
    {
        deltaS += CalculateDistanceBetweenWorldCoordinates(previousCoordinate.second, nextCoordinate.second);

        previousTrajectoryIterator++;
        previousCoordinate = *previousTrajectoryIterator;
        lastCoordinateTimestamp = previousCoordinate.first;

        nextTrajectoryIterator++;
        if(nextTrajectoryIterator != absoluteWorldTrajectory.end())
        {
            nextCoordinate = *nextTrajectoryIterator;
            remainingTime -= timeBetweenCoordinates;
            timeBetweenCoordinates = nextCoordinate.first - previousCoordinate.first;
        }
        else
        {
            remainingTime = 0;
            timeBetweenCoordinates = 1.0;
        }
    }

    const auto& previousPosition = previousCoordinate.second;
    const auto& nextPosition = nextCoordinate.second;

    percentageTraveledBetweenCoordinates = remainingTime / timeBetweenCoordinates;
    Common::Vector2d direction = CalculateScaledVector(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);
    const double deltaYawAngle = CalculateScaledDeltaYawAngle(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);

    deltaS += direction.Length();
    distance = deltaS;

    const double velocity = deltaS / cycleTimeInSeconds;
    const double acceleration = (velocity - lastVelocity) / cycleTimeInSeconds;

    const double deltaTimestamp = (nextCoordinate.first - lastCoordinateTimestamp) * percentageTraveledBetweenCoordinates;
    lastCoordinateTimestamp = lastCoordinateTimestamp + static_cast<int>(deltaTimestamp);
    UpdateDynamics(previousPosition, direction, deltaYawAngle, velocity, acceleration);
}

void AbsoluteWorldCoordinateTrajectoryFollower::Trigger(int time)
{
    currentTime = time;

    if (GetState() == ComponentState::Disabled)
    {
        return;
    }

    lastWorldPosition = currentWorldPosition;
    lastVelocity = currentVelocity;

    if (previousTrajectoryIterator != absoluteWorldTrajectory.end() &&
            (nextTrajectoryIterator) != absoluteWorldTrajectory.end())
    {
        if(initialization)
        {
            UpdateDynamics((*previousTrajectoryIterator).second, {0,0}, 0, 0, 0);
            lastCoordinateTimestamp = (*previousTrajectoryIterator).first;
            initialization = false;
            return;
        }

        if(inputAccelerationActive)
        {
            TriggerWithActiveAccelerationInput();
        }
        else
        {
            TriggerWithInactiveAccelerationInput();
        }
    }
    else
    {
        currentVelocity = 0;
        currentAcceleration = 0;
        currentYawRate = 0;
        distance = 0;

        if(automaticDeactivation)
        {
            UpdateState(ComponentState::Disabled);
        }
    }
}

Common::Vector2d AbsoluteWorldCoordinateTrajectoryFollower::CalculateScaledVector(const Position &previousPosition, const Position &nextPosition, const double &factor)
{
    Common::Vector2d result (nextPosition.xPos - previousPosition.xPos, nextPosition.yPos - previousPosition.yPos);
    result.Scale(factor);

    return result;
}

double AbsoluteWorldCoordinateTrajectoryFollower::CalculateScaledDeltaYawAngle(const Position &previousPosition, const Position &nextPosition, const double &factor)
{
    const double result = (nextPosition.yawAngle - previousPosition.yawAngle) * factor;

    return result;
}


Position AbsoluteWorldCoordinateTrajectoryFollower::CalculateStartPosition(const Position &previousPosition, const Position &nextPosition)
{
    const auto startDirection = CalculateScaledVector(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);
    const double startDeltaYawAngle = CalculateScaledDeltaYawAngle(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);

    Position startPosition = previousPosition;
    startPosition.xPos += startDirection.x;
    startPosition.yPos += startDirection.y;
    startPosition.yawAngle += startDeltaYawAngle;

    return startPosition;
}

std::pair<int, Position> AbsoluteWorldCoordinateTrajectoryFollower::CalculateStartCoordinate(const std::pair<int, Position> &previousPosition, const std::pair<int, Position> &nextPosition)
{
    const auto startPosition = CalculateStartPosition(previousPosition.second, nextPosition.second);

    const double timeDifference = nextPosition.first - previousPosition.first;
    const double startTime = previousPosition.first + timeDifference * percentageTraveledBetweenCoordinates;

    const std::pair<int, Position> result(startTime, startPosition);

    return result;
}

double AbsoluteWorldCoordinateTrajectoryFollower::CalculateDistanceBetweenWorldCoordinates(Position previousPosition, Position nextPosition)
{  
    return std::hypot(nextPosition.xPos - previousPosition.xPos, nextPosition.yPos - previousPosition.yPos);
}

void AbsoluteWorldCoordinateTrajectoryFollower::UpdateDynamics(const Position &previousPosition,
                                                               const Common::Vector2d &direction,
                                                               const double &deltaYawAngle,
                                                               const double &velocity,
                                                               const double &acceleration)
{
    currentWorldPosition.xPos = previousPosition.xPos + direction.x;
    currentWorldPosition.yPos = previousPosition.yPos + direction.y;
    currentWorldPosition.yawAngle = previousPosition.yawAngle + deltaYawAngle;

    currentYawRate = (currentWorldPosition.yawAngle - lastWorldPosition.yawAngle) / cycleTimeInSeconds;

    currentVelocity = velocity;
    currentAcceleration = acceleration;
}
