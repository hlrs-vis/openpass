/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTSCHEDULEINTERFACE_H
#define SYSTEMCOMPONENTSCHEDULEINTERFACE_H

#include "openPASS-Component/ComponentScheduleInterface.h"
#include <QObject>

class SystemComponentScheduleInterface : public QObject
{
    Q_OBJECT

public:
    using Priority = unsigned int;
    using Offset = ComponentScheduleInterface::Offset;
    using Cycle = ComponentScheduleInterface::Cycle;
    using Response = ComponentScheduleInterface::Response;

public:
    explicit SystemComponentScheduleInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~SystemComponentScheduleInterface() = default;

Q_SIGNALS:
    void modifiedPriority() const;
    void modifiedOffset() const;
    void modifiedCycle() const;
    void modifiedResponse() const;

public:
    virtual bool setPriority(SystemComponentScheduleInterface::Priority const & priority) = 0;
    virtual SystemComponentScheduleInterface::Priority getPriority() const = 0;

public:
    virtual bool setOffset(SystemComponentScheduleInterface::Offset const & offset) = 0;
    virtual SystemComponentScheduleInterface::Offset getOffset() const = 0;

public:
    virtual bool setCycle(SystemComponentScheduleInterface::Cycle const & cycle) = 0;
    virtual SystemComponentScheduleInterface::Cycle getCycle() const = 0;

public:
    virtual bool setResponse(SystemComponentScheduleInterface::Response const & response) = 0;
    virtual SystemComponentScheduleInterface::Response getResponse() const = 0;
};

#endif // SYSTEMCOMPONENTSCHEDULEINTERFACE_H
