/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentManagerView.h"
#include "ui_SystemComponentManagerView.h"

#include "openPASS-Project/ProjectInterface.h"
#include "openPASS-System/SystemComponentManagerInterface.h"

SystemComponentManagerView::SystemComponentManagerView(SystemComponentManagerInterface * const components,
                                                       ProjectInterface * const project,
                                                       QWidget * const parent)
    : QWidget(parent)
    , _components(components)
    , _project(project)
    , ui(new Ui::SystemComponentManagerView)
{
    // Setup UI
    ui->setupUi(this);

    // Update the tree view
    updateManagerView();
}

SystemComponentManagerView::~SystemComponentManagerView()
{
    // Destroy UI
    delete ui;
}

void SystemComponentManagerView::show()
{
    // Maximize widget
    ui->button->setChecked(true);
    ui->tree->setVisible(true);
}

void SystemComponentManagerView::hide()
{
    // Minimize widget
    ui->button->setChecked(false);
    ui->tree->setVisible(false);

    // Update the components tree
    updateManagerView();
}

void SystemComponentManagerView::updateManagerView()
{
    // Clear the tree of components
    for (unsigned char index = 0; index < ui->tree->topLevelItemCount(); ++index)
    {
        ui->tree->topLevelItem(index)->takeChildren();
    }

    // Load again the components
    if (_components->loadFromDirectory(_project->getLibrary()))
    {
        for (unsigned char index = 0; index < ui->tree->topLevelItemCount(); ++index)
        {
            for (ComponentItemInterface::Title const & title : _components->
                 listTitlesByType(static_cast<ComponentItemInterface::Type>(index)))
            {
                new QTreeWidgetItem(ui->tree->topLevelItem(index), {title}, 0);
            }
        }
        ui->tree->expandAll();
    }
}

void SystemComponentManagerView::on_button_clicked()
{
    // Toggle widget
    ui->button->setChecked(ui->button->isChecked());
    ui->tree->setVisible(!ui->tree->isVisible());

    if (ui->button->isChecked())
    {
        updateManagerView();
    }
}
