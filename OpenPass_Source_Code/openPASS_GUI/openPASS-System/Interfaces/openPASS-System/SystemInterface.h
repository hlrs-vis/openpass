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
//! @file  SystemInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef SYSTEMINTERFACE_H
#define SYSTEMINTERFACE_H

#include "openPASS/ServiceInterface.h"
#include "openPASS-System/SystemComponentManagerInterface.h"
#include "openPASS-System/SystemMapInterface.h"
#include <QIODevice>
#include <QObject>
#include <QString>

static ServiceManagerInterface::ID const SystemInterfaceID =
        ServiceManagerInterfaceID("openPASS.System.SystemInterface");

class SystemInterface : public QObject,
        public ServiceInterface<SystemInterface, SystemInterfaceID>
{
    Q_OBJECT

public:
    explicit SystemInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~SystemInterface() = default;

Q_SIGNALS:
    void cleared() const;
    void loaded() const;
    void saved() const;

public:
    virtual bool clear() = 0;
    virtual bool load(QString const & filepath) = 0;
    virtual bool load(QIODevice * const device) = 0;
    virtual bool save(QString const & filepath) const = 0;
    virtual bool save(QIODevice * const device) const = 0;

public:
    virtual SystemComponentManagerInterface * getComponents() const = 0;
    virtual SystemMapInterface * getSystems() const = 0;
};

#endif // SYSTEMINTERFACE_H
