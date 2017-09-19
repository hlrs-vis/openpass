/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
