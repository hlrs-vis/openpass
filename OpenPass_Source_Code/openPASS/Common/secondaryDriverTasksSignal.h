/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*               2018 AMFD GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  secondaryDriverTasksSignal.h
//! @brief This file contains all functions for class
//! SecondaryDriverTasksSignal
//!
//! This class contains all functionality of the signal.
//-----------------------------------------------------------------------------
#pragma once

#include "Interfaces/modelInterface.h"

class SecondaryDriverTasksSignal : public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "SecondaryDriverTasksSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    SecondaryDriverTasksSignal()
    {
        componentState = ComponentState::Disabled;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    SecondaryDriverTasksSignal(
            int  indicatorState,
            bool hornSwitch,
            bool headLightSwitch,
            bool highBeamLightSwitch,
            bool flasher,
            ComponentState componentState
            ):
        indicatorState(indicatorState),
        hornSwitch(hornSwitch),
        headLightSwitch(headLightSwitch),
        highBeamLightSwitch(highBeamLightSwitch),
        flasherSwitch(flasher)
    {
        this->componentState = componentState;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    SecondaryDriverTasksSignal(SecondaryDriverTasksSignal &other) :
        SecondaryDriverTasksSignal(other.indicatorState,
                                                     other.hornSwitch,
                                                     other.headLightSwitch,
                                                     other.highBeamLightSwitch,
                                                     other.flasherSwitch,
                                                     other.componentState)

    {

    }

    SecondaryDriverTasksSignal(const SecondaryDriverTasksSignal&) = delete;
    SecondaryDriverTasksSignal(SecondaryDriverTasksSignal&&) = delete;
    SecondaryDriverTasksSignal& operator=(const SecondaryDriverTasksSignal&) = delete;
    SecondaryDriverTasksSignal& operator=(SecondaryDriverTasksSignal&&) = delete;

    virtual ~SecondaryDriverTasksSignal()
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

    int  indicatorState;
    bool hornSwitch;
    bool headLightSwitch;
    bool highBeamLightSwitch;
    bool flasherSwitch;
};
