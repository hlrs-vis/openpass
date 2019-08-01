/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  ALongitudinalDriverToDLongitudinalSignal.h
//! @brief This file contains all functions for class
//! ALongitudinalDriverToDLongitudinalSignal
//!
//! This class contains all functionality of the signal.
//-----------------------------------------------------------------------------
#pragma once

#include "Interfaces/modelInterface.h"

class LongitudinalSignal: public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "LongitudinalSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    LongitudinalSignal(
            ComponentState componentState,
            double accPedalPos,
            double brakePedalPos,
            int gear
            ):
        accPedalPos(accPedalPos),
        brakePedalPos(brakePedalPos),
        gear(gear)
    {
        this->componentState = componentState;
    }

    LongitudinalSignal(const LongitudinalSignal&) = delete;
    LongitudinalSignal(LongitudinalSignal&&) = delete;
    LongitudinalSignal& operator=(const LongitudinalSignal&) = delete;
    LongitudinalSignal& operator=(LongitudinalSignal&&) = delete;

    virtual ~LongitudinalSignal()
    {}

    //-----------------------------------------------------------------------------
    //! Returns the content/payload of the signal as an std::string
    //!
    //! @return     Content/payload of the signal as an std::string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::ostringstream stream;
        stream << COMPONENTNAME;
        return stream.str();
    }

    double accPedalPos;
    double brakePedalPos;
    int gear;
};



