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
//! @file  ComponentParameterItemInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTPARAMETERITEMINTERFACE_H
#define COMPONENTPARAMETERITEMINTERFACE_H

#include <QObject>
#include <QString>

class ComponentParameterItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = unsigned int;
    using Type = QString;
    using Title = QString;
    using Unit = QString;
    using Value = QString;

public:
    explicit ComponentParameterItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentParameterItemInterface() = default;

Q_SIGNALS:
    void cleared() const;
    void modifiedType() const;
    void modifiedTitle() const;
    void modifiedUnit() const;
    void modifiedValue() const;

public:
    virtual bool clear() = 0;

public:
    virtual bool setID(ComponentParameterItemInterface::ID const & id) = 0;
    virtual ComponentParameterItemInterface::ID getID() const = 0;

public:
    virtual bool setType(ComponentParameterItemInterface::Type const & type) = 0;
    virtual ComponentParameterItemInterface::Type getType() const = 0;

public:
    virtual bool setTitle(ComponentParameterItemInterface::Title const & title) = 0;
    virtual ComponentParameterItemInterface::Title getTitle() const = 0;

public:
    virtual bool setUnit(ComponentParameterItemInterface::Unit const & unit) = 0;
    virtual ComponentParameterItemInterface::Unit getUnit() const = 0;

public:
    virtual bool setValue(ComponentParameterItemInterface::Value const & value) = 0;
    virtual ComponentParameterItemInterface::Value getValue() const = 0;
};

#endif // COMPONENTPARAMETERITEMINTERFACE_H
