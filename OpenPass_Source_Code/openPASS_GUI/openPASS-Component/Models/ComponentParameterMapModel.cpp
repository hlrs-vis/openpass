/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentParameterMapModel.h"

#include "Models/ComponentParameterItemModel.h"
#include "Models/ComponentTypedefMapModel.h"

ComponentParameterMapModel::ComponentParameterMapModel(QObject * const parent)
    : ComponentParameterMapInterface(parent)
    , _parameterTypeMap(new ComponentTypedefMapModel(this))
{
}

bool ComponentParameterMapModel::add(ComponentParameterMapInterface::ID const & id)
{
    if (!contains(id))
    {
        map.insert(id, new ComponentParameterItemModel(this));
        Q_EMIT added();
        return true;
    }
    return false;
}

bool ComponentParameterMapModel::add(ComponentParameterMapInterface::ID const & id,
                                     ComponentParameterMapInterface::Item * const item)
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

ComponentParameterMapInterface::Iterator ComponentParameterMapModel::begin()
{
    return map.begin();
}

ComponentParameterMapInterface::ConstIterator ComponentParameterMapModel::begin() const
{
    return map.begin();
}

bool ComponentParameterMapModel::clear()
{
    while (map.count() > 0)
    {
        delete map.take(map.keys().first());
    }
    Q_EMIT cleared();
    return true;
}

int ComponentParameterMapModel::count() const
{
    return map.count();
}

ComponentParameterMapInterface::Iterator ComponentParameterMapModel::end()
{
    return map.end();
}

ComponentParameterMapInterface::ConstIterator ComponentParameterMapModel::end() const
{
    return map.end();
}

bool ComponentParameterMapModel::contains(ComponentParameterMapInterface::ID const & id) const
{
    return map.keys().contains(id);
}

bool ComponentParameterMapModel::contains(ComponentParameterMapInterface::Item * const item) const
{
    return map.values().contains(item);
}

bool ComponentParameterMapModel::contains(ComponentParameterMapInterface::Index const & index) const
{
    return ((0 <= index) && (index < map.count()));
}

bool ComponentParameterMapModel::setID(ComponentParameterMapInterface::Item * const item,
                                       ComponentParameterMapInterface::ID const & id)
{
    if ((contains(item)) && (!contains(id)))
    {
        map.insert(id, map.take(item->getID()));
        Q_EMIT modifiedID();
        return true;
    }
    return false;
}

ComponentParameterMapInterface::ID ComponentParameterMapModel::generateID() const
{
    ComponentParameterMapInterface::ID id = 0;
    for (ComponentParameterMapInterface::ID key : map.keys())
    {
        if (id <= key)
        {
            id = key + 1;
        }
    }
    return id;
}

ComponentParameterMapInterface::ID ComponentParameterMapModel::getID(ComponentParameterMapInterface::Item * const item) const
{
    return map.key(item);
}

ComponentParameterMapInterface::ID ComponentParameterMapModel::getID(ComponentParameterMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.keys().value(index) : ComponentParameterMapInterface::ID());
}

ComponentParameterMapInterface::Index ComponentParameterMapModel::getIndex(ComponentParameterMapInterface::ID const & id) const
{
    return map.keys().indexOf(id);
}

ComponentParameterMapInterface::Index ComponentParameterMapModel::getIndex(ComponentParameterMapInterface::Item * const item) const
{
    return map.values().indexOf(item);
}

ComponentParameterMapInterface::Item * ComponentParameterMapModel::getItem(ComponentParameterMapInterface::ID const & id) const
{
    return map.value(id, nullptr);
}

ComponentParameterMapInterface::Item * ComponentParameterMapModel::getItem(ComponentParameterMapInterface::Index const & index) const
{
    return ((contains(index)) ? map.values().value(index, nullptr) : nullptr);
}

bool ComponentParameterMapModel::remove(ComponentParameterMapInterface::ID const & id)
{
    if (map.contains(id))
    {
        delete map.take(id);
        Q_EMIT removed();
        return true;
    }
    return false;
}

bool ComponentParameterMapModel::remove(ComponentParameterMapInterface::Item * const item)
{
    return ((contains(item)) ? remove(getID(item)) : false);
}

bool ComponentParameterMapModel::remove(ComponentParameterMapInterface::Index const & index)
{
    return ((contains(index)) ? remove(getID(index)) : false);
}

ComponentTypedefMapInterface * ComponentParameterMapModel::getParameterTypeMap() const
{
    return _parameterTypeMap;
}
