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
/** \file  EventNetwork.cpp */
//-----------------------------------------------------------------------------

#include "eventNetwork.h"

using namespace EventDefinitions;

namespace SimulationSlave {

EventNetwork::EventNetwork()
{
}

EventNetwork::~EventNetwork()
{
    activeEvents.clear();
    archivedEvents.clear();
}

Events* EventNetwork::GetActiveEvents()
{
    return &activeEvents;
}

Events *EventNetwork::GetArchivedEvents()
{
    return &archivedEvents;
}

std::list<std::shared_ptr<EventInterface> > *EventNetwork::GetActiveEventCategory(EventCategory eventCategory)
{
    auto iterator = activeEvents.find(eventCategory);
    if(iterator == activeEvents.end())
    {
        return nullptr;
    }
    else
    {
        return &(iterator->second);
    }
}

void EventNetwork::RemoveOldEvents(int time)
{
    for(Events::iterator iterator = archivedEvents.begin(); iterator != archivedEvents.end(); iterator++)
    {
        while((*iterator).second.front()->GetEventTime() < time)
        {
            (*iterator).second.pop_front();
        }
    }
}

void EventNetwork::InsertEvent(std::shared_ptr<EventInterface> event)
{
    EventCategory eventCategory = DefineEventCategory(event->GetEventType());

    event->SetEventId(eventId);
    eventId++;

    if(eventCategory == EventCategory::Undefined)
    {
        return;
    }

    if(activeEvents.find(eventCategory) != activeEvents.end())
    {
        activeEvents.at(eventCategory).push_back(event);
    }
    else
    {
        std::list<std::shared_ptr<EventInterface>> eventsList;
        eventsList.push_back(event);

        activeEvents.insert(std::pair<const EventCategory, std::list<std::shared_ptr<EventInterface>>>(eventCategory, eventsList));
    }
}

void EventNetwork::ClearActiveEvents()
{
    for(std::pair<EventCategory, std::list<std::shared_ptr<EventInterface>>> eventMapEntry : activeEvents)
    {
        if(archivedEvents.find(eventMapEntry.first) != archivedEvents.end())
        {
            std::list<std::shared_ptr<EventInterface>> *eventList = &(archivedEvents.at(eventMapEntry.first));
            eventList->insert(eventList->end(), eventMapEntry.second.begin(), eventMapEntry.second.end());
        }
        else
        {
            archivedEvents.insert(eventMapEntry);
        }
    }

    activeEvents.clear();
}

void EventNetwork::Clear()
{
    eventId = 0;

    activeEvents.clear();
    archivedEvents.clear();

    observer = nullptr;
    respawner = nullptr;
    runResult = nullptr;
}

void EventNetwork::Respawn(int time)
{
    if(respawner != nullptr)
    {
        respawner->RespawnAgent(time);
    }
}

void EventNetwork::AddCollision(const int agentId)
{
    if(runResult != nullptr)
    {
        runResult->AddCollisionId(agentId);
    }
}

void EventNetwork::Initialize(RespawnInterface *respawner,
                              RunResultInterface *runResult,
                              ObservationInterface* observer)
{
    this->observer = observer;
    this->respawner = respawner;
    this->runResult = runResult;
}

EventCategory EventNetwork::DefineEventCategory(EventType eventType)
{
    EventCategory eventCategory;

    switch(eventType)
    {
        case EventType::Undefined:
            eventCategory = EventCategory::Undefined;
            break;

        case EventType::Collision:
            eventCategory = EventCategory::Collision;
            break;

        case EventType::ComponentStateChange:
            eventCategory = EventCategory::ComponentStateChange;
            break;

        default:
            eventCategory = EventCategory::AgentBased;
            break;
    }

    return eventCategory;
}
} //namespace SimulationSlave
