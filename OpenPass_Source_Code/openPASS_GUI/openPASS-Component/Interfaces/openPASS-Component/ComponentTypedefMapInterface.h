/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTTYPEDEFMAPINTERFACE_H
#define COMPONENTTYPEDEFMAPINTERFACE_H

#include "openPASS-Component/ComponentTypedefItemInterface.h"
#include <QObject>
#include <QMap>

class ComponentTypedefMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = ComponentTypedefItemInterface::ID;
    using Item = ComponentTypedefItemInterface;
    using Index = int;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    using Type = ComponentTypedefItemInterface::Type;

public:
    explicit ComponentTypedefMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentTypedefMapInterface() = default;

public:
    virtual bool add(ComponentTypedefMapInterface::Type const &type) = 0;

public:
    virtual ComponentTypedefMapInterface::Iterator begin() = 0;
    virtual ComponentTypedefMapInterface::ConstIterator begin() const = 0;

public:
    virtual bool clear() = 0;

public:
    virtual int count() const = 0;

public:
    virtual bool contains(ComponentTypedefMapInterface::ID const & id) const = 0;
    virtual bool contains(ComponentTypedefMapInterface::Item * const item) const = 0;
    virtual bool contains(ComponentTypedefMapInterface::Index const & index) const = 0;

public:
    virtual ComponentTypedefMapInterface::Iterator end() = 0;
    virtual ComponentTypedefMapInterface::ConstIterator end() const = 0;

public:
    virtual ComponentTypedefMapInterface::ID generateID() const = 0;
    virtual ComponentTypedefMapInterface::ID getID(ComponentTypedefMapInterface::Item * const item) const = 0;
    virtual ComponentTypedefMapInterface::ID getID(ComponentTypedefMapInterface::Index const & index) const = 0;

public:
    virtual ComponentTypedefMapInterface::Index getIndex(ComponentTypedefMapInterface::ID const & id) const = 0;
    virtual ComponentTypedefMapInterface::Index getIndex(ComponentTypedefMapInterface::Item * const item) const = 0;

public:
    virtual ComponentTypedefMapInterface::Item * getItem(ComponentTypedefMapInterface::ID const & id) const = 0;
    virtual ComponentTypedefMapInterface::Item * getItem(ComponentTypedefMapInterface::Index const & index) const = 0;

public:
    virtual bool remove(ComponentTypedefMapInterface::ID const & id) = 0;
    virtual bool remove(ComponentTypedefMapInterface::Item * const item) = 0;
    virtual bool remove(ComponentTypedefMapInterface::Index const & index) = 0;
};

#endif // COMPONENTTYPEDEFMAPINTERFACE_H
