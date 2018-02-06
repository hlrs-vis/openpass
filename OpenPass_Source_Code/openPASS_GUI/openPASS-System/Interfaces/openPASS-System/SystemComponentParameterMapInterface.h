/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERMAPINTERFACE_H
#define SYSTEMCOMPONENTPARAMETERMAPINTERFACE_H

#include "openPASS-System/SystemComponentParameterItemInterface.h"
#include <QMap>
#include <QObject>

class SystemComponentParameterMapInterface : public QObject
{
    Q_OBJECT

public:
    using ID = SystemComponentParameterItemInterface::ID;
    using Item = SystemComponentParameterItemInterface;

public:
    using Map = QMap<ID, Item *>;
    using Iterator = Map::Iterator;
    using ConstIterator = Map::ConstIterator;

public:
    explicit SystemComponentParameterMapInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentParameterMapInterface() = default;

public:
    virtual int count() const = 0;

public:
    virtual SystemComponentParameterMapInterface::Iterator begin() = 0;
    virtual SystemComponentParameterMapInterface::ConstIterator begin() const = 0;

public:
    virtual SystemComponentParameterMapInterface::Iterator end() = 0;
    virtual SystemComponentParameterMapInterface::ConstIterator end() const = 0;

public:
    virtual SystemComponentParameterMapInterface::ID getID(SystemComponentParameterMapInterface::Item * const item) const = 0;
    virtual SystemComponentParameterMapInterface::Item * getItem(SystemComponentParameterMapInterface::ID const & id) const = 0;
};

#endif // SYSTEMCOMPONENTPARAMETERMAPINTERFACE_H
