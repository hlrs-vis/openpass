/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERVALUEINTVIEW_H
#define SYSTEMCOMPONENTPARAMETERVALUEINTVIEW_H

#include "Views/SpinBoxView.h"

class SystemComponentParameterItemInterface;

class SystemComponentParameterValueIntView : public SpinBoxView
{
    Q_OBJECT

public:
    explicit SystemComponentParameterValueIntView(SystemComponentParameterItemInterface * const parameter,
                                                  QWidget * const parent = nullptr);
    virtual ~SystemComponentParameterValueIntView() = default;

protected Q_SLOTS:
    void updateInterface(int value);
    void updateView();

protected:
    SystemComponentParameterItemInterface * const parameter;
};

#endif // SYSTEMCOMPONENTPARAMETERVALUEINTVIEW_H
