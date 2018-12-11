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
//! @file  PluginManagerInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef PLUGINMANAGERINTERFACE_H
#define PLUGINMANAGERINTERFACE_H

#include "openPASS/ServiceInterface.h"
#include <QDir>
#include <QObject>
#include <QString>
#include <opExport.h>

static ServiceManagerInterface::ID const PluginManagerInterfaceID =
        ServiceManagerInterfaceID("openPASS.PluginManagerInterface");

class OPENPASSINTERFACESEXPORT PluginManagerInterface : public QObject,
        public ServiceInterface<PluginManagerInterface, PluginManagerInterfaceID>
{
    Q_OBJECT

public:
    explicit PluginManagerInterface(QObject * const parent = nullptr)
        : QObject(parent), ServiceInterface(this) {}
    virtual ~PluginManagerInterface() = default;

Q_SIGNALS:
    void started() const;

public:
    virtual bool loadDirectory(QDir const & directory) = 0;
    virtual bool isLoaded(QString const & iid) const = 0;
};

#endif // PLUGINMANAGERINTERFACE_H
