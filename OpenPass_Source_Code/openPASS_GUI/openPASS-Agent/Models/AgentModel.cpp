/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/AgentModel.h"

AgentInterface::Filepath const AgentModel::EmptyPath = QStringLiteral("");

AgentModel::AgentModel(QObject *parent)
    : AgentInterface(parent)
    , _filepath(EmptyPath)
{
}

AgentInterface::Filepath AgentModel::getPropertiesFile() const
{
    // Get the path of the directory containing the agent properties
    return _filepath;
}

bool AgentModel::setPropertiesFile(AgentInterface::Filepath const &path)
{
    // Update the path to the directory containing the agent properties
    _filepath = path;
    Q_EMIT update();
    return true;
}
