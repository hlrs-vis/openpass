/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  brakingSignal.h
//! @brief This file contains all functions for class
//! BrakingSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include "modelInterface.h"

//-----------------------------------------------------------------------------
//! Signal class
//-----------------------------------------------------------------------------
class BrakingSignal : public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "BrakingSignal";


    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    BrakingSignal(int senderID)
    {
        componentState = ComponentState::Disabled;
        this->senderID = senderID;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    BrakingSignal(BrakingSignal &other) :
        BrakingSignal(other.senderID,
                      other.componentState,
                      other.acceleration)

    {

    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    BrakingSignal(int senderId,
                  ComponentState componentState,
                  double acceleration) :
        acceleration(acceleration)
    {
        this->senderID = senderId;
        this->componentState = componentState;
    }

    BrakingSignal(const BrakingSignal&) = delete;
    BrakingSignal(BrakingSignal&&) = delete;
    BrakingSignal& operator=(const BrakingSignal&) = delete;
    BrakingSignal& operator=(BrakingSignal&&) = delete;

    virtual ~BrakingSignal()

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
