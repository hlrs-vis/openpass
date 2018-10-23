/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  spawnItemParameter.h
//! @brief This file contains the internal representation of parameter set of a
//!        newly created agent spawn item.
//-----------------------------------------------------------------------------

#ifndef SPAWNITEMPARAMETER_H
#define SPAWNITEMPARAMETER_H

#include "spawnPointInterface.h"

namespace SimulationSlave
{

class SpawnItemParameter : public SpawnItemParameterInterface
{
public:
    SpawnItemParameter() = default;
    SpawnItemParameter(const SpawnItemParameter&) = delete;
    SpawnItemParameter(SpawnItemParameter&&) = delete;
    SpawnItemParameter& operator=(const SpawnItemParameter&) = delete;
    SpawnItemParameter& operator=(SpawnItemParameter&&) = delete;
    virtual ~SpawnItemParameter() = default;

    void SetPositionX(double positionX)
    {
        this->positionX = positionX;
    }

    void SetPositionY(double positionY)
    {
        this->positionY = positionY;
    }

    void SetVelocityX(double velocityX)
    {
        this->velocityX = velocityX;
    }

    void SetVelocityY(double velocityY)
    {
        this->velocityY = velocityY;
    }

    void SetAccelerationX(double accelerationX)
    {
        this->accelerationX = accelerationX;
    }

    void SetAccelerationY(double accelerationY)
    {
        this->accelerationY = accelerationY;
    }

    void SetYawAngle(double yawAngle)
    {
        this->yawAngle = yawAngle;
    }

    void SetNextTimeOffset(int nextTimeOffset)
    {
        this->nextTimeOffset = nextTimeOffset;
    }

    void SetIndex(int index)
    {
        this->index = index;
    }

    double GetPositionX() const
    {
        return positionX;
    }

    double GetPositionY() const
    {
        return positionY;
    }

    double GetVelocityX() const
    {
        return velocityX;
    }

    double GetVelocityY() const
    {
        return velocityY;
    }

    double GetAccelerationX() const
    {
        return accelerationX;
    }

    double GetAccelerationY() const
    {
        return accelerationY;
    }

    double GetYawAngle() const
    {
        return yawAngle;
    }

    int GetNextTimeOffset() const
    {
        return nextTimeOffset;
    }

    int GetIndex() const
    {
        return index;
    }

private:
    double positionX = 0.0;
    double positionY = 0.0;
    double velocityX = 0.0;
    double velocityY = 0.0;
    double accelerationX = 0.0;
    double accelerationY = 0.0;
    double yawAngle = 0.0;
    int nextTimeOffset = -1;
    int index = 0;
};

} // namespace SimulationSlave

#endif // SPAWNITEMPARAMETER_H
