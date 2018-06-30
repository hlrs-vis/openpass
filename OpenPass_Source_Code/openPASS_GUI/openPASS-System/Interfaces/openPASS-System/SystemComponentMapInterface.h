/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTMAPINTERFACE_H
#define SYSTEMCOMPONENTMAPINTERFACE_H

#include "openPASS-System/SystemComponentItemInterface.h"
#include <QMap>
#include <QObject>

class SystemComponentMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = SystemComponentItemInterface::ID;
    using Item = SystemComponentItemInterface;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit SystemComponentMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentMapInterface() = default;

Q_SIGNALS:
    void mode(bool const & modify) const;

Q_SIGNALS:
    void cleared() const;
    void added(SystemComponentMapInterface::Item * const component) const;
    void removed(SystemComponentMapInterface::Item * const component) const;

public:
    virtual SystemComponentMapInterface::Iterator begin() = 0;
    virtual SystemComponentMapInterface::ConstIterator begin() const = 0;

public:
    virtual SystemComponentMapInterface::Iterator end() = 0;
    virtual SystemComponentMapInterface::ConstIterator end() const = 0;

public:
    virtual SystemComponentMapInterface::ID getID(SystemComponentMapInterface::Item * const item) const = 0;
    virtual SystemComponentMapInterface::Item * getItem(SystemComponentMapInterface::ID const & id) const = 0;

public:
    virtual bool clear() = 0;
    virtual bool add(SystemComponentMapInterface::ID const & id,
                     SystemComponentMapInterface::Item * const component) = 0;
    virtual bool add(ComponentItemInterface const * const component,
                     SystemComponentItemInterface::Position const & position) = 0;
    virtual bool remove(SystemComponentMapInterface::ID const & id) = 0;
};

#endif // SYSTEMCOMPONENTMAPINTERFACE_H
