/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentItemView.h"
#include "ui_SystemComponentItemView.h"

#include "Views/SystemComponentInputItemView.h"
#include "Views/SystemComponentOutputItemView.h"
#include "Views/SystemComponentParameterItemView.h"
#include "Views/SystemComponentScheduleView.h"
#include "Views/SystemComponentTitleView.h"
#include <QKeyEvent>
#include <QMouseEvent>

QMap<SystemComponentItemInterface::Type, QString> const SystemComponentItemView::TypeColorMap = {
    {SystemComponentItemInterface::Type::Action, QStringLiteral("#74bd4f")},
    {SystemComponentItemInterface::Type::Algorithm, QStringLiteral("#bd4f4f")},
    {SystemComponentItemInterface::Type::Sensor, QStringLiteral("#4f81bd")},
};
QString const SystemComponentItemView::TypeColorDefault = QStringLiteral("#cdcdcd");

SystemComponentItemView::SystemComponentItemView(SystemComponentItemInterface * const component,
                                                 QWidget * const parent)
    : WidgetView(parent)
    , component(component)
    , ui(new Ui::SystemComponentItemView)
{
    // Setup UI
    ui->setupUi(this);
    ui->rename->hide();
    ui->actionsLayout->insertWidget(0, new SystemComponentTitleView(component, this), 1);
    ui->schedule->insertWidget(0, new SystemComponentScheduleView(component->getSchedule(), this), 1);

    // Connect SystemComponentItemInterface with SystemComponentItemView
    connect(component, &SystemComponentItemInterface::modifiedPosition,
            this, &SystemComponentItemView::updatePosition);
    updatePosition();

    // Create SystemComponentParameterItemViews
    for (SystemComponentParameterItemInterface * const parameterInterface : *component->getParameters())
    {
        ui->parameters->addWidget(new SystemComponentParameterItemView(parameterInterface, this));
    }

    // Create SystemComponentInputItemViews
    for (SystemComponentInputItemInterface * const inputInterface : *component->getInputs())
    {
        SystemComponentInputItemView * const inputView = new SystemComponentInputItemView(inputInterface, this);
        connect(inputView, &SystemComponentInputItemView::mouseEnter,
                this, &SystemComponentItemView::inputMouseEnter);
        connect(inputView, &SystemComponentInputItemView::mouseLeave,
                this, &SystemComponentItemView::inputMouseLeave);
        connect(inputView, &SystemComponentInputItemView::connectionAdd,
                this, &SystemComponentItemView::inputConnectionAdd);
        connect(inputView, &SystemComponentInputItemView::connectionRemove,
                this, &SystemComponentItemView::inputConnectionRemove);
        ui->inputs->addWidget(inputView);
    }

    // Create SystemComponentOutputItemViews
    for (SystemComponentOutputItemInterface * const outputInterface : *component->getOutputs())
    {
        SystemComponentOutputItemView * const outputView = new SystemComponentOutputItemView(outputInterface, this);
        connect(outputView, &SystemComponentOutputItemView::mouseEnter,
                this, &SystemComponentItemView::outputMouseEnter);
        connect(outputView, &SystemComponentOutputItemView::mouseLeave,
                this, &SystemComponentItemView::outputMouseLeave);
        connect(outputView, &SystemComponentOutputItemView::connectionAdd,
                this, &SystemComponentItemView::outputConnectionAdd);
        connect(outputView, &SystemComponentOutputItemView::connectionRemove,
                this, &SystemComponentItemView::outputConnectionRemove);
        ui->outputs->addWidget(outputView);
    }

    // Show UI
    show();

    // Update type
    updateType();
}

SystemComponentItemView::~SystemComponentItemView()
{
    // Destroy UI
    delete ui;
}

SystemComponentItemInterface * SystemComponentItemView::getInterface() const
{
    // Return interface
    return component;
}

void SystemComponentItemView::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Escape)
    {
        Q_EMIT pressEscape();
        WidgetView::keyPressEvent(event);
    }
}

void SystemComponentItemView::updateType()
{
    // Update type
    setStyleSheet(styleSheet().replace(styleSheet().indexOf(objectName()) + 46, 7,
                                       TypeColorMap.value(component->getType(), TypeColorDefault)));
}

void SystemComponentItemView::updatePosition()
{
    // Update position
    move(component->getPosition());
    Q_EMIT moved();
}

void SystemComponentItemView::on_renameEdit_returnPressed()
{
    // Save name and switch to actions widget
    on_renameSave_clicked();
}

void SystemComponentItemView::on_renameCancel_clicked()
{
    // Switch to actions widget
    ui->rename->hide();
    ui->actions->show();
}

void SystemComponentItemView::on_renameSave_clicked()
{
    // Save title
    component->setTitle(ui->renameEdit->text());

    // Switch to actions widget
    ui->rename->hide();
    ui->actions->show();
}

void SystemComponentItemView::on_actionRename_clicked()
{
    // Switch to rename widget
    ui->actions->hide();
    ui->renameEdit->setText(component->getTitle());
    ui->rename->show();
    ui->renameEdit->selectAll();
    ui->renameEdit->setFocus();
}

void SystemComponentItemView::on_actionDuplicate_clicked()
{
    // Duplicate component
    component->duplicate(pos() + QPoint(width() + 20, 10));
}

void SystemComponentItemView::on_actionRemove_clicked()
{
    // Remove component
    component->remove();
}
