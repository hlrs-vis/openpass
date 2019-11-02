/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  SpawnItemParameter.h
//! @brief This file contains the internal representation of parameter set of a
//!        newly created agent spawn item.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/spawnPointInterface.h"

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

    void SetVelocity(double velocity)
    {
        this->velocity = velocity;
    }

    void SetAcceleration(double acceleration)
    {
        this->acceleration = acceleration;
    }

    void SetGear(double gear)
    {
        this->gear = gear;
    }

    void SetYaw(double yawAngle)
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

    void SetVehicleModel(std::string vehicleModel)
    {
        this->vehicleModel = vehicleModel;
    }

    double GetPositionX() const
    {
        return positionX;
    }

    double GetPositionY() const
    {
        return positionY;
    }

    double GetVelocity() const
    {
        return velocity;
    }

    double GetAcceleration() const
    {
        return acceleration;
    }

    double GetYaw() const
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

    std::string GetVehicleModel() const
    {
        return vehicleModel;
    }

private:
    double positionX = 0.0;
    double positionY = 0.0;
    double velocity = 0.0;
    double acceleration = 0.0;
    double gear = 0.0;
    double yawAngle = 0.0;

    int nextTimeOffset = -1;
    int index = 0;

    std::string vehicleModel = "";
};

} // namespace SimulationSlave


