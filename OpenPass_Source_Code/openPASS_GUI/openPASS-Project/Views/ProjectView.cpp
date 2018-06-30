/******************************************************************************
* Copyright (c) 2017 Volkswagen Group of America.
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#include "ProjectView.h"
#include "ui_ProjectView.h"

#include "Models/ProjectModel.h"
#include "Presenters/ProjectPresenter.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QStandardPaths>
#include <QString>

const WindowInterface::ID ProjectView::ViewID = QStringLiteral("openPASS.Project");

ProjectView::ProjectView(WindowInterface *const window,
                         ProjectPresenter *const projectPresenter,
                         QWidget *parent)
    : QWidget(parent)
    , window(window)
    , projectPresenter(projectPresenter)
    , ui(new Ui::ProjectView)
    , actionMenuNew(WindowInterface::createAction(tr("New Project"), this,
                                                  &ProjectView::actionProjectNew))
    , actionMenuLoad(WindowInterface::createAction(tr("Load Project"), this,
                                                   &ProjectView::actionProjectLoad))
    , actionMenuSave(WindowInterface::createAction(tr("Save Project"), this,
                                                   &ProjectView::actionProjectSave))
{
    // Create UI
    ui->setupUi(this);

    // Update the view
    updateView();

    // Signals-Slots connection declaration for the Menu-bar Actions
    connect(projectPresenter, &ProjectPresenter::loaded,
            this, &ProjectView::updateView);
    connect(projectPresenter, &ProjectPresenter::saved,
            this, &ProjectView::updateView);

    // Signals-Slots connection declaration for actions from the Browser Buttons
    connect(projectPresenter, &ProjectPresenter::updateView,
            this, &ProjectView::updateView);

    // Register view
    window->add(ViewID, WindowInterface::createButton(tr("Project"), 0, 01000), this,
    {actionMenuNew, actionMenuLoad, actionMenuSave});
}

ProjectView::~ProjectView()
{
    // Deregister view
    window->remove(ViewID);

    // Destroy UI
    delete ui;
}


// Set the Menu-bar action NEW
void ProjectView::actionProjectNew()
{
    projectPresenter->createNewProject();
}

// Set the Menu-bar action LOAD
void ProjectView::actionProjectLoad()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                                 this, tr("openPASS / Load Project"), root.canonicalPath(),
                                 QStringLiteral("Project (*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        projectPresenter->loadProject(filepath);
    }
}

// Set the Menu-bar action SAVE
void ProjectView::actionProjectSave()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getSaveFileName(
                                 this, tr("openPASS / Save Project"), root.canonicalPath(),
                                 QStringLiteral("XML File (*.xml)"));
    if (!filepath.isNull())
    {
        projectPresenter->saveProject(filepath);
    }
}

// Import the Component Library (load path for the Master)
void ProjectView::on_libraryBrowseButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getExistingDirectory(
                                 this, tr("openPASS / Import Component Library"),
                                 root.canonicalPath());
    if (!filepath.isNull())
    {
        projectPresenter->setLibraryPath(filepath);
    }
}

// Set a Log Level for the Master
void ProjectView::on_logLevelSpinBox_valueChanged(int level)
{
    projectPresenter->setLogLevel(static_cast<ProjectInterface::LogLevel>(level));
}

// Select path to save the Log Master File
void ProjectView::on_logMasterBrowseButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                                 this, tr("openPASS / Directory to save Log files"), root.canonicalPath(),
                                 QStringLiteral("Log Master File (*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        projectPresenter->setLogMaster(filepath);
    }
}

// Select path to save the Log Slave File
void ProjectView::on_logSlaveBrowseButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                                 this, tr("openPASS / Directory to save Log files"), root.canonicalPath(),
                                 QStringLiteral("Log Slave File (*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        projectPresenter->setLogSlave(filepath);
    }
}

// Select path to save the results of the simulation(s)
void ProjectView::on_outputBrowseButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getExistingDirectory(
                                 this, tr("openPASS / Directory to save results"),
                                 root.canonicalPath());
    if (!filepath.isNull())
    {
        projectPresenter->setResultPath(filepath);
    }
}

// Select path to save the Log Slave File
void ProjectView::on_slaveBrowseButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                                 this, tr("openPASS / Select the openPASS Slave"), root.canonicalPath(),
                                 QStringLiteral("Slave (*.exe)"));
    if (!filepath.isNull())
    {
        projectPresenter->setSlave(filepath);
    }
}

// update the information displayed in the View
void ProjectView::updateView()
{
    ui->libraryPathEdit->setText(projectPresenter->getLibraryPath());
    ui->logLevelSpinBox->setValue(projectPresenter->getLogLevel());
    ui->logMasterPathEdit->setText(projectPresenter->getLogMaster());
    ui->logSlavePathEdit->setText(projectPresenter->getLogSlave());
    ui->outputPathEdit->setText(projectPresenter->getResultPath());
    ui->slaveEdit->setText(projectPresenter->getSlave());
}
