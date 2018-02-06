/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef DOUBLESPINBOXVIEW_H
#define DOUBLESPINBOXVIEW_H

#include <QDoubleSpinBox>

class DoubleSpinBoxView : public QDoubleSpinBox
{
    Q_OBJECT

public:
    explicit DoubleSpinBoxView(QWidget * const parent = nullptr);
    virtual ~DoubleSpinBoxView() = default;

protected:
    virtual void wheelEvent(QWheelEvent * event) override;
};

#endif // DOUBLESPINBOXVIEW_H
