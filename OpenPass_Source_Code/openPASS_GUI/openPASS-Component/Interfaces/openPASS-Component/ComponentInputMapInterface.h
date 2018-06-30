/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

//-----------------------------------------------------------------------------
//! @file  ComponentInputMapInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTINPUTMAPINTERFACE_H
#define COMPONENTINPUTMAPINTERFACE_H

#include "openPASS-Component/ComponentInputItemInterface.h"
#include <QMap>
#include <QObject>

class ComponentInputMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = ComponentInputItemInterface::ID;
    using Item = ComponentInputItemInterface;
    using Index = int;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit ComponentInputMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentInputMapInterface() = default;

Q_SIGNALS:
    void added() const;
    void cleared() const;
    void modifiedID() const;
    void removed() const;

public:
    virtual bool add(ComponentInputMapInterface::ID const & id) = 0;
    virtual bool add(ComponentInputMapInterface::ID const & id,
                     ComponentInputMapInterface::Item * const item) = 0;

public:
    virtual ComponentInputMapInterface::Iterator begin() = 0;
    virtual ComponentInputMapInterface::ConstIterator begin() const = 0;

public:
    virtual bool clear() = 0;

public:
    virtual int count() const = 0;

public:
    virtual bool contains(ComponentInputMapInterface::ID const & id) const = 0;
    virtual bool contains(ComponentInputMapInterface::Item * const item) const = 0;
    virtual bool contains(ComponentInputMapInterface::Index const & index) const = 0;

public:
    virtual ComponentInputMapInterface::Iterator end() = 0;
    virtual ComponentInputMapInterface::ConstIterator end() const = 0;

public:
    virtual bool setID(ComponentInputMapInterface::Item * const item,
                       ComponentInputMapInterface::ID const & id) = 0;
    virtual ComponentInputMapInterface::ID generateID() const = 0;
    virtual ComponentInputMapInterface::ID getID(ComponentInputMapInterface::Item * const item) const = 0;
    virtual ComponentInputMapInterface::ID getID(ComponentInputMapInterface::Index const & index) const = 0;

public:
    virtual ComponentInputMapInterface::Index getIndex(ComponentInputMapInterface::ID const & id) const = 0;
    virtual ComponentInputMapInterface::Index getIndex(ComponentInputMapInterface::Item * const item) const = 0;

public:
    virtual ComponentInputMapInterface::Item * getItem(ComponentInputMapInterface::ID const & id) const = 0;
    virtual ComponentInputMapInterface::Item * getItem(ComponentInputMapInterface::Index const & index) const = 0;

public:
    virtual bool remove(ComponentInputMapInterface::ID const & id) = 0;
    virtual bool remove(ComponentInputMapInterface::Item * const item) = 0;
    virtual bool remove(ComponentInputMapInterface::Index const & index) = 0;
};

#endif // COMPONENTINPUTMAPINTERFACE_H
