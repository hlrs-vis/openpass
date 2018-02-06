/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTOUTPUTITEMMODEL_H
#define COMPONENTOUTPUTITEMMODEL_H

#include "openPASS-Component/ComponentOutputItemInterface.h"

class ComponentOutputItemModel : public ComponentOutputItemInterface
{
    Q_OBJECT

public:
    explicit ComponentOutputItemModel(QObject * const parent = nullptr);
    virtual ~ComponentOutputItemModel() = default;

public:
    virtual bool clear() override;

public:
    virtual bool setID(ComponentOutputItemInterface::ID const & id) override;
    virtual ComponentOutputItemInterface::ID getID() const override;

public:
    virtual bool setType(ComponentOutputItemInterface::Type const & type) override;
    virtual ComponentOutputItemInterface::Type getType() const override;

public:
    virtual bool setTitle(ComponentOutputItemInterface::Title const & title) override;
    virtual ComponentOutputItemInterface::Title getTitle() const override;

public:
    virtual bool setUnit(ComponentOutputItemInterface::Unit const & unit) override;
    virtual ComponentOutputItemInterface::Unit getUnit() const override;

protected:
    static ComponentOutputItemInterface::Type const DefaultType;
    static ComponentOutputItemInterface::Title const DefaultTitle;
    static ComponentOutputItemInterface::Unit const DefaultUnit;

protected:
    ComponentOutputItemInterface::Type type;
    ComponentOutputItemInterface::Title title;
    ComponentOutputItemInterface::Unit unit;
};

#endif // COMPONENTOUTPUTITEMMODEL_H
