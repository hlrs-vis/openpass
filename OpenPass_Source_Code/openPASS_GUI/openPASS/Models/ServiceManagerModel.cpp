/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ServiceManagerModel.h"

ServiceManagerModel::ServiceManagerModel(QObject * const parent) :
    QObject(parent)
{
    ServiceManagerInterface::instance(this);
}

void ServiceManagerModel::add(ServiceManagerInterface::ID const & id,
                              ServiceManagerInterface::Service * const service)
{
    services.insert(id, service);
}

bool ServiceManagerModel::remove(ServiceManagerInterface::ID const & id)
{
    return services.remove(id) > 0;
}

bool ServiceManagerModel::exists(ServiceManagerInterface::ID const & id) const
{
    return services.keys().contains(id);
}

ServiceManagerInterface::IDList ServiceManagerModel::list() const
{
    return services.keys();
}

ServiceManagerInterface::Service * ServiceManagerModel::get(ServiceManagerInterface::ID const & id) const
{
    return services.value(id, nullptr);
}
