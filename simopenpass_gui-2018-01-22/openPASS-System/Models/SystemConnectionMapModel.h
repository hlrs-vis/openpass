/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCONNECTIONMAPMODEL_H
#define SYSTEMCONNECTIONMAPMODEL_H

#include "openPASS-System/SystemConnectionMapInterface.h"

class SystemConnectionMapModel : public SystemConnectionMapInterface
{
    Q_OBJECT

public:
    explicit SystemConnectionMapModel(QObject * const parent = nullptr);
    virtual ~SystemConnectionMapModel() = default;

public:
    virtual SystemConnectionMapInterface::Iterator begin() override;
    virtual SystemConnectionMapInterface::ConstIterator begin() const override;

public:
    virtual SystemConnectionMapInterface::Iterator end() override;
    virtual SystemConnectionMapInterface::ConstIterator end() const override;

public:
    virtual SystemConnectionMapInterface::ID getID(SystemConnectionMapInterface::Item * const item) const override;
    virtual SystemConnectionMapInterface::Item * getItem(SystemConnectionMapInterface::ID const & id) const override;

public:
    virtual bool clear() override;
    virtual bool add(SystemConnectionMapInterface::ID const & id,
                     SystemConnectionMapInterface::Source const * const source,
                     SystemConnectionMapInterface::Target const * const target) override;
    virtual bool add(SystemConnectionMapInterface::Source const * const source,
                     SystemConnectionMapInterface::Target const * const target) override;
    virtual bool contains(SystemConnectionMapInterface::Source const * const source,
                          SystemConnectionMapInterface::Target const * const target) const override;
    virtual bool remove(SystemConnectionMapInterface::Component const * const component) override;
    virtual bool remove(SystemConnectionMapInterface::Source const * const source) override;
    virtual bool remove(SystemConnectionMapInterface::Target const * const target) override;

protected:
    virtual SystemConnectionMapInterface::ID next() const;
    virtual bool remove(QList<SystemConnectionMapInterface::ID> const & ids);

protected:
    SystemConnectionMapInterface::Map connections;
};

#endif // SYSTEMCONNECTIONMAPMODEL_H
