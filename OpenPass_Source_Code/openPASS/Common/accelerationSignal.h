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
//! @file  accelerationSignal.h
//! @brief This file contains all functions for class
//! AccelerationSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/modelInterface.h"

//-----------------------------------------------------------------------------
//! Signal class
//-----------------------------------------------------------------------------
class AccelerationSignal : public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "AccelerationSignal";


    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    AccelerationSignal()
    {
        componentState = ComponentState::Disabled;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    AccelerationSignal(AccelerationSignal &other) :
        AccelerationSignal(other.componentState,
                      other.acceleration)

    {

    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    AccelerationSignal(ComponentState componentState,
                  double acceleration) :
        acceleration(acceleration)
    {
        this->componentState = componentState;
    }

    AccelerationSignal(const AccelerationSignal&) = delete;
    AccelerationSignal(AccelerationSignal&&) = delete;
    AccelerationSignal& operator=(const AccelerationSignal&) = delete;
    AccelerationSignal& operator=(AccelerationSignal&&) = delete;

    virtual ~AccelerationSignal()

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

    double acceleration;
};

