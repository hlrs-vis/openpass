/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SimulationModel.h"

#include <QCoreApplication>
#include <QDir>
#include <qprocess.h>

SimulationModel::SimulationModel(ProjectInterface * const project, QObject *parent)
    : QObject(parent)
    , _project(project)
    , _process(new QProcess(this))
    , simulationStatus(false)
{
    // update actions from the Menu-bar
    connect(_project, &ProjectInterface::loaded,
            this, &SimulationModel::loaded);
    connect(_project, &ProjectInterface::saved,
            this, &SimulationModel::saved);

    // update actions from the Browse Buttons
    connect(_project, &ProjectInterface::update,
            this, &SimulationModel::update);

    // update process state
    connect(_process, SIGNAL(finished(int exitCode)), this, SLOT(setSimulationStatus()));
}

bool SimulationModel::load(QString const &filepath)
{
    return _project->load(filepath);
}

bool SimulationModel::save(QString const &filepath) const
{
    return _project->save(filepath);
}

ProjectInterface::SceneryConfig SimulationModel::getSceneryConfigFile() const
{
    return _project->getSceneryConfigFile();
}

bool SimulationModel::setAgentConfigFile(ProjectInterface::AgentConfig const &agentConfig)
{
    return _project->setAgentConfigFile(agentConfig);
}

ProjectInterface::AgentConfig SimulationModel::getAgentConfigFile() const
{
    return _project->getAgentConfigFile();
}

bool SimulationModel::setRunConfigFile(ProjectInterface::RunConfig const &runConfig)
{
    return _project->setRunConfigFile(runConfig);
}

ProjectInterface::RunConfig SimulationModel::getRunConfigFile() const
{
    return _project->getRunConfigFile();
}

bool SimulationModel::getProjectStatus() const
{
    return _project->getProjectStatus();
}

void SimulationModel::setSimulationStatus()
{
    simulationStatus = false;
    Q_EMIT update();
}

bool SimulationModel::getSimulationStatus() const
{
    return simulationStatus;
}

bool SimulationModel::startSimulation()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString master = (root.canonicalPath()).append("/OpenPassMaster.exe");
    if (!master.isNull()){
        _process->start(master);
        simulationStatus = true;
        Q_EMIT update();
        return true;
    }
    return false;
}

bool SimulationModel::stopSimulation()
{
    _process->kill();
    simulationStatus = false;
    Q_EMIT update();
    return true;
}
