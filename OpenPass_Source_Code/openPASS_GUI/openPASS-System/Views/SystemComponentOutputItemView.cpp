/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentOutputItemView.h"
#include "ui_SystemComponentOutputItemView.h"

#include "openPASS-System/SystemComponentOutputItemInterface.h"
#include <QEvent>

SystemComponentOutputItemView::SystemComponentOutputItemView(SystemComponentOutputItemInterface * const output,
                                                             QWidget * const parent)
    : WidgetView(parent)
    , output(output)
    , ui(new Ui::SystemComponentOutputItemView)
{
    ui->setupUi(this);
    ui->title->setText((output->getUnit().isEmpty())
                       ? QString(QStringLiteral("%1")).
                         arg(output->getTitle())
                       : QString(QStringLiteral("%1 [%2]")).
                         arg(output->getTitle(), output->getUnit()));

    // Inform the user about the type of the output
    ui->title->setToolTip(QString(QStringLiteral("output type = %1")).arg(output->getType()));
}

SystemComponentOutputItemView::~SystemComponentOutputItemView()
{
    delete ui;
}

SystemComponentOutputItemInterface * SystemComponentOutputItemView::getInterface() const
{
    return output;
}

void SystemComponentOutputItemView::setAddChecked(bool const & checked)
{
    ui->add->setChecked(checked);
}

void SystemComponentOutputItemView::setAddEnabled(bool const & enabled)
{
    ui->add->setEnabled(enabled);
}

void SystemComponentOutputItemView::enterEvent(QEvent * event)
{
    Q_EMIT mouseEnter(this);
    WidgetView::enterEvent(event);
}

void SystemComponentOutputItemView::leaveEvent(QEvent * event)
{
    Q_EMIT mouseLeave(this);
    WidgetView::leaveEvent(event);
}

void SystemComponentOutputItemView::on_add_clicked()
{
    Q_EMIT connectionAdd(this);
}

void SystemComponentOutputItemView::on_remove_clicked()
{
    Q_EMIT connectionRemove(this);
}
