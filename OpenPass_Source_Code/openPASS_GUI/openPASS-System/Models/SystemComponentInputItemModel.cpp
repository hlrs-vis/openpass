/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentInputItemModel.h"

#include "openPASS-System/SystemComponentItemInterface.h"
#include "Models/SystemComponentInputMapModel.h"

SystemComponentInputItemModel::SystemComponentInputItemModel(ComponentInputItemInterface const * const input,
                                                             QObject * const parent)
    : SystemComponentInputItemInterface(parent)
    , input(input)
{
}

SystemComponentItemInterface * SystemComponentInputItemModel::getComponent() const
{
    if ((parent()) && (parent()->parent()))
    {
        SystemComponentItemInterface * const item =
                qobject_cast<SystemComponentItemInterface * const>(parent()->parent());
        return ((item) ? item : nullptr);
    }
    return nullptr;
}

SystemComponentInputItemInterface::ID SystemComponentInputItemModel::getID() const
{
    SystemComponentInputMapModel const * const inputs =
            qobject_cast<SystemComponentInputMapModel const * const>(parent());
    return ((inputs) ? inputs->getID(const_cast<SystemComponentInputItemModel *>(this))
                     : SystemComponentInputItemInterface::ID());
}

SystemComponentInputItemInterface::Type SystemComponentInputItemModel::getType() const
{
    return input->getType();
}

SystemComponentInputItemInterface::Title SystemComponentInputItemModel::getTitle() const
{
    return input->getTitle();
}

SystemComponentInputItemInterface::Unit SystemComponentInputItemModel::getUnit() const
{
    return input->getUnit();
}

SystemComponentInputItemInterface::Cardinality SystemComponentInputItemModel::getCardinality() const
{
    return input->getCardinality();
}
