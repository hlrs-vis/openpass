/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTOUTPUTMAPINTERFACE_H
#define SYSTEMCOMPONENTOUTPUTMAPINTERFACE_H

#include "openPASS-System/SystemComponentOutputItemInterface.h"
#include <QMap>
#include <QObject>

class SystemComponentOutputMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = SystemComponentOutputItemInterface::ID;
    using Item = SystemComponentOutputItemInterface;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit SystemComponentOutputMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentOutputMapInterface() = default;

public:
    virtual SystemComponentOutputMapInterface::Iterator begin() = 0;
    virtual SystemComponentOutputMapInterface::ConstIterator begin() const = 0;

public:
    virtual SystemComponentOutputMapInterface::Iterator end() = 0;
    virtual SystemComponentOutputMapInterface::ConstIterator end() const = 0;

public:
    virtual SystemComponentOutputMapInterface::ID getID(SystemComponentOutputMapInterface::Item * const item) const = 0;
    virtual SystemComponentOutputMapInterface::Item * getItem(SystemComponentOutputMapInterface::ID const & id) const = 0;
};

#endif // SYSTEMCOMPONENTOUTPUTMAPMODEL_H
