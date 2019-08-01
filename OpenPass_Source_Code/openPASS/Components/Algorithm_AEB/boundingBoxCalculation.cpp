/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "boundingBoxCalculation.h"

BoundingBoxCalculation::BoundingBoxCalculation(
        AgentInterface* agent,
        double collisionDetectionLongitudinalBoundary,
        double collisionDetectionLateralBoundary) :
    agent(agent),
    collisionDetectionLongitudinalBoundary(collisionDetectionLongitudinalBoundary),
    collisionDetectionLateralBoundary(collisionDetectionLateralBoundary)
{
}

polygon_t BoundingBoxCalculation::CalculateBoundingBox(double timeStepInSeconds, const osi3::BaseMoving &baseMoving)
{
    // Initial bounding box in locale coordinate system
    double lengthHalf = (baseMoving.dimension().length() + collisionDetectionLongitudinalBoundary) / 2.0;
    double widthHalf  = (baseMoving.dimension().width() + collisionDetectionLateralBoundary) / 2.0;

    double boxPoints[][2]
    {
        { -lengthHalf, -widthHalf },
        { lengthHalf, -widthHalf },
        { lengthHalf, widthHalf },
        { -lengthHalf, widthHalf },
        { -lengthHalf, -widthHalf }
    };

    polygon_t box;
    bg::append(box, boxPoints);

    // inital object values at current position
    point_t initialPosition = { baseMoving.position().x(), baseMoving.position().y()};
    double initialVelocityX = baseMoving.velocity().x() + agent->GetVelocity(); //Velocity in BaseMoving is relative to own vehicle
    double initialVelocityY = baseMoving.velocity().y();
    double initialVelocity = std::hypot(initialVelocityX, initialVelocityY);
    double initialYaw = baseMoving.orientation().yaw();
    double yawRate = baseMoving.orientation_rate().yaw() + agent->GetYawRate();
    double accelerationX = baseMoving.acceleration().x() + agent->GetAcceleration(); //Acceleration in BaseMoving is relative to own vehicle
    double accelerationY = baseMoving.acceleration().y();
//    double acceleration = std::hypot(accelerationX, accelerationY);
    double traveledDistanceX = initialVelocityX * timeStepInSeconds + 0.5 * accelerationX * timeStepInSeconds * timeStepInSeconds;
    double traveledDistanceY = initialVelocityY * timeStepInSeconds + 0.5 * accelerationY * timeStepInSeconds * timeStepInSeconds;
    double traveledDistance = std::hypot(traveledDistanceX, traveledDistanceY);

    // delta from initial position (object's perspective)
    point_t deltaPosition = { 0.0, 0.0 };
    point_t rotatedDeltaPosition;
    double deltaYaw = 0.0;

    if (yawRate != 0.0)
    {
        // Radius of the circle the agent is moving on radius = arc length / angle (or their deriviates)
        double radius = initialVelocity / yawRate;
        deltaYaw = traveledDistance / radius;
        deltaPosition.x(radius * std::sin(deltaYaw));
        deltaPosition.y(radius * (1 - std::cos(deltaYaw)));
    }
    else
    {
        // Just driving in a straight line
        deltaPosition.x(traveledDistance);
    }

    // rotation in mathematical negative orientation (boost) -> invert to match
    bt::rotate_transformer<bg::radian, double, 2, 2> totalRotation(-initialYaw - deltaYaw);
    bt::rotate_transformer<bg::radian, double, 2, 2> initialRotation(-initialYaw);
    bg::transform(deltaPosition, rotatedDeltaPosition, initialRotation);

    bt::translate_transformer<double, 2, 2> totalTranslation(initialPosition.x() + rotatedDeltaPosition.x(),
                                                             initialPosition.y() + rotatedDeltaPosition.y());

    // rotate, then translate
    polygon_t boxTemp;
    bg::transform(box, boxTemp, totalRotation);
    bg::transform(boxTemp, box, totalTranslation);
    return box;
}

polygon_t BoundingBoxCalculation::CalculateBoundingBox(const osi3::BaseStationary &baseStationary)
{
    // Initial bounding box in locale coordinate system
    double lengthHalf = (baseStationary.dimension().length() + collisionDetectionLongitudinalBoundary) / 2.0;
    double widthHalf  = (baseStationary.dimension().width() + collisionDetectionLateralBoundary) / 2.0;
    double yaw = baseStationary.orientation().yaw();

    double boxPoints[][2]
    {
        { -lengthHalf, -widthHalf },
        { lengthHalf, -widthHalf },
        { lengthHalf, widthHalf },
        { -lengthHalf, widthHalf },
        { -lengthHalf, -widthHalf }
    };

    polygon_t box;
    bg::append(box, boxPoints);

    bt::rotate_transformer<bg::radian, double, 2, 2> rotation(-yaw);
    bt::translate_transformer<double, 2, 2> translation(baseStationary.position().x(),
                                                        baseStationary.position().y());

    // rotate, then translate
    polygon_t boxTemp;
    bg::transform(box, boxTemp, rotation);
    bg::transform(boxTemp, box, translation);
    return box;
}

polygon_t BoundingBoxCalculation::CalculateOwnBoundingBox(double timeStepInSeconds)
{
    // Initial bounding box in locale coordinate system
    double frontLength = agent->GetDistanceReferencePointToLeadingEdge() + collisionDetectionLongitudinalBoundary / 2.0;
    double rearLength = agent->GetLength() - agent->GetDistanceReferencePointToLeadingEdge() + collisionDetectionLongitudinalBoundary / 2.0;
    double widthHalf  = (agent->GetWidth() + collisionDetectionLateralBoundary) / 2.0;

    double boxPoints[][2]
    {
        { -rearLength, -widthHalf },
        { frontLength, -widthHalf },
        { frontLength, widthHalf },
        { -rearLength, widthHalf },
        { -rearLength, -widthHalf }
    };

    polygon_t box;
    bg::append(box, boxPoints);

    double initialVelocity = agent->GetVelocity();
    double yawRate = agent->GetYawRate();
    double acceleration = agent->GetAcceleration();
    double traveledDistance = initialVelocity * timeStepInSeconds + 0.5 * acceleration * timeStepInSeconds * timeStepInSeconds;

    point_t deltaPosition = { 0.0, 0.0 };
    double deltaYaw = 0.0;

    if (yawRate != 0.0)
    {
        // Radius of the circle the agent is moving on radius = arc length / angle (or their deriviates)
        double radius = initialVelocity / yawRate;
        deltaYaw = traveledDistance / radius;
        deltaPosition.x(radius * std::sin(deltaYaw));
        deltaPosition.y(radius * (1 - std::cos(deltaYaw)));
    }
    else
    {
        // Just driving in a straight line
        deltaPosition.x(traveledDistance);
    }

    // rotation in mathematical negative orientation (boost) -> invert to match
    bt::rotate_transformer<bg::radian, double, 2, 2> totalRotation(- deltaYaw);

    bt::translate_transformer<double, 2, 2> totalTranslation(deltaPosition.x(),
                                                             deltaPosition.y());

    // rotate, then translate
    polygon_t boxTemp;
    bg::transform(box, boxTemp, totalRotation);
    bg::transform(boxTemp, box, totalTranslation);
    return box;
}
