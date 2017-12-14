/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  externalTrajectory.h
//! @brief This file contains class for a trajectory that might be exchanged
//!        between modules and its signal class.
//-----------------------------------------------------------------------------
#ifndef TRAJECTORYSIGNAL_H
#define TRAJECTORYSIGNAL_H

#include "trajectory.h"
#include "modelInterface.h"

//-----------------------------------------------------------------------------
//! Signal class that enables the exchange of trajectories between modules
//-----------------------------------------------------------------------------
class TrajectorySignal : public SignalInterface
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     inValue      The object that is the content of the signal
    //-----------------------------------------------------------------------------
    TrajectorySignal(Trajectory inValue):
        SignalInterface(), value(inValue)
    {}

    //-----------------------------------------------------------------------------
    //! Operator to output the value of this signal as a string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::ostringstream stream;
        stream << "no value - Trajectory requires specialized output format"
               <<   "- alternatively specify << overload";
        return stream.str();
    }

    Trajectory value;               //!< the content of the signal
};

#endif // TRAJECTORYSIGNAL_H
