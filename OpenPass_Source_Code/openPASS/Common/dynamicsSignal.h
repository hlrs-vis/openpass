/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  DynamicsSignal.h
//! @brief This file contains all functions for class
//! DynamicsSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/modelInterface.h"

//-----------------------------------------------------------------------------
//! Signal class
//-----------------------------------------------------------------------------
class DynamicsSignal : public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "DynamicsSignal";


    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    DynamicsSignal()
    {
        componentState = ComponentState::Disabled;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    DynamicsSignal(ComponentState componentState)
    {
        this->componentState = componentState;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    DynamicsSignal(DynamicsSignal &other) :
        DynamicsSignal(other.componentState,
                       other.acceleration,
                       other.velocity,
                       other.positionX,
                       other.positionY,
                       other.yaw,
                       other.yawRate,
                       other.steeringWheelAngle,
                       other.travelDistance)

    {
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    DynamicsSignal(ComponentState componentState,
                  double acceleration,
                  double velocity,
                  double positionX,
                  double positionY,
                  double yaw,
                  double yawRate,
                  double steeringWheelAngle,
                  double travelDistance) :
        acceleration(acceleration),
        velocity(velocity),
        positionX(positionX),
        positionY(positionY),
        yaw(yaw),
        yawRate(yawRate),
        steeringWheelAngle(steeringWheelAngle),
        travelDistance(travelDistance)
    {
        this->componentState = componentState;
    }

    DynamicsSignal(const DynamicsSignal&) = delete;
    DynamicsSignal(DynamicsSignal&&) = delete;
    DynamicsSignal& operator=(const DynamicsSignal&) = delete;
    DynamicsSignal& operator=(DynamicsSignal&&) = delete;

    virtual ~DynamicsSignal()

    {}

    //-----------------------------------------------------------------------------
    //! Returns the content/payload of the signal as an std::string
    //!
    //! @return                       Content/payload of the signal as an std::string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::ostringstream stream;
        stream << COMPONENTNAME;
        return stream.str();
    }

    double acceleration = 0.0;
    double velocity = 0.0;
    double positionX = 0.0;
    double positionY = 0.0;
    double yaw = 0.0;
    double yawRate = 0.0;
    double steeringWheelAngle = 0.0;
    double travelDistance = 0.0;
};

