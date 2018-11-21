/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "PluginPcm_Eval.h"

#include "openPASS-Window/WindowInterface.h"

PluginPcm_Eval::PluginPcm_Eval(QObject *const parent)
    : QObject(parent)
    , modelPcm_Eval(nullptr)
    , viewPcm_Eval(nullptr)
    , presenterPcm_Eval(nullptr)
{
}

bool PluginPcm_Eval::initialize()
{
    WindowInterface *const window = WindowInterface::instance();
    if (window)
    {
        modelPcm_Eval = new ModelPcm_Eval();
        viewPcm_Eval = new ViewPcm_Eval(window);
        presenterPcm_Eval = new PresenterPcm_Eval(modelPcm_Eval, viewPcm_Eval);

        return true;
    }
    return false;
}

bool PluginPcm_Eval::deinitialize()
{
    delete presenterPcm_Eval;
    delete viewPcm_Eval;
    delete modelPcm_Eval;
    return true;
}
