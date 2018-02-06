/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTINPUTMAPINTERFACE_H
#define SYSTEMCOMPONENTINPUTMAPINTERFACE_H

#include "openPASS-System/SystemComponentInputItemInterface.h"
#include <QMap>
#include <QObject>

class SystemComponentInputMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = SystemComponentInputItemInterface::ID;
    using Item = SystemComponentInputItemInterface;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit SystemComponentInputMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentInputMapInterface() = default;

public:
    virtual SystemComponentInputMapInterface::Iterator begin() = 0;
    virtual SystemComponentInputMapInterface::ConstIterator begin() const = 0;

public:
    virtual SystemComponentInputMapInterface::Iterator end() = 0;
    virtual SystemComponentInputMapInterface::ConstIterator end() const = 0;

public:
    virtual SystemComponentInputMapInterface::ID getID(SystemComponentInputMapInterface::Item * const item) const = 0;
    virtual SystemComponentInputMapInterface::Item * getItem(SystemComponentInputMapInterface::ID const & id) const = 0;
};

#endif // SYSTEMCOMPONENTINPUTMAPINTERFACE_H
