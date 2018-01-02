/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemComponentParameterValueBoolView.h"

#include "openPASS-System/SystemComponentParameterItemInterface.h"

SystemComponentParameterValueBoolView::SystemComponentParameterValueBoolView(SystemComponentParameterItemInterface * const parameter,
                                                                             QWidget * const parent)
    : QCheckBox(parent)
    , parameter(parameter)
{
    connect(parameter, &SystemComponentParameterItemInterface::modifiedValue,
            this, &SystemComponentParameterValueBoolView::updateView);
    connect(this, &SystemComponentParameterValueBoolView::toggled,
            this, &SystemComponentParameterValueBoolView::updateInterface);
    updateView();
}

void SystemComponentParameterValueBoolView::updateInterface(bool checked)
{
    parameter->setValue((checked) ? QStringLiteral("1") : QStringLiteral("0"));
}

void SystemComponentParameterValueBoolView::updateView()
{
    setChecked(parameter->getValue() == QStringLiteral("1"));
}
