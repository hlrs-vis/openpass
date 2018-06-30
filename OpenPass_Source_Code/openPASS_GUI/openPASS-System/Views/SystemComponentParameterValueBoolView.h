/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERVALUEBOOLVIEW_H
#define SYSTEMCOMPONENTPARAMETERVALUEBOOLVIEW_H

#include <QCheckBox>

class SystemComponentParameterItemInterface;

class SystemComponentParameterValueBoolView : public QCheckBox
{
    Q_OBJECT

public:
    explicit SystemComponentParameterValueBoolView(SystemComponentParameterItemInterface * const parameter,
                                                   QWidget * const parent = nullptr);
    virtual ~SystemComponentParameterValueBoolView() = default;

protected Q_SLOTS:
    void updateInterface(bool checked);
    void updateView();

protected:
    SystemComponentParameterItemInterface * const parameter;
};

#endif // SYSTEMCOMPONENTPARAMETERVALUEBOOLVIEW_H
