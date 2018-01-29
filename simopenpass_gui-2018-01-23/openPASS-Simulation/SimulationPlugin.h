/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SIMULATIONPLUGIN_H
#define SIMULATIONPLUGIN_H

#include "openPASS/PluginInterface.h"
#include <QObject>

class SimulationModel;
class SimulationPresenter;
class SimulationView;

class SimulationPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "openPASS.Simulation")
    Q_INTERFACES(PluginInterface)

public:
    explicit SimulationPlugin(QObject * const parent = nullptr);
    virtual ~SimulationPlugin() = default;

public:
    virtual bool initialize() override;
    virtual bool deinitialize() override;

protected:
    SimulationModel * simulationModel;
    SimulationPresenter * simulationPresenter;
    SimulationView * simulationView;
};

#endif // SIMULATIONPLUGIN_H
