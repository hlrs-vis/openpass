/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  externalTrajectory.h
//! @brief This file contains class for a trajectory that might be exchanged
//!        between modules and its signal class.
//-----------------------------------------------------------------------------
#ifndef TRAJECTORYSIGNAL_H
#define TRAJECTORYSIGNAL_H

#include "PCM_Data/pcm_trajectory.h"
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
    TrajectorySignal(PCM_Trajectory inValue):
        SignalInterface(), value(inValue)
    {}

    TrajectorySignal(const TrajectorySignal &) = default;
    TrajectorySignal(TrajectorySignal &&) = default;
    TrajectorySignal &operator=(const TrajectorySignal &) = default;
    TrajectorySignal &operator=(TrajectorySignal &&) = default;
    virtual ~TrajectorySignal() = default;

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

    PCM_Trajectory value;               //!< the content of the signal
};

#endif // TRAJECTORYSIGNAL_H
