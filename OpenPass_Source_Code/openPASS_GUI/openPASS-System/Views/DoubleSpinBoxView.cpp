/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/DoubleSpinBoxView.h"

#include <QWheelEvent>

DoubleSpinBoxView::DoubleSpinBoxView(QWidget * const parent)
    : QDoubleSpinBox(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void DoubleSpinBoxView::wheelEvent(QWheelEvent * event)
{
    if (!hasFocus())
    {
        event->ignore();
    }
    else
    {
        QDoubleSpinBox::wheelEvent(event);
    }
}
