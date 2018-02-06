/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTSCHEDULEVIEW_H
#define SYSTEMCOMPONENTSCHEDULEVIEW_H

#include "Views/WidgetView.h"

class SystemComponentScheduleInterface;

namespace Ui {
class SystemComponentScheduleView;
}

class SystemComponentScheduleView : public WidgetView
{
    Q_OBJECT

public:
    explicit SystemComponentScheduleView(SystemComponentScheduleInterface * const schedule,
                                         QWidget * const parent = nullptr);
    virtual ~SystemComponentScheduleView();

protected Q_SLOTS:
    void updatePriority();
    void updateOffset();
    void updateCycle();
    void updateResponse();

protected Q_SLOTS:
    void on_priority_valueChanged(int arg1);
    void on_offset_valueChanged(int arg1);
    void on_cycle_valueChanged(int arg1);
    void on_response_valueChanged(int arg1);

protected:
    SystemComponentScheduleInterface * const schedule;

protected:
    Ui::SystemComponentScheduleView * const ui;
};

#endif // SYSTEMCOMPONENTSCHEDULEVIEW_H
