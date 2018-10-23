/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef AGENTPLUGIN_H
#define AGENTPLUGIN_H

#include "openPASS/PluginInterface.h"
#include <QObject>

class AgentModel;
class AgentPresenter;
class AgentView;

class AgentPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.Agent")
    Q_INTERFACES(PluginInterface)

public:
    explicit AgentPlugin(QObject * const parent = nullptr);
    virtual ~AgentPlugin() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    AgentModel * agentModel;
    AgentPresenter * agentPresenter;
    AgentView * agentView;
};

#endif // AGENTPLUGIN_H
