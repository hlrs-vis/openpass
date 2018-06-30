/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTINPUTMAPMODEL_H
#define COMPONENTINPUTMAPMODEL_H

#include "openPASS-Component/ComponentInputMapInterface.h"

class ComponentInputMapModel : public ComponentInputMapInterface
{
    Q_OBJECT

public:
    explicit ComponentInputMapModel(QObject * const parent = nullptr);
    virtual ~ComponentInputMapModel() = default;

public:
    virtual bool add(ComponentInputMapInterface::ID const & id) override;
    virtual bool add(ComponentInputMapInterface::ID const & id,
                     ComponentInputMapInterface::Item * const item) override;

public:
    virtual ComponentInputMapInterface::Iterator begin() override;
    virtual ComponentInputMapInterface::ConstIterator begin() const override;

public:
    virtual bool clear() override;

public:
    virtual int count() const override;

public:
    virtual bool contains(ComponentInputMapInterface::ID const & id) const override;
    virtual bool contains(ComponentInputMapInterface::Item * const item) const override;
    virtual bool contains(ComponentInputMapInterface::Index const & index) const override;

public:
    virtual ComponentInputMapInterface::Iterator end() override;
    virtual ComponentInputMapInterface::ConstIterator end() const override;

public:
    virtual bool setID(ComponentInputMapInterface::Item * const item,
                       ComponentInputMapInterface::ID const & id) override;
    virtual ComponentInputMapInterface::ID generateID() const override;
    virtual ComponentInputMapInterface::ID getID(ComponentInputMapInterface::Item * const item) const override;
    virtual ComponentInputMapInterface::ID getID(ComponentInputMapInterface::Index const & index) const override;

public:
    virtual ComponentInputMapInterface::Index getIndex(ComponentInputMapInterface::ID const & id) const override;
    virtual ComponentInputMapInterface::Index getIndex(ComponentInputMapInterface::Item * const item) const override;

public:
    virtual ComponentInputMapInterface::Item * getItem(ComponentInputMapInterface::ID const & id) const override;
    virtual ComponentInputMapInterface::Item * getItem(ComponentInputMapInterface::Index const & index) const override;

public:
    virtual bool remove(ComponentInputMapInterface::ID const & id) override;
    virtual bool remove(ComponentInputMapInterface::Item * const item) override;
    virtual bool remove(ComponentInputMapInterface::Index const & index) override;

protected:
    ComponentInputMapInterface::Map map;
};

#endif // COMPONENTINPUTMAPMODEL_H
