/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMITEMINTERFACE_H
#define SYSTEMITEMINTERFACE_H

#include "openPASS-System/SystemComponentMapInterface.h"
#include "openPASS-System/SystemConnectionMapInterface.h"
#include <QObject>
#include <QString>

class SystemItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = unsigned int;
    using Title = QString;
    using Priority = unsigned int;
    using ComponentMap = SystemComponentMapInterface;
    using ConnectionMap = SystemConnectionMapInterface;

public:
    explicit SystemItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemItemInterface() = default;

Q_SIGNALS:
    void modifiedTitle() const;
    void modifiedPriority() const;

public:
    virtual SystemItemInterface::ID getID() const = 0;

public:
    virtual bool setTitle(SystemItemInterface::Title const & title) = 0;
    virtual SystemItemInterface::Title getTitle() const = 0;

public:
    virtual bool setPriority(SystemItemInterface::Priority const & priority) = 0;
    virtual SystemItemInterface::Priority getPriority() const = 0;

public:
    virtual SystemItemInterface::ComponentMap * getComponents() const = 0;
    virtual SystemItemInterface::ConnectionMap * getConnections() const = 0;

public:
    virtual QList<SystemConnectionMapInterface::Source const *> getCompatibleSources(
            SystemConnectionMapInterface::Target const * const target) = 0;
    virtual QList<SystemConnectionMapInterface::Target const *> getCompatibleTargets(
            SystemConnectionMapInterface::Source const * const source) = 0;
};

#endif // SYSTEMITEMINTERFACE_H
