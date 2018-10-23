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
//! @file  ComponentInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTINTERFACE_H
#define COMPONENTINTERFACE_H

#include "openPASS/ServiceInterface.h"
#include "openPASS-Component/ComponentItemInterface.h"
#include "openPASS-Component/ComponentMapInterface.h"
#include <QObject>

static ServiceManagerInterface::ID const ComponentInterfaceID =
        ServiceManagerInterfaceID("openPASS.Component.ComponentInterface");

class ComponentInterface : public QObject,
        public ServiceInterface<ComponentInterface, ComponentInterfaceID>
{
    Q_OBJECT

public:
    using Item = ComponentItemInterface;
    using Map = ComponentMapInterface;

public:
    explicit ComponentInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~ComponentInterface() = default;

public:
    virtual ComponentInterface::Item * createComponentItem(QObject * const parent) const = 0;
    virtual ComponentInterface::Map * createComponentMap(QObject * const parent) const = 0;
};

#endif // COMPONENTINTERFACE_H
