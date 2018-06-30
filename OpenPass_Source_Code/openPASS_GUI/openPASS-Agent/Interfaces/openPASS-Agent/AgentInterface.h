/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

//-----------------------------------------------------------------------------
//! @file  AgentInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef AGENTINTERFACE_H
#define AGENTINTERFACE_H

#include "openPASS/ServiceInterface.h"

#include <QObject>

static ServiceManagerInterface::ID const AgentInterfaceID =
        ServiceManagerInterfaceID("openPASS.Agent.AgentInterface");

class AgentInterface : public QObject,
        public ServiceInterface<AgentInterface, AgentInterfaceID>
{
    Q_OBJECT

public:
    using Filepath = QString;

public:
    explicit AgentInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~AgentInterface() = default;

Q_SIGNALS: // update action from the Browse Button
    void update();

public:
    virtual AgentInterface::Filepath getPropertiesFile() const = 0;
};

#endif // AGENTINTERFACE_H
