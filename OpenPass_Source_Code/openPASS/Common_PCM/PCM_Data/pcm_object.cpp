/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_object.h"

PCM_Object::PCM_Object(int objectType):
    objectType(objectType)
{}

bool PCM_Object::SetObjectType(int objectType)
{
    this->objectType = objectType;
    return true;
}

int PCM_Object::GetObjectType() const
{
    return objectType;
}
