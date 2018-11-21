/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentInputItemModel.h"

#include "Models/ComponentInputMapModel.h"

ComponentInputItemInterface::Type const ComponentInputItemModel::DefaultType = QStringLiteral("");
ComponentInputItemInterface::Title const ComponentInputItemModel::DefaultTitle = QStringLiteral("");
ComponentInputItemInterface::Unit const ComponentInputItemModel::DefaultUnit = QStringLiteral("");
ComponentInputItemInterface::Cardinality const ComponentInputItemModel::DefaultCardinality = QStringLiteral("1");

ComponentInputItemModel::ComponentInputItemModel(QObject * const parent)
    : ComponentInputItemInterface(parent)
    , type(DefaultType)
    , title(DefaultTitle)
    , unit(DefaultUnit)
    , cardinality(DefaultCardinality)
{
}

bool ComponentInputItemModel::clear()
{
    setType(DefaultType);
    setTitle(DefaultTitle);
    setUnit(DefaultUnit);
    setCardinality(DefaultCardinality);
    Q_EMIT cleared();
    return true;
}

bool ComponentInputItemModel::setID(ComponentInputItemInterface::ID const & id)
{
    ComponentInputMapModel * const map =
            qobject_cast<ComponentInputMapModel * const>(parent());
    return ((map) ? map->setID(const_cast<ComponentInputItemModel *>(this), id)
                  : false);
}

ComponentInputItemInterface::ID ComponentInputItemModel::getID() const
{
    ComponentInputMapModel const * const map =
            qobject_cast<ComponentInputMapModel const * const>(parent());
    return ((map) ? map->getID(const_cast<ComponentInputItemModel *>(this))
                  : ComponentInputItemInterface::ID());
}

bool ComponentInputItemModel::setType(ComponentInputItemInterface::Type const & _type)
{
    type = _type;
    Q_EMIT modifiedType();
    return true;
}

ComponentInputItemInterface::Type ComponentInputItemModel::getType() const
{
    return type;
}

bool ComponentInputItemModel::setTitle(ComponentInputItemInterface::Title const & _title)
{
    title = _title;
    Q_EMIT modifiedTitle();
    return true;
}

ComponentInputItemInterface::Title ComponentInputItemModel::getTitle() const
{
    return title;
}

bool ComponentInputItemModel::setUnit(ComponentInputItemInterface::Unit const & _unit)
{
    unit = _unit;
    Q_EMIT modifiedUnit();
    return true;
}

ComponentInputItemInterface::Unit ComponentInputItemModel::getUnit() const
{
    return unit;
}

bool ComponentInputItemModel::setCardinality(ComponentInputItemInterface::Cardinality const & _cardinality)
{
    cardinality = _cardinality;
    Q_EMIT modifiedCardinality();
    return true;
}

ComponentInputItemInterface::Cardinality ComponentInputItemModel::getCardinality() const
{
    return cardinality;
}
