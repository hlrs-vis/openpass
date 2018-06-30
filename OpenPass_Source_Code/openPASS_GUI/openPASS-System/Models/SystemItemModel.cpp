/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemItemModel.h"

#include "Models/SystemComponentMapModel.h"
#include "Models/SystemConnectionItemModel.h"
#include "Models/SystemConnectionMapModel.h"
#include "Models/SystemMapModel.h"

SystemItemModel::SystemItemModel(QObject * const parent)
    : SystemItemInterface(parent)
    , title(SystemItemInterface::Title(QStringLiteral("")))
    , priority(0)
    , connections(new SystemConnectionMapModel(this))
    , components(new SystemComponentMapModel(connections, this))
{
}

SystemItemInterface::ID SystemItemModel::getID() const
{
    SystemMapModel const * const systems =
            qobject_cast<SystemMapModel const * const>(parent());
    return ((systems) ? systems->getID(const_cast<SystemItemModel *>(this))
                      : SystemItemInterface::ID());
}

bool SystemItemModel::setTitle(SystemItemInterface::Title const & _title)
{
    title = _title;
    Q_EMIT modifiedTitle();
    return true;
}

SystemItemInterface::Title SystemItemModel::getTitle() const
{
    return title;
}

bool SystemItemModel::setPriority(SystemItemInterface::Priority const & _priority)
{
    priority = _priority;
    Q_EMIT modifiedPriority();
    return true;
}

SystemItemInterface::Priority SystemItemModel::getPriority() const
{
    return priority;
}

SystemItemInterface::ComponentMap * SystemItemModel::getComponents() const
{
    return components;
}

SystemItemInterface::ConnectionMap * SystemItemModel::getConnections() const
{
    return connections;
}

QList<SystemConnectionMapInterface::Source const *> SystemItemModel::getCompatibleSources(
        SystemConnectionMapInterface::Target const * const target)
{
    QList<SystemConnectionMapInterface::Source const *> sources;
    for (SystemComponentItemInterface const * const component : *components)
    {
        for (SystemComponentOutputItemInterface const * const output : *component->getOutputs())
        {
            if ((SystemConnectionItemModel::isCompatible(output, target)) &&
                    (!connections->contains(output, target)))
            {
                sources << output;
            }
        }
    }
    return sources;
}

QList<SystemConnectionMapInterface::Target const *> SystemItemModel::getCompatibleTargets(
        SystemConnectionMapInterface::Source const * const source)
{
    QList<SystemConnectionMapInterface::Target const *> targets;
    for (SystemComponentItemInterface const * const component : *components)
    {
        for (SystemComponentInputItemInterface const * const input : *component->getInputs())
        {
            if ((SystemConnectionItemModel::isCompatible(source, input)) &&
                    (!connections->contains(source, input)))
            {
                targets << input;
            }
        }
    }
    return targets;
}
