/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Presenters/SimulationPresenter.h"

#include "Models/SimulationModel.h"

SimulationPresenter::SimulationPresenter(SimulationModel *simulationModel,
                                         QObject *parent)
    : QObject(parent)
    , _simulationModel(simulationModel)
{
    // Signals-Signals connection declaration for the Menu-bar Actions
    connect(simulationModel, &SimulationModel::loaded,
            this, &SimulationPresenter::loaded);
    connect(simulationModel, &SimulationModel::saved,
            this, &SimulationPresenter::saved);

    // Signals-Signals connection declaration for actions from the Browser Buttons
    connect(simulationModel, &SimulationModel::update,
            this, &SimulationPresenter::updateView);
}

bool SimulationPresenter::loadProject(QString const &filepath) const
{
    return _simulationModel->load(filepath);
}

bool SimulationPresenter::saveProject(QString const &filepath) const
{
    return _simulationModel->save(filepath);
}

ProjectInterface::SceneryConfig SimulationPresenter::getSceneryConfig() const
{
    return _simulationModel->getSceneryConfigFile();
}

bool SimulationPresenter::setAgentConfigFile(ProjectInterface::AgentConfig const &agentConfig)
{
    return _simulationModel->setAgentConfigFile(agentConfig);
}

ProjectInterface::AgentConfig SimulationPresenter::getAgentConfigFile() const
{
    return _simulationModel->getAgentConfigFile();
}

bool SimulationPresenter::setRunConfigFile(ProjectInterface::RunConfig const &runConfig)
{
    return _simulationModel->setRunConfigFile(runConfig);
}

ProjectInterface::RunConfig SimulationPresenter::getRunConfigFile() const
{
    return _simulationModel->getRunConfigFile();
}

bool SimulationPresenter::getProjectStatus() const
{
    return _simulationModel->getProjectStatus();
}

bool SimulationPresenter::getSimulationStatus() const
{
    return _simulationModel->getSimulationStatus();
}

bool SimulationPresenter::startSimulation() const
{
    return _simulationModel->startSimulation();
}

bool SimulationPresenter::stopSimulation() const
{
    return _simulationModel->stopSimulation();
}
