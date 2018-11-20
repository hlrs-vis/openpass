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
//! @file  ALongitudinalDriverToDLongitudinalSignal.h
//! @brief This file contains all functions for class
//! ALongitudinalDriverToDLongitudinalSignal
//!
//! This class contains all functionality of the signal.
//-----------------------------------------------------------------------------
#pragma once

#include "modelInterface.h"
#include "../Algorithm_LongitudinalDriver_OSI//AlgorithmLongitudinalDriverDataprovider.h"

class ALongitudinalDriverToDLongitudinalSignal: public SignalInterface
{
public:
    const std::string COMPONENTNAME = "ALongitudinalDriverToDLongitudinalSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    ALongitudinalDriverToDLongitudinalSignal(
            double accPedalPos,
            double brakePedalPos,
            std::vector<int> gear,
            ALongitudinalDataProvider::CarParameters carParameters
            ):
        accPedalPos(accPedalPos),
        brakePedalPos(brakePedalPos),
        gear(gear),
        carParameters(carParameters)
    {
    }

    ALongitudinalDriverToDLongitudinalSignal(const ALongitudinalDriverToDLongitudinalSignal&) = delete;
    ALongitudinalDriverToDLongitudinalSignal(ALongitudinalDriverToDLongitudinalSignal&&) = delete;
    ALongitudinalDriverToDLongitudinalSignal& operator=(const ALongitudinalDriverToDLongitudinalSignal&) = delete;
    ALongitudinalDriverToDLongitudinalSignal& operator=(ALongitudinalDriverToDLongitudinalSignal&&) = delete;

    virtual ~ALongitudinalDriverToDLongitudinalSignal()
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
    std::vector<int> gear;
    ALongitudinalDataProvider::CarParameters carParameters;
};



