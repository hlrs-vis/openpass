/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentMapModel.h"

#include "Models/ComponentItemModel.h"

ComponentMapModel::ComponentMapModel(QObject * const parent)
    : ComponentMapInterface(parent)
{
}

bool ComponentMapModel::add(ComponentMapInterface::Name const & name)
{
    if (!contains(name))
    {
        map.insert(name, new ComponentItemModel(this));
        Q_EMIT added();
        return true;
    }
    return false;
}

bool ComponentMapModel::add(ComponentMapInterface::Name const & name,
                            ComponentMapInterface::Item * const item)
{
    if (!contains(name))
    {
        map.insert(name, item);
        item->setParent(this);
        Q_EMIT added();
        return true;
    }
    return false;
}

bool ComponentMapModel::add(ComponentMapInterface::Name const & name,
                            QString const & filepath)
{
    if (!contains(name))
    {
        ComponentItemModel * const item = new ComponentItemModel(this);
        if (item->load(filepath))
        {
            map.insert(name, item);
            Q_EMIT added();
            return true;
        }
        delete item;
        return false;
    }
    return false;
}

ComponentMapInterface::Iterator ComponentMapModel::begin()
{
    return map.begin();
}

ComponentMapInterface::ConstIterator ComponentMapModel::begin() const
{
    return map.begin();
}

bool ComponentMapModel::clear()
{
    while (map.count() > 0)
    {
        delete map.take(map.keys().first());
    }
    return true;
}

int ComponentMapModel::count() const
{
    return map.count();
}

bool ComponentMapModel::contains(ComponentMapInterface::Name const & name) const
{
    return map.keys().contains(name);
}

bool ComponentMapModel::contains(ComponentMapInterface::Item * const item) const
{
    return map.values().contains(item);
}

bool ComponentMapModel::contains(ComponentMapInterface::Index const & index) const
{
    return ((0 <= index) && (index < map.count()));
}

ComponentMapInterface::Iterator ComponentMapModel::end()
{
    return map.end();
}

ComponentMapInterface::ConstIterator ComponentMapModel::end() const
{
    return map.end();
}

bool ComponentMapModel::setName(ComponentMapInterface::Item * const item,
                                ComponentMapInterface::Name const & name)
{
    if ((contains(item)) && (!contains(name)))
    {
        map.insert(name, map.take(item->getName()));
        Q_EMIT modifiedName();
        return true;
    }
    return false;
}

ComponentMapInterface::Name ComponentMapModel::getName(ComponentMapInterface::Item * const item) const
{
    return map.key(item);
}

ComponentMapInterface::Name ComponentMapModel::getName(ComponentMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.keys().value(index) : ComponentMapInterface::Name());
}

ComponentMapInterface::Index ComponentMapModel::getIndex(ComponentMapInterface::Name const & name) const
{
    return map.keys().indexOf(name);
}

ComponentMapInterface::Index ComponentMapModel::getIndex(ComponentMapInterface::Item * const item) const
{
    return map.values().indexOf(item);
}

ComponentMapInterface::Item * ComponentMapModel::getItem(ComponentMapInterface::Name const & name) const
{
    return map.value(name, nullptr);
}

ComponentMapInterface::Item * ComponentMapModel::getItem(ComponentMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.values().value(index, nullptr) : nullptr);
}

bool ComponentMapModel::remove(ComponentMapInterface::Name const & name)
{
    if (map.contains(name))
    {
        delete map.take(name);
        Q_EMIT removed();
        return true;
    }
    return false;
}

bool ComponentMapModel::remove(ComponentMapInterface::Item * const item)
{
    return ((contains(item)) ? remove(getName(item)) : false);
}

bool ComponentMapModel::remove(ComponentMapInterface::Index const & index)
{
    return ((contains(index)) ? remove(getName(index)) : false);
}
