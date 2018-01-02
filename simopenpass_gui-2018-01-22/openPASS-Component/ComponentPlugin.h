/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef COMPONENTPLUGIN_H
#define COMPONENTPLUGIN_H

#include "openPASS/PluginInterface.h"
#include <QObject>

class ComponentModel;

class ComponentPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.Component")
    Q_INTERFACES(PluginInterface)

public:
    explicit ComponentPlugin(QObject * const parent = nullptr);
    virtual ~ComponentPlugin() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    ComponentModel * componentModel;
};

#endif // COMPONENTPLUGIN_H
