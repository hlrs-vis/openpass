/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTINPUTITEMMODEL_H
#define COMPONENTINPUTITEMMODEL_H

#include "openPASS-Component/ComponentInputItemInterface.h"

class ComponentInputItemModel : public ComponentInputItemInterface
{
    Q_OBJECT

public:
    explicit ComponentInputItemModel(QObject * const parent = nullptr);
    virtual ~ComponentInputItemModel() = default;

public:
    virtual bool clear() override;

public:
    virtual bool setID(ComponentInputItemInterface::ID const & id) override;
    virtual ComponentInputItemInterface::ID getID() const override;

public:
    virtual bool setType(ComponentInputItemInterface::Type const & type) override;
    virtual ComponentInputItemInterface::Type getType() const override;

public:
    virtual bool setTitle(ComponentInputItemInterface::Title const & title) override;
    virtual ComponentInputItemInterface::Title getTitle() const override;

public:
    virtual bool setUnit(ComponentInputItemInterface::Unit const & unit) override;
    virtual ComponentInputItemInterface::Unit getUnit() const override;

public:
    virtual bool setCardinality(ComponentInputItemInterface::Cardinality const & cardinality) override;
    virtual ComponentInputItemInterface::Cardinality getCardinality() const override;

protected:
    static ComponentInputItemInterface::Type const DefaultType;
    static ComponentInputItemInterface::Title const DefaultTitle;
    static ComponentInputItemInterface::Unit const DefaultUnit;
    static ComponentInputItemInterface::Cardinality const DefaultCardinality;

protected:
    ComponentInputItemInterface::Type type;
    ComponentInputItemInterface::Title title;
    ComponentInputItemInterface::Unit unit;
    ComponentInputItemInterface::Cardinality cardinality;
};

#endif // COMPONENTINPUTITEMMODEL_H
