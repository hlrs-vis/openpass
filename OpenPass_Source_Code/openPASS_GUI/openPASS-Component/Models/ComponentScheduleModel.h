/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTSCHEDULEMODEL_H
#define COMPONENTSCHEDULEMODEL_H

#include "openPASS-Component/ComponentScheduleInterface.h"

class ComponentScheduleModel : public ComponentScheduleInterface
{
    Q_OBJECT

public:
    explicit ComponentScheduleModel(QObject * const parent = nullptr);
    virtual ~ComponentScheduleModel() = default;

public:
    virtual bool clear() override;

public:
    virtual bool setOffset(ComponentScheduleInterface::Offset const & offset) override;
    virtual ComponentScheduleInterface::Offset getOffset() const override;

public:
    virtual bool setCycle(ComponentScheduleInterface::Cycle const & cycle) override;
    virtual ComponentScheduleInterface::Cycle getCycle() const override;

public:
    virtual bool setResponse(ComponentScheduleInterface::Response const & response) override;
    virtual ComponentScheduleInterface::Response getResponse() const override;

protected:
    static ComponentScheduleInterface::Offset const DefaultOffset;
    static ComponentScheduleInterface::Cycle const DefaultCycle;
    static ComponentScheduleInterface::Response const DefaultResponse;

protected:
    ComponentScheduleInterface::Offset offset;
    ComponentScheduleInterface::Cycle cycle;
    ComponentScheduleInterface::Response response;
};

#endif // COMPONENTSCHEDULEMODEL_H
