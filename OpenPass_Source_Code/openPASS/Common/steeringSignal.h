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
//! @file  SteeringSignal.h
//! @brief This file contains all functions for class
//! SteeringSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/modelInterface.h"

//-----------------------------------------------------------------------------
//! Signal class
//-----------------------------------------------------------------------------
class SteeringSignal : public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "SteeringSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    SteeringSignal(ComponentState componentState,
                  double steeringWheelAngle) :
        steeringWheelAngle(steeringWheelAngle)
    {
        this->componentState = componentState;
    }

    SteeringSignal(const SteeringSignal&) = delete;
    SteeringSignal(SteeringSignal&&) = delete;
    SteeringSignal& operator=(const SteeringSignal&) = delete;
    SteeringSignal& operator=(SteeringSignal&&) = delete;

    virtual ~SteeringSignal()

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

    double steeringWheelAngle;
};

