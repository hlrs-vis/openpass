/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentParameterItemModel.h"

#include "Models/SystemComponentParameterMapModel.h"

SystemComponentParameterItemModel::SystemComponentParameterItemModel(ComponentParameterItemInterface const * const parameter,
                                                                     QObject * const parent)
    : SystemComponentParameterItemInterface(parent)
    , parameter(parameter)
    , value(parameter->getValue())
{
}

SystemComponentParameterItemInterface::ID SystemComponentParameterItemModel::getID() const
{
    SystemComponentParameterMapModel const * const parameters =
            qobject_cast<SystemComponentParameterMapModel const * const>(parent());
    return ((parameters) ? parameters->getID(const_cast<SystemComponentParameterItemModel *>(this))
                         : SystemComponentParameterItemInterface::ID());
}

SystemComponentParameterItemInterface::Type SystemComponentParameterItemModel::getType() const
{
    return parameter->getType();
}

SystemComponentParameterItemInterface::Title SystemComponentParameterItemModel::getTitle() const
{
    return parameter->getTitle();
}

SystemComponentParameterItemInterface::Unit SystemComponentParameterItemModel::getUnit() const
{
    return parameter->getUnit();
}

bool SystemComponentParameterItemModel::setValue(SystemComponentParameterItemInterface::Value const & _value)
{
    value = _value;
    Q_EMIT modifiedValue();
    return true;
}

SystemComponentParameterItemInterface::Value SystemComponentParameterItemModel::getValue() const
{
    return value;
}
