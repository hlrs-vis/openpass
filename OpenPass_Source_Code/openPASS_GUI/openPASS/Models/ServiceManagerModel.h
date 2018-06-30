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
//! @file  ServiceManagerModel.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef SERVICEMANAGERMODEL_H
#define SERVICEMANAGERMODEL_H

#include "openPASS/ServiceManagerInterface.h"
#include <QMap>
#include <QObject>

class ServiceManagerModel : public QObject, public ServiceManagerInterface
{
    Q_OBJECT

public:
    explicit ServiceManagerModel(QObject * const parent = nullptr);
    virtual ~ServiceManagerModel() = default;

public:
    virtual void add(ServiceManagerInterface::ID const & id,
                     ServiceManagerInterface::Service * const service) override;
    virtual bool remove(ServiceManagerInterface::ID const & id) override;
    virtual bool exists(ServiceManagerInterface::ID const & id) const override;
    virtual ServiceManagerInterface::IDList list() const override;
    virtual ServiceManagerInterface::Service * get(ServiceManagerInterface::ID const & id) const override;

protected:
    QMap<ServiceManagerInterface::ID, ServiceManagerInterface::Service *> services;
};

#endif // SERVICEMANAGERMODEL_H
