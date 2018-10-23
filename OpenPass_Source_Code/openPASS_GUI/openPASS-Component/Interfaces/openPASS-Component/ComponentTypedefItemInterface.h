/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTTYPEDEFITEMINTERFACE_H
#define COMPONENTTYPEDEFITEMINTERFACE_H

#include <QObject>
#include <QString>

class ComponentTypedefItemInterface : public QObject
{
    Q_OBJECT

public:
    using ID = unsigned int;
    using Type = QString;
    using xmlType = QString;
    using dllType = QString;
    using editorType = QString;

public:
    explicit ComponentTypedefItemInterface(QObject * const parent = nullptr)
        : QObject(parent) {}
    virtual ~ComponentTypedefItemInterface() = default;

public:
    virtual ComponentTypedefItemInterface::ID getID() const = 0;

public:
    virtual bool setXmlType(ComponentTypedefItemInterface::xmlType const & type) = 0;
    virtual ComponentTypedefItemInterface::xmlType getXmlType() const = 0;

public:
    virtual bool setDllType(ComponentTypedefItemInterface::xmlType const & type) = 0;
    virtual ComponentTypedefItemInterface::dllType getDllType() const = 0;

public:
    virtual bool setEditorType(ComponentTypedefItemInterface::xmlType const & type) = 0;
    virtual ComponentTypedefItemInterface::editorType getEditorType() const = 0;
};

#endif // COMPONENTTYPEDEFITEMINTERFACE_H
