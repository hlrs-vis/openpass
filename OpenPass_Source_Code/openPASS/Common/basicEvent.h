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
/** \addtogroup Event
* @{
* \file  BasicEvent.h
* \brief This file contains all basic functionality of events.
*
* \details This file contains the common event member variables and the basic functions of all events.
* @} */
//-----------------------------------------------------------------------------

#pragma once

#include "Interfaces/eventInterface.h"

//-----------------------------------------------------------------------------
/** Signal This class contains all functionality of the BasicEvent.
 *
 * \ingroup Event */
//-----------------------------------------------------------------------------
class BasicEvent : public EventInterface
{
public:
    BasicEvent(int time,
               std::string source,
               std::string sequenceName,
               EventDefinitions::EventType eventType):
        time(time),
        source(source),
        sequenceName(sequenceName),
        eventType(eventType)
    {}
    BasicEvent(const BasicEvent&) = delete;
    BasicEvent(BasicEvent&&) = delete;
    BasicEvent& operator=(const BasicEvent&) = delete;
    BasicEvent& operator=(BasicEvent&&) = delete;
    virtual ~BasicEvent() = default;


    /*!
    * \brief Sets the Id of the event.
    *
    * @param[in]	Id of the event.
    */
    virtual void SetEventId(const int eventId)
    {
        this->eventId = eventId;
    }

    /*!
    * \brief Returns the Id of the event.
    *
    * @return	     Id.
    */
    virtual int GetId()
    {
        return eventId;
    }

    /*!
    * \brief Returns the time of the event.
    *
    * @return	     Time in milliseconds.
    */
    virtual int GetEventTime()
    {
        return time;
    }

    /*!
    * \brief Sets the Id of the event which triggered this event.
    *
    * @param[in]	     Event Id.
    */
    virtual void SetTriggeringEventId(const int triggeringEventId)
    {
        this->triggeringEventId = triggeringEventId;
    }

    /*!
    * \brief Returns the Id of the event which triggered this event.
    *
    * @return	     Event Id.
    */
    virtual int GetTriggeringEventId()
    {
        return triggeringEventId;
    }

    /*!
    * \brief Returns the name of the source component.
    *
    * @return	     Name of source component as string.
    */
    virtual std::string GetSource()
    {
        return source;
    }

    /*!
    * \brief Returns the sequenceName of the event.
    *
    * @return	     Sequence name as string.
    */
    virtual std::string GetSequenceName()
    {
        return sequenceName;
    }

    /*!
    * \brief Returns the type of the EventDetector.
    *
    * @return	     Type of the EventDetector.
    */
    virtual EventDefinitions::EventType GetEventType()
    {
        return eventType;
    }

    /*!
    * \brief Returns all parameters of the event as string list.
    * \details Returns all parameters of the event as a generic string list.
    *          The parameter name and the value are returned as string.
    *
    * @return	     List of string pairs of the event parameters.
    */
    virtual std::list<std::pair<std::string, std::string>> GetEventParametersAsString()
    {
        std::list<std::pair<std::string, std::string>> eventParameters;

        return eventParameters;
    }

private:
    int eventId;
    const int time;
    int triggeringEventId {-1};
    const std::string source;
    const std::string sequenceName;
    const EventDefinitions::EventType eventType;
};

