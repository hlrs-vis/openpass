/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentParameterValueDoubleView.h"

#include "openPASS-System/SystemComponentParameterItemInterface.h"
#include <limits>
#include <QtMath>

SystemComponentParameterValueDoubleView::SystemComponentParameterValueDoubleView(SystemComponentParameterItemInterface * const parameter,
                                                                                 QWidget * const parent)
    : DoubleSpinBoxView(parent)
    , parameter(parameter)
{
    setMinimumSize(0, 20);
    setMaximumSize(64, 20);
    setFrame(false);
    setDecimals(3);
    setSingleStep(pow(10,adjustStepSize(parameter->getValue().toDouble())));
    setMinimum(std::numeric_limits<double>::lowest());
    setMaximum(std::numeric_limits<double>::max());
    setKeyboardTracking(false);
    connect(parameter, &SystemComponentParameterItemInterface::modifiedValue,
            this, &SystemComponentParameterValueDoubleView::updateView);
    connect(this, static_cast<void (SystemComponentParameterValueDoubleView::*)(double)>(
                &SystemComponentParameterValueDoubleView::valueChanged),
            this, &SystemComponentParameterValueDoubleView::updateInterface);
    updateView();
}

int SystemComponentParameterValueDoubleView::adjustStepSize(double value)
{
    // Determine the number of decimals (with 3 max) in the value
    int i= 0;
    value = (value - qFloor(value)) * 10;
    while (value != 0 && i>-3)
    {
        value = (value - qFloor(value)) * 10;
        i -= 1;
    }
    return i;
}

void SystemComponentParameterValueDoubleView::updateInterface(double value)
{
    parameter->setValue(QString::number(value));
}

void SystemComponentParameterValueDoubleView::updateView()
{
    setValue(parameter->getValue().toDouble());
}
