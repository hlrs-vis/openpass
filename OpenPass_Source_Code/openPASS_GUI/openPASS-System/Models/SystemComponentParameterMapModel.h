/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERMAPMODEL_H
#define SYSTEMCOMPONENTPARAMETERMAPMODEL_H

#include "openPASS-Component/ComponentParameterMapInterface.h"
#include "openPASS-System/SystemComponentParameterMapInterface.h"

class SystemComponentParameterMapModel : public SystemComponentParameterMapInterface
{
    Q_OBJECT

public:
    explicit SystemComponentParameterMapModel(ComponentParameterMapInterface const * const parameters,
                                              QObject * const parent = nullptr);
    virtual ~SystemComponentParameterMapModel() = default;

public:
    virtual int count() const override;

public:
    virtual SystemComponentParameterMapInterface::Iterator begin() override;
    virtual SystemComponentParameterMapInterface::ConstIterator begin() const override;

public:
    virtual SystemComponentParameterMapInterface::Iterator end() override;
    virtual SystemComponentParameterMapInterface::ConstIterator end() const override;

public:
    virtual SystemComponentParameterMapInterface::ID getID(SystemComponentParameterMapInterface::Item * const item) const override;
    virtual SystemComponentParameterMapInterface::Item * getItem(SystemComponentParameterMapInterface::ID const & id) const override;

protected:
    SystemComponentParameterMapInterface::Map parameters;
};

#endif // SYSTEMCOMPONENTPARAMETERMAPMODEL_H
