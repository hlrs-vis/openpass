/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTOUTPUTMAPMODEL_H
#define COMPONENTOUTPUTMAPMODEL_H

#include "openPASS-Component/ComponentOutputMapInterface.h"

class ComponentOutputMapModel : public ComponentOutputMapInterface
{
    Q_OBJECT

public:
    explicit ComponentOutputMapModel(QObject * const parent = nullptr);
    virtual ~ComponentOutputMapModel() = default;

public:
    virtual bool add(ComponentOutputMapInterface::ID const & id) override;
    virtual bool add(ComponentOutputMapInterface::ID const & id,
                     ComponentOutputMapInterface::Item * const item) override;

public:
    virtual ComponentOutputMapInterface::Iterator begin() override;
    virtual ComponentOutputMapInterface::ConstIterator begin() const override;

public:
    virtual bool clear() override;

public:
    virtual int count() const override;

public:
    virtual bool contains(ComponentOutputMapInterface::ID const & id) const override;
    virtual bool contains(ComponentOutputMapInterface::Item * const item) const override;
    virtual bool contains(ComponentOutputMapInterface::Index const & index) const override;

public:
    virtual ComponentOutputMapInterface::Iterator end() override;
    virtual ComponentOutputMapInterface::ConstIterator end() const override;

public:
    virtual bool setID(ComponentOutputMapInterface::Item * const item,
                       ComponentOutputMapInterface::ID const & id) override;
    virtual ComponentOutputMapInterface::ID generateID() const override;
    virtual ComponentOutputMapInterface::ID getID(ComponentOutputMapInterface::Item * const item) const override;
    virtual ComponentOutputMapInterface::ID getID(ComponentOutputMapInterface::Index const & index) const override;

public:
    virtual ComponentOutputMapInterface::Index getIndex(ComponentOutputMapInterface::ID const & id) const override;
    virtual ComponentOutputMapInterface::Index getIndex(ComponentOutputMapInterface::Item * const item) const override;

public:
    virtual ComponentOutputMapInterface::Item * getItem(ComponentOutputMapInterface::ID const & id) const override;
    virtual ComponentOutputMapInterface::Item * getItem(ComponentOutputMapInterface::Index const & index) const override;

public:
    virtual bool remove(ComponentOutputMapInterface::ID const & id) override;
    virtual bool remove(ComponentOutputMapInterface::Item * const item) override;
    virtual bool remove(ComponentOutputMapInterface::Index const & index) override;

protected:
    ComponentOutputMapInterface::Map map;
};

#endif // COMPONENTOUTPUTMAPMODEL_H
