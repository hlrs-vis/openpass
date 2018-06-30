/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentParameterItemView.h"
#include "ui_SystemComponentParameterItemView.h"

#include "openPASS-System/SystemComponentParameterItemInterface.h"
#include "Views/SystemComponentParameterValueBoolView.h"
#include "Views/SystemComponentParameterValueDoubleView.h"
#include "Views/SystemComponentParameterValueFloatView.h"
#include "Views/SystemComponentParameterValueIntView.h"
#include "Views/SystemComponentParameterValueStringView.h"

SystemComponentParameterItemView::SystemComponentParameterItemView(SystemComponentParameterItemInterface * const parameter,
                                                                   QWidget * const parent)
    : WidgetView(parent)
    , parameter(parameter)
    , ui(new Ui::SystemComponentParameterItemViews)
{
    // Setup UI
    ui->setupUi(this);
    updateTitle();

    // Setup value / bool
    if (parameter->getType() == QStringLiteral("bool"))
        ui->layout->addWidget(new SystemComponentParameterValueBoolView
                              (parameter, this), 0, Qt::AlignRight);

    // Setup value / int
    if (parameter->getType() == QStringLiteral("int"))
        ui->layout->addWidget(new SystemComponentParameterValueIntView
                              (parameter, this), 0, Qt::AlignRight);

    // Setup value / float
    if (parameter->getType() == QStringLiteral("float"))
        ui->layout->addWidget(new SystemComponentParameterValueFloatView
                              (parameter, this), 0, Qt::AlignRight);

    // Setup value / double
    if (parameter->getType() == QStringLiteral("double"))
        ui->layout->addWidget(new SystemComponentParameterValueDoubleView
                              (parameter, this), 0, Qt::AlignRight);

    // Setup value / string
    if (parameter->getType() == QStringLiteral("string"))
        ui->layout->addWidget(new SystemComponentParameterValueStringView
                              (parameter, this), 0, Qt::AlignRight);
}

SystemComponentParameterItemView::~SystemComponentParameterItemView()
{
    // Destroy UI
    delete ui;
}

void SystemComponentParameterItemView::updateTitle()
{
    // Update title
    ui->title->setText((parameter->getUnit().isEmpty())
                       ? QString(QStringLiteral("%1")).
                         arg(parameter->getTitle())
                       : QString(QStringLiteral("%1 [%2]")).
                         arg(parameter->getTitle(), parameter->getUnit()));
}
