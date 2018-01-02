/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SIMULATIONVIEW_H
#define SIMULATIONVIEW_H

#include "openPASS-Window/WindowInterface.h"

#include <QString>
#include <QWidget>

namespace Ui {
class SimulationView;
}

class SimulationPresenter;

class SimulationView : public QWidget
{
    Q_OBJECT

public:
    explicit SimulationView(WindowInterface * const window,
                            SimulationPresenter * const simulationPresenter,
                            QWidget *parent = 0);
    virtual ~SimulationView();

private:
    static WindowInterface::ID const ViewID;

private:
    WindowInterface * const window;
    SimulationPresenter * const _simulationPresenter;

private:
    Ui::SimulationView *ui;

private:
    WindowInterface::Action * const actionMenuLoad;
    WindowInterface::Action * const actionMenuSave;

private Q_SLOTS: // Define actions in the Menu-bar
    void actionLoad();
    void actionSave();

private Q_SLOTS: // Define actions for the Browse Buttons
    void on_agentBrowserButton_clicked();
    void on_runBrowserButton_clicked();
    void on_simulationButton_clicked();

private Q_SLOTS: // Update actions from the Browser Buttons and also the load action
    void updateView();
};

#endif // SIMULATIONVIEW_H
