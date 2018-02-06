/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERITEMMODEL_H
#define SYSTEMCOMPONENTPARAMETERITEMMODEL_H

#include "openPASS-System/SystemComponentParameterItemInterface.h"

class SystemComponentParameterItemModel : public SystemComponentParameterItemInterface
{
    Q_OBJECT

public:
    explicit SystemComponentParameterItemModel(ComponentParameterItemInterface const * const parameter,
                                               QObject * const parent = nullptr);
    virtual ~SystemComponentParameterItemModel() = default;

public:
    virtual SystemComponentParameterItemInterface::ID getID() const override;
    virtual SystemComponentParameterItemInterface::Type getType() const override;
    virtual SystemComponentParameterItemInterface::Title getTitle() const override;
    virtual SystemComponentParameterItemInterface::Unit getUnit() const override;

public:
    virtual bool setValue(SystemComponentParameterItemInterface::Value const & value) override;
    virtual SystemComponentParameterItemInterface::Value getValue() const override;

protected:
    ComponentParameterItemInterface const * const parameter;
    SystemComponentParameterItemInterface::Value value;
};

#endif // SYSTEMCOMPONENTPARAMETERITEMMODEL_H
