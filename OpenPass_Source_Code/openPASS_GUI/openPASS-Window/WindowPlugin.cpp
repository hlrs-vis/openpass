/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "WindowPlugin.h"

#include "openPASS/PluginManagerInterface.h"

#include "Models/WindowModel.h"
#include "Presenters/WindowPresenter.h"
#include "Views/WindowView.h"

WindowPlugin::WindowPlugin(QObject * const parent)
    : QObject(parent)
    , windowModel(nullptr)
    , windowPresenter(nullptr)
    , windowView(nullptr)
{
}

bool WindowPlugin::initialize()
{
    PluginManagerInterface * const plugins =
            PluginManagerInterface::instance();
    if (plugins)
    {
        windowModel = new WindowModel();
        windowPresenter = new WindowPresenter(windowModel);
        windowView = new WindowView(windowPresenter);
        connect(plugins, &PluginManagerInterface::started,
                windowView, &WindowView::show);
        return true;
    }
    return false;
}

bool WindowPlugin::deinitialize()
{
    delete windowView;
    delete windowPresenter;
    delete windowModel;
    return true;
}
