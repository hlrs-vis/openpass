/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTMAPMODEL_H
#define COMPONENTMAPMODEL_H

#include "openPASS-Component/ComponentMapInterface.h"

class ComponentMapModel : public ComponentMapInterface
{
    Q_OBJECT

public:
    explicit ComponentMapModel(QObject * const parent = nullptr);
    virtual ~ComponentMapModel() = default;

public:
    virtual bool add(ComponentMapInterface::Name const & name) override;
    virtual bool add(ComponentMapInterface::Name const & name,
                     ComponentMapInterface::Item * const item) override;
    virtual bool add(ComponentMapInterface::Name const & name,
                     QString const & filepath) override;

public:
    virtual ComponentMapInterface::Iterator begin() override;
    virtual ComponentMapInterface::ConstIterator begin() const override;

public:
    virtual bool clear() override;

public:
    virtual int count() const override;

public:
    virtual bool contains(ComponentMapInterface::Name const & name) const override;
    virtual bool contains(ComponentMapInterface::Item * const item) const override;
    virtual bool contains(ComponentMapInterface::Index const & index) const override;

public:
    virtual ComponentMapInterface::Iterator end() override;
    virtual ComponentMapInterface::ConstIterator end() const override;

public:
    virtual bool setName(ComponentMapInterface::Item * const item,
                         ComponentMapInterface::Name const & name) override;
    virtual ComponentMapInterface::Name getName(ComponentMapInterface::Item * const item) const override;
    virtual ComponentMapInterface::Name getName(ComponentMapInterface::Index const & index) const override;

public:
    virtual ComponentMapInterface::Index getIndex(ComponentMapInterface::Name const & name) const override;
    virtual ComponentMapInterface::Index getIndex(ComponentMapInterface::Item * const item) const override;

public:
    virtual ComponentMapInterface::Item * getItem(ComponentMapInterface::Name const & name) const override;
    virtual ComponentMapInterface::Item * getItem(ComponentMapInterface::Index const & index) const override;

public:
    virtual bool remove(ComponentMapInterface::Name const & name) override;
    virtual bool remove(ComponentMapInterface::Item * const item) override;
    virtual bool remove(ComponentMapInterface::Index const & index) override;

protected:
    ComponentMapInterface::Map map;
};

#endif // COMPONENTMAPMODEL_H
