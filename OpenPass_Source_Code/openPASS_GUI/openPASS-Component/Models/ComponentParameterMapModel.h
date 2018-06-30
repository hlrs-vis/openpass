/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTPARAMETERMAPMODEL_H
#define COMPONENTPARAMETERMAPMODEL_H

#include "openPASS-Component/ComponentParameterMapInterface.h"
#include "openPASS-Component/ComponentTypedefMapInterface.h"

class ComponentParameterMapModel : public ComponentParameterMapInterface
{
    Q_OBJECT

public:
    explicit ComponentParameterMapModel(QObject * const parent = nullptr);
    virtual ~ComponentParameterMapModel() = default;

public:
    virtual bool add(ComponentParameterMapInterface::ID const & id) override;
    virtual bool add(ComponentParameterMapInterface::ID const & id,
                     ComponentParameterMapInterface::Item * const item) override;

public:
    virtual ComponentParameterMapInterface::Iterator begin() override;
    virtual ComponentParameterMapInterface::ConstIterator begin() const override;

public:
    virtual bool clear() override;

public:
    virtual int count() const override;

public:
    virtual bool contains(ComponentParameterMapInterface::ID const & id) const override;
    virtual bool contains(ComponentParameterMapInterface::Item * const item) const override;
    virtual bool contains(ComponentParameterMapInterface::Index const & index) const override;

public:
    virtual ComponentParameterMapInterface::Iterator end() override;
    virtual ComponentParameterMapInterface::ConstIterator end() const override;

public:
    virtual bool setID(ComponentParameterMapInterface::Item * const item,
                       ComponentParameterMapInterface::ID const & id) override;
    virtual ComponentParameterMapInterface::ID generateID() const override;
    virtual ComponentParameterMapInterface::ID getID(ComponentParameterMapInterface::Item * const item) const override;
    virtual ComponentParameterMapInterface::ID getID(ComponentParameterMapInterface::Index const & index) const override;

public:
    virtual ComponentParameterMapInterface::Index getIndex(ComponentParameterMapInterface::ID const & id) const override;
    virtual ComponentParameterMapInterface::Index getIndex(ComponentParameterMapInterface::Item * const item) const override;

public:
    virtual ComponentParameterMapInterface::Item * getItem(ComponentParameterMapInterface::ID const & id) const override;
    virtual ComponentParameterMapInterface::Item * getItem(ComponentParameterMapInterface::Index const & index) const override;

public:
    virtual bool remove(ComponentParameterMapInterface::ID const & id) override;
    virtual bool remove(ComponentParameterMapInterface::Item * const item) override;
    virtual bool remove(ComponentParameterMapInterface::Index const & index) override;

public:
    virtual ComponentTypedefMapInterface * getParameterTypeMap() const override;

protected:
    ComponentParameterMapInterface::Map map;
    ComponentTypedefMapInterface * const _parameterTypeMap;
};

#endif // COMPONENTPARAMETERMAPMODEL_H
