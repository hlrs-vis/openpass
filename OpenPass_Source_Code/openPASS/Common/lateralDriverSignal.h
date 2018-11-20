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
//! @file  LateralDriverSignal.h
//! @brief This file contains all functions for class LateralDriverSignal
//!
//! This class contains all functionality of the signal.
//-----------------------------------------------------------------------------
#pragma once

#include "modelInterface.h"

class LateralDriverSignal : public ComponentStateSignalInterface
{
public:
    const std::string COMPONENTNAME = "LateralDriverSignal";

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    LateralDriverSignal(int senderId)
    {
        componentState = ComponentState::Disabled;
        this->senderID = senderId;
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    LateralDriverSignal(LateralDriverSignal &other) :
        LateralDriverSignal(
                other.currCurvature,
                other.laneWidth,
                other.vLatEgo,
                other.vLongEgo,
                other.notifyCollision,
                other.freqLateralOscillation,
                other.zeta,
                other.LatDisplacement,
                other.DeltaPhi,
                other.GainDeltaPhi,
                other.isLaneChangeAborted,
                other.componentState,
                other.senderID)
    {
    }

    //-----------------------------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------------------------
    LateralDriverSignal(
            double currCurvature,
            double laneWidth,
            double vLatEgo,
            double vLongEgo,
            bool notifyCollision,
            double freqLateralOscillation,
            double zeta,
            double LatDisplacement,
            double DeltaPhi,
            double GainDeltaPhi,
            bool   isLaneChangeAborted, //LaneChangeState laneChangeStateEgo,
            ComponentState componentStateP,
            int senderId
            ):
        currCurvature(currCurvature),
        laneWidth(laneWidth),
        vLatEgo(vLatEgo),
        vLongEgo(vLongEgo),
        notifyCollision(notifyCollision),
        freqLateralOscillation(freqLateralOscillation),
        zeta(zeta),
        LatDisplacement(LatDisplacement),
        DeltaPhi(DeltaPhi),
        GainDeltaPhi(GainDeltaPhi),
        isLaneChangeAborted(isLaneChangeAborted)
    {
        componentState = componentStateP;
        senderID = senderId;
    }

    virtual ~LateralDriverSignal()
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

    double currCurvature {0};
    double laneWidth {0};
    double vLatEgo {0};
    double vLongEgo {0};
    bool notifyCollision {false};
    double freqLateralOscillation {0};
    double zeta {0};
    double LatDisplacement {0};
    double DeltaPhi {0};
    double GainDeltaPhi {0};
    bool isLaneChangeAborted {false};
};
