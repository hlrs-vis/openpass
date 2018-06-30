/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMITEMMODEL_H
#define SYSTEMITEMMODEL_H

#include "openPASS-System/SystemItemInterface.h"

class SystemItemModel : public SystemItemInterface
{
    Q_OBJECT

public:
    explicit SystemItemModel(QObject * const parent = nullptr);
    virtual ~SystemItemModel() = default;

public:
    virtual SystemItemInterface::ID getID() const override;

public:
    virtual bool setTitle(SystemItemInterface::Title const & title) override;
    virtual SystemItemInterface::Title getTitle() const override;

public:
    virtual bool setPriority(SystemItemInterface::Priority const & priority) override;
    virtual SystemItemInterface::Priority getPriority() const override;

public:
    virtual SystemItemInterface::ComponentMap * getComponents() const override;
    virtual SystemItemInterface::ConnectionMap * getConnections() const override;

public:
    virtual QList<SystemConnectionMapInterface::Source const *> getCompatibleSources(
            SystemConnectionMapInterface::Target const * const target) override;
    virtual QList<SystemConnectionMapInterface::Target const *> getCompatibleTargets(
            SystemConnectionMapInterface::Source const * const source) override;

protected:
    SystemItemInterface::Title title;
    SystemItemInterface::Priority priority;
    SystemItemInterface::ConnectionMap * const connections;
    SystemItemInterface::ComponentMap * const components;
};

#endif // SYSTEMITEMMODEL_H
