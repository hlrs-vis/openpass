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
//! @file  SystemComponentManagerInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef SYSTEMCOMPONENTMANAGERINTERFACE_H
#define SYSTEMCOMPONENTMANAGERINTERFACE_H

#include "openPASS-Component/ComponentItemInterface.h"
#include <QDir>
#include <QList>
#include <QObject>

class SystemComponentManagerInterface : public QObject
{
    Q_OBJECT

public:
    explicit SystemComponentManagerInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentManagerInterface() = default;

public:
    virtual bool loadFromDirectory(QDir const & directory) = 0;
    virtual QList<ComponentItemInterface::Title> listTitlesByType(ComponentItemInterface::Type const & type) const = 0;
    virtual ComponentItemInterface * lookupItemByName(ComponentItemInterface::Name const & name) const = 0;
    virtual ComponentItemInterface * lookupItemByTitle(ComponentItemInterface::Title const & title) const = 0;
};

#endif // SYSTEMCOMPONENTMANAGERINTERFACE_H
