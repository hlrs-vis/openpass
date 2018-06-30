/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentOutputItemModel.h"

#include "openPASS-System/SystemComponentItemInterface.h"
#include "Models/SystemComponentOutputMapModel.h"

SystemComponentOutputItemModel::SystemComponentOutputItemModel(ComponentOutputItemInterface const * const output,
                                                               QObject * const parent)
    : SystemComponentOutputItemInterface(parent)
    , output(output)
{
}

SystemComponentItemInterface * SystemComponentOutputItemModel::getComponent() const
{
    if ((parent()) && (parent()->parent()))
    {
        SystemComponentItemInterface * const item =
                qobject_cast<SystemComponentItemInterface * const>(parent()->parent());
        return ((item) ? item : nullptr);
    }
    return nullptr;
}

SystemComponentOutputItemInterface::ID SystemComponentOutputItemModel::getID() const
{
    SystemComponentOutputMapModel const * const outputs =
            qobject_cast<SystemComponentOutputMapModel const * const>(parent());
    return ((outputs) ? outputs->getID(const_cast<SystemComponentOutputItemModel *>(this))
                      : SystemComponentOutputItemInterface::ID());
}

SystemComponentOutputItemInterface::Type SystemComponentOutputItemModel::getType() const
{
    return output->getType();
}

SystemComponentOutputItemInterface::Title SystemComponentOutputItemModel::getTitle() const
{
    return output->getTitle();
}

SystemComponentOutputItemInterface::Unit SystemComponentOutputItemModel::getUnit() const
{
    return output->getUnit();
}
