/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentModel.h"

#include "Models/ComponentItemModel.h"
#include "Models/ComponentMapModel.h"

ComponentModel::ComponentModel(QObject * const parent)
    : ComponentInterface(parent)
{
}

ComponentInterface::Item * ComponentModel::createComponentItem(QObject * const parent) const
{
    return new ComponentItemModel(parent);
}

ComponentInterface::Map * ComponentModel::createComponentMap(QObject * const parent) const
{
    return new ComponentMapModel(parent);
}
