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
//! @file  ServiceInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef SERVICEINTERFACE_H
#define SERVICEINTERFACE_H

#include "openPASS/ServiceManagerInterface.h"

constexpr ServiceManagerInterface::ID ServiceManagerInterfaceID(
        char const * string, unsigned int value = 0)
{
    return *string ? ServiceManagerInterfaceID(
                         string + 1, value + *string * *string) : value;
}

template <typename Interface, ServiceManagerInterface::ID ID>
class ServiceInterface
{
public:
    explicit ServiceInterface(ServiceManagerInterface::Service * const service)
    {
        ServiceManagerInterface::instance()->add(ID, service);
    }

    virtual ~ServiceInterface()
    {
        ServiceManagerInterface::instance()->remove(ID);
    }

public:
    static Interface * instance()
    {
        ServiceManagerInterface const * const services =
                ServiceManagerInterface::instance();
        return services ? dynamic_cast<Interface *>(services->get(ID)) : nullptr;
    }
};

#endif // SERVICEINTERFACE_H
