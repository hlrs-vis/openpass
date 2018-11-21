/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef AGENTMODEL_H
#define AGENTMODEL_H

#include "openPASS-Agent/AgentInterface.h"

class AgentModel : public AgentInterface
{
    Q_OBJECT

public:
    explicit AgentModel(QObject *parent = nullptr);
    virtual ~AgentModel() = default;

public:
    virtual AgentInterface::Filepath getPropertiesFile() const override;

public:
    virtual bool setPropertiesFile(AgentInterface::Filepath const &path);

protected:
    static AgentInterface::Filepath const EmptyPath;

protected:
    AgentInterface::Filepath _filepath;
};

#endif // AGENTMODEL_H
