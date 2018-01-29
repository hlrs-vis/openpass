/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERITEMVIEW_H
#define SYSTEMCOMPONENTPARAMETERITEMVIEW_H

#include "Views/WidgetView.h"

class SystemComponentParameterItemInterface;

namespace Ui {
class SystemComponentParameterItemViews;
}

class SystemComponentParameterItemView : public WidgetView
{
    Q_OBJECT

public:
    explicit SystemComponentParameterItemView(SystemComponentParameterItemInterface * const parameter,
                                              QWidget * const parent = nullptr);
    virtual ~SystemComponentParameterItemView();

protected Q_SLOTS:
    void updateTitle();

protected:
    SystemComponentParameterItemInterface * const parameter;

protected:
    Ui::SystemComponentParameterItemViews * const ui;
};

#endif // SYSTEMCOMPONENTPARAMETERITEMVIEW_H
