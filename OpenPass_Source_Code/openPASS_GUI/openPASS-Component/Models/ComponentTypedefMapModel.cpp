/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentTypedefMapModel.h"

#include "Models/ComponentTypedefItemModel.h"

ComponentTypedefMapModel::ComponentTypedefMapModel(QObject * const parent)
    : ComponentTypedefMapInterface(parent)
{
}

bool ComponentTypedefMapModel::add(ComponentTypedefMapInterface::Type const &type)
{
    ComponentTypedefItemModel * const item = new ComponentTypedefItemModel(type, this);
    if (!contains (item))
    {
        map.insert(generateID(), item);
        item->setParent(this);
        return true;
    }
    return false;
}

ComponentTypedefMapInterface::Iterator ComponentTypedefMapModel::begin()
{
    return map.begin();
}

ComponentTypedefMapInterface::ConstIterator ComponentTypedefMapModel::begin() const
{
    return map.begin();
}

bool ComponentTypedefMapModel::clear()
{
    while (map.count() > 0)
    {
        delete map.take(map.keys().first());
    }
//    Q_EMIT cleared();
    return true;
}

int ComponentTypedefMapModel::count() const
{
    return map.count();
}

ComponentTypedefMapInterface::Iterator ComponentTypedefMapModel::end()
{
    return map.end();
}

ComponentTypedefMapInterface::ConstIterator ComponentTypedefMapModel::end() const
{
    return map.end();
}

bool ComponentTypedefMapModel::contains(ComponentTypedefMapInterface::ID const & id) const
{
    return map.keys().contains(id);
}

bool ComponentTypedefMapModel::contains(ComponentTypedefMapInterface::Item * const item) const
{
    return map.values().contains(item);
}

bool ComponentTypedefMapModel::contains(ComponentTypedefMapInterface::Index const & index) const
{
    return ((0 <= index) && (index < map.count()));
}

ComponentTypedefMapInterface::ID ComponentTypedefMapModel::generateID() const
{
    ComponentTypedefMapInterface::ID id = 0;
    for (ComponentTypedefMapInterface::ID key : map.keys())
    {
        if (id <= key)
        {
            id = key + 1;
        }
    }
    return id;
}

ComponentTypedefMapInterface::ID ComponentTypedefMapModel::getID(ComponentTypedefMapInterface::Item * const item) const
{
    return map.key(item);
}

ComponentTypedefMapInterface::ID ComponentTypedefMapModel::getID(ComponentTypedefMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.keys().value(index) : ComponentTypedefMapInterface::ID());
}

ComponentTypedefMapInterface::Index ComponentTypedefMapModel::getIndex(ComponentTypedefMapInterface::ID const & id) const
{
    return map.keys().indexOf(id);
}

ComponentTypedefMapInterface::Index ComponentTypedefMapModel::getIndex(ComponentTypedefMapInterface::Item * const item) const
{
    return map.values().indexOf(item);
}

ComponentTypedefMapInterface::Item * ComponentTypedefMapModel::getItem(ComponentTypedefMapInterface::ID const & id) const
{
    return map.value(id, nullptr);
}

ComponentTypedefMapInterface::Item * ComponentTypedefMapModel::getItem(ComponentTypedefMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.values().value(index, nullptr) : nullptr);
}

bool ComponentTypedefMapModel::remove(ComponentTypedefMapInterface::ID const & id)
{
    if (map.contains(id))
    {
        delete map.take(id);
//        Q_EMIT removed();
        return true;
    }
    return false;
}

bool ComponentTypedefMapModel::remove(ComponentTypedefMapInterface::Item * const item)
{
    return ((contains(item)) ? remove(getID(item)) : false);
}

bool ComponentTypedefMapModel::remove(ComponentTypedefMapInterface::Index const & index)
{
    return ((contains(index)) ? remove(getID(index)) : false);
}
