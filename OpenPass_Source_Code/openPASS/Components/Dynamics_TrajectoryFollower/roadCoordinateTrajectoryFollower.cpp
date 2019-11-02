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
/** \file  roadCoordinateTrajectoryFollower.cpp */
//-----------------------------------------------------------------------------

#include "roadCoordinateTrajectoryFollower.h"

RoadCoordinateTrajectoryFollower::RoadCoordinateTrajectoryFollower(std::string componentName,
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
    roadTrajectory = *(trajectory->GetRoadCoordinates());
    previousTrajectoryIterator = roadTrajectory.begin();
    nextTrajectoryIterator = ++(roadTrajectory.begin());
    isRelativeTrajectory = trajectory->GetTrajectoryType() == TrajectoryType::RoadCoordinatesRelative;
}

void RoadCoordinateTrajectoryFollower::Trigger(int time)
{
    currentTime = time;

    if (GetState() == ComponentState::Disabled)
    {
        return;
    }

    lastVelocity = currentVelocity;

    if (previousTrajectoryIterator != roadTrajectory.end() &&
            (nextTrajectoryIterator) != roadTrajectory.end())
    {
        if (initialization)
        {
            lastRoadPosition = (*previousTrajectoryIterator).second;

            if (isRelativeTrajectory)
            {
                startPosition = GetAgent()->GetRoadPosition();

                //Adjust t-coordinate to middle of the road by subtracting lane widths
                startPosition.t -= 0.5 * GetAgent()->GetLaneWidth();
                startLaneId = GetAgent()->GetMainLaneId(MeasurementPoint::Reference);
                const int numberOfLanesToTheLeft = -startLaneId - 1;
                for (int i = 1; i <= numberOfLanesToTheLeft; i++)
                {
                    startPosition.t -= GetAgent()->GetLaneWidth(i);
                }
            }
            lastCoordinateTimestamp = (*previousTrajectoryIterator).first;
            UpdateDynamics((*previousTrajectoryIterator).second, {0,0}, 0, 0, 0);

            initialization = false;
            return;
        }

        if (inputAccelerationActive)
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

Common::Vector2d RoadCoordinateTrajectoryFollower::CalculateScaledVector(const RoadPosition &previousPosition, const RoadPosition &nextPosition, const double &factor)
{
    Common::Vector2d result (nextPosition.s - previousPosition.s, nextPosition.t - previousPosition.t);
    result.Scale(factor);

    return result;
}

double RoadCoordinateTrajectoryFollower::CalculateScaledDeltaHeading(const RoadPosition &previousPosition, const RoadPosition &nextPosition, const double &factor)
{
    const double result = (nextPosition.hdg - previousPosition.hdg) * factor;

    return result;
}

RoadPosition RoadCoordinateTrajectoryFollower::CalculateStartPosition(const RoadPosition &previousPosition, const RoadPosition &nextPosition)
{
    const auto startDirection = CalculateScaledVector(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);
    const double startDeltaHeading = CalculateScaledDeltaHeading(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);

    RoadPosition startPosition = previousPosition;
    startPosition.s += startDirection.x;
    startPosition.t += startDirection.y;
    startPosition.hdg += startDeltaHeading;

    return startPosition;
}

std::pair<int, RoadPosition> RoadCoordinateTrajectoryFollower::CalculateStartCoordinate(const std::pair<int, RoadPosition> &previousPosition, const std::pair<int, RoadPosition> &nextPosition)
{
    const auto startPosition = CalculateStartPosition(previousPosition.second, nextPosition.second);

    const double timeDifference = nextPosition.first - previousPosition.first;
    const double startTime = previousPosition.first + timeDifference * percentageTraveledBetweenCoordinates;

    const std::pair<int, RoadPosition> result(startTime, startPosition);

    return result;
}

void RoadCoordinateTrajectoryFollower::TriggerWithActiveAccelerationInput()
{
    RoadPosition previousPosition = lastRoadPosition;
    if (isRelativeTrajectory)
    {
        previousPosition.s -= startPosition.s;
        previousPosition.t -= startPosition.t;
        previousPosition.hdg -= startPosition.hdg;
    }
    RoadPosition nextPosition = (*nextTrajectoryIterator).second;

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

    double distanceRemaining = velocity * cycleTimeInSeconds;
    distance = distanceRemaining;
    double percentageTraveledBetweenCoordinates = 0.0;

    while (distanceRemaining > 0.0)
    {
        double distanceBetweenPoints = std::hypot(nextPosition.s - previousPosition.s, nextPosition.t - previousPosition.t);
        if (distanceBetweenPoints < distanceRemaining)
        {
            previousTrajectoryIterator++;
            nextTrajectoryIterator++;

            previousPosition = (*previousTrajectoryIterator).second;

            if(nextTrajectoryIterator != roadTrajectory.end())
            {
                nextPosition = (*nextTrajectoryIterator).second;
            }
        }
        else
        {
            percentageTraveledBetweenCoordinates = distanceRemaining / distanceBetweenPoints;

        }
        distanceRemaining -= distanceBetweenPoints;
    }
    Common::Vector2d direction = CalculateScaledVector(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);
    const double deltaHeading = CalculateScaledDeltaHeading(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);

    UpdateDynamics(previousPosition, direction, deltaHeading, velocity, inputAcceleration);
}

void RoadCoordinateTrajectoryFollower::TriggerWithInactiveAccelerationInput()
{
    std::pair<int, RoadPosition> previousCoordinate = {lastCoordinateTimestamp, lastRoadPosition};
    if (isRelativeTrajectory)
    {
        previousCoordinate.second.s -= startPosition.s;
        previousCoordinate.second.t -= startPosition.t;
        previousCoordinate.second.hdg -= startPosition.hdg;
    }
    std::pair<int, RoadPosition> nextCoordinate = *nextTrajectoryIterator;

    double remainingTime = GetCycleTime();
    double timeBetweenCoordinates = nextCoordinate.first - previousCoordinate.first;
    double deltaS {0};

    while (timeBetweenCoordinates <= remainingTime &&
           (nextTrajectoryIterator) != roadTrajectory.end())
    {
        //This contains a slight error since road curvature is not considered
        deltaS += std::hypot(nextCoordinate.second.s - previousCoordinate.second.s, nextCoordinate.second.t - previousCoordinate.second.t);

        previousTrajectoryIterator++;
        previousCoordinate = *previousTrajectoryIterator;
        lastCoordinateTimestamp = previousCoordinate.first;

        nextTrajectoryIterator++;
        if(nextTrajectoryIterator != roadTrajectory.end())
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
    const double deltaYawAngle = CalculateScaledDeltaHeading(previousPosition, nextPosition, percentageTraveledBetweenCoordinates);

    deltaS += direction.Length();
    distance = deltaS;

    const double velocity = deltaS / cycleTimeInSeconds;
    const double acceleration = (velocity - lastVelocity) / cycleTimeInSeconds;

    const double deltaTimestamp = (nextCoordinate.first - lastCoordinateTimestamp) * percentageTraveledBetweenCoordinates;
    lastCoordinateTimestamp = lastCoordinateTimestamp + static_cast<int>(deltaTimestamp);

    UpdateDynamics(previousPosition, direction, deltaYawAngle, velocity, acceleration);
}

void RoadCoordinateTrajectoryFollower::UpdateDynamics(const RoadPosition &previousPosition,
                                                              const Common::Vector2d &direction,
                                                              const double &deltaHeading,
                                                              const double &velocity,
                                                              const double &acceleration)
{
    RoadPosition currentRoadPosition;

    currentRoadPosition.s = previousPosition.s + direction.x;
    currentRoadPosition.t = previousPosition.t + direction.y;
    currentRoadPosition.hdg = previousPosition.hdg + deltaHeading;

    if(isRelativeTrajectory)
    {
        currentRoadPosition.s += startPosition.s;
        currentRoadPosition.t += startPosition.t;
        currentRoadPosition.hdg += startPosition.hdg;
    }

    Position result = GetWorld()->RoadCoord2WorldCoord(currentRoadPosition);

    currentWorldPosition.xPos = result.xPos;
    currentWorldPosition.yPos = result.yPos;
    currentWorldPosition.yawAngle = result.yawAngle;

    currentYawRate = (currentWorldPosition.yawAngle - lastWorldPosition.yawAngle) / cycleTimeInSeconds;

    currentVelocity = velocity;
    currentAcceleration = acceleration;

    lastRoadPosition = currentRoadPosition;
    lastWorldPosition = currentWorldPosition;
}
