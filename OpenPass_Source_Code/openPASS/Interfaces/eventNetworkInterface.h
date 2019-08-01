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
/** \file  EventNetworkInterface.h
*	\brief This file provides the interface for the EventNetwork
*/
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/agentInterface.h"
#include "Interfaces/eventInterface.h"
#include "Interfaces/observationInterface.h"
#include "Interfaces/respawnInterface.h"
#include "Interfaces/runResultInterface.h"

using Events = std::map<EventDefinitions::EventCategory, std::list<std::shared_ptr<EventInterface>>>;

namespace SimulationSlave
{

//-----------------------------------------------------------------------------
/** \brief This class provides the interface for the EventNetwork
*
* 	\ingroup EventNetwork */
//-----------------------------------------------------------------------------
class EventNetworkInterface
{
public:
    EventNetworkInterface() = default;
    EventNetworkInterface(const EventNetworkInterface&) = delete;
    EventNetworkInterface(EventNetworkInterface&&) = delete;
    EventNetworkInterface& operator=(const EventNetworkInterface&) = delete;
    EventNetworkInterface& operator=(EventNetworkInterface&&) = delete;
    virtual ~EventNetworkInterface() = default;

    virtual Events *GetActiveEvents() = 0;

    virtual Events *GetArchivedEvents() = 0;

    virtual std::list<std::shared_ptr<EventInterface>> *GetActiveEventCategory(EventDefinitions::EventCategory eventCategory) = 0;

    virtual void RemoveOldEvents(int time) = 0;

    virtual void InsertEvent(std::shared_ptr<EventInterface> event) = 0;

    virtual void ClearActiveEvents() = 0;

    virtual void Clear() = 0;

    virtual void Respawn(int time) = 0;

    virtual void AddCollision(const int agentId) = 0;

    virtual void Initialize(RespawnInterface *respawner,
                            RunResultInterface *runResult,
                            ObservationInterface *observer) = 0;
};

} //namespace SimulationSlave
