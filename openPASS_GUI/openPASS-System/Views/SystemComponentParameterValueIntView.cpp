/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentParameterValueIntView.h"

#include "openPASS-System/SystemComponentParameterItemInterface.h"
#include <limits>

SystemComponentParameterValueIntView::SystemComponentParameterValueIntView(SystemComponentParameterItemInterface * const parameter,
                                                                           QWidget * const parent)
    : SpinBoxView(parent)
    , parameter(parameter)
{
    setMinimumSize(0, 20);
    setMaximumSize(64, 20);
    setFrame(false);
    setMinimum(std::numeric_limits<int>::lowest());
    setMaximum(std::numeric_limits<int>::max());
    setKeyboardTracking(false);
    connect(parameter, &SystemComponentParameterItemInterface::modifiedValue,
            this, &SystemComponentParameterValueIntView::updateView);
    connect(this, static_cast<void (SystemComponentParameterValueIntView::*)(int)>(
                &SystemComponentParameterValueIntView::valueChanged),
            this, &SystemComponentParameterValueIntView::updateInterface);
    updateView();
}

void SystemComponentParameterValueIntView::updateInterface(int value)
{
    parameter->setValue(QString::number(value));
}

void SystemComponentParameterValueIntView::updateView()
{
    setValue(parameter->getValue().toInt());
}
