/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2018, 2019 AMFD GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  lateralSignal.h
//! @brief This file contains all functions for class
//! LateralSignal
//!
//! This class contains all functionality of the signal.
//-----------------------------------------------------------------------------
#pragma once

#include "Interfaces/modelInterface.h"

class LateralSignal: public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "LateralSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    LateralSignal()
    {
        componentState = ComponentState::Disabled;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    LateralSignal(LateralSignal &other) :
        LateralSignal(
                other.laneWidth,
                other.lateralDeviation,
                other.gainLateralDeviation,
                other.headingError,
                other.gainHeadingError,
                other.kappaSet,
                other.componentState)
    {
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    LateralSignal(
            double laneWidth,
            double lateralDeviation,
            double gainLateralDeviation,
            double headingError,
            double gainHeadingError,
            double kappaSet,
            ComponentState componentStateP
            ):
        laneWidth(laneWidth),
        lateralDeviation(lateralDeviation),
        gainLateralDeviation(gainLateralDeviation),
        headingError(headingError),
        gainHeadingError(gainHeadingError),
        kappaSet(kappaSet)
    {
        componentState = componentStateP;
    }

    virtual ~LateralSignal()
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

    double laneWidth {0};
    double lateralDeviation {0};
    double gainLateralDeviation {20.0};
    double headingError {0};
    double gainHeadingError {7.5};
    double kappaSet {0};
};
