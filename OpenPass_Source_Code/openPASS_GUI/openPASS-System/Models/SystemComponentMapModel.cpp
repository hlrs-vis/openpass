/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentMapModel.h"

#include "Models/SystemComponentItemModel.h"
#include "Models/SystemConnectionMapModel.h"

SystemComponentMapModel::SystemComponentMapModel(SystemConnectionMapInterface * const connections,
                                                 QObject * const parent)
    : SystemComponentMapInterface(parent)
    , connections(connections)
{
}

SystemComponentMapInterface::Iterator SystemComponentMapModel::begin()
{
    return components.begin();
}

SystemComponentMapInterface::ConstIterator SystemComponentMapModel::begin() const
{
    return components.begin();
}

SystemComponentMapInterface::Iterator SystemComponentMapModel::end()
{
    return components.end();
}

SystemComponentMapInterface::ConstIterator SystemComponentMapModel::end() const
{
    return components.end();
}

SystemComponentMapInterface::ID SystemComponentMapModel::getID(SystemComponentMapInterface::Item * const item) const
{
    return components.key(item);
}

SystemComponentMapInterface::Item * SystemComponentMapModel::getItem(SystemComponentMapInterface::ID const & id) const
{
    return components.value(id, nullptr);
}

bool SystemComponentMapModel::clear()
{
    if (connections->clear())
    {
        qDeleteAll(components);
        components.clear();
        Q_EMIT cleared();
        return true;
    }
    return false;
}

bool SystemComponentMapModel::add(SystemComponentMapInterface::ID const & id,
                                  SystemComponentMapInterface::Item * const component)
{
    if (!components.keys().contains(id))
    {
        component->setParent(this);
        Q_EMIT added(*components.insert(id, component));
        return true;
    }
    return false;
}

bool SystemComponentMapModel::add(ComponentItemInterface const * const component,
                                  SystemComponentItemInterface::Position const & position)
{
    Q_EMIT added(*components.insert(next(), new SystemComponentItemModel(
                                        component, position, this)));
    return true;
}

bool SystemComponentMapModel::remove(SystemComponentMapInterface::ID const & id)
{
    if (components.contains(id))
    {
        SystemComponentMapInterface::Item * const component = components.take(id);
        connections->remove(component);
        Q_EMIT removed(component);
        delete component;
        return true;
    }
    return false;
}

bool SystemComponentMapModel::duplicate(SystemComponentItemModel const * const component,
                                        SystemComponentItemInterface::Position const & position)
{
    Q_EMIT added(*components.insert(next(), new SystemComponentItemModel(
                                        *component, position, this)));
    return true;
}

SystemComponentMapInterface::ID SystemComponentMapModel::next() const
{
    SystemComponentMapInterface::ID id = 0;
    for (SystemComponentMapInterface::ID key : components.keys())
    {
        if (id <= key)
        {
            id = key + 1;
        }
    }
    return id;
}
