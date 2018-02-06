/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentOutputMapModel.h"

#include "Models/SystemComponentOutputItemModel.h"

SystemComponentOutputMapModel::SystemComponentOutputMapModel(ComponentOutputMapInterface const * const _outputs,
                                                             QObject * const parent)
    : SystemComponentOutputMapInterface(parent)
{
    for (ComponentOutputItemInterface const * const output : *_outputs)
    {
        outputs.insert(output->getID(), new SystemComponentOutputItemModel(output, this));
    }
}

SystemComponentOutputMapInterface::Iterator SystemComponentOutputMapModel::begin()
{
    return outputs.begin();
}

SystemComponentOutputMapInterface::ConstIterator SystemComponentOutputMapModel::begin() const
{
    return outputs.begin();
}

SystemComponentOutputMapInterface::Iterator SystemComponentOutputMapModel::end()
{
    return outputs.end();
}

SystemComponentOutputMapInterface::ConstIterator SystemComponentOutputMapModel::end() const
{
    return outputs.end();
}

SystemComponentOutputMapInterface::ID SystemComponentOutputMapModel::getID(SystemComponentOutputMapInterface::Item * const item) const
{
    return outputs.key(item);
}

SystemComponentOutputMapInterface::Item * SystemComponentOutputMapModel::getItem(SystemComponentOutputMapInterface::ID const & id) const
{
    return outputs.value(id, nullptr);
}
