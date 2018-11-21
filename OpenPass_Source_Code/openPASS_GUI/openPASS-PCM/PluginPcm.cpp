/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "PluginPcm.h"

#include "openPASS-Window/WindowInterface.h"

PluginPcm::PluginPcm(QObject *const parent)
    : QObject(parent)
    , modelPcm(nullptr)
    , viewPcm(nullptr)
    , presenterPcm(nullptr)
{
}

bool PluginPcm::initialize()
{
    WindowInterface *const window = WindowInterface::instance();
    if (window)
    {
        modelPcm = new ModelPcm();
        viewPcm = new ViewPcm(window);
        presenterPcm = new PresenterPcm(modelPcm, viewPcm);

        return true;
    }
    return false;
}

bool PluginPcm::deinitialize()
{
    delete presenterPcm;
    delete viewPcm;
    delete modelPcm;
    return true;
}
