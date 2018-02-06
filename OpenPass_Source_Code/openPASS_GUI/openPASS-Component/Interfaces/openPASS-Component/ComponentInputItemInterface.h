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
//! @file  ComponentInputItemInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef COMPONENTINPUTITEMINTERFACE_H
#define COMPONENTINPUTITEMINTERFACE_H

#include <QObject>
#include <QString>

class ComponentInputItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = unsigned int;
    using Type = QString;
    using Title = QString;
    using Unit = QString;
    using Cardinality = QString;

public:
    explicit ComponentInputItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentInputItemInterface() = default;

Q_SIGNALS:
    void cleared() const;
    void modifiedType() const;
    void modifiedTitle() const;
    void modifiedUnit() const;
    void modifiedCardinality() const;

public:
    virtual bool clear() = 0;

public:
    virtual bool setID(ComponentInputItemInterface::ID const & id) = 0;
    virtual ComponentInputItemInterface::ID getID() const = 0;

public:
    virtual bool setType(ComponentInputItemInterface::Type const & type) = 0;
    virtual ComponentInputItemInterface::Type getType() const = 0;

public:
    virtual bool setTitle(ComponentInputItemInterface::Title const & title) = 0;
    virtual ComponentInputItemInterface::Title getTitle() const = 0;

public:
    virtual bool setUnit(ComponentInputItemInterface::Unit const & unit) = 0;
    virtual ComponentInputItemInterface::Unit getUnit() const = 0;

public:
    virtual bool setCardinality(ComponentInputItemInterface::Cardinality const & cardinality) = 0;
    virtual ComponentInputItemInterface::Cardinality getCardinality() const = 0;
};

#endif // COMPONENTINPUTITEMINTERFACE_H
