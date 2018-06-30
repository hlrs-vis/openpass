/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SCENERYPRESENTER_H
#define SCENERYPRESENTER_H

#include "Models/SceneryModel.h"

#include <QObject>

class SceneryPresenter : public QObject
{
    Q_OBJECT

public:
    explicit SceneryPresenter(SceneryModel *sceneryModel,
                              QObject *parent = nullptr);
    virtual ~SceneryPresenter() = default;

public:
    bool setSceneryConfigFile(ProjectInterface::SceneryConfig const &path);
    ProjectInterface::SceneryConfig getSceneryConfigFile() const;

public:
    SceneryModel::Source getScenerySource() const;
    SceneryModel::Agents getSceneryAgents() const;

private:
    SceneryModel * _sceneryModel;

Q_SIGNALS: // Action from the Browse Button
    void updateView() const;
};

#endif // SCENERYPRESENTER_H
