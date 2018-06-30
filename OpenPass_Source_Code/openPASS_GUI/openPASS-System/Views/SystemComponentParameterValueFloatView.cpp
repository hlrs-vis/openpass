/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentParameterValueFloatView.h"

#include "openPASS-System/SystemComponentParameterItemInterface.h"
#include <limits>
#include <QtMath>

SystemComponentParameterValueFloatView::SystemComponentParameterValueFloatView(SystemComponentParameterItemInterface * const parameter,
                                                                               QWidget * const parent)
    : DoubleSpinBoxView(parent)
    , parameter(parameter)
{
    setMinimumSize(0, 20);
    setMaximumSize(64, 20);
    setFrame(false);
    setDecimals(3);
    setSingleStep(pow(10,adjustStepSize(parameter->getValue().toFloat())));
    setMinimum(std::numeric_limits<float>::lowest());
    setMaximum(std::numeric_limits<float>::max());
    setKeyboardTracking(false);
    connect(parameter, &SystemComponentParameterItemInterface::modifiedValue,
            this, &SystemComponentParameterValueFloatView::updateView);
    connect(this, static_cast<void (SystemComponentParameterValueFloatView::*)(double)>(
                &SystemComponentParameterValueFloatView::valueChanged),
            this, &SystemComponentParameterValueFloatView::updateInterface);
    updateView();
}

int SystemComponentParameterValueFloatView::adjustStepSize(double value)
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

void SystemComponentParameterValueFloatView::updateInterface(double value)
{
    parameter->setValue(QString::number(static_cast<float>(value)));
}

void SystemComponentParameterValueFloatView::updateView()
{
    setValue(static_cast<double>(parameter->getValue().toFloat()));
}
