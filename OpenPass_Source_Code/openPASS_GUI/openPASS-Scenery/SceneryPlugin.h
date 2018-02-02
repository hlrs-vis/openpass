/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SCENERYPLUGIN_H
#define SCENERYPLUGIN_H

#include "openPASS/PluginInterface.h"
#include <QObject>

class SceneryModel;
class SceneryPresenter;
class SceneryView;

class SceneryPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.Scenery")
    Q_INTERFACES(PluginInterface)

public:
    explicit SceneryPlugin(QObject * const parent = nullptr);
    virtual ~SceneryPlugin() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    SceneryModel * sceneryModel;
    SceneryPresenter * sceneryPresenter;
    SceneryView * sceneryView;
};

#endif // SCENERYPLUGIN_H
