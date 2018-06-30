/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentInputMapModel.h"

#include "Models/ComponentInputItemModel.h"

ComponentInputMapModel::ComponentInputMapModel(QObject * const parent)
    : ComponentInputMapInterface(parent)
{
}

bool ComponentInputMapModel::add(ComponentInputMapInterface::ID const & id)
{
    if (!contains(id))
    {
        map.insert(id, new ComponentInputItemModel(this));
        Q_EMIT added();
        return true;
    }
    return false;
}

bool ComponentInputMapModel::add(ComponentInputMapInterface::ID const & id,
                                 ComponentInputMapInterface::Item * const item)
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

ComponentInputMapInterface::Iterator ComponentInputMapModel::begin()
{
    return map.begin();
}

ComponentInputMapInterface::ConstIterator ComponentInputMapModel::begin() const
{
    return map.begin();
}

bool ComponentInputMapModel::clear()
{
    while (map.count() > 0)
    {
        delete map.take(map.keys().first());
    }
    Q_EMIT cleared();
    return true;
}

int ComponentInputMapModel::count() const
{
    return map.count();
}

bool ComponentInputMapModel::contains(ComponentInputMapInterface::ID const & id) const
{
    return map.keys().contains(id);
}

bool ComponentInputMapModel::contains(ComponentInputMapInterface::Item * const item) const
{
    return map.values().contains(item);
}

bool ComponentInputMapModel::contains(ComponentInputMapInterface::Index const & index) const
{
    return ((0 <= index) && (index < map.count()));
}

ComponentInputMapInterface::Iterator ComponentInputMapModel::end()
{
    return map.end();
}

ComponentInputMapInterface::ConstIterator ComponentInputMapModel::end() const
{
    return map.end();
}

bool ComponentInputMapModel::setID(ComponentInputMapInterface::Item * const item,
                                   ComponentInputMapInterface::ID const & id)
{
    if ((contains(item)) && (!contains(id)))
    {
        map.insert(id, map.take(item->getID()));
        Q_EMIT modifiedID();
        return true;
    }
    return false;
}

ComponentInputMapInterface::ID ComponentInputMapModel::generateID() const
{
    ComponentInputMapInterface::ID id = 0;
    for (ComponentInputMapInterface::ID key : map.keys())
    {
        if (id <= key)
        {
            id = key + 1;
        }
    }
    return id;
}

ComponentInputMapInterface::ID ComponentInputMapModel::getID(ComponentInputMapInterface::Item * const item) const
{
    return map.key(item);
}

ComponentInputMapInterface::ID ComponentInputMapModel::getID(ComponentInputMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.keys().value(index) : ComponentInputMapInterface::ID());
}

ComponentInputMapInterface::Index ComponentInputMapModel::getIndex(ComponentInputMapInterface::ID const & id) const
{
    return map.keys().indexOf(id);
}

ComponentInputMapInterface::Index ComponentInputMapModel::getIndex(ComponentInputMapInterface::Item * const item) const
{
    return map.values().indexOf(item);
}

ComponentInputMapInterface::Item * ComponentInputMapModel::getItem(ComponentInputMapInterface::ID const & id) const
{
    return map.value(id, nullptr);
}

ComponentInputMapInterface::Item * ComponentInputMapModel::getItem(ComponentInputMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.values().value(index, nullptr) : nullptr);
}

bool ComponentInputMapModel::remove(ComponentInputMapInterface::ID const & id)
{
    if (map.contains(id))
    {
        delete map.take(id);
        Q_EMIT removed();
        return true;
    }
    return false;
}

bool ComponentInputMapModel::remove(ComponentInputMapInterface::Item * const item)
{
    return ((contains(item)) ? remove(getID(item)) : false);
}

bool ComponentInputMapModel::remove(ComponentInputMapInterface::Index const & index)
{
    return ((contains(index)) ? remove(getID(index)) : false);
}
