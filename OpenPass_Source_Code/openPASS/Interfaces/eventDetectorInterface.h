/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  EventDetectorInterface.h
//! @brief This file contains the interface to communicate between event detector network
//!        and the event detectors.
//-----------------------------------------------------------------------------

#pragma once

#include <vector>
#include "Interfaces/eventNetworkInterface.h"

//-----------------------------------------------------------------------------
//! Interface for event detector.
//! Implement algorithm for detecting an event
//-----------------------------------------------------------------------------
class EventDetectorInterface
{
public:
    EventDetectorInterface() = default;
    EventDetectorInterface(const EventDetectorInterface&) = delete;
    EventDetectorInterface(EventDetectorInterface&&) = delete;
    EventDetectorInterface& operator=(const EventDetectorInterface&) = delete;
    EventDetectorInterface& operator=(EventDetectorInterface&&) = delete;
    virtual ~EventDetectorInterface() = default;

    virtual void Trigger(int time) = 0;
    virtual int GetCycleTime() = 0;
    virtual void Reset() = 0;
  };



