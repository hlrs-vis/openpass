/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#ifndef CONNECTIONINTERFACE_H
#define CONNECTIONINTERFACE_H

#include "list"
#include "string"
#include "map"
#include "roadElementTypes.h"


class ConnectionInterface
{
public:
    ConnectionInterface() = default;
    ConnectionInterface(const ConnectionInterface&) = delete;
    ConnectionInterface(ConnectionInterface&&) = delete;
    ConnectionInterface& operator=(const ConnectionInterface&) = delete;
    ConnectionInterface& operator=(ConnectionInterface&&) = delete;
    virtual ~ConnectionInterface() = default;

    virtual void AddLink(int from, int to) = 0;

    virtual std::string GetConnectingRoadId() = 0;

    virtual std::string GetIncommingRoadId() = 0;

    virtual std::map<int,int> GetLinks() = 0;

    virtual ContactPointType GetContactPoint() = 0;


};

#endif // CONNECTIONINTERFACE_H
