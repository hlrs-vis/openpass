/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentParameterItemModel.h"

#include "Models/ComponentParameterMapModel.h"

ComponentParameterItemInterface::Type const ComponentParameterItemModel::DefaultType = QStringLiteral("double");
ComponentParameterItemInterface::Title const ComponentParameterItemModel::DefaultTitle = QStringLiteral("");
ComponentParameterItemInterface::Unit const ComponentParameterItemModel::DefaultUnit = QStringLiteral("");
ComponentParameterItemInterface::Value const ComponentParameterItemModel::DefaultValue = QStringLiteral("");

ComponentParameterItemModel::ComponentParameterItemModel(QObject * const parent)
    : ComponentParameterItemInterface(parent)
    , type(DefaultType)
    , title(DefaultTitle)
    , unit(DefaultUnit)
    , value(DefaultValue)
{
}

bool ComponentParameterItemModel::clear()
{
    setType(DefaultType);
    setTitle(DefaultTitle);
    setUnit(DefaultUnit);
    setValue(DefaultValue);
    Q_EMIT cleared();
    return true;
}

bool ComponentParameterItemModel::setID(ComponentParameterItemInterface::ID const & id)
{
    ComponentParameterMapModel * const map =
            qobject_cast<ComponentParameterMapModel * const>(parent());
    return ((map) ? map->setID(const_cast<ComponentParameterItemModel *>(this), id)
                  : false);
}

ComponentParameterItemInterface::ID ComponentParameterItemModel::getID() const
{
    ComponentParameterMapModel const * const map =
            qobject_cast<ComponentParameterMapModel const * const>(parent());
    return ((map) ? map->getID(const_cast<ComponentParameterItemModel *>(this))
                  : ComponentParameterItemInterface::ID());
}

bool ComponentParameterItemModel::setType(ComponentParameterItemInterface::Type const & _type)
{
    type = _type;
    Q_EMIT modifiedType();
    return true;
}

ComponentParameterItemInterface::Type ComponentParameterItemModel::getType() const
{
    return type;
}

bool ComponentParameterItemModel::setTitle(ComponentParameterItemInterface::Title const & _title)
{
    title = _title;
    Q_EMIT modifiedTitle();
    return true;
}

ComponentParameterItemInterface::Title ComponentParameterItemModel::getTitle() const
{
    return title;
}

bool ComponentParameterItemModel::setUnit(ComponentParameterItemInterface::Unit const & _unit)
{
    unit = _unit;
    Q_EMIT modifiedUnit();
    return true;
}

ComponentParameterItemInterface::Unit ComponentParameterItemModel::getUnit() const
{
    return unit;
}

bool ComponentParameterItemModel::setValue(ComponentParameterItemInterface::Value const & _value)
{
    value = _value;
    Q_EMIT modifiedValue();
    return true;
}

ComponentParameterItemInterface::Value ComponentParameterItemModel::getValue() const
{
    return value;
}
