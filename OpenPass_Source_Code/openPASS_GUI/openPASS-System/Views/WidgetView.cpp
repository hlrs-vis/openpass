/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/WidgetView.h"

#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>

WidgetView::WidgetView(QWidget * const parent)
    : QWidget(parent)
{
}

void WidgetView::paintEvent(QPaintEvent *)
{
    QStyleOption styleOption;
    styleOption.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}
