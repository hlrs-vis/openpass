/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTOUTPUTMAPMODEL_H
#define SYSTEMCOMPONENTOUTPUTMAPMODEL_H

#include "openPASS-Component/ComponentOutputMapInterface.h"
#include "openPASS-System/SystemComponentOutputMapInterface.h"

class SystemComponentOutputMapModel : public SystemComponentOutputMapInterface
{
    Q_OBJECT

public:
    explicit SystemComponentOutputMapModel(ComponentOutputMapInterface const * const outputs,
                                           QObject * const parent = nullptr);
    virtual ~SystemComponentOutputMapModel() = default;

public:
    virtual SystemComponentOutputMapInterface::Iterator begin() override;
    virtual SystemComponentOutputMapInterface::ConstIterator begin() const override;

public:
    virtual SystemComponentOutputMapInterface::Iterator end() override;
    virtual SystemComponentOutputMapInterface::ConstIterator end() const override;

public:
    virtual SystemComponentOutputMapInterface::ID getID(SystemComponentOutputMapInterface::Item * const item) const override;
    virtual SystemComponentOutputMapInterface::Item * getItem(SystemComponentOutputMapInterface::ID const & id) const override;

protected:
    SystemComponentOutputMapInterface::Map outputs;
};

#endif // SYSTEMCOMPONENTOUTPUTMAPMODEL_H
