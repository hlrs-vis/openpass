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
/** @file  CollisionEvent.h
* @brief This file contains all functions for collision based events.
*
* This class contains all functionality of the module. */
//-----------------------------------------------------------------------------

#pragma once

#include "basicEvent.h"

//-----------------------------------------------------------------------------
/** This class implements all functionality of the CollisionEvent.
 *
 * \ingroup Events */
//-----------------------------------------------------------------------------
class CollisionEvent : public BasicEvent
{
public:
    CollisionEvent(int time,
                   std::string source,
                   std::string sequenceName,
                   EventDefinitions::EventType eventType,
                   bool collisionWithAgent,
                   int collisionAgentId,
                   int collisionOpponentId):
        BasicEvent(time,
                   source,
                   sequenceName,
                   eventType),
        collisionWithAgent(collisionWithAgent),
        collisionAgentId(collisionAgentId),
        collisionOpponentId(collisionOpponentId)
    {}
    CollisionEvent(const CollisionEvent&) = delete;
    CollisionEvent(CollisionEvent&&) = delete;
    CollisionEvent& operator=(const CollisionEvent&) = delete;
    CollisionEvent& operator=(CollisionEvent&&) = delete;
    virtual ~CollisionEvent() = default;

    /*!
    * \brief Returns all parameters of the event as string list.
    * \details Returns the CollisionWithAgent flag and both worldobject ids as string list.
    *
    * @return	     List of string pairs of the event parameters.
    */
    virtual std::list<std::pair<std::string, std::string>> GetEventParametersAsString()
    {
        std::list<std::pair<std::string, std::string>> eventParameters;

        eventParameters.push_back(std::pair<std::string, std::string>("CollisionWithAgent", collisionWithAgent ? "true" : "false"));
        eventParameters.push_back(std::pair<std::string, std::string>("CollisionAgentId", std::to_string(collisionAgentId)));
        eventParameters.push_back(std::pair<std::string, std::string>("CollisionOpponentId", std::to_string(collisionOpponentId)));

        return eventParameters;
    }


    bool collisionWithAgent;
    int collisionAgentId;
    int collisionOpponentId;
};

