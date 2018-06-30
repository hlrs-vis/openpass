/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTOUTPUTITEMINTERFACE_H
#define SYSTEMCOMPONENTOUTPUTITEMINTERFACE_H

#include "openPASS-Component/ComponentOutputItemInterface.h"
#include <QObject>

class SystemComponentItemInterface;

class SystemComponentOutputItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = ComponentOutputItemInterface::ID;
    using Type = ComponentOutputItemInterface::Type;
    using Title = ComponentOutputItemInterface::Title;
    using Unit = ComponentOutputItemInterface::Unit;

public:
    explicit SystemComponentOutputItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentOutputItemInterface() = default;

public:
    virtual SystemComponentItemInterface * getComponent() const = 0;

public:
    virtual SystemComponentOutputItemInterface::ID getID() const = 0;
    virtual SystemComponentOutputItemInterface::Type getType() const = 0;
    virtual SystemComponentOutputItemInterface::Title getTitle() const = 0;
    virtual SystemComponentOutputItemInterface::Unit getUnit() const = 0;
};

#endif // SYSTEMCOMPONENTOUTPUTITEMINTERFACE_H
