/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentTitleView.h"
#include "ui_SystemComponentTitleView.h"

#include "openPASS-System/SystemComponentItemInterface.h"
#include <QMouseEvent>

QPoint const SystemComponentTitleView::MouseInvalidPosition = QPoint(-1, -1);

SystemComponentTitleView::SystemComponentTitleView(SystemComponentItemInterface * const component,
                                                   QWidget * const parent)
    : WidgetView(parent)
    , component(component)
    , ui(new Ui::SystemComponentTitleView)
{
    // Setup UI
    ui->setupUi(this);

    // Connect SystemComponentItemInterface with SystemComponentTitleView
    connect(component, &SystemComponentItemInterface::modifiedTitle,
            this, &SystemComponentTitleView::updateTitle);
    updateTitle();

    // Inform the user about the component name in the library
    ui->name->setToolTip(component->getTitle());
}

SystemComponentTitleView::~SystemComponentTitleView()
{
    // Destroy UI
    delete ui;
}

void SystemComponentTitleView::updateTitle()
{
    // Update name
    ui->name->setText(component->getTitle());
}

void SystemComponentTitleView::mousePressEvent(QMouseEvent * event)
{
    mouseGlobalPosition = MouseInvalidPosition;
    if (event->button() == Qt::LeftButton)
    {
        mouseGlobalPosition = event->globalPos();
    }
}

void SystemComponentTitleView::mouseMoveEvent(QMouseEvent * event)
{
    if (mouseGlobalPosition != MouseInvalidPosition)
    {
        component->setPosition(qobject_cast<QWidget *>(parent()->parent())->pos() +
                               event->globalPos() - mouseGlobalPosition);
        mouseGlobalPosition = event->globalPos();
    }
}
