/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentOutputMapModel.h"

#include "Models/ComponentOutputItemModel.h"

ComponentOutputMapModel::ComponentOutputMapModel(QObject * const parent)
    : ComponentOutputMapInterface(parent)
{
}

bool ComponentOutputMapModel::add(ComponentOutputMapInterface::ID const & id)
{
    if (!contains(id))
    {
        map.insert(id, new ComponentOutputItemModel(this));
        Q_EMIT added();
        return true;
    }
    return false;
}

bool ComponentOutputMapModel::add(ComponentOutputMapInterface::ID const & id,
                                  ComponentOutputMapInterface::Item * const item)
{
    if (!contains(id))
    {
        map.insert(id, item);
        item->setParent(this);
        Q_EMIT added();
        return true;
    }
    return false;
}

ComponentOutputMapInterface::Iterator ComponentOutputMapModel::begin()
{
    return map.begin();
}

ComponentOutputMapInterface::ConstIterator ComponentOutputMapModel::begin() const
{
    return map.begin();
}

bool ComponentOutputMapModel::clear()
{
    while (map.count() > 0)
    {
        delete map.take(map.keys().first());
    }
    Q_EMIT cleared();
    return true;
}

int ComponentOutputMapModel::count() const
{
    return map.count();
}

bool ComponentOutputMapModel::contains(ComponentOutputMapInterface::ID const & id) const
{
    return map.keys().contains(id);
}

bool ComponentOutputMapModel::contains(ComponentOutputMapInterface::Item * const item) const
{
    return map.values().contains(item);
}

bool ComponentOutputMapModel::contains(ComponentOutputMapInterface::Index const & index) const
{
    return ((0 <= index) && (index < map.count()));
}

ComponentOutputMapInterface::Iterator ComponentOutputMapModel::end()
{
    return map.end();
}

ComponentOutputMapInterface::ConstIterator ComponentOutputMapModel::end() const
{
    return map.end();
}

bool ComponentOutputMapModel::setID(ComponentOutputMapInterface::Item * const item,
                                    ComponentOutputMapInterface::ID const & id)
{
    if ((contains(item)) && (!contains(id)))
    {
        map.insert(id, map.take(item->getID()));
        Q_EMIT modifiedID();
        return true;
    }
    return false;
}

ComponentOutputMapInterface::ID ComponentOutputMapModel::generateID() const
{
    ComponentOutputMapInterface::ID id = 0;
    for (ComponentOutputMapInterface::ID key : map.keys())
    {
        if (id <= key)
        {
            id = key + 1;
        }
    }
    return id;
}

ComponentOutputMapInterface::ID ComponentOutputMapModel::getID(ComponentOutputMapInterface::Item * const item) const
{
    return map.key(item);
}

ComponentOutputMapInterface::ID ComponentOutputMapModel::getID(ComponentOutputMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.keys().value(index) : ComponentOutputMapInterface::ID());
}

ComponentOutputMapInterface::Index ComponentOutputMapModel::getIndex(ComponentOutputMapInterface::ID const & id) const
{
    return map.keys().indexOf(id);
}

ComponentOutputMapInterface::Index ComponentOutputMapModel::getIndex(ComponentOutputMapInterface::Item * const item) const
{
    return map.values().indexOf(item);
}

ComponentOutputMapInterface::Item * ComponentOutputMapModel::getItem(ComponentOutputMapInterface::ID const & id) const
{
    return map.value(id, nullptr);
}

ComponentOutputMapInterface::Item * ComponentOutputMapModel::getItem(ComponentOutputMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.values().value(index, nullptr) : nullptr);
}

bool ComponentOutputMapModel::remove(ComponentOutputMapInterface::ID const & id)
{
    if (map.contains(id))
    {
        delete map.take(id);
        Q_EMIT removed();
        return true;
    }
    return false;
}

bool ComponentOutputMapModel::remove(ComponentOutputMapInterface::Item * const item)
{
    return ((contains(item)) ? remove(getID(item)) : false);
}

bool ComponentOutputMapModel::remove(ComponentOutputMapInterface::Index const & index)
{
    return ((contains(index)) ? remove(getID(index)) : false);
}
