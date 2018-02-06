/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTPARAMETERITEMMODEL_H
#define COMPONENTPARAMETERITEMMODEL_H

#include "openPASS-Component/ComponentParameterItemInterface.h"

class ComponentParameterItemModel : public ComponentParameterItemInterface
{
    Q_OBJECT

public:
    explicit ComponentParameterItemModel(QObject * const parent = nullptr);
    virtual ~ComponentParameterItemModel() = default;

public:
    virtual bool clear() override;

public:
    virtual bool setID(ComponentParameterItemInterface::ID const & id) override;
    virtual ComponentParameterItemInterface::ID getID() const override;

public:
    virtual bool setType(ComponentParameterItemInterface::Type const & type) override;
    virtual ComponentParameterItemInterface::Type getType() const override;

public:
    virtual bool setTitle(ComponentParameterItemInterface::Title const & title) override;
    virtual ComponentParameterItemInterface::Title getTitle() const override;

public:
    virtual bool setUnit(ComponentParameterItemInterface::Unit const & unit) override;
    virtual ComponentParameterItemInterface::Unit getUnit() const override;

public:
    virtual bool setValue(ComponentParameterItemInterface::Value const & value) override;
    virtual ComponentParameterItemInterface::Value getValue() const override;

protected:
    static ComponentParameterItemInterface::Type const DefaultType;
    static ComponentParameterItemInterface::Title const DefaultTitle;
    static ComponentParameterItemInterface::Unit const DefaultUnit;
    static ComponentParameterItemInterface::Value const DefaultValue;

protected:
    ComponentParameterItemInterface::Type type;
    ComponentParameterItemInterface::Title title;
    ComponentParameterItemInterface::Unit unit;
    ComponentParameterItemInterface::Value value;
};

#endif // COMPONENTPARAMETERITEMMODEL_H
