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
//! @file  ComponentOutputItemInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTOUTPUTITEMINTERFACE_H
#define COMPONENTOUTPUTITEMINTERFACE_H

#include <QObject>
#include <QString>

class ComponentOutputItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = unsigned int;
    using Type = QString;
    using Title = QString;
    using Unit = QString;

public:
    explicit ComponentOutputItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentOutputItemInterface() = default;

Q_SIGNALS:
    void cleared() const;
    void modifiedType() const;
    void modifiedTitle() const;
    void modifiedUnit() const;

public:
    virtual bool clear() = 0;

public:
    virtual bool setID(ComponentOutputItemInterface::ID const & id) = 0;
    virtual ComponentOutputItemInterface::ID getID() const = 0;

public:
    virtual bool setType(ComponentOutputItemInterface::Type const & type) = 0;
    virtual ComponentOutputItemInterface::Type getType() const = 0;

public:
    virtual bool setTitle(ComponentOutputItemInterface::Title const & title) = 0;
    virtual ComponentOutputItemInterface::Title getTitle() const = 0;

public:
    virtual bool setUnit(ComponentOutputItemInterface::Unit const & unit) = 0;
    virtual ComponentOutputItemInterface::Unit getUnit() const = 0;
};

#endif // COMPONENTOUTPUTITEMINTERFACE_H
