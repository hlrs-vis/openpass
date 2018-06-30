/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentInputMapModel.h"

#include "Models/SystemComponentInputItemModel.h"

SystemComponentInputMapModel::SystemComponentInputMapModel(ComponentInputMapInterface const * const _inputs,
                                                           QObject * const parent)
    : SystemComponentInputMapInterface(parent)
{
    for (ComponentInputItemInterface const * const input : *_inputs)
    {
        inputs.insert(input->getID(), new SystemComponentInputItemModel(input, this));
    }
}

SystemComponentInputMapInterface::Iterator SystemComponentInputMapModel::begin()
{
    return inputs.begin();
}

SystemComponentInputMapInterface::ConstIterator SystemComponentInputMapModel::begin() const
{
    return inputs.begin();
}

SystemComponentInputMapInterface::Iterator SystemComponentInputMapModel::end()
{
    return inputs.end();
}

SystemComponentInputMapInterface::ConstIterator SystemComponentInputMapModel::end() const
{
    return inputs.end();
}

SystemComponentInputMapInterface::ID SystemComponentInputMapModel::getID(SystemComponentInputMapInterface::Item * const item) const
{
    return inputs.key(item);
}

SystemComponentInputMapInterface::Item * SystemComponentInputMapModel::getItem(SystemComponentInputMapInterface::ID const & id) const
{
    return inputs.value(id, nullptr);
}
