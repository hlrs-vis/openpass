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
//! @file  PluginInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "openPASS/ServiceManagerInterface.h"
#include <QtPlugin>

class PluginInterface
{
public:
    virtual ~PluginInterface() = default;

public:
    virtual bool services(ServiceManagerInterface * const services)
    {
        ServiceManagerInterface::instance(services);
        return true;
    }

public:
    virtual bool initialize()
    {
        return true;
    }

    virtual bool deinitialize()
    {
        return true;
    }
};

Q_DECLARE_INTERFACE(PluginInterface, "openPASS.PluginInterface")

#endif // PLUGININTERFACE_H
