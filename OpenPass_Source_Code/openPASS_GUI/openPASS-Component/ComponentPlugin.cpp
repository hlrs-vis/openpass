/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "ComponentPlugin.h"

#include "Models/ComponentModel.h"

ComponentPlugin::ComponentPlugin(QObject * const parent)
    : QObject(parent)
{
}

bool ComponentPlugin::initialize()
{
    componentModel = new ComponentModel();
    return true;
}

bool ComponentPlugin::deinitialize()
{
    delete componentModel;
    return true;
}
