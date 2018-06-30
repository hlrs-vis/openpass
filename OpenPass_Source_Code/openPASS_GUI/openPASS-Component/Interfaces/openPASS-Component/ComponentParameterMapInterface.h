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
//! @file  ComponentParameterMapInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTPARAMETERMAPINTERFACE_H
#define COMPONENTPARAMETERMAPINTERFACE_H

#include "openPASS-Component/ComponentParameterItemInterface.h"
#include "openPASS-Component/ComponentTypedefMapInterface.h"
#include <QMap>
#include <QObject>

class ComponentParameterMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = ComponentParameterItemInterface::ID;
    using Item = ComponentParameterItemInterface;
    using Index = int;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit ComponentParameterMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentParameterMapInterface() = default;

Q_SIGNALS:
    void added() const;
    void cleared() const;
    void modifiedID() const;
    void removed() const;

public:
    virtual bool add(ComponentParameterMapInterface::ID const & id) = 0;
    virtual bool add(ComponentParameterMapInterface::ID const & id,
                     ComponentParameterMapInterface::Item * const item) = 0;

public:
    virtual ComponentParameterMapInterface::Iterator begin() = 0;
    virtual ComponentParameterMapInterface::ConstIterator begin() const = 0;

public:
    virtual bool clear() = 0;

public:
    virtual int count() const = 0;

public:
    virtual bool contains(ComponentParameterMapInterface::ID const & id) const = 0;
    virtual bool contains(ComponentParameterMapInterface::Item * const item) const = 0;
    virtual bool contains(ComponentParameterMapInterface::Index const & index) const = 0;

public:
    virtual ComponentParameterMapInterface::Iterator end() = 0;
    virtual ComponentParameterMapInterface::ConstIterator end() const = 0;

public:
    virtual bool setID(ComponentParameterMapInterface::Item * const item,
                       ComponentParameterMapInterface::ID const & id) = 0;
    virtual ComponentParameterMapInterface::ID generateID() const = 0;
    virtual ComponentParameterMapInterface::ID getID(ComponentParameterMapInterface::Item * const item) const = 0;
    virtual ComponentParameterMapInterface::ID getID(ComponentParameterMapInterface::Index const & index) const = 0;

public:
    virtual ComponentParameterMapInterface::Index getIndex(ComponentParameterMapInterface::ID const & id) const = 0;
    virtual ComponentParameterMapInterface::Index getIndex(ComponentParameterMapInterface::Item * const item) const = 0;

public:
    virtual ComponentParameterMapInterface::Item * getItem(ComponentParameterMapInterface::ID const & id) const = 0;
    virtual ComponentParameterMapInterface::Item * getItem(ComponentParameterMapInterface::Index const & index) const = 0;

public:
    virtual bool remove(ComponentParameterMapInterface::ID const & id) = 0;
    virtual bool remove(ComponentParameterMapInterface::Item * const item) = 0;
    virtual bool remove(ComponentParameterMapInterface::Index const & index) = 0;

public:
    virtual ComponentTypedefMapInterface * getParameterTypeMap() const = 0;
};

#endif // COMPONENTOUTPUTMAPINTERFACE_H
