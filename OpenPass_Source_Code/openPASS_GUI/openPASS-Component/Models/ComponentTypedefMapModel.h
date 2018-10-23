/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTTYPEDEFMAPMODEL_H
#define COMPONENTTYPEDEFMAPMODEL_H

#include "openPASS-Component/ComponentTypedefMapInterface.h"

class ComponentTypedefMapModel : public ComponentTypedefMapInterface
{
    Q_OBJECT

public:
    explicit ComponentTypedefMapModel(QObject * const parent = nullptr);
    virtual ~ComponentTypedefMapModel() = default;

public:
    virtual bool add(ComponentTypedefMapInterface::Type const &type) override;

public:
    virtual ComponentTypedefMapInterface::Iterator begin() override;
    virtual ComponentTypedefMapInterface::ConstIterator begin() const override;

public:
    virtual bool clear() override;

public:
    virtual int count() const override;

public:
    virtual bool contains(ComponentTypedefMapInterface::ID const & id) const override;
    virtual bool contains(ComponentTypedefMapInterface::Item * const item) const override;
    virtual bool contains(ComponentTypedefMapInterface::Index const & index) const override;

public:
    virtual ComponentTypedefMapInterface::Iterator end() override;
    virtual ComponentTypedefMapInterface::ConstIterator end() const override;

public:
    virtual ComponentTypedefMapInterface::ID generateID() const override;
    virtual ComponentTypedefMapInterface::ID getID(ComponentTypedefMapInterface::Item * const item) const override;
    virtual ComponentTypedefMapInterface::ID getID(ComponentTypedefMapInterface::Index const & index) const override;

public:
    virtual ComponentTypedefMapInterface::Index getIndex(ComponentTypedefMapInterface::ID const & id) const override;
    virtual ComponentTypedefMapInterface::Index getIndex(ComponentTypedefMapInterface::Item * const item) const override;

public:
    virtual ComponentTypedefMapInterface::Item * getItem(ComponentTypedefMapInterface::ID const & id) const override;
    virtual ComponentTypedefMapInterface::Item * getItem(ComponentTypedefMapInterface::Index const & index) const override;

public:
    virtual bool remove(ComponentTypedefMapInterface::ID const & id) override;
    virtual bool remove(ComponentTypedefMapInterface::Item * const item) override;
    virtual bool remove(ComponentTypedefMapInterface::Index const & index) override;

protected:
    ComponentTypedefMapInterface::Map map;
};

#endif // COMPONENTTYPEDEFMAPMODEL_H
