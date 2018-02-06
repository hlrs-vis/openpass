/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERITEMINTERFACE_H
#define SYSTEMCOMPONENTPARAMETERITEMINTERFACE_H

#include "openPASS-Component/ComponentParameterItemInterface.h"
#include <QObject>

class SystemComponentParameterItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = ComponentParameterItemInterface::ID;
    using Type = ComponentParameterItemInterface::Type;
    using Title = ComponentParameterItemInterface::Title;
    using Unit = ComponentParameterItemInterface::Unit;
    using Value = ComponentParameterItemInterface::Value;

public:
    explicit SystemComponentParameterItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentParameterItemInterface() = default;

Q_SIGNALS:
    void modifiedValue() const;

public:
    virtual SystemComponentParameterItemInterface::ID getID() const = 0;
    virtual SystemComponentParameterItemInterface::Type getType() const = 0;
    virtual SystemComponentParameterItemInterface::Title getTitle() const = 0;
    virtual SystemComponentParameterItemInterface::Unit getUnit() const = 0;

public:
    virtual bool setValue(SystemComponentParameterItemInterface::Value const & value) = 0;
    virtual SystemComponentParameterItemInterface::Value getValue() const = 0;
};

#endif // SYSTEMCOMPONENTPARAMETERITEMINTERFACE_H
