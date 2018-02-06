/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemView.h"
#include "ui_SystemView.h"

#include "openPASS-Project/ProjectInterface.h"
#include "openPASS-System/SystemInterface.h"
#include "Views/SystemComponentManagerView.h"
#include "Views/SystemBaseView.h"
#include "Views/SystemItemView.h"
#include <QFileDialog>
#include <QFileInfo>

WindowInterface::ID const SystemView::ViewID = QStringLiteral("openPASS.System");

SystemView::SystemView(WindowInterface * const window,
                       ProjectInterface * const project,
                       SystemInterface * const system,
                       QWidget * const parent)
    : WidgetView(parent)
    , window(window)
    , project(project)
    , system(system)
    , ui(new Ui::SystemView)
{
    // Setup UI
    ui->setupUi(this);
    ui->layout->insertWidget(0, new SystemItemView(*system->getSystems()->begin(), system->getComponents(), this), 1);
    ui->layout->insertWidget(1, new SystemComponentManagerView(system->getComponents(), project, this), 0);

    // Register view
    window->add(ViewID, WindowInterface::createButton(tr("System"), 0, 04000), this, {
                    WindowInterface::createAction(tr("New System"), this, &SystemView::actionNewSystem_clicked),
                    WindowInterface::createAction(tr("Load System"), this, &SystemView::actionLoadSystem_clicked),
                    WindowInterface::createAction(tr("Save System"), this, &SystemView::actionSaveSystem_clicked),
                    WindowInterface::createAction(tr("Save Screenshot"), this, &SystemView::actionSaveScreenshot_clicked),
                });
}

SystemView::~SystemView()
{
    // Deregister view
    window->remove(ViewID);

    // Destroy UI
    delete ui;
}

void SystemView::actionNewSystem_clicked()
{
    // Clear system
    system->clear();

    // Show component list
    findChild<SystemComponentManagerView *>(QString(), Qt::FindDirectChildrenOnly)->show();
}

void SystemView::actionLoadSystem_clicked()
{
    // Prompt file dialog to load system
    QString const filepath = QFileDialog::getOpenFileName(
                this, QObject::tr("openPASS / Load System"),
                project->getDirectory(), QStringLiteral("System (*.xml)"));
    if (!filepath.isNull())
    {
        // Update project directory
        project->setDirectory(QFileInfo(filepath).absolutePath());

        // Load system from file
        system->load(filepath);

        // Hide component list
        findChild<SystemComponentManagerView *>(QString(), Qt::FindDirectChildrenOnly)->hide();
    }
}

void SystemView::actionSaveSystem_clicked()
{
    // Prompt file dialog to save system
    QString const filepath = QFileDialog::getSaveFileName(
                this, QObject::tr("openPASS / Save System"),
                project->getDirectory(), QStringLiteral("System (*.xml)"));
    if (!filepath.isNull())
    {
        // Update project directory
        project->setDirectory(QFileInfo(filepath).absolutePath());

        // Save system to file
        system->save(filepath);
    }
}

void SystemView::actionSaveScreenshot_clicked()
{
    // Prompt file dialog to save screenshot
    QString const filepath = QFileDialog::getSaveFileName(
                this, QObject::tr("openPASS / Save Screenshot"),
                project->getDirectory(), QStringLiteral("Image (*.png)"));
    if (!filepath.isNull())
    {
        // Update project directory
        project->setDirectory(QFileInfo(filepath).absolutePath());

        // Save screenshot to file
        findChild<SystemBaseView *>()->grab().save(filepath);
    }
}
