/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "AgentView.h"
#include "ui_AgentView.h"

#include "Models/AgentModel.h"
#include "Presenters/AgentPresenter.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>

const WindowInterface::ID AgentView::ViewID = QStringLiteral("openPASS.Agent");

AgentView::AgentView(WindowInterface * const window,
                     AgentPresenter * const agentPresenter,
                     QWidget *parent)
    : QWidget(parent)
    , window(window)
    , _agentPresenter(agentPresenter)
    , ui(new Ui::AgentView)
{
    // Create UI
    ui->setupUi(this);

    // Signals-Slots connection declaration for actions from the Browser Buttons
    connect(agentPresenter, &AgentPresenter::updateView,
            this, &AgentView::updateView);

    // Register view
    window->add(ViewID, WindowInterface::createButton(tr("Agent"), 0, 03000), this,
                {});
}

AgentView::~AgentView()
{
    // Deregister view
    window->remove(ViewID);

    // Destroy UI
    delete ui;
}

// Import the Scenery Configuration (load file path for the Master)
void AgentView::on_browseButton_clicked()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const filepath = QFileDialog::getOpenFileName(
                this, tr("openPASS / Import Agent Properties"), root.canonicalPath(),
                QStringLiteral("(*.xml);;All files (*)"));
    if (!filepath.isNull())
    {
        _agentPresenter->setPropertiesFile(filepath);
    }
}

// update the information displayed in the View
void AgentView::updateView()
{
    ui->agentProperties->setText(_agentPresenter->getPropertiesFile());
}
