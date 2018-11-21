/******************************************************************************
* Copyright (c) 2017 Volkswagen Group of America.
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "ProjectPlugin.h"

#include "openPASS-Window/WindowInterface.h"

#include "Models/ProjectModel.h"
#include "Presenters/ProjectPresenter.h"
#include "Views/ProjectView.h"

ProjectPlugin::ProjectPlugin(QObject *const parent)
    : QObject(parent)
    , projectModel(nullptr)
    , projectPresenter(nullptr)
    , projectView(nullptr)
{
}

bool ProjectPlugin::initialize()
{
    WindowInterface *const window = WindowInterface::instance();
    if (window)
    {
        projectModel = new ProjectModel();
        projectPresenter = new ProjectPresenter(projectModel);
        //projectView = new ProjectView(window, projectPresenter);
        return true;
    }
    return false;
}

bool ProjectPlugin::deinitialize()
{
    delete projectView;
    delete projectPresenter;
    delete projectModel;
    return true;
}
