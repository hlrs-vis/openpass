/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTTYPEDEFITEMMODEL_H
#define COMPONENTTYPEDEFITEMMODEL_H

#include "openPASS-Component/ComponentTypedefItemInterface.h"

class ComponentTypedefItemModel : public ComponentTypedefItemInterface
{
    Q_OBJECT

public:
    explicit ComponentTypedefItemModel(ComponentTypedefItemInterface::Type const & type,
                                       QObject * const parent = nullptr);
    virtual ~ComponentTypedefItemModel() = default;

public:
    virtual ComponentTypedefItemInterface::ID getID() const override;

public:
    virtual bool setXmlType(ComponentTypedefItemInterface::xmlType const & type) override;
    virtual ComponentTypedefItemInterface::xmlType getXmlType() const override;

public:
    virtual bool setDllType(ComponentTypedefItemInterface::xmlType const & type) override;
    virtual ComponentTypedefItemInterface::dllType getDllType() const override;

public:
    virtual bool setEditorType(ComponentTypedefItemInterface::xmlType const & type) override;
    virtual ComponentTypedefItemInterface::editorType getEditorType() const override;

protected:
    ComponentTypedefItemInterface::xmlType _xmlType;
    ComponentTypedefItemInterface::dllType _dllType;
    ComponentTypedefItemInterface::editorType _editType;
};

#endif // COMPONENTTYPEDEFITEMMODEL_H
