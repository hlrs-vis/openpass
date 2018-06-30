/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "SimulationView.h"
#include "ui_SimulationView.h"

#include "Models/SimulationModel.h"
#include "Presenters/SimulationPresenter.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>

const WindowInterface::ID SimulationView::ViewID = QStringLiteral("openPASS.Simulation");

SimulationView::SimulationView(WindowInterface * const window,
                               SimulationPresenter * const simulationPresenter,
                               QWidget *parent)
    : QWidget(parent)
    , window(window)
    , _simulationPresenter(simulationPresenter)
    , ui(new Ui::SimulationView)
    , actionMenuLoad(WindowInterface::createAction(tr("Load Project"), this, &SimulationView::actionLoad))
    , actionMenuSave(WindowInterface::createAction(tr("Save Project"), this, &SimulationView::actionSave))
{
    // Create UI
    ui->setupUi(this);

    // initialize the view
    ui->simulationButton->setEnabled(_simulationPresenter->getProjectStatus());
    updateView();

    // Signals-Slots connection declaration for the Menu-bar Actions
    connect(simulationPresenter, &SimulationPresenter::loaded,
            this, &SimulationView::updateView);
    connect(simulationPresenter, &SimulationPresenter::saved,
            this, &SimulationView::updateView);

    // Signals-Slots connection declaration for actions from the Browser Buttons
    connect(simulationPresenter, &SimulationPresenter::updateView,
            this, &SimulationView::updateView);

    // Register view
    window->add(ViewID, WindowInterface::createButton(tr("Simulation"), 0, 05000), this,
                {actionMenuLoad, actionMenuSave});
}

SimulationView::~SimulationView()
{
    // Deregister view
    window->remove(ViewID);

    // Destroy UI
    delete ui;
}

// Set the Menu-bar action LOAD
void SimulationView::actionLoad()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                this, tr("openPASS / Load Project"), root.canonicalPath(),
                QStringLiteral("Project (*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        _simulationPresenter->loadProject(filepath);
    }
}

// Set the Menu-bar action SAVE
void SimulationView::actionSave()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getSaveFileName(
                this, tr("openPASS / Save Project"), root.canonicalPath(),
                QStringLiteral("XML File (*.xml)"));
    if (!filepath.isNull())
    {
        _simulationPresenter->saveProject(filepath);
    }
}

// Import the Agent Configuration (load file path for the Master)
void SimulationView::on_agentBrowserButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                this, tr("openPASS / Import Agent Configuration"), root.canonicalPath(),
                QStringLiteral("Agent Configuration (*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        _simulationPresenter->setAgentConfigFile(filepath);
    }
}

// Import the Run Configuration (load file path for the Master)
void SimulationView::on_runBrowserButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                this, tr("openPASS / Import Run Configuration"), root.canonicalPath(),
                QStringLiteral("Run Configuration (*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        _simulationPresenter->setRunConfigFile(filepath);
    }
}

// Start/Stop simulation
void SimulationView::on_simulationButton_clicked()
{
    if (!_simulationPresenter->getSimulationStatus()){
        QDir const root = QDir(QCoreApplication::applicationDirPath());
        const QString filepath = QFileDialog::getSaveFileName(
                    this, QStringLiteral("openPASS / Save to Simulate Project"),
                    root.canonicalPath(), QStringLiteral("XML File (*.xml)"));
        if (!filepath.isNull()){
            _simulationPresenter->startSimulation();
//            ui->simulationButton->setText("Stop Simulation");
        }
    }
    else
        _simulationPresenter->stopSimulation();
}

// update the information displayed in the View
void SimulationView::updateView()
{
    ui->sceneryEdit->setText(_simulationPresenter->getSceneryConfig());
    ui->agentConfEdit->setText(_simulationPresenter->getAgentConfigFile());
    ui->runConfEdit->setText(_simulationPresenter->getRunConfigFile());
    ui->simulationButton->setEnabled(_simulationPresenter->getProjectStatus());

    if (_simulationPresenter->getSimulationStatus())
        ui->simulationButton->setText("Stop Simulation");
    else
        ui->simulationButton->setText("Start Simulation");
}
