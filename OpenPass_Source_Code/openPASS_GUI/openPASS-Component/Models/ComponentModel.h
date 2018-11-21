/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include "openPASS-Component/ComponentInterface.h"

class ComponentMapModel;

class ComponentModel : public ComponentInterface
{
    Q_OBJECT

public:
    explicit ComponentModel(QObject * const parent = nullptr);
    virtual ~ComponentModel() = default;

public:
    virtual ComponentInterface::Item * createComponentItem(QObject * const parent) const override;
    virtual ComponentInterface::Map * createComponentMap(QObject * const parent) const override;
};

#endif // COMPONENTMODEL_H
