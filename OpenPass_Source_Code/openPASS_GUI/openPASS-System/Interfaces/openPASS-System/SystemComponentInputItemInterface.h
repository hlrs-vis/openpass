/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTINPUTITEMINTERFACE_H
#define SYSTEMCOMPONENTINPUTITEMINTERFACE_H

#include "openPASS-Component/ComponentInputItemInterface.h"
#include <QObject>

class SystemComponentItemInterface;

class SystemComponentInputItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = ComponentInputItemInterface::ID;
    using Type = ComponentInputItemInterface::Type;
    using Title = ComponentInputItemInterface::Title;
    using Unit = ComponentInputItemInterface::Unit;
    using Cardinality = ComponentInputItemInterface::Cardinality;

public:
    explicit SystemComponentInputItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentInputItemInterface() = default;

public:
    virtual SystemComponentItemInterface * getComponent() const = 0;

public:
    virtual SystemComponentInputItemInterface::ID getID() const = 0;
    virtual SystemComponentInputItemInterface::Type getType() const = 0;
    virtual SystemComponentInputItemInterface::Title getTitle() const = 0;
    virtual SystemComponentInputItemInterface::Unit getUnit() const = 0;
    virtual SystemComponentInputItemInterface::Cardinality getCardinality() const = 0;
};

#endif // SYSTEMCOMPONENTINPUTITEMINTERFACE_H
