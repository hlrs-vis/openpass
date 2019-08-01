/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#ifndef JUNCTIONCONNECTION_H
#define JUNCTIONCONNECTION_H


#include "Interfaces/roadInterface/connectionInterface.h"
#include <map>

class Connection : public ConnectionInterface
{
public:
    Connection(std::string id, std::string incomingRoad, std::string connectingRoad, ContactPointType contactPoint);
    ~Connection();

    void virtual AddLink(int from, int to);

    virtual std::string GetConnectingRoadId();

    virtual std::string GetIncommingRoadId();

    virtual std::map<int, int> GetLinks();

    virtual ContactPointType GetContactPoint();


private:
    std::string id;
    std::string incommingRoadId;
    std::string connectingRoadId;
    ContactPointType contactPoint;
    std::map<int,int> links;
};

#endif // JUNCTIONCONNECTION_H
