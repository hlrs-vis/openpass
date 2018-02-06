/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "SimulationPlugin.h"

#include "openPASS-Project/ProjectInterface.h"
#include "openPASS-Window/WindowInterface.h"

#include "Models/SimulationModel.h"
#include "Presenters/SimulationPresenter.h"
#include "Views/SimulationView.h"

SimulationPlugin::SimulationPlugin(QObject * const parent)
    : QObject(parent)
    , simulationModel(nullptr)
    , simulationPresenter(nullptr)
    , simulationView(nullptr)
{
}

bool SimulationPlugin::initialize()
{
    ProjectInterface * const project = ProjectInterface::instance();
    WindowInterface * const window = WindowInterface::instance();
    if (project && window)
    {
        simulationModel = new SimulationModel(project);
        simulationPresenter = new SimulationPresenter(simulationModel);
        simulationView = new SimulationView(window, simulationPresenter);
        return true;
    }
    return false;
}

bool SimulationPlugin::deinitialize()
{
    delete simulationView;
    delete simulationPresenter;
    delete simulationModel;
    return true;
}
