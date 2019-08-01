/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#ifndef JUNCTION_H
#define JUNCTION_H


#include "Interfaces/roadInterface/junctionInterface.h"
#include "connection.h"

#include <list>
#include <map>
#include <stdio.h>


class Junction : public JunctionInterface
{
public:
    Junction(const std::string &id)
    {
        this->id = id;
    }
    ~Junction();

    virtual ConnectionInterface* AddConnection(std::string id,std::string incomingRoadId, std::string connectingRoadId, ContactPointType contactPoint);

    virtual std::map<std::string, ConnectionInterface*> GetConnections() const override;

    std::string GetId() const override;

private:
    std::string id;
    std::map<std::string, ConnectionInterface*> connections;
};

#endif // JUNCTION_H
