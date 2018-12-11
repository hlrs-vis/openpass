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
//! @file  ServiceManagerInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef SERVICEMANAGERINTERFACE_H
#define SERVICEMANAGERINTERFACE_H

#include <QList>
#include <QObject>
#include <opExport.h>

class OPENPASSINTERFACESEXPORT ServiceManagerInterface
{
public:
    using ID = unsigned int;
    using Service = QObject;

public:
    using IDList = QList<ServiceManagerInterface::ID>;

public:
    virtual ~ServiceManagerInterface() = default;

public:
    virtual void add(ServiceManagerInterface::ID const & id,
                     ServiceManagerInterface::Service * const service) = 0;
    virtual bool remove(ServiceManagerInterface::ID const & id) = 0;
    virtual bool exists(ServiceManagerInterface::ID const & id) const = 0;
    virtual ServiceManagerInterface::IDList list() const = 0;
    virtual ServiceManagerInterface::Service * get(ServiceManagerInterface::ID const & id) const = 0;

public:
    static ServiceManagerInterface * instance(ServiceManagerInterface * const _services = nullptr)
    {
        static ServiceManagerInterface * services = nullptr;
        if (_services != nullptr)
        {
            services = _services;
        }
        return services;
    }
};

#endif // SERVICEMANAGERINTERFACE_H
