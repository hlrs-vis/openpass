/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include "openPASS-Project/ProjectInterface.h"

#include <QObject>
#include <QProcess>

class SimulationModel : public QObject
{
    Q_OBJECT

public:
    using Status = bool;

public:
    explicit SimulationModel(ProjectInterface * const project, QObject *parent = nullptr);
    virtual ~SimulationModel() = default;

public:
    virtual bool load(QString const &filepath);
    virtual bool save(QString const &filepath) const;

public:
    virtual ProjectInterface::SceneryConfig getSceneryConfigFile() const;

public:
    virtual bool setAgentConfigFile(ProjectInterface::AgentConfig const &agentConfig);
    virtual ProjectInterface::AgentConfig getAgentConfigFile() const;

public:
    virtual bool setRunConfigFile(ProjectInterface::RunConfig const &runConfig);
    virtual ProjectInterface::RunConfig getRunConfigFile() const;

public:
    virtual bool getProjectStatus() const;
    virtual bool getSimulationStatus() const;

public:
    virtual bool startSimulation();
    virtual bool stopSimulation();

protected:
    ProjectInterface * const _project;

protected:
    QProcess * _process;

private:
    SimulationModel::Status simulationStatus; // simulation is running = 1;

public Q_SLOTS:
    void setSimulationStatus();

Q_SIGNALS: // update actions from the Menu-bar
    void loaded() const;
    void saved() const;

Q_SIGNALS: // update actions from the Browse Buttons
    void update();
};

#endif // SIMULATIONMODEL_H
