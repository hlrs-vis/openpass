/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  SensorDataSignal.h
//! @brief This file contains all functions for class SensorDataSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include "osi/osi_sensordata.pb.h"

class SensorDataSignal : public SignalInterface
{
public:
    const std::string COMPONENTNAME = "SensorDataSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    SensorDataSignal(osi3::SensorData sensorData) :
        sensorData(sensorData)
    {}

    SensorDataSignal(const SensorDataSignal&) = delete;
    SensorDataSignal(SensorDataSignal&&) = delete;
    SensorDataSignal& operator=(const SensorDataSignal&) = delete;
    SensorDataSignal& operator=(SensorDataSignal&&) = delete;

    virtual ~SensorDataSignal()
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

    osi3::SensorData sensorData;    //!< OSI SensorData payload of this signal
};

