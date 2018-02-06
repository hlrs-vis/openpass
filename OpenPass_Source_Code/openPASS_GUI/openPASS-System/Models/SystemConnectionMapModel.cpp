/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemConnectionMapModel.h"

#include "Models/SystemConnectionItemModel.h"

SystemConnectionMapModel::SystemConnectionMapModel(QObject * const parent)
    : SystemConnectionMapInterface(parent)
{
}

SystemConnectionMapInterface::Iterator SystemConnectionMapModel::begin()
{
    return connections.begin();
}

SystemConnectionMapInterface::ConstIterator SystemConnectionMapModel::begin() const
{
    return connections.begin();
}

SystemConnectionMapInterface::Iterator SystemConnectionMapModel::end()
{
    return connections.end();
}

SystemConnectionMapInterface::ConstIterator SystemConnectionMapModel::end() const
{
    return connections.end();
}

SystemConnectionMapInterface::ID SystemConnectionMapModel::getID(SystemConnectionMapInterface::Item * const item) const
{
    return connections.key(item);
}

SystemConnectionMapInterface::Item * SystemConnectionMapModel::getItem(SystemConnectionMapInterface::ID const & id) const
{
    return connections.value(id, nullptr);
}

bool SystemConnectionMapModel::clear()
{
    qDeleteAll(connections);
    connections.clear();
    Q_EMIT modified();
    return true;
}

bool SystemConnectionMapModel::add(SystemConnectionMapInterface::ID const & id,
                                   SystemConnectionMapInterface::Source const * const source,
                                   SystemConnectionMapInterface::Target const * const target)
{
    if ((!connections.keys().contains(id)) && (!contains(source, target)) &&
            (SystemConnectionItemModel::isCompatible(source, target)))
    {
        connections.insert(id, new SystemConnectionItemModel(source, target, this));
        Q_EMIT modified();
        return true;
    }
    return false;
}

bool SystemConnectionMapModel::add(SystemConnectionMapInterface::Source const * const source,
                                   SystemConnectionMapInterface::Target const * const target)
{
    return add(next(), source, target);
}

bool SystemConnectionMapModel::contains(SystemConnectionMapInterface::Source const * const source,
                                        SystemConnectionMapInterface::Target const * const target) const
{
    for (SystemConnectionMapInterface::Item const * const connection : connections)
    {
        if ((connection->getSource() == source) && (connection->getTarget() == target))
        {
            return true;
        }
    }
    return false;
}

bool SystemConnectionMapModel::remove(SystemConnectionMapInterface::Component const * const component)
{
    QList<SystemConnectionMapInterface::ID> ids;
    for (SystemConnectionMapInterface::Item const * const connection : connections)
    {
        if ((connection->getSource()->getComponent() == component) ||
                (connection->getTarget()->getComponent() == component))
        {
            ids << connection->getID();
        }
    }
    return remove(ids);
}

bool SystemConnectionMapModel::remove(SystemConnectionMapInterface::Source const * const source)
{
    QList<SystemConnectionMapInterface::ID> ids;
    for (SystemConnectionMapInterface::Item const * const connection : connections)
    {
        if (connection->getSource() == source)
        {
            ids << connection->getID();
        }
    }
    return remove(ids);
}

bool SystemConnectionMapModel::remove(SystemConnectionMapInterface::Target const * const target)
{
    QList<SystemConnectionMapInterface::ID> ids;
    for (SystemConnectionMapInterface::Item const * const connection : connections)
    {
        if (connection->getTarget() == target)
        {
            ids << connection->getID();
        }
    }
    return remove(ids);
}

SystemConnectionMapInterface::ID SystemConnectionMapModel::next() const
{
    SystemConnectionMapInterface::ID id = 0;
    for (SystemConnectionMapInterface::ID key : connections.keys())
    {
        if (id <= key)
        {
            id = key + 1;
        }
    }
    return id;
}

bool SystemConnectionMapModel::remove(QList<SystemConnectionMapInterface::ID> const & ids)
{
    for (SystemConnectionMapInterface::ID const & id : ids)
    {
        delete connections.take(id);
    }
    if (ids.count() > 0)
    {
        Q_EMIT modified();
    }
    return true;
}
