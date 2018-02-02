/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SYSTEMCOMPONENTPARAMETERVALUESTRINGVIEW_H
#define SYSTEMCOMPONENTPARAMETERVALUESTRINGVIEW_H

#include <QLineEdit>

class SystemComponentParameterItemInterface;

class SystemComponentParameterValueStringView : public QLineEdit
{
    Q_OBJECT

public:
    explicit SystemComponentParameterValueStringView(SystemComponentParameterItemInterface * const parameter,
                                                     QWidget * const parent = nullptr);
    virtual ~SystemComponentParameterValueStringView() = default;

protected Q_SLOTS:
    void updateInterface(QString const & text);
    void updateView();

protected:
    SystemComponentParameterItemInterface * const parameter;
};

#endif // SYSTEMCOMPONENTPARAMETERVALUESTRINGVIEW_H
