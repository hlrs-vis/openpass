/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentScheduleView.h"
#include "ui_SystemComponentScheduleView.h"

#include "openPASS-System/SystemComponentScheduleInterface.h"
#include <limits>

SystemComponentScheduleView::SystemComponentScheduleView(SystemComponentScheduleInterface * const schedule,
                                                         QWidget * const parent)
    : WidgetView(parent)
    , schedule(schedule)
    , ui(new Ui::SystemComponentScheduleView)
{
    // Setup UI
    ui->setupUi(this);
    ui->priority->setMaximum(std::numeric_limits<int>::max());
    ui->offset->setMaximum(std::numeric_limits<int>::max());
    ui->cycle->setMaximum(std::numeric_limits<int>::max());
    ui->response->setMaximum(std::numeric_limits<int>::max());

    // Connect SystemComponentScheduleInterface with SystemComponentScheduleView
    connect(schedule, &SystemComponentScheduleInterface::modifiedPriority,
            this, &SystemComponentScheduleView::updatePriority);
    connect(schedule, &SystemComponentScheduleInterface::modifiedOffset,
            this, &SystemComponentScheduleView::updateOffset);
    connect(schedule, &SystemComponentScheduleInterface::modifiedCycle,
            this, &SystemComponentScheduleView::updateCycle);
    connect(schedule, &SystemComponentScheduleInterface::modifiedResponse,
            this, &SystemComponentScheduleView::updateResponse);

    // Update UI
    updatePriority();
    updateOffset();
    updateCycle();
    updateResponse();
}

SystemComponentScheduleView::~SystemComponentScheduleView()
{
    // Destroy UI
    delete ui;
}

void SystemComponentScheduleView::updatePriority()
{
    ui->priority->setValue(schedule->getPriority());
}

void SystemComponentScheduleView::updateOffset()
{
    ui->offset->setValue(schedule->getOffset());
}

void SystemComponentScheduleView::updateCycle()
{
    ui->cycle->setValue(schedule->getCycle());
}

void SystemComponentScheduleView::updateResponse()
{
    ui->response->setValue(schedule->getResponse());
}

void SystemComponentScheduleView::on_priority_valueChanged(int arg1)
{
    schedule->setPriority(arg1);
}

void SystemComponentScheduleView::on_offset_valueChanged(int arg1)
{
    schedule->setOffset(arg1);
}

void SystemComponentScheduleView::on_cycle_valueChanged(int arg1)
{
    schedule->setCycle(arg1);
}

void SystemComponentScheduleView::on_response_valueChanged(int arg1)
{
    schedule->setResponse(arg1);
}
