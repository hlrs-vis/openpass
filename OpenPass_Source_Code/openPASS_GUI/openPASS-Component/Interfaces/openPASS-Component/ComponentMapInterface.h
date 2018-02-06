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
//! @file  ComponentMapInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTMAPINTERFACE_H
#define COMPONENTMAPINTERFACE_H

#include "openPASS-Component/ComponentItemInterface.h"
#include <QMap>
#include <QObject>
#include <QString>

class ComponentMapInterface : public QObject
{
    Q_OBJECT

public:
    using Name = ComponentItemInterface::Name;
    using Item = ComponentItemInterface;
    using Index = int;

public:
    using Map = QMap<Name, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit ComponentMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentMapInterface() = default;

Q_SIGNALS:
    void added() const;
    void modifiedName() const;
    void removed() const;

public:
    virtual bool add(ComponentMapInterface::Name const & name) = 0;
    virtual bool add(ComponentMapInterface::Name const & name,
                     ComponentMapInterface::Item * const item) = 0;
    virtual bool add(ComponentMapInterface::Name const & name,
                     QString const & filepath) = 0;

public:
    virtual ComponentMapInterface::Iterator begin() = 0;
    virtual ComponentMapInterface::ConstIterator begin() const = 0;

public:
    virtual bool clear() = 0;

public:
    virtual int count() const = 0;

public:
    virtual bool contains(ComponentMapInterface::Name const & name) const = 0;
    virtual bool contains(ComponentMapInterface::Item * const item) const = 0;
    virtual bool contains(ComponentMapInterface::Index const & index) const = 0;

public:
    virtual ComponentMapInterface::Iterator end() = 0;
    virtual ComponentMapInterface::ConstIterator end() const = 0;

public:
    virtual bool setName(ComponentMapInterface::Item * const item,
                         ComponentMapInterface::Name const & name) = 0;
    virtual ComponentMapInterface::Name getName(ComponentMapInterface::Item * const item) const = 0;
    virtual ComponentMapInterface::Name getName(ComponentMapInterface::Index const & index) const = 0;

public:
    virtual ComponentMapInterface::Index getIndex(ComponentMapInterface::Name const & name) const = 0;
    virtual ComponentMapInterface::Index getIndex(ComponentMapInterface::Item * const item) const = 0;

public:
    virtual ComponentMapInterface::Item * getItem(ComponentMapInterface::Name const & name) const = 0;
    virtual ComponentMapInterface::Item * getItem(ComponentMapInterface::Index const & index) const = 0;

public:
    virtual bool remove(ComponentMapInterface::Name const & name) = 0;
    virtual bool remove(ComponentMapInterface::Item * const item) = 0;
    virtual bool remove(ComponentMapInterface::Index const & index) = 0;
};

#endif // COMPONENTMAPINTERFACE_H
