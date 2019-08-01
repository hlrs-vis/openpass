/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#ifndef JUNCTIONINTERFACE
#define JUNCTIONINTERFACE


#include "Interfaces/roadInterface/connectionInterface.h"
#include <map>

//-----------------------------------------------------------------------------
//! Class representing a junction
//-----------------------------------------------------------------------------
class JunctionInterface
{
public:
    JunctionInterface() = default;
    JunctionInterface(const JunctionInterface&) = delete;
    JunctionInterface(JunctionInterface&&) = delete;
    JunctionInterface& operator=(const JunctionInterface&) = delete;
    JunctionInterface& operator=(JunctionInterface&&) = delete;
    virtual ~JunctionInterface() = default;


    virtual ConnectionInterface* AddConnection(std::string id,std::string incomingRoadId, std::string connectingRoadId, ContactPointType conatctPoint) = 0;


    virtual std::map<std::string, ConnectionInterface*> GetConnections() const = 0;

    virtual std::string GetId() const = 0;


private:

};


#endif // JUNCTIONINTERFACE

