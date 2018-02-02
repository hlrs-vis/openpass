/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Presenters/SceneryPresenter.h"

#include "Models/SceneryModel.h"

SceneryPresenter::SceneryPresenter(SceneryModel *sceneryModel,
                                   QObject *parent)
    : QObject(parent)
    , _sceneryModel(sceneryModel)
{
    // Signals-Signals connection declaration for actions from the Browser Buttons
    connect(sceneryModel, &SceneryModel::update,
            this, &SceneryPresenter::updateView);
}

bool SceneryPresenter::setSceneryConfigFile(ProjectInterface::SceneryConfig const &path)
{
    return _sceneryModel->setSceneryConfigFile(path);
}

ProjectInterface::SceneryConfig SceneryPresenter::getSceneryConfigFile() const
{
    return _sceneryModel->getSceneryConfigFile();
}

SceneryModel::Source SceneryPresenter::getScenerySource() const
{
    return _sceneryModel->getScenerySource();
}

SceneryModel::Agents SceneryPresenter::getSceneryAgents() const
{
    return _sceneryModel->getSceneryAgents();
}
