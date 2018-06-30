/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCONNECTIONMAPINTERFACE_H
#define SYSTEMCONNECTIONMAPINTERFACE_H

#include "openPASS-System/SystemComponentItemInterface.h"
#include "openPASS-System/SystemConnectionItemInterface.h"
#include <QList>
#include <QMap>
#include <QObject>

class SystemConnectionMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = SystemConnectionItemInterface::ID;
    using Item = SystemConnectionItemInterface;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    using Component = SystemComponentItemInterface;
    using Source = SystemConnectionItemInterface::Source;
    using Target = SystemConnectionItemInterface::Target;

public:
    explicit SystemConnectionMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemConnectionMapInterface() = default;

Q_SIGNALS:
    void mode(bool const & modify) const;

Q_SIGNALS:
    void modified() const;

public:
    virtual SystemConnectionMapInterface::Iterator begin() = 0;
    virtual SystemConnectionMapInterface::ConstIterator begin() const = 0;

public:
    virtual SystemConnectionMapInterface::Iterator end() = 0;
    virtual SystemConnectionMapInterface::ConstIterator end() const = 0;

public:
    virtual SystemConnectionMapInterface::ID getID(SystemConnectionMapInterface::Item * const item) const = 0;
    virtual SystemConnectionMapInterface::Item * getItem(SystemConnectionMapInterface::ID const & id) const = 0;

public:
    virtual bool clear() = 0;
    virtual bool add(SystemConnectionMapInterface::ID const & id,
                     SystemConnectionMapInterface::Source const * const source,
                     SystemConnectionMapInterface::Target const * const target) = 0;
    virtual bool add(SystemConnectionMapInterface::Source const * const source,
                     SystemConnectionMapInterface::Target const * const target) = 0;
    virtual bool contains(SystemConnectionMapInterface::Source const * const source,
                          SystemConnectionMapInterface::Target const * const target) const = 0;
    virtual bool remove(SystemConnectionMapInterface::Component const * const component) = 0;
    virtual bool remove(SystemConnectionMapInterface::Source const * const source) = 0;
    virtual bool remove(SystemConnectionMapInterface::Target const * const target) = 0;
};

#endif // SYSTEMCONNECTIONMAPINTERFACE_H
