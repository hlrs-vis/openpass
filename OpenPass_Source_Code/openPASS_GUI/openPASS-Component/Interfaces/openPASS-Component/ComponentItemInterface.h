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
//! @file  ComponentItemInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTITEMINTERFACE_H
#define COMPONENTITEMINTERFACE_H

#include "openPASS-Component/ComponentInputMapInterface.h"
#include "openPASS-Component/ComponentOutputMapInterface.h"
#include "openPASS-Component/ComponentParameterMapInterface.h"
#include "openPASS-Component/ComponentScheduleInterface.h"
#include <QObject>
#include <QString>

class ComponentItemInterface : public QObject
{
    Q_OBJECT

public:
    using Name = QString;
    enum class Type : unsigned char
    {
        Action = 0,
        Algorithm = 1,
        Sensor = 2,
    };
    using Title = QString;
    using Schedule = ComponentScheduleInterface;
    using ParameterMap = ComponentParameterMapInterface;
    using InputMap = ComponentInputMapInterface;
    using OutputMap = ComponentOutputMapInterface;

public:
    explicit ComponentItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentItemInterface() = default;

Q_SIGNALS:
    void cleared() const;
    void loaded() const;
    void saved() const;

Q_SIGNALS:
    void modifiedType() const;
    void modifiedTitle() const;

public:
    virtual bool clear() = 0;
    virtual bool load(QString const & filepath) = 0;
    virtual bool save(QString const & filepath) const = 0;

public:
    virtual ComponentItemInterface::Name getName() const = 0;

public:
    virtual bool setType(ComponentItemInterface::Type const & type) = 0;
    virtual ComponentItemInterface::Type getType() const = 0;

public:
    virtual bool setTitle(ComponentItemInterface::Title const & title) = 0;
    virtual ComponentItemInterface::Title getTitle() const = 0;

public:
    virtual ComponentItemInterface::Schedule * getSchedule() const = 0;
    virtual ComponentItemInterface::ParameterMap * getParameters() const = 0;
    virtual ComponentItemInterface::InputMap * getInputs() const = 0;
    virtual ComponentItemInterface::OutputMap * getOutputs() const = 0;
};

#endif // COMPONENTITEMINTERFACE_H
