/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "SceneryView.h"
#include "ui_SceneryView.h"

#include "Models/SceneryModel.h"
#include "Presenters/SceneryPresenter.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>

const WindowInterface::ID SceneryView::ViewID = QStringLiteral("openPASS.Scenery");

SceneryView::SceneryView(WindowInterface * const window,
                         SceneryPresenter * const sceneryPresenter,
                         QWidget *parent)
    : QWidget(parent)
    , window(window)
    , _sceneryPresenter(sceneryPresenter)
    , ui(new Ui::SceneryView)
{
    // Create UI
    ui->setupUi(this);

    // initial settings
    ui->hideButton->hide();
    ui->detailsWidget->hide();
    ui->showButton->setEnabled(false);

    // Signals-Slots connection declaration for actions from the Browser Buttons
    connect(sceneryPresenter, &SceneryPresenter::updateView,
            this, &SceneryView::updateView);

    // Register view
    window->add(ViewID, WindowInterface::createButton(tr("Scenery"), 0, 02000), this,
                {});
}

SceneryView::~SceneryView()
{
    // Deregister view
    window->remove(ViewID);

    // Destroy UI
    delete ui;
}

// Import the Scenery Configuration (load file path for the Master)
void SceneryView::on_browseButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                this, tr("openPASS / Import Scenery Configuration"), root.canonicalPath(),
                QStringLiteral("Scenery Configuration (*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        _sceneryPresenter->setSceneryConfigFile(filepath);
    }
}

void SceneryView::on_showButton_clicked()
{
    ui->showButton->hide();
    ui->frame->resize(461, 121);
    ui->detailsWidget->show();
    ui->hideButton->show();
}

void SceneryView::on_hideButton_clicked()
{
    ui->detailsWidget->hide();
    ui->hideButton->hide();
    ui->frame->resize(461, 71);
    ui->showButton->show();
}

// update the information displayed in the View
void SceneryView::updateView()
{
    ui->showButton->setEnabled(true);
    ui->sceneryPath->setText(_sceneryPresenter->getSceneryConfigFile());
    ui->sourceEdit->setText(_sceneryPresenter->getScenerySource());
    ui->agentsEdit->setText(_sceneryPresenter->getSceneryAgents());
}
