/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMPLUGIN_H
#define SYSTEMPLUGIN_H

#include "openPASS/PluginInterface.h"
#include <QObject>

class SystemModel;
class SystemView;

class SystemPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.System")
    Q_INTERFACES(PluginInterface)

public:
    explicit SystemPlugin(QObject * const parent = nullptr);
    virtual ~SystemPlugin() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    SystemModel * systemModel;
    SystemView * systemView;
};

#endif // SYSTEMPLUGIN_H
