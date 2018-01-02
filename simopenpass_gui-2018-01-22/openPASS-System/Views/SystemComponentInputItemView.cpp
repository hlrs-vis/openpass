/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentInputItemView.h"
#include "ui_SystemComponentInputItemView.h"

#include "openPASS-System/SystemComponentInputItemInterface.h"
#include <QEvent>

SystemComponentInputItemView::SystemComponentInputItemView(SystemComponentInputItemInterface * const input,
                                                           QWidget * const parent)
    : WidgetView(parent)
    , input(input)
    , ui(new Ui::SystemComponentInputItemView)
{
    ui->setupUi(this);
    ui->title->setText((input->getUnit().isEmpty())
                       ? QString(QStringLiteral("%1")).
                         arg(input->getTitle())
                       : QString(QStringLiteral("%1 [%2]")).
                         arg(input->getTitle(), input->getUnit()));

    // Inform the user about the type of the input
    ui->title->setToolTip(QString(QStringLiteral("input type = %1")).arg(input->getType()));
}

SystemComponentInputItemView::~SystemComponentInputItemView()
{
    delete ui;
}

SystemComponentInputItemInterface * SystemComponentInputItemView::getInterface() const
{
    return input;
}

void SystemComponentInputItemView::setAddChecked(bool const & checked)
{
    ui->add->setChecked(checked);
}

void SystemComponentInputItemView::setAddEnabled(bool const & enabled)
{
    ui->add->setEnabled(enabled);
}

void SystemComponentInputItemView::enterEvent(QEvent * event)
{
    Q_EMIT mouseEnter(this);
    WidgetView::enterEvent(event);
}

void SystemComponentInputItemView::leaveEvent(QEvent * event)
{
    Q_EMIT mouseLeave(this);
    WidgetView::leaveEvent(event);
}

void SystemComponentInputItemView::on_add_clicked()
{
    Q_EMIT connectionAdd(this);
}

void SystemComponentInputItemView::on_remove_clicked()
{
    Q_EMIT connectionRemove(this);
}
