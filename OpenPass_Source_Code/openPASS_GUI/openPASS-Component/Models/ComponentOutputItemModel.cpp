/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentOutputItemModel.h"

#include "Models/ComponentOutputMapModel.h"

ComponentOutputItemInterface::Type const ComponentOutputItemModel::DefaultType = QStringLiteral("");
ComponentOutputItemInterface::Title const ComponentOutputItemModel::DefaultTitle = QStringLiteral("");
ComponentOutputItemInterface::Unit const ComponentOutputItemModel::DefaultUnit = QStringLiteral("");

ComponentOutputItemModel::ComponentOutputItemModel(QObject * const parent)
    : ComponentOutputItemInterface(parent)
    , type(DefaultType)
    , title(DefaultTitle)
    , unit(DefaultUnit)
{
}

bool ComponentOutputItemModel::clear()
{
    setType(DefaultType);
    setTitle(DefaultTitle);
    setUnit(DefaultUnit);
    Q_EMIT cleared();
    return true;
}

bool ComponentOutputItemModel::setID(ComponentOutputItemInterface::ID const & id)
{
    ComponentOutputMapModel * const map =
            qobject_cast<ComponentOutputMapModel * const>(parent());
    return ((map) ? map->setID(const_cast<ComponentOutputItemModel *>(this), id)
                  : false);
}

ComponentOutputItemInterface::ID ComponentOutputItemModel::getID() const
{
    ComponentOutputMapModel const * const map =
            qobject_cast<ComponentOutputMapModel const * const>(parent());
    return ((map) ? map->getID(const_cast<ComponentOutputItemModel *>(this))
                  : ComponentOutputItemInterface::ID());
}

bool ComponentOutputItemModel::setType(ComponentOutputItemInterface::Type const & _type)
{
    type = _type;
    Q_EMIT modifiedType();
    return true;
}

ComponentOutputItemInterface::Type ComponentOutputItemModel::getType() const
{
    return type;
}

bool ComponentOutputItemModel::setTitle(ComponentOutputItemInterface::Title const & _title)
{
    title = _title;
    Q_EMIT modifiedTitle();
    return true;
}

ComponentOutputItemInterface::Title ComponentOutputItemModel::getTitle() const
{
    return title;
}

bool ComponentOutputItemModel::setUnit(ComponentOutputItemInterface::Unit const & _unit)
{
    unit = _unit;
    Q_EMIT modifiedUnit();
    return true;
}

ComponentOutputItemInterface::Unit ComponentOutputItemModel::getUnit() const
{
    return unit;
}
