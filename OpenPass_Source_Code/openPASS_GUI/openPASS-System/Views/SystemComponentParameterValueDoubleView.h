/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERVALUEDOUBLEVIEW_H
#define SYSTEMCOMPONENTPARAMETERVALUEDOUBLEVIEW_H

#include "Views/DoubleSpinBoxView.h"

class SystemComponentParameterItemInterface;

class SystemComponentParameterValueDoubleView : public DoubleSpinBoxView
{
    Q_OBJECT

public:
    explicit SystemComponentParameterValueDoubleView(SystemComponentParameterItemInterface * const parameter,
                                                     QWidget * const parent = nullptr);
    virtual ~SystemComponentParameterValueDoubleView() = default;

public:
    int adjustStepSize(double value);

protected Q_SLOTS:
    void updateInterface(double value);
    void updateView();

protected:
    SystemComponentParameterItemInterface * const parameter;
};

#endif // SYSTEMCOMPONENTPARAMETERVALUEDOUBLEVIEW_H
