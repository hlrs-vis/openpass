/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef WINDOWPLUGIN_H
#define WINDOWPLUGIN_H

#include "openPASS/PluginInterface.h"
#include <QObject>

class WindowModel;
class WindowPresenter;
class WindowView;

class WindowPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.Window")
    Q_INTERFACES(PluginInterface)

public:
    explicit WindowPlugin(QObject * const parent = nullptr);
    virtual ~WindowPlugin() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    WindowModel * windowModel;
    WindowPresenter * windowPresenter;
    WindowView * windowView;
};

#endif // WINDOWPLUGIN_H
