/*********************************************************************
* Copyright (c) 2016 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  DLateralToPRfsSignal.h
//! @brief This file contains all functions for class DLateralToPRfsSignal
//!
//! This class contains all functionality of the module.
//-----------------------------------------------------------------------------

#pragma once

#include "signalInterface.h"

//-----------------------------------------------------------------------------
//! Signal class
//-----------------------------------------------------------------------------
class DLateralToPRfsSignal : public SignalInterface
{
public:
    const std::string COMPONENTNAME = "DLateralToPRfsSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    DLateralToPRfsSignal(
            double steeringAngleDegrees,
            double steeringCurvature) :
        steeringAngleDegrees(steeringAngleDegrees),
        steeringCurvature(steeringCurvature)
    {}

    DLateralToPRfsSignal(const DLateralToPRfsSignal&) = delete;
    DLateralToPRfsSignal(DLateralToPRfsSignal&&) = delete;
    DLateralToPRfsSignal& operator=(const DLateralToPRfsSignal&) = delete;
    DLateralToPRfsSignal& operator=(DLateralToPRfsSignal&&) = delete;

    virtual ~DLateralToPRfsSignal()
    {}

    //-----------------------------------------------------------------------------
    //! Returns the content/payload of the signal as an std::string
    //!
    //! @return     Content/payload of the signal as an std::string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::ostringstream stream;
        stream << COMPONENTNAME << ": steeringAngleDegrees = " << steeringAngleDegrees << ", steeringCurvature = " << steeringCurvature;
        return stream.str();
    }

    double steeringAngleDegrees;
    double steeringCurvature;
};


