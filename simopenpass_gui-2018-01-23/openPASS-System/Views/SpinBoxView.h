/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef SPINBOXVIEW_H
#define SPINBOXVIEW_H

#include <QSpinBox>

class SpinBoxView : public QSpinBox
{
    Q_OBJECT

public:
    explicit SpinBoxView(QWidget * const parent = nullptr);
    virtual ~SpinBoxView() = default;

protected:
    virtual void wheelEvent(QWheelEvent * event) override;
};

#endif // SPINBOXVIEW_H
