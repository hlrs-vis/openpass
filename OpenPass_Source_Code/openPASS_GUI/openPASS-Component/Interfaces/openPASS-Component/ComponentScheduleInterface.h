/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

//-----------------------------------------------------------------------------
//! @file  ComponentScheduleInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTSCHEDULEINTERFACE_H
#define COMPONENTSCHEDULEINTERFACE_H

#include <QObject>

class ComponentScheduleInterface : public QObject
{
    Q_OBJECT

public:
    using Offset = unsigned int;
    using Cycle = unsigned int;
    using Response = unsigned int;

public:
    explicit ComponentScheduleInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentScheduleInterface() = default;

Q_SIGNALS:
    void cleared() const;
    void modifiedOffset() const;
    void modifiedCycle() const;
    void modifiedResponse() const;

public:
    virtual bool clear() = 0;

public:
    virtual bool setOffset(ComponentScheduleInterface::Offset const & offset) = 0;
    virtual ComponentScheduleInterface::Offset getOffset() const = 0;

public:
    virtual bool setCycle(ComponentScheduleInterface::Cycle const & cycle) = 0;
    virtual ComponentScheduleInterface::Cycle getCycle() const = 0;

public:
    virtual bool setResponse(ComponentScheduleInterface::Response const & response) = 0;
    virtual ComponentScheduleInterface::Response getResponse() const = 0;
};

#endif // COMPONENTSCHEDULEINTERFACE_H
