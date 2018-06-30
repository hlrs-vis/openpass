/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef AGENTPRESENTER_H
#define AGENTPRESENTER_H

#include "openPASS-Agent/AgentInterface.h"
#include "Models/AgentModel.h"

#include <QObject>

class AgentPresenter : public QObject
{
    Q_OBJECT

public:
    explicit AgentPresenter(AgentModel *agentModel,
                            QObject *parent = nullptr);
    virtual ~AgentPresenter() = default;

public:
    bool setPropertiesFile(AgentInterface::Filepath const &path);
    AgentInterface::Filepath getPropertiesFile() const;

private:
    AgentModel * _agentModel;

Q_SIGNALS: // Action from the Browse Button
    void updateView() const;
};

#endif // AGENTPRESENTER_H
