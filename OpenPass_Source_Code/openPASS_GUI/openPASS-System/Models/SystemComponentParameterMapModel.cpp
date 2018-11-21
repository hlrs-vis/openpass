/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentParameterMapModel.h"

#include "Models/SystemComponentParameterItemModel.h"

SystemComponentParameterMapModel::SystemComponentParameterMapModel(ComponentParameterMapInterface const * const _parameters,
                                                                   QObject * const parent)
    : SystemComponentParameterMapInterface(parent)
{
    for (ComponentParameterItemInterface const * const parameter : *_parameters)
    {
        parameters.insert(parameter->getID(), new SystemComponentParameterItemModel(parameter, this));
    }
}

int SystemComponentParameterMapModel::count() const
{
    return parameters.count();
}

SystemComponentParameterMapInterface::Iterator SystemComponentParameterMapModel::begin()
{
    return parameters.begin();
}

SystemComponentParameterMapInterface::ConstIterator SystemComponentParameterMapModel::begin() const
{
    return parameters.begin();
}

SystemComponentParameterMapInterface::Iterator SystemComponentParameterMapModel::end()
{
    return parameters.end();
}

SystemComponentParameterMapInterface::ConstIterator SystemComponentParameterMapModel::end() const
{
    return parameters.end();
}

SystemComponentParameterMapInterface::ID SystemComponentParameterMapModel::getID(SystemComponentParameterMapInterface::Item * const item) const
{
    return parameters.key(item);
}

SystemComponentParameterMapInterface::Item * SystemComponentParameterMapModel::getItem(SystemComponentParameterMapInterface::ID const & id) const
{
    return parameters.value(id, nullptr);
}
