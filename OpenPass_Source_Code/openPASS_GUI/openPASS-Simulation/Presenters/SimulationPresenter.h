/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SIMULATIONPRESENTER_H
#define SIMULATIONPRESENTER_H

#include "Models/SimulationModel.h"

#include <QObject>
#include <QString>

class SimulationPresenter : public QObject
{
    Q_OBJECT

public:
    explicit SimulationPresenter(SimulationModel *simulationModel,
                                 QObject *parent = nullptr);
    virtual ~SimulationPresenter() = default;

public:
    virtual bool loadProject(QString const &filepath) const;
    virtual bool saveProject(QString const &filepath) const;

public:
    virtual ProjectInterface::SceneryConfig getSceneryConfig() const;

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
    virtual bool startSimulation() const;
    virtual bool stopSimulation() const;

private:
    SimulationModel * _simulationModel;

Q_SIGNALS: // Actions from the menu bar
    void loaded() const;
    void saved() const;

Q_SIGNALS: // Actions from the Browse Buttons
    void updateView() const;
};

#endif // SIMULATIONPRESENTER_H
