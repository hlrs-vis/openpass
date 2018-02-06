/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMMAPMODEL_H
#define SYSTEMMAPMODEL_H

#include "openPASS-System/SystemMapInterface.h"

class SystemMapModel : public SystemMapInterface
{
    Q_OBJECT

public:
    explicit SystemMapModel(QObject * const parent = nullptr);
    virtual ~SystemMapModel() = default;

public:
    virtual SystemMapInterface::Iterator begin() override;
    virtual SystemMapInterface::ConstIterator begin() const override;

public:
    virtual SystemMapInterface::Iterator end() override;
    virtual SystemMapInterface::ConstIterator end() const override;

public:
    virtual SystemMapInterface::ID getID(SystemMapInterface::Item * const item) const override;
    virtual SystemMapInterface::Item * getItem(SystemMapInterface::ID const & id) const override;

public:
    virtual bool clear() override;

protected:
    SystemMapInterface::Map systems;
};

#endif // SYSTEMMAPMODEL_H
