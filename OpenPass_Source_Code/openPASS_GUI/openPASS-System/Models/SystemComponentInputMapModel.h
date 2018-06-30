/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTINPUTMAPMODEL_H
#define SYSTEMCOMPONENTINPUTMAPMODEL_H

#include "openPASS-Component/ComponentInputMapInterface.h"
#include "openPASS-System/SystemComponentInputMapInterface.h"

class SystemComponentInputMapModel : public SystemComponentInputMapInterface
{
    Q_OBJECT

public:
    explicit SystemComponentInputMapModel(ComponentInputMapInterface const * const inputs,
                                          QObject * const parent = nullptr);
    virtual ~SystemComponentInputMapModel() = default;

public:
    virtual SystemComponentInputMapInterface::Iterator begin() override;
    virtual SystemComponentInputMapInterface::ConstIterator begin() const override;

public:
    virtual SystemComponentInputMapInterface::Iterator end() override;
    virtual SystemComponentInputMapInterface::ConstIterator end() const override;

public:
    virtual SystemComponentInputMapInterface::ID getID(SystemComponentInputMapInterface::Item * const item) const override;
    virtual SystemComponentInputMapInterface::Item * getItem(SystemComponentInputMapInterface::ID const & id) const override;

protected:
    SystemComponentInputMapInterface::Map inputs;
};

#endif // SYSTEMCOMPONENTINPUTMAPMODEL_H
