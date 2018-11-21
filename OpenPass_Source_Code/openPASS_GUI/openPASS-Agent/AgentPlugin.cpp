/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "AgentPlugin.h"

#include "openPASS-Window/WindowInterface.h"

#include "Models/AgentModel.h"
#include "Presenters/AgentPresenter.h"
#include "Views/AgentView.h"

AgentPlugin::AgentPlugin(QObject * const parent)
    : QObject(parent)
    , agentModel(nullptr)
    , agentPresenter(nullptr)
    , agentView(nullptr)
{
}

bool AgentPlugin::initialize()
{
    WindowInterface * const window = WindowInterface::instance();
    if (window)
    {
        agentModel = new AgentModel();
        agentPresenter = new AgentPresenter(agentModel);
        agentView = new AgentView(window, agentPresenter);
        return true;
    }
    return false;
}

bool AgentPlugin::deinitialize()
{
    delete agentView;
    delete agentPresenter;
    delete agentModel;
    return true;
}
