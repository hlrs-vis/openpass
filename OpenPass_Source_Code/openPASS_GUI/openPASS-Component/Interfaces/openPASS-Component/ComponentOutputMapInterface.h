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
//! @file  ComponentOutputMapInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTOUTPUTMAPINTERFACE_H
#define COMPONENTOUTPUTMAPINTERFACE_H

#include "openPASS-Component/ComponentOutputItemInterface.h"
#include <QMap>
#include <QObject>

class ComponentOutputMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = ComponentOutputItemInterface::ID;
    using Item = ComponentOutputItemInterface;
    using Index = int;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit ComponentOutputMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentOutputMapInterface() = default;

Q_SIGNALS:
    void added() const;
    void cleared() const;
    void modifiedID() const;
    void removed() const;

public:
    virtual bool add(ComponentOutputMapInterface::ID const & id) = 0;
    virtual bool add(ComponentOutputMapInterface::ID const & id,
                     ComponentOutputMapInterface::Item * const item) = 0;

public:
    virtual ComponentOutputMapInterface::Iterator begin() = 0;
    virtual ComponentOutputMapInterface::ConstIterator begin() const = 0;

public:
    virtual bool clear() = 0;

public:
    virtual int count() const = 0;

public:
    virtual bool contains(ComponentOutputMapInterface::ID const & id) const = 0;
    virtual bool contains(ComponentOutputMapInterface::Item * const item) const = 0;
    virtual bool contains(ComponentOutputMapInterface::Index const & index) const = 0;

public:
    virtual ComponentOutputMapInterface::Iterator end() = 0;
    virtual ComponentOutputMapInterface::ConstIterator end() const = 0;

public:
    virtual bool setID(ComponentOutputMapInterface::Item * const item,
                       ComponentOutputMapInterface::ID const & id) = 0;
    virtual ComponentOutputMapInterface::ID generateID() const = 0;
    virtual ComponentOutputMapInterface::ID getID(ComponentOutputMapInterface::Item * const item) const = 0;
    virtual ComponentOutputMapInterface::ID getID(ComponentOutputMapInterface::Index const & index) const = 0;

public:
    virtual ComponentOutputMapInterface::Index getIndex(ComponentOutputMapInterface::ID const & id) const = 0;
    virtual ComponentOutputMapInterface::Index getIndex(ComponentOutputMapInterface::Item * const item) const = 0;

public:
    virtual ComponentOutputMapInterface::Item * getItem(ComponentOutputMapInterface::ID const & id) const = 0;
    virtual ComponentOutputMapInterface::Item * getItem(ComponentOutputMapInterface::Index const & index) const = 0;

public:
    virtual bool remove(ComponentOutputMapInterface::ID const & id) = 0;
    virtual bool remove(ComponentOutputMapInterface::Item * const item) = 0;
    virtual bool remove(ComponentOutputMapInterface::Index const & index) = 0;
};

#endif // COMPONENTOUTPUTMAPINTERFACE_H
