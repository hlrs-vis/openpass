/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemMapModel.h"

#include "Models/SystemItemModel.h"

SystemMapModel::SystemMapModel(QObject * const parent)
    : SystemMapInterface(parent)
{
    // Create default system
    systems.insert(0, new SystemItemModel(this));
}

SystemMapInterface::Iterator SystemMapModel::begin()
{
    return systems.begin();
}

SystemMapInterface::ConstIterator SystemMapModel::begin() const
{
    return systems.begin();
}

bool SystemMapModel::clear()
{
    // Clear components (and connections) of first system
    return systems.first()->getComponents()->clear();
}

SystemMapInterface::Iterator SystemMapModel::end()
{
    return systems.end();
}

SystemMapInterface::ConstIterator SystemMapModel::end() const
{
    return systems.end();
}

SystemMapInterface::ID SystemMapModel::getID(SystemMapInterface::Item * const item) const
{
    return systems.key(item);
}

SystemMapInterface::Item * SystemMapModel::getItem(SystemMapInterface::ID const & id) const
{
    return systems.value(id, nullptr);
}
