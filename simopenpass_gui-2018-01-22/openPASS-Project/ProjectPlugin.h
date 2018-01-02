/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef PROJECTPLUGIN_H
#define PROJECTPLUGIN_H

#include "openPASS/PluginInterface.h"
#include <QObject>

class ProjectModel;
class ProjectPresenter;
class ProjectView;

class ProjectPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.Project")
    Q_INTERFACES(PluginInterface)

public:
    explicit ProjectPlugin(QObject * const parent = nullptr);
    virtual ~ProjectPlugin() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    ProjectModel * projectModel;
    ProjectPresenter * projectPresenter;
    ProjectView * projectView;
};

#endif // PROJECTPLUGIN_H
