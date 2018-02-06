/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTSCHEDULEMODEL_H
#define SYSTEMCOMPONENTSCHEDULEMODEL_H

#include "openPASS-System/SystemComponentScheduleInterface.h"

class SystemComponentScheduleModel : public SystemComponentScheduleInterface
{
    Q_OBJECT

public:
    explicit SystemComponentScheduleModel(ComponentScheduleInterface const * const schedule,
                                          QObject * const parent = nullptr);
    virtual ~SystemComponentScheduleModel() = default;

public:
    virtual bool setPriority(SystemComponentScheduleInterface::Priority const & priority) override;
    virtual bool checkValidPriority(SystemComponentScheduleInterface::Priority const & priority);
    virtual SystemComponentScheduleInterface::Priority getPriority() const override;

public:
    virtual bool setOffset(SystemComponentScheduleInterface::Offset const & offset) override;
    virtual SystemComponentScheduleInterface::Offset getOffset() const override;

public:
    virtual bool setCycle(SystemComponentScheduleInterface::Cycle const & cycle) override;
    virtual SystemComponentScheduleInterface::Cycle getCycle() const override;

public:
    virtual bool setResponse(SystemComponentScheduleInterface::Response const & response) override;
    virtual SystemComponentScheduleInterface::Response getResponse() const override;

protected:
    SystemComponentScheduleInterface::Priority priority;
    SystemComponentScheduleInterface::Offset offset;
    SystemComponentScheduleInterface::Cycle cycle;
    SystemComponentScheduleInterface::Response response;
};

#endif // SYSTEMCOMPONENTSCHEDULEMODEL_H
