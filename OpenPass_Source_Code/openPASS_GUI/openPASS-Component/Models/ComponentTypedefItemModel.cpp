/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/ComponentTypedefItemModel.h"

#include "Models/ComponentTypedefMapModel.h"

ComponentTypedefItemModel::ComponentTypedefItemModel(ComponentTypedefItemInterface::Type const &type,
                                                     QObject * const parent)
    : ComponentTypedefItemInterface(parent)
    , _xmlType(type)
    , _dllType(type)
    , _editType(type)
{
}

ComponentTypedefItemInterface::ID ComponentTypedefItemModel::getID() const
{
    ComponentTypedefMapModel const * const map =
            qobject_cast<ComponentTypedefMapModel const * const>(parent());
    return ((map) ? map->getID(const_cast<ComponentTypedefItemModel *>(this))
                  : ComponentTypedefItemInterface::ID());
}

bool ComponentTypedefItemModel::setXmlType(ComponentTypedefItemInterface::xmlType const &type)
{
    _xmlType = type;
    return true;
}

ComponentTypedefItemInterface::xmlType ComponentTypedefItemModel::getXmlType() const
{
    return _xmlType;
}

bool ComponentTypedefItemModel::setDllType(ComponentTypedefItemInterface::xmlType const &type)
{
    if (type.compare("string") == 0)
    {
        _dllType = "std::wstring";
    }
    else
    {
        _dllType = type;
    }
    return true;
}

ComponentTypedefItemInterface::dllType ComponentTypedefItemModel::getDllType() const
{
    return _dllType;
}

bool ComponentTypedefItemModel::setEditorType(ComponentTypedefItemInterface::xmlType const &type)
{
    if (type.compare("bool") == 0)
    {
        _editType = "Boolean";
    }
    else if (type.compare("int") == 0)
    {
        _editType = "Integer";
    }
    else
    {
        _editType = type.left(1).toUpper()+type.mid(1);
    }
    return true;
}

ComponentTypedefItemInterface::editorType ComponentTypedefItemModel::getEditorType() const
{
    return _editType;
}
