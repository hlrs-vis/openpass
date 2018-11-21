/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "SystemPlugin.h"

#include "openPASS-Component/ComponentInterface.h"
#include "openPASS-Project/ProjectInterface.h"
#include "openPASS-Window/WindowInterface.h"

#include "Models/SystemModel.h"
#include "Views/SystemView.h"

SystemPlugin::SystemPlugin(QObject * const parent)
    : QObject(parent)
    , systemModel(nullptr)
    , systemView(nullptr)
{
}

bool SystemPlugin::initialize()
{
    ComponentInterface * const component = ComponentInterface::instance();
    ProjectInterface * const project = ProjectInterface::instance();
    WindowInterface * const window = WindowInterface::instance();
    if (component && project && window)
    {
        systemModel = new SystemModel(component, project);
        systemView = new SystemView(window, project, systemModel);
        return true;
    }
    return false;
}

bool SystemPlugin::deinitialize()
{
    delete systemView;
    delete systemModel;
    return true;
}
