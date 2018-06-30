/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "SceneryPlugin.h"

#include "openPASS-Project/ProjectInterface.h"
#include "openPASS-Window/WindowInterface.h"

#include "Models/SceneryModel.h"
#include "Presenters/SceneryPresenter.h"
#include "Views/SceneryView.h"

SceneryPlugin::SceneryPlugin(QObject * const parent)
    : QObject(parent)
    , sceneryModel(nullptr)
    , sceneryPresenter(nullptr)
    , sceneryView(nullptr)
{
}

bool SceneryPlugin::initialize()
{
    ProjectInterface * const project = ProjectInterface::instance();
    WindowInterface * const window = WindowInterface::instance();
    if (project && window)
    {
        sceneryModel = new SceneryModel(project);
        sceneryPresenter = new SceneryPresenter(sceneryModel);
        sceneryView = new SceneryView(window, sceneryPresenter);
        return true;
    }
    return false;
}

bool SceneryPlugin::deinitialize()
{
    delete sceneryView;
    delete sceneryPresenter;
    delete sceneryModel;
    return true;
}
