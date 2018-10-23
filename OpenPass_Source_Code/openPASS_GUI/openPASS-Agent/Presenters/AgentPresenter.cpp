/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Presenters/AgentPresenter.h"

#include "Models/AgentModel.h"

AgentPresenter::AgentPresenter(AgentModel *agentModel,
                               QObject *parent)
    : QObject(parent)
    , _agentModel(agentModel)
{
    // Signals-Signals connection declaration for actions from the Browser Buttons
    connect(agentModel, &AgentModel::update,
            this, &AgentPresenter::updateView);
}

bool AgentPresenter::setPropertiesFile(AgentInterface::Filepath const &path)
{
    return _agentModel->setPropertiesFile(path);
}

AgentInterface::Filepath AgentPresenter::getPropertiesFile() const
{
    return _agentModel->getPropertiesFile();
}
