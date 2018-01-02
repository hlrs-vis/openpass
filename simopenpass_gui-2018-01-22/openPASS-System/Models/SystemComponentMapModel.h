/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTMAPMODEL_H
#define SYSTEMCOMPONENTMAPMODEL_H

#include "openPASS-System/SystemComponentMapInterface.h"

class SystemComponentItemModel;
class SystemConnectionMapInterface;

class SystemComponentMapModel : public SystemComponentMapInterface
{
    Q_OBJECT

public:
    explicit SystemComponentMapModel(SystemConnectionMapInterface * const connections,
                                     QObject * const parent = nullptr);
    virtual ~SystemComponentMapModel() = default;

public:
    virtual SystemComponentMapInterface::Iterator begin() override;
    virtual SystemComponentMapInterface::ConstIterator begin() const override;

public:
    virtual SystemComponentMapInterface::Iterator end() override;
    virtual SystemComponentMapInterface::ConstIterator end() const override;

public:
    virtual SystemComponentMapInterface::ID getID(SystemComponentMapInterface::Item * const item) const override;
    virtual SystemComponentMapInterface::Item * getItem(SystemComponentMapInterface::ID const & id) const override;

public:
    virtual bool clear() override;
    virtual bool add(SystemComponentMapInterface::ID const & id,
                     SystemComponentMapInterface::Item * const component) override;
    virtual bool add(ComponentItemInterface const * const component,
                     SystemComponentItemInterface::Position const & position) override;
    virtual bool remove(SystemComponentMapInterface::ID const & id) override;

public:
    virtual bool duplicate(SystemComponentItemModel const * const component,
                           SystemComponentItemInterface::Position const & position);

protected:
    virtual SystemComponentMapInterface::ID next() const;

protected:
    SystemConnectionMapInterface * const connections;
    SystemComponentMapInterface::Map components;
};

#endif // SYSTEMCOMPONENTMAPMODEL_H
